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

XnVMultipleHands::XnVMultipleHands() :
	m_nPrimaryID(0),
	m_nNextAvailable(0),
	m_nAvailable(32)
{
	m_pActiveIDs = XN_NEW(XnVIntList);
	m_pNewIDs = XN_NEW(XnVIntList);
	m_pOldIDs = XN_NEW(XnVIntList);
	Clear();
} // XnVMultipleHands::XnVMultipleHands

XnVMultipleHands::XnVMultipleHands(const XnVMultipleHands& other)
{
	m_pActiveIDs = XN_NEW(XnVIntList);
	m_pNewIDs = XN_NEW(XnVIntList);
	m_pOldIDs = XN_NEW(XnVIntList);
	other.Clone(*this);
} // XnVMultipleHands::XnVMultipleHands

XnVMultipleHands::~XnVMultipleHands()
{
	Clear();

	XN_DELETE(m_pActiveIDs);
	XN_DELETE(m_pNewIDs);
	XN_DELETE(m_pOldIDs);
} // XnVMultipleHands::~XnVMultipleHands

void XnVMultipleHands::ReassignPrimary(XnUInt32 nHint)
{
	// Pick the single interesting point: Use smallest ID
	m_nPrimaryID = 0;

	if (nHint != 0 && m_pActiveIDs->Find(nHint) != m_pActiveIDs->end())
	{
		m_nPrimaryID = nHint;
		return;
	}

	if (ActiveEntries() > 0)
	{
		for (XnVIntList::Iterator iter = m_pActiveIDs->begin(); iter != m_pActiveIDs->end(); ++iter)
		{
			XnUInt32 nID = XnUInt32(*iter);

			if (m_nPrimaryID == 0 || nID < m_nPrimaryID)
			{
				m_nPrimaryID = nID;
			}
		}
	}
} // XnVMultipleHands::ReassignPrimary

XnStatus XnVMultipleHands::Add(const XnVHandPointContext* pContext)
{
	if (m_nAvailable == 0)
	{
		return XN_STATUS_ERROR;
	}

	for (Iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->nID == pContext->nID)
		{
			*(*iter) = *pContext;
			return XN_STATUS_OK;
		}
	}

	XnUInt32 nPosition = m_nNextAvailable;

	m_nAvailable--;
	if (m_nAvailable > 0)
	{
		do
		{
			m_nNextAvailable++;
			if (m_nNextAvailable == 32)
				m_nNextAvailable = 0;
		}
		while (m_Hands[m_nNextAvailable].nID != 0);
	}

	m_Hands[nPosition] = *pContext;

	return XN_STATUS_OK;
} // XnVMultipleHands::Add

XnStatus XnVMultipleHands::Remove(XnUInt32 nID)
{
	XnUInt32 nPosition = 0;
	
	for (Iterator iter = begin(); iter != end(); ++iter, ++nPosition)
	{
		if ((*iter)->nID == nID)
		{
			(*iter)->nID = 0;
			if (m_nAvailable == 0)
			{
				m_nNextAvailable = nPosition;
			}
			m_nAvailable++;
			RemoveActive(nID);
			RemoveNew(nID);
			break;
		}
	}

	return XN_STATUS_OK;
} // XnVMultipleHands::Remove

XnVHandPointContext* XnVMultipleHands::GetContext(XnUInt32 nID)
{
	for (Iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->nID == nID)
		{
			return (*iter);
		}
	}

	return NULL;
} // XnVMultipleHands::GetContext

const XnVHandPointContext* XnVMultipleHands::GetContext(XnUInt32 nID) const
{
	for (ConstIterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->nID == nID)
		{
			return (*iter);
		}
	}

	return NULL;
} // XnVMultipleHands::GetContext

XnVHandPointContext* XnVMultipleHands::GetPrimaryContext()
{
	return GetContext(m_nPrimaryID);
} // XnVMultipleHands::GetPrimaryContext

const XnVHandPointContext* XnVMultipleHands::GetPrimaryContext() const
{
	return GetContext(m_nPrimaryID);
} // XnVMultipleHands::GetPrimaryContext

void AddUnique(XnVIntList& list, XnUInt32 nValue)
{
	if (list.Find(nValue) == list.end())
	{
		list.AddLast(nValue);
	}
}

void XnVMultipleHands::MarkNew(XnUInt32 nID)
{
	AddUnique(*m_pNewIDs, nID);
} // XnVMultipleHands::MarkNew

void XnVMultipleHands::MarkOld(XnUInt32 nID)
{
	AddUnique(*m_pOldIDs, nID);
} // XnVMultipleHands::MarkOld

void XnVMultipleHands::MarkActive(XnUInt32 nID)
{
	AddUnique(*m_pActiveIDs, nID);
	if (ActiveEntries() == 1)
	{
		m_nPrimaryID = nID;
	}
} // XnVMultipleHands::MarkActive

void XnVMultipleHands::RemoveNew(XnUInt32 nID)
{
	m_pNewIDs->Remove(m_pNewIDs->Find(nID));
} // XnVMultipleHands::RemoveNew

void XnVMultipleHands::RemoveOld(XnUInt32 nID)
{
	m_pOldIDs->Remove(m_pOldIDs->Find(nID));
} // XnVMultipleHands::RemoveOld

void XnVMultipleHands::RemoveActive(XnUInt32 nID)
{
	m_pActiveIDs->Remove(m_pActiveIDs->Find(nID));
	if (nID == m_nPrimaryID)
	{
		ReassignPrimary();
	}
} // XnVMultipleHands::RemoveActive

XnUInt32 XnVMultipleHands::ActiveEntries() const
{
	return m_pActiveIDs->Size();
} // XnVMultipleHands::ActiveEntries

XnUInt32 XnVMultipleHands::OldEntries() const
{
	return m_pOldIDs->Size();
} // XnVMultipleHands::OldEntries

XnUInt32 XnVMultipleHands::NewEntries() const
{
	return m_pNewIDs->Size();
} // XnVMultipleHands::NewEntries

void XnVMultipleHands::SetFocusPoint(const XnPoint3D& ptFocusPoint)
{
	m_ptFocusPoint = ptFocusPoint;
} // XnVMultipleHands::SetFocusPoint

const XnPoint3D& XnVMultipleHands::GetFocusPoint() const
{
	return m_ptFocusPoint;
} // XnVMultipleHands::GetFocusPoint

void XnVMultipleHands::Clone(XnVMultipleHands& other) const
{
	other.Clear();
	for (ConstIterator iter = begin(); iter != end(); ++iter)
	{
		other.Add(*iter);
		other.MarkActive((*iter)->nID);

		if (iter.IsNew())
		{
			other.MarkNew((*iter)->nID);
		}
	}

	for (ConstIteratorOld oter = beginOld(); oter != endOld(); ++oter)
	{
		other.MarkOld(*oter);
	}

	other.m_nPrimaryID = m_nPrimaryID;
	other.SetFocusPoint(GetFocusPoint());
} // XnVMultipleHands::Clone

void XnVMultipleHands::Clear()
{
	ClearLists();

	m_nNextAvailable = 0;
	m_nAvailable = 32;

	for (int i = 0; i < 32; i++)
	{
		m_Hands[i].nID = 0;
	}
} // XnVMultipleHands::Clear

void XnVMultipleHands::ClearLists()
{
	ClearNewList();
	ClearOldList();
	m_pActiveIDs->Clear();
	m_nPrimaryID = 0;
} // XnVMultipleHands::ClearLists

void XnVMultipleHands::ClearNewList()
{
	m_pNewIDs->Clear();
} // XnVMultipleHands::ClearNewList

void XnVMultipleHands::ClearOldList()
{
	m_pOldIDs->Clear();
} // XnVMultipleHands::ClearOldList

XnVMultipleHands::Iterator XnVMultipleHands::begin()
{
	return Iterator(this, 0);
} // XnVMultipleHands::begin

XnVMultipleHands::ConstIterator XnVMultipleHands::begin() const
{
	return ConstIterator(this, 0);
} // XnVMultipleHands::begin

XnVMultipleHands::Iterator XnVMultipleHands::end()
{
	return Iterator(this, 32);
} // XnVMultipleHands::end
XnVMultipleHands::ConstIterator XnVMultipleHands::end() const
{
	return ConstIterator(this, 32);
} // XnVMultipleHands::end

XnVMultipleHands::Iterator XnVMultipleHands::Find(XnUInt32 nID)
{
	Iterator iter = begin();
	for (; iter != end(); ++iter)
	{
		if ((*iter)->nID == nID)
		{
			return iter;
		}
	}
	return end();
} // XnVMultipleHands::Find
XnVMultipleHands::ConstIterator XnVMultipleHands::Find(XnUInt32 nID) const
{
	ConstIterator iter = begin();
	for (; iter != end(); ++iter)
		if ((*iter)->nID == nID)
			return iter;
	return end();
} // XnVMultipleHands::Find

XnBool XnVMultipleHands::IsNew(XnUInt32 nID) const
{
	return m_pNewIDs->Find(nID) != m_pNewIDs->end();
} // XnVMultipleHands::IsNew

XnBool XnVMultipleHands::IsOld(XnUInt32 nID) const
{
	return m_pOldIDs->Find(nID) != m_pOldIDs->end();
} // XnVMultipleHands::IsOld

XnBool XnVMultipleHands::IsActive(XnUInt32 nID) const
{
	return m_pActiveIDs->Find(nID) != m_pActiveIDs->end();
} // XnVMultipleHands::IsActive

XnVMultipleHands::IteratorOld XnVMultipleHands::beginOld()
{
	return IteratorOld(m_pOldIDs, 0);
} // XnVMultipleHands::beginOld
XnVMultipleHands::ConstIteratorOld XnVMultipleHands::beginOld() const
{
	return ConstIteratorOld(m_pOldIDs, 0);
} // XnVMultipleHands::beginOld
XnVMultipleHands::IteratorOld XnVMultipleHands::endOld()
{
	return IteratorOld(m_pOldIDs, -1);
} // XnVMultipleHands::endOld
XnVMultipleHands::ConstIteratorOld XnVMultipleHands::endOld() const
{
	return ConstIteratorOld(m_pOldIDs, -1);
} // XnVMultipleHands::endOld
