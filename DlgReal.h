#if !defined(AFX_DLGREAL_H__68511DBD_BA72_47BF_981C_543F2C5CED55__INCLUDED_)
#define AFX_DLGREAL_H__68511DBD_BA72_47BF_981C_543F2C5CED55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgReal dialog
#include "ReportCtrl.h"
class CDlgReal : public CDialog
{
// Construction
public:
	CDlgReal(CWnd* pParent = NULL);   // standard constructor
	CFont m_font;
	CString m_strMark;
	int m_nSelectItem;
	int m_nLockItem;
	int m_nLockSN;

//	void ShowReal(TRealtimeData* prd);
	void SetControlPostion();
// Dialog Data
	//{{AFX_DATA(CDlgReal)
	enum { IDD = IDD_DLG_REALTIME };
	CReportCtrl	m_listData;
	CReportCtrl	m_listModule;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgReal)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLockModule();
	afx_msg void OnReleaseLock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREAL_H__68511DBD_BA72_47BF_981C_543F2C5CED55__INCLUDED_)
