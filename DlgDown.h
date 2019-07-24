#if !defined(AFX_DLGDOWN_H__10D5E830_7B30_4D99_BE8A_3E7B7EBC5EB0__INCLUDED_)
#define AFX_DLGDOWN_H__10D5E830_7B30_4D99_BE8A_3E7B7EBC5EB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDown.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgDown dialog

class DlgDown : public CDialog
{
// Construction
public:
	DlgDown(CWnd* pParent = NULL);   // standard constructor
	FILE* pfTmp;
	CFont m_font;
	void DispRecvedMsg(int sock_id, char* byBuf);
	void SendDownCmd(int sock_id);
	void SendCmdData(int sock_id, char* buf, int len);
	void OpenUnsaveData();
	bool ExportToExcel(CString filename);
	bool ExportToTxt(CString filename);
	void LoadFromExcel() ;

	CString m_strFile;
// Dialog Data
	//{{AFX_DATA(DlgDown)
	enum { IDD = IDD_DLG_DOWN };
	CButton	m_btnTime;
	CButton	m_btnDown;
	CListBox	m_list;
	int		m_nSel;
	BOOL	m_bChk;
	CString	m_strName2;
	CString	m_strName1;
	CString	m_strName3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgDown)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgDown)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnButton2();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton18();
	afx_msg void OnButton17();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDOWN_H__10D5E830_7B30_4D99_BE8A_3E7B7EBC5EB0__INCLUDED_)
