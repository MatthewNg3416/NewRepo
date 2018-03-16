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
 * MVector.h : This file defines the vector class
 *
 * @ author : 
 * @ version :
 * @ compiler Used :
 * @ support tools : 
 * @ update History 
 *		- 04/03/02 : Major, Created
 * @ see also 
*/

#ifndef __MVECTOR_H
#define __MVECTOR_H


#ifndef __MOBJECT_H
#include "MObject.h"
#endif


/*
 *	MObject를 Vector로 관리하기 위한 Class
 */
class MVector : public MObject {
protected:
	int			m_length;		// number of entries
	int			m_capacity;		// capacity
	int			m_incr;			// capacity increment
	MObject		**m_vec;

	enum { DEF_INCREMENT = 64 };

public:
	MVector();
	MVector(int capacity, int incr=DEF_INCREMENT);
	~MVector();

	// manipulators
	MObject *Add(MObject *pObj) { return Append(pObj); }
	MObject *Append(MObject *pObj);
	MObject *Insert(int offs, MObject *pObj);
	MObject *SetElementAt(int offs, MObject *pObj);
	MObject *Remove(MObject *pObj);
	MObject *Remove(int offs);
	void Remove(int startIdx, int endIdx);
	void Clear();
	void Swap(int i, int j);
	MObject *Replace(int idx, MObject *pObj);
	void RemoveAndDestroy(MObject *pObj);
	void RemoveAndDestroy(int offs);
	void RemoveAndDestroy(int startIdx, int endIdx);
	void ClearAndDestroy();
	int IndexOf(MObject* pObj) const { return IndexOf( 0, pObj ); }
	int IndexOf(int offs, MObject* pObj) const;
	int LastIndexOf(MObject* pObj) const { return LastIndexOf(0, pObj); }
	int LastIndexOf(int offs, MObject* pObj) const;
	int EnsureCapacity(int size);
	int SetCapacity(int size);

	// block operations
	void CopyTo(int startIdx, int endIdx, MVector& v, int offs) const;
	void MergeTo(int startIdx, int endIdx, MVector& v, int offs);
	void Append(const MVector& v, int start, int len)
		{ v.CopyTo( start, len, *this, Length() ); }
	void Insert(int offs, const MVector& v, int start, int len)
		{ v.CopyTo( start, len, *this, offs ); }

	// accessors
	int Length() const { return m_length; }
	int Capacity() const { return m_capacity; }
	bool Contains(MObject *pObj) const; // use pointer comparison
	MObject *ElementAt(int offs) const { return m_vec[offs]; }
	bool IsEmpty() const { return ( m_length <= 0 ); }
	MObject *FirstElement() const;
	MObject *LastElement() const;

	// overloadings
	virtual CString ToString() const;
protected:
	void MakeRoom(int offs, int len);
};

#endif
