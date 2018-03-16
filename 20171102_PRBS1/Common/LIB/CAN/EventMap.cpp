/********************************************************/
/*                                                      */
/*  Copley Motion Libraries                             */
/*                                                      */
/*  Copyright (c) 2002 Copley Controls Corp.            */
/*                     http://www.copleycontrols.com    */
/*                                                      */
/********************************************************/

/** \file
This file contains the implementation of the EventMap class.
*/

#include "stdafx.h"
#include "CML.h"

CML_NAMESPACE_USE();

/**************************************************
* Event related error objects
**************************************************/
CML_NEW_ERROR( EventError, AlreadyOwned, "The event is already mapped to another object" );
CML_NEW_ERROR( EventError, NotMapped,    "The event is not mapped to this object." );

/***************************************************************************/
/**
Default constructor for an Event object.
@param val The value that the event will wait for.  If not specified, 
       defaults to zero.
*/
/***************************************************************************/
Event::Event( uint32 val )
{
   map = 0; 
   semPtr = 0; 
   chainMap = 0;
   value = val;
}

/***************************************************************************/
/**
Event distructor.  This makes sure the event isn't mapped when it's destroyed
*/
/***************************************************************************/
Event::~Event( void )
{
   if( map )
      map->Remove( this );
}

/***************************************************************************/
/**
Change the value that the event will wait for.  The event's value can not
be changed while it is attached to an EventMap object.  If this is attempted, 
then &EventError::AlreadyOwned will be returned.

@param val The new event value
@return A pointer to an error object, or NULL on success.
*/
/***************************************************************************/
const Error *Event::setValue( uint32 val )
{
   if( map ) return &EventError::AlreadyOwned;
   value = val;
   return 0;
}

/***************************************************************************/
/**
Update this event's mask.  If this event has a semaphore associated with it,
and this update causes it to become true, then the semaphore will be posted
and the event will be removed from it's map.
@param newMask The new mask value
*/
/***************************************************************************/
void Event::update( uint32 mask )
{
   bool t = isTrue( mask );

   if( t )
   {
      trueMask = mask;

      if( chainMap )
	 chainMap->setBits( chainMask );

      if( semPtr )
	 semPtr->Put();
   }

   else if( chainMap )
      chainMap->clrBits( chainMask );
}

/***************************************************************************/
/**
Wait on an event.  This function causes the calling thread to pend until
the event is true, or the timeout expires.

Note that the event should not be owned by any event map when this is called.

@param m The event map that this event should watch.

@param timeout The maximum amount of time to wait (milliseconds).  If < 0,
       then the task will wait forever.
		 
@return A pointer to an error object on failure, or NULL on success.
*/
/***************************************************************************/
const Error *Event::Wait( EventMap &m, int32 timeout )
{
   Semaphore sem;
   semPtr = &sem;

   const Error *err = m.Add( this );
   if( err )
   {
      semPtr = 0;
      return err;
   }

   err = sem.Get(timeout);

   m.Remove( this );
   semPtr = 0;
   return err;
}

/***************************************************************************/
/**
Setup event chaining.  Each time the event is updated, it will either set
or clear the bits specified by mask in the referenced EventMap object.
The bits will be set if the update causes the event to be true, and cleared
otherwise.

@param map  The map to chain.
@param mask The bit(s) to set/clear in the chained map based on the state 
            of this event.
*/
/***************************************************************************/
void Event::setChain( class EventMap &map, uint32 mask )
{
   chainMap = &map;
   chainMask = mask;
}

/***************************************************************************/
/**
Remove any pointer to a chained event map.  This undoes any chaining that was
setup using the Event::setChain method.
*/
/***************************************************************************/
void Event::delChain( void )
{
   chainMap = 0;
}

/***************************************************************************/
/**
Event map destructor.  Removes any attached events
Note that it is an error to destroy an EventMap if it is currently pointed
to by any events using 'setChain'.  Make sure to remove any such chaining
before the event map is destroyed.
*/
/***************************************************************************/
EventMap::~EventMap( void )
{
   while( list ) Remove( list );
}

/***************************************************************************/
/**
Add the passed event to the list of events pending on this map.
If the event is already mapped to a map (this one or another)
then this function will return &EventError::AlreadyOwned.

@param e Points to the event to add
@return A pointer to an error object on failure, or NULL on success.
*/
/***************************************************************************/
const Error *EventMap::Add( Event *e )
{
   // Make sure the event is available
   if( e->map ) 
      return &EventError::AlreadyOwned;

   // Add it to my list
   e->map = this;
   e->prev = 0;

   mutex.Lock();
   e->next = list;
   if( list ) list->prev = e;
   list = e;

   // Update the event 
   e->update( mask );
   mutex.Unlock();

   return 0;
}

/***************************************************************************/
/**
Remove the passed event from the list of events pending on this map.  If
the event is not presently attached to this map, then this function will
return &EventError::NotMapped.

@param e Points to the event to remove
@return A pointer to an error object on failure, or NULL on success.
*/
/***************************************************************************/
const Error *EventMap::Remove( Event *e )
{
   // Make sure I own it
   if( e->map != this ) 
      return &EventError::NotMapped;

   mutex.Lock();

   // Remove it from my list
   if( e->next ) e->next->prev = e->prev;
   if( e->prev ) e->prev->next = e->next;
   else list = e->next;
   mutex.Unlock();

   e->map = 0;
   return 0;
}

/***************************************************************************/
/**
Update the mask associated with this event map.
It is assumed that the mutex is already locked when this function is called.
*/
/***************************************************************************/
void EventMap::update( uint32 newMask )
{
   mask = newMask;

   for( Event *e = list; e; e = e->next )
      e->update( mask );
}

