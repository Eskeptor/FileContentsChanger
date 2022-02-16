#pragma once


class CDropListCtrl : public CListCtrl
{
// Enums and Structure =======================================================================================================================
public:
	enum eListCtrlEvent
	{
		Evt_DragDropResult = 0,
		EventMax
	};
// ===========================================================================================================================================


// Variables =================================================================================================================================
private:
	HWND m_hwndParent;

	UINT m_nArrEventID[eListCtrlEvent::EventMax];

public:
	DECLARE_DYNAMIC(CDropListCtrl)

// ===========================================================================================================================================


// Functions =================================================================================================================================
protected:
	DECLARE_MESSAGE_MAP()

public:
	CDropListCtrl();
	virtual ~CDropListCtrl();

	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();

	void SetParentHWND(HWND pParent);
	void SetEventID(eListCtrlEvent eEvt, UINT nID);

// ===========================================================================================================================================
	
};


