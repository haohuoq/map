#if !defined(AFX_DLGPREVIEW_H__976E1397_75E6_45DA_9453_CD648DBF4011__INCLUDED_)
#define AFX_DLGPREVIEW_H__976E1397_75E6_45DA_9453_CD648DBF4011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPreview.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPreview dialog
#include "ReportCtrl.h"
class CDlgPreview : public CDialog
{
// Construction
public:
	CDlgPreview(CWnd* pParent = NULL);   // standard constructor
	void SetControlPostion();
	void LoadDB();
	CFont m_font;
	int m_nSelectItem;
	int m_nDeleteItem;
	int cell_select_list[5];
	int m_nMapSwitch; //0 ±í¸ñ  1 µØÍ¼
	BOOL isExistCellID(int cellid);
// Dialog Data
	//{{AFX_DATA(CDlgPreview)
	enum { IDD = IDD_DLG_PREVIEW };
	CReportCtrl	m_list5;
	CReportCtrl	m_list4;
	CReportCtrl	m_list3;
	CReportCtrl	m_list2;
	CReportCtrl	m_list1;

	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPreview)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPreview)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickList4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectCell();
	afx_msg void OnRclickList5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteCell();
	afx_msg void OnBtnMapSwitch();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPREVIEW_H__976E1397_75E6_45DA_9453_CD648DBF4011__INCLUDED_)
