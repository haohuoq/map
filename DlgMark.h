#if !defined(AFX_DLGMARK_H__5070AA18_D66D_4439_A85E_1A90B5801488__INCLUDED_)
#define AFX_DLGMARK_H__5070AA18_D66D_4439_A85E_1A90B5801488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMark.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMark dialog

class CDlgMark : public CDialog
{
// Construction
public:
	CDlgMark(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMark)
	enum { IDD = IDD_DLG_MARK };
	CString	m_strMark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMark)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMark)
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMARK_H__5070AA18_D66D_4439_A85E_1A90B5801488__INCLUDED_)
