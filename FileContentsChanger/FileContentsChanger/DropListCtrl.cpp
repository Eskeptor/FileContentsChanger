#include "pch.h"
#include "FileContentsChanger.h"
#include "DropListCtrl.h"


IMPLEMENT_DYNAMIC(CDropListCtrl, CListCtrl)

CDropListCtrl::CDropListCtrl()
	: m_hwndParent(nullptr)
	, m_nArrEventID{ 0, }
{

}

CDropListCtrl::~CDropListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDropListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


/**
OnCreate()
*/
int CDropListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Drag Enable
	::DragAcceptFiles(m_hWnd, TRUE);

	return 0;
}


/**
OnDropFiles()
*/
void CDropListCtrl::OnDropFiles(HDROP hDropInfo)
{
	const int BUFFER = 1024;

	if (m_hwndParent != nullptr &&
		m_nArrEventID[eListCtrlEvent::Evt_DragDropResult] > 0)
	{
		UINT nFileNum = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		TCHAR szPath[BUFFER] = { 0, };

		for (int i = 0; i < (int)nFileNum; i++)
		{
			DragQueryFile(hDropInfo, i, szPath, BUFFER - 1);
			CString* pStrFilePath = new CString;
			pStrFilePath->Format(_T("%s"), szPath);
			::SendMessage(m_hwndParent, m_nArrEventID[eListCtrlEvent::Evt_DragDropResult], 0, reinterpret_cast<LPARAM>(pStrFilePath));

			pStrFilePath->Empty();
			delete pStrFilePath;
			pStrFilePath = nullptr;

		}
	}

	CListCtrl::OnDropFiles(hDropInfo);
}


/**
PreSubclassWindow()
*/
void CDropListCtrl::PreSubclassWindow()
{
	// Drag Enable
	::DragAcceptFiles(m_hWnd, TRUE);

	CListCtrl::PreSubclassWindow();
}


/**
Set Parent HWND (m_hwndParent)
@access		public
@param		pParent			Parent's HWND
@return
*/
void CDropListCtrl::SetParentHWND(HWND pParent)
{
	m_hwndParent = pParent;
}


/**
Set Event ID Number
@access		public
@param		eEvt		Event
@param		nID			Event's Set ID
@return
*/
void CDropListCtrl::SetEventID(eListCtrlEvent eEvt, UINT nID)
{
	if (eEvt >= 0 &&
		eEvt < eListCtrlEvent::EventMax)
	{
		m_nArrEventID[eEvt] = nID;
	}
}