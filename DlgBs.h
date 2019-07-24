#if !defined(AFX_DLGBS_H__F8C9E2E9_9E87_4F13_BEA6_2F0A8F0C944D__INCLUDED_)
#define AFX_DLGBS_H__F8C9E2E9_9E87_4F13_BEA6_2F0A8F0C944D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBs dialog
#include "ado.h"
#include "ComboListCtrl.h"
#include "ReportCtrl.h"
class CDlgBs : public CDialog
{
// Construction
public:
	CDlgBs(CWnd* pParent = NULL);   // standard constructor
void SetControlPostion();
//	void LoadFromexcel();
//	void InsertDataToList(int idx, int count);

	CFont m_font;
// Dialog Data
	//{{AFX_DATA(CDlgBs)
	enum { IDD = IDD_DLG_BS_LOAD };

	BOOL	m_bDel;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBs)
	virtual BOOL OnInitDialog();

	//}}AFX_MSG
//		afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBS_H__F8C9E2E9_9E87_4F13_BEA6_2F0A8F0C944D__INCLUDED_)
