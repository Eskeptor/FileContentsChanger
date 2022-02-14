#include "pch.h"
#include "framework.h"
#include "FileContentsChanger.h"
#include "FileContentsChangerDlg.h"
#include "afxdialogex.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFileContentsChangerDlg::CFileContentsChangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILECONTENTSCHANGER_DIALOG, pParent)
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
	ON_BN_CLICKED(IDC_BTN_LOAD, &CFileContentsChangerDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_DEL, &CFileContentsChangerDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_RESET, &CFileContentsChangerDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_REP_ADD, &CFileContentsChangerDlg::OnBnClickedBtnRepAdd)
	ON_BN_CLICKED(IDC_BTN_REP_DEL, &CFileContentsChangerDlg::OnBnClickedBtnRepDel)
	ON_BN_CLICKED(IDC_BTN_REP_START, &CFileContentsChangerDlg::OnBnClickedBtnRepStart)
	ON_BN_CLICKED(IDC_BTN_REP_RESET, &CFileContentsChangerDlg::OnBnClickedBtnRepReset)
END_MESSAGE_MAP()


BOOL CFileContentsChangerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	InitContorls();

	return TRUE;  
}


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
	// List Control
	{
		const LPCTSTR STR_FILE[] = { _T("No"), _T("File") };
		const int SIZE_FILE[] = { 2, 11 };
		ASSERT(sizeof(STR_FILE) / sizeof(LPCTSTR) == sizeof(SIZE_FILE) / sizeof(int));
		int nSize = sizeof(STR_FILE) / sizeof(LPCTSTR);
		InitListCtrl(&m_ctrlList[eListCtrl::IdxFileList], IDC_LIST_FILE, STR_FILE, nSize, true, SIZE_FILE);

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
		EnableReplaceCtrl(false);
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
void CFileContentsChangerDlg::InitListCtrl(CListCtrl* pCtrl, int nID, const LPCTSTR* arrStrText, int nStrSize, bool bRatWidth, const int* arrWidthSize)
{
	if (pCtrl != nullptr &&
		nID > 0)
	{
		if (pCtrl->m_hWnd == NULL)
			pCtrl->SubclassDlgItem(nID, this);

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
Add File List Item
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

	EnableReplaceCtrl(true);
}


/**
Add Replace List Item
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
}


/**
Replace Control Enabler
@access		private
@param		bIsEnable
@return
*/
void CFileContentsChangerDlg::EnableReplaceCtrl(bool bIsEnable)
{
	BOOL bEnable = bIsEnable ? TRUE : FALSE;

	GetDlgItem(IDC_BTN_REP_ADD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_REP_DEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_REP_RESET)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_REP_START)->EnableWindow(bEnable);
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
	EnableReplaceCtrl(false);
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
	}
}


/**
Replace -> Delete -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnRepDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
Replace -> Start -> Button Click
@param
@return
*/
void CFileContentsChangerDlg::OnBnClickedBtnRepStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
}
