/*
 * S/W Standardization for T7 Module Line Equipments
 * 
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc.("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license aggrement
 * you entered into with Samsung.
 */

/*
 * MVector.cpp : This file implements the MVector class
 *
 * @ author : 
 * @ version :
 * @ compiler Used :
 * @ support tools : 
 * @ update history 
 *		- 04/03/02 : Major, Created
 * @ see also : 
*/


#include "StdAfx.h"
#include "MVector.h"
#include "MObject.H"


/*
 *	MVector Class Implementation
 */
MVector::MVector()
	: m_length(0), m_capacity(0), m_incr(DEF_INCREMENT), m_vec(0)
{
//	TRACE("MVector :  Default 持失切 m_incr = %d ------------- \n", m_incr);
}

MVector::MVector(int capacity, int incr)
	: m_length(0), m_capacity(0), m_incr(incr), m_vec(0)
{
	if ( m_incr <= 0 ) m_incr = DEF_INCREMENT;
	SetCapacity(capacity);
//	TRACE("MVector :  Default 持失切 2 m_incr =  %d --------- \n", m_incr);
}

MVector::~MVector()
{
	Clear();
	if ( m_vec ) delete [] m_vec;
}

MObject *MVector::Append(MObject *pObj)
{
	EnsureCapacity( m_length+1 );
	return SetElementAt( m_length++, pObj );
}

MObject *MVector::Insert(int offs, MObject *pObj)
{
	if ( offs >= Length() )
		Append( pObj );
	else
		{
		if ( offs < 0 ) offs = 0;
		EnsureCapacity( m_length+1 );
		for (int i=m_length; i>offs; --i)
			m_vec[i] = m_vec[i-1];
		m_vec[i] = pObj;
		++m_length;
		}
	return pObj;
}

MObject *MVector::SetElementAt(int offs, MObject *pObj)
{
	if ( offs < 0 || offs >= Length() ) return 0;
	if ( m_vec[offs] ) delete m_vec[offs];
	m_vec[offs] = pObj;
	return pObj;
}

MObject *MVector::Remove(MObject *pObj)
{
	return Remove( IndexOf(pObj) );
}

MObject *MVector::Remove(int offs)
{
	if ( offs < 0 || offs >= Length() ) return 0;
	MObject *pObj = m_vec[offs];
	--m_length;
	for (int i=offs; i<m_length; ++i)
		m_vec[i] = m_vec[i+1];
	m_vec[i] = 0;
	SetCapacity( m_length );
	return pObj;
}

void MVector::Remove(int startIdx, int endIdx)
{
	if ( startIdx < 0 ) startIdx = 0;
	if ( endIdx > m_length ) endIdx = m_length;
	int nCount = endIdx - startIdx;
	for (int i=startIdx; i<endIdx; ++i)
		m_vec[i] = 0;
	for (i=endIdx; i<m_length; ++i)
		{
		m_vec[i-nCount] = m_vec[i];
		m_vec[i] = 0;
		}
	m_length -= nCount;
	SetCapacity( m_length );
}

void MVector::Clear()
{
	Remove( 0, Length() );
}

void MVector::Swap(int i, int j)
{
	if ( i < 0 || i >= Length() || j < 0 || j >= Length() ) return;
	MObject *p = m_vec[i];
	m_vec[i] = m_vec[j];
	m_vec[j] = p;
}

MObject *MVector::Replace(int idx, MObject *pObj)
{
	if ( idx < 0 || idx >= m_length ) return 0;
	MObject *p = m_vec[idx];
	m_vec[idx] = pObj;
	return p;
}

void MVector::RemoveAndDestroy(MObject *pObj)
{
	Remove(pObj);
	delete pObj;
}

void MVector::RemoveAndDestroy(int offs)
{
	MObject *p = Remove(offs);
	if ( p ) delete p;
}

void MVector::RemoveAndDestroy(int startIdx, int endIdx)
{
	if ( startIdx < 0 ) startIdx = 0;
	if ( endIdx > m_length ) endIdx = m_length;
	int nCount = endIdx - startIdx;
	for (int i=startIdx; i<endIdx; ++i)
		{
		if ( m_vec[i] ) delete m_vec[i];
		m_vec[i] = 0;
		}
	for (i=endIdx; i<m_length; ++i)
		{
		m_vec[i-nCount] = m_vec[i];
		m_vec[i] = 0;
		}
	m_length -= nCount;
	SetCapacity( m_length );
}

void MVector::ClearAndDestroy()
{
	RemoveAndDestroy( 0, Length() );
}

int MVector::IndexOf(int offs, MObject* pObj) const
{
	if ( offs < 0 ) offs = 0;
	for (int i=offs; i<Length(); ++i)
		if ( m_vec[i] == pObj ) return i;
	return -1;
}

int MVector::LastIndexOf(int offs, MObject* pObj) const
{
	for (int i=Length()-offs-1; i>=0; --i)
		if ( m_vec[i] == pObj ) return i;
	return -1;
}

int MVector::EnsureCapacity(int size)
{
	return ( size > m_capacity ) ? SetCapacity( size ) : m_capacity;
}

int MVector::SetCapacity(int size)
{
	int newSize = (size / m_incr) * m_incr;
	if ( newSize < size ) newSize += m_incr;
	if ( newSize == m_capacity ) return m_capacity;
	MObject **pBuf = m_vec;
	m_vec = (newSize > 0) ? new MObject*[newSize] : 0;
	if ( newSize > m_capacity )
		{
		for (int i=0; i<m_length; ++i)
			m_vec[i] = pBuf[i];
		while ( i < newSize )
			m_vec[i++] = 0;
		}
	else
		{
		for (int i=newSize; i<m_length; ++i)
			if ( pBuf[i] ) delete pBuf[i];
		for (i=0; i<newSize; ++i)
			m_vec[i] = pBuf[i];
		if ( m_length > newSize ) m_length = newSize;
		}
	if ( pBuf ) delete [] pBuf;
	m_capacity = newSize;
	return m_capacity;
}

void MVector::CopyTo(int startIdx, int endIdx, MVector& v, int offs) const
{
	if ( startIdx < 0 ) startIdx = 0;
	if ( endIdx > m_length ) endIdx = m_length;
	if ( offs > v.Length() ) offs = v.Length();
	v.MakeRoom(offs, endIdx-startIdx);
	for (int i=startIdx; i<endIdx; ++i)
		v.SetElementAt( offs++, m_vec[i] );
}

void MVector::MergeTo(int startIdx, int endIdx, MVector& v, int offs)
{
	CopyTo( startIdx, endIdx, v, offs );
	Remove( startIdx, endIdx );
}

bool MVector::Contains(MObject *pObj) const
{
	return ( IndexOf(pObj) >= 0 );
}

MObject *MVector::FirstElement() const
{
	return (Length() > 0) ? m_vec[0] : 0;
}

MObject *MVector::LastElement() const
{
	return (Length() > 0) ? m_vec[Length()-1] : 0;
}

CString MVector::ToString() const
{
	return CString("MVector");
}

void MVector::MakeRoom(int offs, int len)
{
	if ( len <= 0 ) return;
	EnsureCapacity( m_length + len );
	for (int i=m_length-1; i>=offs; --i)
		{
		m_vec[i+len] = m_vec[i];
		m_vec[i] = 0;
		m_length += len;
		}
}
