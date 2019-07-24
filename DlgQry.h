#if !defined(AFX_DLGQRY_H__CE216162_F131_4175_80FA_038369445471__INCLUDED_)
#define AFX_DLGQRY_H__CE216162_F131_4175_80FA_038369445471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgQry.h : header file
//
#include "ReportCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgQry dialog
class Point
{
public:
	double lat;// Î³¶È
    double lng;// ¾­¶È
	
    Point() {
    }
	
    Point(double lng1, double lat1) {
        lng = lng1;
        lat = lat1;
    }
	
    
    double getLat() {
        return lat;
    }
    void setLat(double lat1) {
        lat = lat1;
    }
    double getLng() {
        return lng;
    }
    void setLng(double lng1) {
        lng = lng1;
    }
	
    
	
};



class CDlgQry : public CDialog
{
// Construction
public:
	CDlgQry(CWnd* pParent = NULL);   // standard constructor
	void SaveQryRecord();
	void LoadQryRecord();
	void InsertToList(int m, int n, CString strContent);
	CFont m_font;

	int m_nDelItem;
	int m_nSelItem;
	int m_nQid;


// Dialog Data
	//{{AFX_DATA(CDlgQry)
	enum { IDD = IDD_DLG_QRY };
	CReportCtrl	m_list;
	CButton	m_btnExp;
	CEdit	m_edtRst;
	CComboBox	m_cbSel;
	CStatic	m_stCur;
	CDateTimeCtrl	m_tmPk2;
	CDateTimeCtrl	m_tmPk1;
	int		m_nIndex;
	int		m_nComboSel;
	int		m_nlac;
	int		m_ncid;
	double	m_dLng;
	double	m_dLat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgQry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgQry)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnBtnQry();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnButton8();
	afx_msg void OnBtnGetSurnd();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBtnExport();
	afx_msg void OnRadio11();
	afx_msg void OnButton16();
	afx_msg void OnDestroy();
	afx_msg void OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuDelAll();
	afx_msg void OnMenuDelCur();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQRY_H__CE216162_F131_4175_80FA_038369445471__INCLUDED_)
