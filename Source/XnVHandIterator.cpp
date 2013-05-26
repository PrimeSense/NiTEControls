/*****************************************************************************
*                                                                            *
*  NiTE Controls 1.x Alpha                                                   *
*  Copyright (C) 2013 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of NiTE Controls Lab.                                   *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
#include "XnVMultipleHands.h"
#include "XnVNiteEvents.h"

XnVMultipleHands::ConstIterator::ConstIterator(const ConstIterator& other) :
	m_bValid(other.m_bValid),
	m_pHands(other.m_pHands),
	m_nPosition(other.m_nPosition)
	{}

XnVMultipleHands::ConstIterator::ConstIterator(const XnVMultipleHands* pHands) :
	m_bValid(false),
	m_pHands(pHands),
	m_nPosition(0)
{
	if (m_pHands != NULL)
	{
		m_bValid = true;
		while (m_nPosition < 32 && m_pHands->m_Hands[m_nPosition].nID == 0)
		{
			++m_nPosition;
		}
	}
} // ConstIterator::ConstIterator

XnVMultipleHands::ConstIterator::ConstIterator(const XnVMultipleHands* pHands, XnUInt32 nPosition) :
	m_bValid(true),
	m_pHands(pHands),
	m_nPosition(nPosition)
{
	while (m_nPosition < 32 && m_pHands->m_Hands[m_nPosition].nID == 0)
	{
		++m_nPosition;
	}
} // ConstIterator::ConstIterator

XnVMultipleHands::ConstIterator& XnVMultipleHands::ConstIterator::operator++()
{
	if (!m_bValid)
	{
		return *this;
	}

	++m_nPosition;
	while (m_nPosition < 32 && m_pHands->m_Hands[m_nPosition].nID == 0)
	{
		++m_nPosition;
	}
	return *this;
} // ConstIterator::operator++

XnVMultipleHands::ConstIterator XnVMultipleHands::ConstIterator::operator++(int)
{
	if (!m_bValid)
	{
		return *this;
	}

	ConstIterator other(*this);
	++m_nPosition;
	while (m_nPosition < 32 && m_pHands->m_Hands[m_nPosition].nID == 0)
	{
		++m_nPosition;
	}
	return other;
} // ConstIterator::operator++

XnBool XnVMultipleHands::ConstIterator::operator==(const ConstIterator& other) const
{
	if (!m_bValid || !other.m_bValid)
	{
		return false;
	}
	return (m_pHands == other.m_pHands && m_nPosition == other.m_nPosition);
} // ConstIterator::operator==

XnBool XnVMultipleHands::ConstIterator::operator!=(const ConstIterator& other) const
{
	return !operator==(other);
} // ConstIterator::operator!=

const XnVHandPointContext* XnVMultipleHands::ConstIterator::operator*() const
{
	if (!m_bValid)
	{
		return NULL;
	}
	return &m_pHands->m_Hands[m_nPosition];
} // ConstIterator::operator*

XnBool XnVMultipleHands::ConstIterator::IsNew() const
{
	XnUInt32 key = m_pHands->m_Hands[m_nPosition].nID;

	return (m_pHands->m_pNewIDs->Find(key) != m_pHands->m_pNewIDs->end());
} // ConstIterator::IsNew

XnBool XnVMultipleHands::ConstIterator::IsActive() const
{
	XnUInt32 key = m_pHands->m_Hands[m_nPosition].nID;

	return (m_pHands->m_pActiveIDs->Find(key) != m_pHands->m_pActiveIDs->end());
} // ConstIterator::IsActive
XnBool XnVMultipleHands::ConstIterator::IsValid() const
{
	return m_bValid;
} // ConstIterator::IsValid

///////////////////////////////////////////////////////////////////

XnVMultipleHands::Iterator::Iterator(const Iterator& other) :
	ConstIterator(other)
{}
XnVMultipleHands::Iterator::Iterator(XnVMultipleHands* pHands) :
	ConstIterator(pHands)
{}
XnVMultipleHands::Iterator::Iterator(XnVMultipleHands* pHands, XnUInt32 nPosition) :
	ConstIterator(pHands, nPosition)
{}


XnVMultipleHands::Iterator& XnVMultipleHands::Iterator::operator++()
{
	++(*(ConstIterator*)this);
	return *this;
} // Iterator::operator++

XnVMultipleHands::Iterator XnVMultipleHands::Iterator::operator++(int)
{
	Iterator result = *this;
	++*this;
	return result;
} // Iterator::operator++

XnVHandPointContext* XnVMultipleHands::Iterator::operator*() const
{
	return ((XnVHandPointContext*)**(ConstIterator*)this);
} // Iterator::operator*

/////////////////////////////////////////////////
XnVMultipleHands::ConstIteratorOld::ConstIteratorOld(const XnVMultipleHands::ConstIteratorOld& other) :
	m_pList(other.m_pList),
	m_nPosition(other.m_nPosition),
	m_bValid(other.m_bValid),
	m_nSize(other.m_nSize)
	{}
XnVMultipleHands::ConstIteratorOld& XnVMultipleHands::ConstIteratorOld::operator++()
{
	m_nPosition++;
	if (m_nPosition >= m_nSize)
	{
		m_nPosition = -1;
	}
	return *this;
}
XnVMultipleHands::ConstIteratorOld XnVMultipleHands::ConstIteratorOld::operator++(int)
{
	ConstIteratorOld other(*this);
	operator++();
	return other;
}
XnBool XnVMultipleHands::ConstIteratorOld::operator==(const XnVMultipleHands::ConstIteratorOld& other) const
{
	if (m_pList != other.m_pList)
		return FALSE;
	if (m_nPosition != other.m_nPosition)
		return FALSE;
	return TRUE;
}
XnBool XnVMultipleHands::ConstIteratorOld::operator!=(const XnVMultipleHands::ConstIteratorOld& other) const
{
	return !operator==(other);
}
XnUInt32 XnVMultipleHands::ConstIteratorOld::operator*() const
{
	XnUInt32 nCounter = 0;
	for (XnVIntList::ConstIterator iter = m_pList->begin(); iter != m_pList->end(); ++iter, ++nCounter)
	{
		if (nCounter == m_nPosition)
		{
			return *iter;
		}
	}
	return 0;
}
//	protected:
XnVMultipleHands::ConstIteratorOld::ConstIteratorOld(const XnVIntList* pList) :
	m_pList(pList),
	m_nPosition(0),
	m_bValid(TRUE)
{
	m_nSize = m_pList->Size();
 	if (m_nSize == 0)
 	{
 		m_nPosition = -1;
 	}
}
XnVMultipleHands::ConstIteratorOld::ConstIteratorOld(const XnVIntList* pList, XnUInt32 nPosition) :
	m_pList(pList),
	m_nPosition(nPosition),
	m_bValid(TRUE)
{
	m_nSize = m_pList->Size();
	if (m_nPosition >= m_nSize)
	{
		m_nPosition = -1;
	}
}

// 		const XnVIntList* m_pList;
// 		XnUInt32 m_nPosition;
// 		XnBool m_bValid;
//////////////////////////////////////

XnVMultipleHands::IteratorOld::IteratorOld(const XnVMultipleHands::IteratorOld& other) :
	ConstIteratorOld(other)
	{}
XnVMultipleHands::IteratorOld& XnVMultipleHands::IteratorOld::operator++()
{
	++(*(ConstIteratorOld*)this);
	return *this;
}
XnVMultipleHands::IteratorOld XnVMultipleHands::IteratorOld::operator++(int)
{
	IteratorOld result = *this;
	++*this;
	return result;
}
// XnUInt32& XnVMultipleHands::IteratorOld::operator*() const
// {
// 	return (*(ConstIteratorOld*)this);
// }
//	protected:
XnVMultipleHands::IteratorOld::IteratorOld(XnVIntList* pList) :
	ConstIteratorOld(pList)
	{}
XnVMultipleHands::IteratorOld::IteratorOld(XnVIntList* pList, XnUInt32 nPosition) :
	ConstIteratorOld(pList, nPosition)
	{}
