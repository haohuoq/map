#if !defined(AFX_DLGRST_H__94ECD671_6767_4CC5_8ECA_F92993AC1687__INCLUDED_)
#define AFX_DLGRST_H__94ECD671_6767_4CC5_8ECA_F92993AC1687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRst.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRst dialog
#include "ReportCtrl.h"
class CDlgRst : public CDialog
{
// Construction
public:
	CDlgRst(CWnd* pParent = NULL);   // standard constructor
void SetControlPostion();
void LoadToList(CString s);

int IsCidInArr(CString s);
int AddCidToArr(CString s);

bool ExportToExcel(CString filename);
bool ExportToTxt(CString filename);
int m_nOldIndex;
int m_nDelItem;
CStringArray m_arrCID[7];
int m_nArrAdd;
// Dialog Data
	//{{AFX_DATA(CDlgRst)
	enum { IDD = IDD_DLG_RST };
	CReportCtrl	m_list;
	int		m_nIdx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRst)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRst)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	afx_msg void OnRadio9();
	afx_msg void OnRadio10();
	afx_msg void OnRadio12();
	afx_msg void OnButton1();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuDelAll();
	afx_msg void OnMenuDelCur();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		CBrush m_brs;
	CBrush m_brs2;
	CBrush m_brs3;
	CBrush m_brs4;
	CBrush m_brs5;
	CBrush m_brs6;
	CBrush m_brs7;
	CBrush m_brs8;
	CBrush m_brs9;
	CFont m_font;	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRST_H__94ECD671_6767_4CC5_8ECA_F92993AC1687__INCLUDED_)
