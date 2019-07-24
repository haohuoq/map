#if !defined(AFX_DLGEDIT_H__4E45C446_FE10_4268_B8CA_39F30DFA4FBE__INCLUDED_)
#define AFX_DLGEDIT_H__4E45C446_FE10_4268_B8CA_39F30DFA4FBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEdit dialog

class CDlgEdit : public CDialog
{
// Construction
public:
	CDlgEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEdit)
	enum { IDD = IDD_DLG_EDIT };
	CStatic	m_stLng;
	CStatic	m_stLat;
	CString	m_strMark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEdit)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDIT_H__4E45C446_FE10_4268_B8CA_39F30DFA4FBE__INCLUDED_)
