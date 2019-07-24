#if !defined(AFX_DLGLOAD_H__4E6C3973_CBC9_4DC7_AFA3_1A4A437C42B6__INCLUDED_)
#define AFX_DLGLOAD_H__4E6C3973_CBC9_4DC7_AFA3_1A4A437C42B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLoad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLoad dialog
#include "ado.h"
#include "reportctrl.h"
class CDlgLoad : public CDialog
{
// Construction
public:
	CDlgLoad(CWnd* pParent = NULL);   // standard constructor
	void LoadDB();
	void SetControlPostion();
int m_nDelItem;
	
CFont m_font;
// Dialog Data
	//{{AFX_DATA(CDlgLoad)
	enum { IDD = IDD_DLG_LOAD };
	CButton	m_btnTime;
	CStatic	m_infost;
	CButton	m_btnDown;
	CReportCtrl	m_list1;
	CStatic	m_stLink;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoad)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton9();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRecDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOAD_H__4E6C3973_CBC9_4DC7_AFA3_1A4A437C42B6__INCLUDED_)
