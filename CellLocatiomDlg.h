// CellLocatiomDlg.h : header file
//
//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES

#if !defined(AFX_CELLLOCATIOMDLG_H__E26557E2_33F1_4A75_BDB7_AB7DC0B1531D__INCLUDED_)
#define AFX_CELLLOCATIOMDLG_H__E26557E2_33F1_4A75_BDB7_AB7DC0B1531D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DlgReal.h"
#include "dlgload.h"
#include "dlgqry.h"
#include "DlgDown.h"
#include "DlgPreview.h"
#include "DlgRst.h"
#include "WebPage.h"
#include <MSHTML.H>
#include "SerialPort.h"
#include "DlgEdit.h"
#include "dlgbs.h"
#include <vector>
using namespace std;

class CInfoData
{
public:
	CInfoData()
	{
		 m_g1="-";
		 m_g2="-";
		 m_c="-";
		 m_w="-";
		 m_t="-";
		 m_l1="-";
		 m_l2="-";
		 m_l3="-";
		 m_tm="-";
		 m_mk="-";
		 long1=0.0;
		 lat1=0.0;//0yiban 1 hei  2bai
		
	}
	
	CString m_g1;
	CString m_g2;
	
	CString m_c;
	CString m_w;
	CString m_t;
	CString m_l1;
	CString m_l2;
	CString m_l3;
	CString m_tm;
	CString m_mk;
	double long1;
	double lat1;
	
	
};




/////////////////////////////////////////////////////////////////////////////
// CCellLocatiomDlg dialog
struct TabCtrlComplex : public TabCtrl
{	TabCtrlStyle_base styleBase;
// 
TabCtrlStyle_VS2003_client styleVS2003_client;
TabCtrlStyle_VS2003_client_custom1 styleVS2003_client_custom1;
TabCtrlStyle_VS2003_bars styleVS2003_bars;
TabCtrlStyle_VS2003_bars_custom1 styleVS2003_bars_custom1;
// 
TabCtrlStyle_VS2008_client_classic styleVS2008_client_classic;
TabCtrlStyle_VS2008_client_blue styleVS2008_client_blue;
TabCtrlStyle_VS2008_client_silver styleVS2008_client_silver;
TabCtrlStyle_VS2008_client_olive styleVS2008_client_olive;
TabCtrlStyle_VS2008_bars_classic styleVS2008_bars_classic;
TabCtrlStyle_VS2008_bars_classic_custom1 styleVS2008_bars_classic_custom1;
TabCtrlStyle_VS2008_bars_blue styleVS2008_bars_blue;
TabCtrlStyle_VS2008_bars_blue_custom1 styleVS2008_bars_blue_custom1;
TabCtrlStyle_VS2008_bars_silver styleVS2008_bars_silver;
TabCtrlStyle_VS2008_bars_silver_custom1 styleVS2008_bars_silver_custom1;
TabCtrlStyle_VS2008_bars_olive styleVS2008_bars_olive;
TabCtrlStyle_VS2008_bars_olive_custom1 styleVS2008_bars_olive_custom1;
// 
TabCtrlStyle_VS2010_client styleVS2010_client;
TabCtrlStyle_VS2010_client_custom1 styleVS2010_client_custom1;
TabCtrlStyle_VS2010_client_custom2 styleVS2010_client_custom2;
TabCtrlStyle_VS2010_bars styleVS2010_bars;
};

#include "ReportCtrl.h"
class CCellLocatiomDlg : public CDialog,public TabCtrlNotify
{
// Construction
public:
	CCellLocatiomDlg(CWnd* pParent = NULL);	// standard constructor
	vector<CInfoData> m_database;
//	MYSQL mysql;

	CString m_strLng,m_strLat;
	double m_dSelectedLng,m_dSelectedLat;
	void AddMark(CString strMk);
	void QryData(int,int,int);
	void QryData2(DWORD a,DWORD b);
	void QryData3(double d1,double d2,double d3,double d4);
	void	InvokeFunc();
	void	InvokeFuncWithParams(const CString& strParam1, int iParam2);
	void SetControlPostion();
	void SetControlPostion1();
	void SetControlPostion2();
	void SetControlPostion3();
	void SetControlPostion4();
	void SetControlPostion5();	
	void SetControlPostion6();
	void ShowMapData(double d1,double d2,double d3,double d4);
	bool ExportToExcel(CString filename);
	void LoadLocList();
	void DispRecvedMsg(char*buf);
	void SendLogOn();
	CDlgQry m_dlgQry;
	DlgDown m_dlgDown;
	CDlgLoad m_dlgLoad;
	CDlgReal m_dlgReal;
	CDlgRst  m_dlgRst;
	CDlgBs m_dlgBs;
	CDlgPreview m_dlgPreview;
	CWebPage m_webPage;

	CDlgEdit*m_pDlgEdit;
	IHTMLDocument2*	m_pIHTMLDoc;
	TabCtrlComplex m_TabCtrl1;
	bool m_bDelDragOutside;
	void SetTabsPosition(int n);
	void SetImages1();
	bool IsExist1(CWnd *pWnd) const;
	CString GetInstallPath(void);
void ShowLatLng(double dbLng, double dbLat);
	void ShowModalDlg(double dbLng, double dbLat);
	void ShowCurLevel(int level);
	CFont font;
	double m_dSelLat,m_dSelLong;
//	BOOL m_bSerialPortOpened;

	TRealtimeData m_curRealData;

	void SaveMark(CString strMark);
	void RecvReal();
	BOOL m_bCanShow;
	int m_nTabID;

	int m_nDelItem;
// Dialog Data
	//{{AFX_DATA(CCellLocatiomDlg)
	enum { IDD = IDD_CELLLOCATIOM_DIALOG };
	CComboBox	m_cbList;
	CReportCtrl	m_list;
	CStatic	m_stBtm;
	CReportCtrl	m_list2;
	CStatic	m_stTab;
	CWebBrowser2	m_brow;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellLocatiomDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCellLocatiomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton11();
	afx_msg void OnButton1();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuDel();
	afx_msg void OnButton15();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnButtonTest();
	//}}AFX_MSG
	virtual void OnCloseButtonClicked(TabCtrl *pCtrl, CRect const *pRect, CPoint ptScr);
	virtual void OnMenuButtonClicked(TabCtrl *pCtrl, CRect const *pRect, CPoint ptScr);
	virtual void OnTabSelected(TabCtrl *pCtrl, HTAB hTab);
	virtual void OnDrag(TabCtrl *pCtrl, HTAB hTab, CPoint ptScr, bool outside);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLLOCATIOMDLG_H__E26557E2_33F1_4A75_BDB7_AB7DC0B1531D__INCLUDED_)
