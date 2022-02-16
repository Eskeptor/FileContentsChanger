#pragma once
#include <vector>
#include "DropListCtrl.h"

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
	enum eEventID
	{
		Evt_FileAdd = WM_USER + 1000,
		Evt_ReplaceEnd,
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
	CDropListCtrl m_ctrlList[eListCtrl::ListIdxMax];		// List Controls
	CEdit m_ctrlEdit[eEdit::EditIdxMax];				// Edit Controls

	std::vector<CString> m_vFilePaths;					// File Paths
	std::vector<stReplace> m_vReplace;					// Replace Commands

	CWinThread* m_thrReplace;							// Replace Thread
	bool m_bIsExit;										// Exit Flag

protected:
	HICON m_hIcon;

// ===========================================================================================================================================


// Functions =================================================================================================================================
private:
	// MFC Control Initialize
	void InitContorls();

	// MFC List Control Initialize
	void InitListCtrl(CDropListCtrl* pCtrl, int nID, const LPCTSTR* arrStrText = nullptr, int nStrSize = 0, bool bRatWidth = false, const int* arrWidthSize = nullptr, bool bIsOnFileAddEvent = false);
	// Edit Control Initialize
	void InitEditCtrl(CEdit* pCtrl, int nID, CString strText = _T(""));

	// Add File List Item
	void AddFileList(CString strFilePath);
	void AddFileList(const std::vector<CString>& vFileList);
	// Add Replace List Item
	void AddReplaceList(stReplace stReplaceItem);
	void AddReplaceList(const std::vector<stReplace>& vReplaceItems);

	// Refresh File List
	void RefreshFileList();
	// Refresh Replace List
	void RefreshReplaceList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// File Add Event
	afx_msg LRESULT EventFileAdd(WPARAM, LPARAM lParam);
	// Replace End Event
	afx_msg LRESULT EventReplaceEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	CFileContentsChangerDlg(CWnd* pParent = nullptr);

	// Replace Thread Function
	static UINT ReplaceThread(LPVOID pParams);

	// Reset Replace Thread (nullptr)
	void ResetReplaceThread();
	// Get File List Iterator Begin
	std::vector<CString>::iterator GetFileListIterBegin();
	// Get File List Iterator End
	std::vector<CString>::iterator GetFileListIterEnd();
	// Get Replace List Iterator Begin
	std::vector<stReplace>::iterator GetReplaceListIterBegin();
	// Get Replace List Iterator End
	std::vector<stReplace>::iterator GetReplaceListIterEnd();
	// Get Exit Flag
	bool GetExitFlag() const;

	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRepAdd();
	afx_msg void OnBnClickedBtnRepDel();
	afx_msg void OnBnClickedBtnRepStart();
	afx_msg void OnBnClickedBtnRepReset();
	afx_msg void OnClose();
	afx_msg void OnNMClickListFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListReplace(NMHDR* pNMHDR, LRESULT* pResult);

// ===========================================================================================================================================

};
