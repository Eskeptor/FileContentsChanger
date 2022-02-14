#pragma once
#include <vector>

class CFileContentsChangerDlg : public CDialogEx
{
// Enums and Structure =======================================================================================================================
private:
	enum eListCtrl
	{
		IdxFileList = 0,
		IdxReplaceList,
		ListIdxMax,

		FileColNo = 0,
		FileColFile,
		FileColMax,

		ReplaceColNo = 0,
		ReplaceColFind,
		ReplaceColReplace,
		ReplaceColMax,
	};
	enum eEdit
	{
		IdxFind = 0,
		IdxReplace,
		EditIdxMax
	};

	struct stReplace
	{
		CString strFind;
		CString strReplace;

		stReplace(CString strFind = _T(""), CString strReplace = _T("")) :
			strFind(strFind), strReplace(strReplace)
		{}

		void Clear()
		{
			strFind.Empty();
			strReplace.Empty();
		}
	};

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILECONTENTSCHANGER_DIALOG };
#endif
// ===========================================================================================================================================


// Variables =================================================================================================================================
private:
	CListCtrl m_ctrlList[eListCtrl::ListIdxMax];		// List Controls
	CEdit m_ctrlEdit[eEdit::EditIdxMax];				// Edit Controls

	std::vector<CString> m_vFilePaths;					// File Paths
	std::vector<stReplace> m_vReplace;					// Replace Commands

protected:
	HICON m_hIcon;

// ===========================================================================================================================================


// Functions =================================================================================================================================
private:
	void InitContorls();

	void InitListCtrl(CListCtrl* pCtrl, int nID, const LPCTSTR* arrStrText = nullptr, int nStrSize = 0, bool bRatWidth = false, const int* arrWidthSize = nullptr);
	void InitEditCtrl(CEdit* pCtrl, int nID, CString strText = _T(""));

	void AddFileList(CString strFilePath);
	void AddReplaceList(stReplace stReplaceItem);

	void EnableReplaceCtrl(bool bIsEnable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CFileContentsChangerDlg(CWnd* pParent = nullptr);

	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRepAdd();
	afx_msg void OnBnClickedBtnRepDel();
	afx_msg void OnBnClickedBtnRepStart();
	afx_msg void OnBnClickedBtnRepReset();

// ===========================================================================================================================================

};
