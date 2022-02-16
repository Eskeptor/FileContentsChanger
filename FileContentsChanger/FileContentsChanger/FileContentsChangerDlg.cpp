#include "pch.h"
#include "framework.h"
#include "FileContentsChanger.h"
#include "FileContentsChangerDlg.h"
#include "afxdialogex.h"
#include <algorithm>

#include "include/CppUtil/CppUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFileContentsChangerDlg::CFileContentsChangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILECONTENTSCHANGER_DIALOG, pParent)
	, m_thrReplace(nullptr), m_bIsExit(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileContentsChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileContentsChangerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(eEventID::Evt_FileAdd, &CFileContentsChangerDlg::EventFileAdd)
	ON_MESSAGE(eEventID::Evt_ReplaceEnd, &CFileContentsChangerDlg::EventReplaceEnd)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CFileContentsChangerDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_DEL, &CFileContentsChangerDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_RESET, &CFileContentsChangerDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_REP_ADD, &CFileContentsChangerDlg::OnBnClickedBtnRepAdd)
	ON_BN_CLICKED(IDC_BTN_REP_DEL, &CFileContentsChangerDlg::OnBnClickedBtnRepDel)
	ON_BN_CLICKED(IDC_BTN_REP_START, &CFileContentsChangerDlg::OnBnClickedBtnRepStart)
	ON_BN_CLICKED(IDC_BTN_REP_RESET, &CFileContentsChangerDlg::OnBnClickedBtnRepReset)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILE, &CFileContentsChangerDlg::OnNMClickListFile)
	ON_NOTIFY(NM_CLICK, IDC_LIST_REPLACE, &CFileContentsChangerDlg::OnNMClickListReplace)
END_MESSAGE_MAP()


/**
OnInitDialog()
*/
BOOL CFileContentsChangerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	InitContorls();

	return TRUE;  
}


/**
OnPaint()
*/
void CFileContentsChangerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


/**
OnQueryDragIcon()
*/
HCURSOR CFileContentsChangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/**
MFC Control Initialize
@access		private
@param		
@return
*/
void CFileContentsChangerDlg::InitContorls()
{
	// Dialog
	{
		// Title
		SetWindowText(_T("File Contents Changer 1.0.0 (by Eskeptor)"));
	}

	// List Control
	{
		const LPCTSTR STR_FILE[] = { _T("No"), _T("File") };
		const int SIZE_FILE[] = { 2, 11 };
		ASSERT(sizeof(STR_FILE) / sizeof(LPCTSTR) == sizeof(SIZE_FILE) / sizeof(int));
		int nSize = sizeof(STR_FILE) / sizeof(LPCTSTR);
		InitListCtrl(&m_ctrlList[eListCtrl::IdxFileList], IDC_LIST_FILE, STR_FILE, nSize, true, SIZE_FILE, true);

		const LPCTSTR STR_REPLACE[] = { _T("No"), _T("Find"), _T("Replace")};
		const int SIZE_REPLACE[] = { 2, 7, 7 };
		ASSERT(sizeof(STR_REPLACE) / sizeof(LPCTSTR) == sizeof(SIZE_REPLACE) / sizeof(int));
		nSize = sizeof(STR_REPLACE) / sizeof(LPCTSTR);
		InitListCtrl(&m_ctrlList[eListCtrl::IdxReplaceList], IDC_LIST_REPLACE, STR_REPLACE, nSize, true, SIZE_REPLACE);
	}

	// Edit Control
	{
		InitEditCtrl(&m_ctrlEdit[eEdit::IdxFind], IDC_EDIT_REP_FIND);
		InitEditCtrl(&m_ctrlEdit[eEdit::IdxReplace], IDC_EDIT_REP_REPLACE);
	}

	// Button Control
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_REP_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REP_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REP_START)->EnableWindow(FALSE);
	}
}


/**
MFC List Control Initialize
@access		private
@param		pCtrl			ListControl
@param		nID				ListControl ID
@param		arrStrText		ListControl's Columns Text
@param		nStrSize		ListControl's Columns Size
@param		bRatWidth		Is arrWidthSize Ratio (true : Ratio, false : Width)
@param		arrWidthSize	ListControl's Columns Width Size
@return
*/
void CFileContentsChangerDlg::InitListCtrl(CDropListCtrl* pCtrl, int nID, const LPCTSTR* arrStrText, int nStrSize, bool bRatWidth, const int* arrWidthSize, bool bIsOnFileAddEvent)
{
	if (pCtrl != nullptr &&
		nID > 0)
	{
		if (pCtrl->m_hWnd == NULL)
		{
			pCtrl->SubclassDlgItem(nID, this);
			DWORD dwStyle = pCtrl->GetExtendedStyle();
			pCtrl->SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			if (bIsOnFileAddEvent)
			{
				pCtrl->SetParentHWND(m_hWnd);
				pCtrl->SetEventID(CDropListCtrl::eListCtrlEvent::Evt_DragDropResult, eEventID::Evt_FileAdd);
			}
		}

		if (nStrSize > 0 &&
			arrStrText != nullptr)
		{
			const int GAP = 4;

			int nRatWidth = 0;
			int nRatTotal = 0;
			int nWidth = 0;

			if (bRatWidth)
			{
				CRect rcCtrl;
				pCtrl->GetWindowRect(rcCtrl);
				ScreenToClient(rcCtrl);
				nWidth = rcCtrl.Width();

				std::for_each(arrWidthSize, arrWidthSize + nStrSize, [&nRatTotal](int nSize) { nRatTotal += nSize; });
				if (nRatTotal != 0)
					nRatWidth = nWidth / nRatTotal;
			}

			for (int i = 0; i < nStrSize; i++)
			{
				pCtrl->InsertColumn(i, arrStrText[i], LVCFMT_LEFT, arrWidthSize != nullptr ? arrWidthSize[i] : -1);

				if (arrWidthSize != nullptr)
				{
					if (bRatWidth)
					{
						if (i != nStrSize - 1)
							pCtrl->SetColumnWidth(i, nRatWidth * arrWidthSize[i]);
						else
							pCtrl->SetColumnWidth(i, nWidth - (nRatWidth * (nRatTotal - arrWidthSize[i])) - GAP);
					}
					else
					{
						pCtrl->SetColumnWidth(i, arrWidthSize[i]);
					}
				}
			}
		}
	}
}


/**
Edit Control Initialize
@access		private
@param		pCtrl		Edit Control
@param		nID			Edit Control ID
@return
*/
void CFileContentsChangerDlg::InitEditCtrl(CEdit* pCtrl, int nID, CString strText)
{
	if (pCtrl != nullptr &&
		nID > 0)
	{
		if (pCtrl->m_hWnd == NULL)
			pCtrl->SubclassDlgItem(nID, this);

		if (strText.IsEmpty() == false)
			pCtrl->SetWindowText(strText);
	}
}


/**
Add File List Item (and Push m_vFilePaths)
@access		private
@param		strFilePath
@return
*/
void CFileContentsChangerDlg::AddFileList(CString strFilePath)
{
	int nCurIdx = m_ctrlList[eListCtrl::IdxFileList].GetItemCount();

	CString strIdx = _T("");
	strIdx.Format(_T("%d"), nCurIdx);
	m_ctrlList[eListCtrl::IdxFileList].InsertItem(nCurIdx, strIdx);
	m_ctrlList[eListCtrl::IdxFileList].SetItemText(nCurIdx, eListCtrl::FileColFile, strFilePath);

	m_vFilePaths.push_back(strFilePath);

	// Button
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESET)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_REP_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REP_DEL)->EnableWindow(FALSE);

		if (m_vReplace.empty())
		{
			GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_REP_START)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_REP_START)->EnableWindow(TRUE);
		}
	}
}


/**
Add File List Item (Just Add to ListCtrl)
@access		private
@param		vFileList
@return
*/
void CFileContentsChangerDlg::AddFileList(const std::vector<CString>& vFileList)
{
	int nCurIdx = m_ctrlList[eListCtrl::IdxFileList].GetItemCount();
	CString strIdx = _T("");
	int nSize = (int)vFileList.size();

	for (int i = 0; i < nSize; i++)
	{
		strIdx.Format(_T("%d"), nCurIdx + i);
		m_ctrlList[eListCtrl::IdxFileList].InsertItem(nCurIdx, strIdx);
		m_ctrlList[eListCtrl::IdxFileList].SetItemText(nCurIdx, eListCtrl::FileColFile, vFileList.at(i));
	}

	// Button
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESET)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_REP_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REP_DEL)->EnableWindow(FALSE);

		if (m_vReplace.empty())
		{
			GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_REP_START)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_REP_START)->EnableWindow(TRUE);
		}
	}
}


/**
Add Replace List Item (and Push m_vReplace)
@access		private
@param		stReplaceItem
@return
*/
void CFileContentsChangerDlg::AddReplaceList(stReplace stReplaceItem)
{
	int nCurIdx = m_ctrlList[eListCtrl::IdxReplaceList].GetItemCount();

	CString strIdx = _T("");
	strIdx.Format(_T("%d"), nCurIdx + 1);
	m_ctrlList[eListCtrl::IdxReplaceList].InsertItem(nCurIdx, strIdx);
	m_ctrlList[eListCtrl::IdxReplaceList].SetItemText(nCurIdx, eListCtrl::ReplaceColFind, stReplaceItem.strFind);
	m_ctrlList[eListCtrl::IdxReplaceList].SetItemText(nCurIdx, eListCtrl::ReplaceColReplace, stReplaceItem.strReplace);

	m_vReplace.push_back(stReplaceItem);

	// Button
	{
		GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REP_START)->EnableWindow(TRUE);
	}
}


/**
Add Replace List Item (Just Add to ListCtrl)
@access		private
@param		vReplaceItems
@return
*/
void CFileContentsChangerDlg::AddReplaceList(const std::vector<stReplace>& vReplaceItems)
{
	int nCurIdx = m_ctrlList[eListCtrl::IdxReplaceList].GetItemCount();
	CString strIdx = _T("");
	int nSize = (int)vReplaceItems.size();

	for (int i = 0; i < nSize; i++)
	{
		strIdx.Format(_T("%d"), nCurIdx + i);
		m_ctrlList[eListCtrl::IdxReplaceList].InsertItem(nCurIdx, strIdx);
		m_ctrlList[eListCtrl::IdxReplaceList].SetItemText(nCurIdx, eListCtrl::ReplaceColFind, vReplaceItems.at(i).strFind);
		m_ctrlList[eListCtrl::IdxReplaceList].SetItemText(nCurIdx, eListCtrl::ReplaceColReplace, vReplaceItems.at(i).strReplace);
	}

	// Button
	{
		GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REP_START)->EnableWindow(TRUE);
	}
}


/**
Refresh File List
@access		private
@param
@return
*/
void CFileContentsChangerDlg::RefreshFileList()
{
	m_ctrlList[eListCtrl::IdxFileList].DeleteAllItems();
	
	if (m_vFilePaths.empty() == false)
	{
		AddFileList(m_vFilePaths);
	}
}


/**
Refresh Replace List
@access		private
@param
@return
*/
void CFileContentsChangerDlg::RefreshReplaceList()
{
	m_ctrlList[eListCtrl::IdxReplaceList].DeleteAllItems();

	if (m_vReplace.empty() == false)
	{
		AddReplaceList(m_vReplace);
	}
}


/**
Reset Replace Thread (nullptr)
@access		public
@param
@return
*/
void CFileContentsChangerDlg::ResetReplaceThread()
{
	m_thrReplace = nullptr;
}


/**
Get File List Iterator Begin
@access		public
@param
@return		m_vFilePaths.begin()
*/
std::vector<CString>::iterator CFileContentsChangerDlg::GetFileListIterBegin()
{
	return m_vFilePaths.begin();
}


/**
Get File List Iterator End
@access		public
@param
@return		m_vFilePaths.end()
*/
std::vector<CString>::iterator CFileContentsChangerDlg::GetFileListIterEnd()
{
	return m_vFilePaths.end();
}


/**
Get Replace List Iterator Begin
@access		public
@param
@return		m_vReplace.begin();
*/
std::vector<CFileContentsChangerDlg::stReplace>::iterator CFileContentsChangerDlg::GetReplaceListIterBegin()
{
	return m_vReplace.begin();
}

/**
Get Replace List Iterator End
@access		public
@param
@return		m_vReplace.end();
*/
std::vector<CFileContentsChangerDlg::stReplace>::iterator CFileContentsChangerDlg::GetReplaceListIterEnd()
{
	return m_vReplace.end();
}


/**
Get Exit Flag
@access		public
@param
@return		m_bIsExit
*/
bool CFileContentsChangerDlg::GetExitFlag() const
{
	return m_bIsExit;
}


/**
Files -> ... -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnLoad()
{
	const int MAX_FILE = 400;
	const int MAX_BUFFER = (MAX_FILE * (MAX_PATH + 1)) + 1;
	CString strFilePathBuffer = _T("");
	const TCHAR FILTER[] = _T("모든 파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, FILTER);
	dlg.GetOFN().lpstrFile = strFilePathBuffer.GetBuffer(MAX_BUFFER);

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		CString strPath = _T("");

		while (pos != nullptr)
		{
			strPath = dlg.GetNextPathName(pos);
			AddFileList(strPath);
		}
	}

	strFilePathBuffer.ReleaseBuffer();
}


/**
Files -> Delete -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnDel()
{
	int nCurSelCnt = m_ctrlList[eListCtrl::IdxFileList].GetSelectedCount();

	if (nCurSelCnt > 0)
	{
		std::vector<int> vSelectedIdx;
		POSITION posItem = m_ctrlList[eListCtrl::IdxFileList].GetFirstSelectedItemPosition();
		int nCurSel = 0;

		while (posItem != nullptr)
		{
			nCurSel = m_ctrlList[eListCtrl::IdxFileList].GetNextSelectedItem(posItem);
			vSelectedIdx.push_back(nCurSel);
		}

		auto iter = vSelectedIdx.rbegin();
		while (iter != vSelectedIdx.rend())
		{
			m_vFilePaths.erase(m_vFilePaths.begin() + (*iter));

			iter++;
		}

		RefreshFileList();
	}
}


/**
Files -> Reset -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnReset()
{
	if (m_vFilePaths.empty() == false)
		m_vFilePaths.clear();

	m_ctrlList[eListCtrl::IdxFileList].DeleteAllItems();

	// Button
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);
	}
}


/**
Replace -> Add -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnRepAdd()
{
	stReplace stReplaceItem;
	m_ctrlEdit[eEdit::IdxFind].GetWindowText(stReplaceItem.strFind);
	m_ctrlEdit[eEdit::IdxReplace].GetWindowText(stReplaceItem.strReplace);

	if (stReplaceItem.strFind.IsEmpty())
	{
		MessageBox(_T("Please input find text."), _T("Warning"));
	}
	else
	{
		AddReplaceList(stReplaceItem);

		GetDlgItem(IDC_BTN_REP_START)->EnableWindow(TRUE);
	}
}


/**
Replace -> Delete -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnRepDel()
{
	int nCurSelCnt = m_ctrlList[eListCtrl::IdxReplaceList].GetSelectedCount();

	if (nCurSelCnt > 0)
	{
		std::vector<int> vSelectedIdx;
		POSITION posItem = m_ctrlList[eListCtrl::IdxReplaceList].GetFirstSelectedItemPosition();
		int nCurSel = 0;

		while (posItem != nullptr)
		{
			nCurSel = m_ctrlList[eListCtrl::IdxReplaceList].GetNextSelectedItem(posItem);
			vSelectedIdx.push_back(nCurSel);
		}

		auto iter = vSelectedIdx.rbegin();
		while (iter != vSelectedIdx.rend())
		{
			m_vReplace.erase(m_vReplace.begin() + (*iter));

			iter++;
		}

		RefreshReplaceList();
	}
}


/**
Replace -> Start -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnRepStart()
{
	if (m_vReplace.empty())
	{
		MessageBox(_T("Please Add Replace Item"), _T("Warning"));
		return;
	}

	if (m_thrReplace == nullptr)
	{
		m_bIsExit = false;
		m_thrReplace = AfxBeginThread(ReplaceThread, (LPVOID)this);
	}
}


/**
Replace -> Reset -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnRepReset()
{
	if (m_vReplace.empty() == false)
		m_vReplace.clear();

	m_ctrlList[eListCtrl::IdxReplaceList].DeleteAllItems();

	// Button
	{
		GetDlgItem(IDC_BTN_REP_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REP_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REP_START)->EnableWindow(FALSE);
	}
}


UINT CFileContentsChangerDlg::ReplaceThread(LPVOID pParams)
{
	CFileContentsChangerDlg* pMainDlg = (CFileContentsChangerDlg*)pParams;

	if (pMainDlg != nullptr)
	{
		auto iterFile = pMainDlg->GetFileListIterBegin();
		auto iterFileEnd = pMainDlg->GetFileListIterEnd();
		CString strContents = _T("");
		CFileContentsChangerDlg::stReplace stReplaceItem;
		int nCurEncode = 0;
		int nCompleteCount = 0;
		int nFileCount = 0;

		while (iterFile != iterFileEnd &&
			   pMainDlg->GetExitFlag() == false)
		{
			strContents = CppUtil::ReadFileAll(*iterFile, nCurEncode);
			if (strContents.IsEmpty() == false)
			{
				auto iterReplace = pMainDlg->GetReplaceListIterBegin();
				auto iterReplaceEnd = pMainDlg->GetReplaceListIterEnd();

				while (iterReplace != iterReplaceEnd &&
						pMainDlg->GetExitFlag() == false)
				{
					strContents.Replace((*iterReplace).strFind, (*iterReplace).strReplace);

					iterReplace++;
				}

				// TODO File Save (Encoding)
				if (CppUtil::WriteFile(*iterFile, strContents, nCurEncode))
				{
					nCompleteCount++;
					TRACE(_T("Save Complete : %s\n"), *iterFile);
				}
				else 
				{
					TRACE(_T("Save Fail : %s\n"), *iterFile);
				}
			}

			nFileCount++;
			strContents.Empty();
			iterFile++;
		}

		if (nFileCount > 0)
		{
			::PostMessage(pMainDlg->m_hWnd, CFileContentsChangerDlg::eEventID::Evt_ReplaceEnd, nCompleteCount, nFileCount - nCompleteCount);
		}

		strContents.Empty();
		pMainDlg->ResetReplaceThread();
	}

	return 0;
}


/**
OnClose()
*/
void CFileContentsChangerDlg::OnClose()
{
	m_bIsExit = true;

	if (m_thrReplace != nullptr)
	{
		while (m_thrReplace != nullptr)
			Sleep(1);
	}

	CDialogEx::OnClose();
}


/**
File Add Event
@Event		eEventID::Evt_FileAdd
@param		lParam			File Path (CString*)
@return		1L
*/
LRESULT CFileContentsChangerDlg::EventFileAdd(WPARAM, LPARAM lParam)
{
	// CString point delete from CDropListCtrl->OnDropFiles after EventFileAdd() End
	CString* pStrFilePath = reinterpret_cast<CString*>(lParam);

	if (pStrFilePath != nullptr)
	{
		CString strFilePath = _T("");
		strFilePath.Format(_T("%s"), (*pStrFilePath).GetString());	

		AddFileList(strFilePath);
	}

	return 1L;
}


/**
Replace End Event
@Event		eEventID::Evt_ReplaceEnd
@param		wParam			Successed File Count
@param		lParam			Failed File Count
@return		1L
*/
LRESULT CFileContentsChangerDlg::EventReplaceEnd(WPARAM wParam, LPARAM lParam)
{
	int nSuccessFileCnt = (int)wParam;
	int nFailFileCnt = (int)lParam;
	int nTotalFileCnt = nSuccessFileCnt + nFailFileCnt;

	if (nSuccessFileCnt == nTotalFileCnt)
	{
		MessageBox(_T("File Contents Replace All Complete."), _T("Complete"));
	}
	else
	{
		CString strText = _T("");
		strText.Format(_T("File Contents Replace End.\n Success : %d\n Fail : %d"), nSuccessFileCnt, nFailFileCnt);
		MessageBox(strText, _T("End"));
	}

	return 1L;
}


/**
Files -> File List -> Item Click Event
*/
void CFileContentsChangerDlg::OnNMClickListFile(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelCount = m_ctrlList[eListCtrl::IdxFileList].GetSelectedCount();

	if (nSelCount > 0)
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
	}

	*pResult = 0;
}


/**
Replace -> Replace List -> Item Click Event
*/
void CFileContentsChangerDlg::OnNMClickListReplace(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelCount = m_ctrlList[eListCtrl::IdxReplaceList].GetSelectedCount();

	if (nSelCount > 0)
	{
		GetDlgItem(IDC_BTN_REP_DEL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_REP_DEL)->EnableWindow(FALSE);
	}

	*pResult = 0;
}
