// DlgReal.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgReal.h"
#include "DlgMark.h"
#include "CellLocatiomDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReal dialog
extern CCellLocatiomDlg * pMainDlg;
extern Point * wgs_gcj_encrypts(double wgLat, double wgLon) ;
extern CString strIcon[10];
CDlgReal::CDlgReal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReal)
m_nLockItem = -1;
m_nLockSN = 0;
m_nSelectItem =  -1;
	//}}AFX_DATA_INIT
}


void CDlgReal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReal)
	DDX_Control(pDX, IDC_LIST3, m_listData);
	DDX_Control(pDX, IDC_LIST1, m_listModule);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReal, CDialog)
	//{{AFX_MSG_MAP(CDlgReal)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_COMMAND(MENU_LOCK_MODULE, OnLockModule)
	ON_COMMAND(MENU_RELEASE_LOCK, OnReleaseLock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReal message handlers

BOOL CDlgReal::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_font.CreateFont(20, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "微软雅黑");
	GetDlgItem(IDC_LIST1)->SetFont(&m_font);
	GetDlgItem(IDC_LIST3)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_T2)->SetFont(&m_font);

	GetDlgItem(IDC_STATIC_T1)->SetFont(&m_font);


	
	m_listModule.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_listModule.SetColumnHeader(_T("编号, 90;名称,110;任务,120"));
	m_listModule.SetGridLines(TRUE);
	m_listModule.SetSortable(FALSE);

	m_listData.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_listData.SetColumnHeader(_T("模式, 57;LAC,55;CID,85;CN,55;PCI,40;RSSI,48"));
	m_listData.SetGridLines(TRUE);
	m_listData.SetSortable(FALSE);


/*	GetDlgItem(IDC_RADIO1)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO4)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO5)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO6)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO7)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO8)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO9)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO10)->SetFont(&m_font);

	
	GetDlgItem(IDC_CHECK2)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT1)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT2)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_MK)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON11)->SetFont(&m_font);

	m_brs2.CreateSolidBrush(RGB(156,235,123));
	m_brs3.CreateSolidBrush(RGB(156,123,235));
	m_brs4.CreateSolidBrush(RGB(255,190,125));
	m_brs5.CreateSolidBrush(RGB(255,255,149));
	
	m_brs6.CreateSolidBrush(RGB(255,149,255));
	m_brs7.CreateSolidBrush(RGB(123,156,235));
	m_brs8.CreateSolidBrush(RGB(125,255,190));
	m_brs9.CreateSolidBrush(RGB(149,255,255));

	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list.SetColumnHeader(_T("a, 150;b,460"));
	m_list.SetGridLines(TRUE);
	m_list.SetSortable(FALSE);


	OnRadio1();*/
//	SetControlPostion();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*void CDlgReal::ShowReal(TRealtimeData* prd)
{

	
	UpdateData(TRUE);

	CString strPLMN;
	CString strLAC;
	CString strCid;
	CString strArfcn;
	CString strRssi;
	CString strBsid;
	CString strNum;
	CString strPN;
	CString strNb="";

	CString strTmp;
	m_list.DeleteAllItems();
	if(m_nIndex==0)
	{
		char PLMN[6];
		
		PLMN[0]=(prd->gsm1.plmn_id[0]&0xf)+'0';
		
		PLMN[1]=((prd->gsm1.plmn_id[0]&0xf0)>>4)+'0';
		PLMN[2]=(prd->gsm1.plmn_id[1]&0xf)+'0';
		PLMN[3]=(prd->gsm1.plmn_id[2]&0xf)+'0';
		PLMN[4]=((prd->gsm1.plmn_id[2]&0xf0)>>4)+'0';
		PLMN[5]='\0';

		strPLMN.Format("%s",PLMN);
		
		WORD LAC=(prd->gsm1.lac[0]<<8)|(prd->gsm1.lac[1]);

		if(LAC==257)
		{
			m_list.InsertItem(0,"-");
			m_list.SetItemText(0,1,"-");
			m_list.SetItemText(0,2,"-");
			m_list.SetItemText(0,3,"-");
			m_list.SetItemText(0,4,"-");
			m_list.SetItemText(0,5,"-");
			m_list.SetItemText(0,6,"-");
			return;
			
		}
		
		strLAC.Format("%d",LAC);

		strCid.Format("%d",prd->gsm1.cell_id);

		strArfcn.Format("%d",prd->gsm1.srv_cell.arfcn);

		strRssi.Format("-%d",prd->gsm1.srv_cell.rssi);

		strNum.Format("%d",prd->gsm1.nghbor_cell_num);
		strNb="";
		for(int i=0;i<prd->gsm1.nghbor_cell_num;i++)
		{

			strTmp.Format("%d( -%d ) ", prd->gsm1.nghbor_cell[i].arfcn,prd->gsm1.nghbor_cell[i].rssi);
			strNb+=strTmp;
		}

	

		m_list.InsertItem(0,strPLMN);
		m_list.SetItemText(0,1,strLAC);
		m_list.SetItemText(0,2,strCid);
		m_list.SetItemText(0,3,strArfcn);
		m_list.SetItemText(0,4,strRssi);
		m_list.SetItemText(0,5,strNum);
		m_list.SetItemText(0,6,strNb);

	

	}
	else if(m_nIndex==1)
	{
		char PLMN[6];
		
		PLMN[0]=(prd->gsm2.plmn_id[0]&0xf)+'0';
		
		PLMN[1]=((prd->gsm2.plmn_id[0]&0xf0)>>4)+'0';
		PLMN[2]=(prd->gsm2.plmn_id[1]&0xf)+'0';
		PLMN[3]=(prd->gsm2.plmn_id[2]&0xf)+'0';
		PLMN[4]=((prd->gsm2.plmn_id[2]&0xf0)>>4)+'0';
		PLMN[5]='\0';
		
		strPLMN.Format("%s",PLMN);
		
		WORD LAC=(prd->gsm2.lac[0]<<8)|(prd->gsm2.lac[1]);

		if(LAC==257)
		{
			m_list.InsertItem(0,"-");
			m_list.SetItemText(0,1,"-");
			m_list.SetItemText(0,2,"-");
			m_list.SetItemText(0,3,"-");
			m_list.SetItemText(0,4,"-");
			m_list.SetItemText(0,5,"-");
			m_list.SetItemText(0,6,"-");
			return;
			
		}

		strLAC.Format("%d",LAC);
		
		strCid.Format("%d",prd->gsm2.cell_id);
		
		strArfcn.Format("%d",prd->gsm2.srv_cell.arfcn);
		
		strRssi.Format("-%d",prd->gsm2.srv_cell.rssi);
		
		strNum.Format("%d",prd->gsm2.nghbor_cell_num);
		strNb="";
		for(int i=0;i<prd->gsm2.nghbor_cell_num;i++)
		{
			
			strTmp.Format("%d( -%d ) ", prd->gsm2.nghbor_cell[i].arfcn,prd->gsm2.nghbor_cell[i].rssi);
			strNb+=strTmp;
		}
		

		
		m_list.InsertItem(0,strPLMN);
		m_list.SetItemText(0,1,strLAC);
		m_list.SetItemText(0,2,strCid);
		m_list.SetItemText(0,3,strArfcn);
		m_list.SetItemText(0,4,strRssi);
		m_list.SetItemText(0,5,strNum);
		m_list.SetItemText(0,6,strNb);

	}
	else if(m_nIndex==2)
	{
		if(prd->cdma.sid==257)
		{
			m_list.InsertItem(0,"-");
			m_list.SetItemText(0,1,"-");
			m_list.SetItemText(0,2,"-");
			m_list.SetItemText(0,3,"-");
			m_list.SetItemText(0,4,"-");
			m_list.SetItemText(0,5,"-");
			m_list.SetItemText(0,6,"-");
			return;
			
		}
		strPLMN="46003";
		strLAC.Format("%d",prd->cdma.sid);
		strCid.Format("%d",prd->cdma.nid);
		strBsid.Format("%d",prd->cdma.base_id);
		strArfcn.Format("%d",prd->cdma.cdma_ch);
		strPN.Format("%d",prd->cdma.cdma_pn);

		if(prd->cdma.pilot_num>6)
			prd->cdma.pilot_num=6;

		strNb="";
		for(int i=0;i<prd->cdma.pilot_num;i++)
		{
			strTmp.Format("%d ",prd->cdma.pilot_sets[i]);
			strNb+=strTmp;
				
		}




		
		
		m_list.InsertItem(0,strPLMN);
		m_list.SetItemText(0,1,strLAC);
		m_list.SetItemText(0,2,strCid);
		m_list.SetItemText(0,3,strBsid);
		m_list.SetItemText(0,4,strArfcn);
		m_list.SetItemText(0,5,strPN);
		m_list.SetItemText(0,6,strNb);



	}
	else if(m_nIndex==3)
	{
		char PLMN[6];
		
		PLMN[0]=(prd->wcdma.plmn_id[0]&0xf)+'0';
		
		PLMN[1]=((prd->wcdma.plmn_id[0]&0xf0)>>4)+'0';
		PLMN[2]=(prd->wcdma.plmn_id[1]&0xf)+'0';
		PLMN[3]=(prd->wcdma.plmn_id[2]&0xf)+'0';
		PLMN[4]=((prd->wcdma.plmn_id[2]&0xf0)>>4)+'0';
		PLMN[5]='\0';
		
		strPLMN.Format("%s",PLMN);

		CString strRealRssi ;
		WORD LAC=(prd->wcdma.lac[0]<<8)|(prd->wcdma.lac[1]);

		if(LAC==257)
		{
			m_list.InsertItem(0,"-");
			m_list.SetItemText(0,1,"-");
			m_list.SetItemText(0,2,"-");
			m_list.SetItemText(0,3,"-");
			m_list.SetItemText(0,4,"-");
			m_list.SetItemText(0,5,"-");
			m_list.SetItemText(0,6,"-");
			m_list.SetItemText(0,7,"-");
			return;
			
		}

		strLAC.Format("%d",LAC);
		strCid.Format("%d",prd->wcdma.cell_id%0x10000);
		strArfcn.Format("%d",prd->wcdma.srv_cell.UARFCN);
		strRssi.Format("%d",prd->wcdma.srv_cell.PSC);
		strRealRssi.Format("-%d",prd->wcdma.srv_cell.RSSI);
		strNum.Format("%d",prd->wcdma.neighbor_cell_num);
		strNb="";
		for(int i=0;i<prd->wcdma.neighbor_cell_num;i++)
		{
			strTmp.Format("%d (%d)",prd->wcdma.neighbor_cell[i].UARFCN,prd->wcdma.neighbor_cell[i].PSC);
			strNb+=strTmp;
			
		}



		
		
		
		m_list.InsertItem(0,strPLMN);
		m_list.SetItemText(0,1,strLAC);
		m_list.SetItemText(0,2,strCid);
		m_list.SetItemText(0,3,strArfcn);
		m_list.SetItemText(0,4,strRssi);
		m_list.SetItemText(0,5,strRealRssi);
		m_list.SetItemText(0,6,strNum);
		m_list.SetItemText(0,7,strNb);




		
	}
	else if(m_nIndex==4)
	{


		
		if(prd->td.lmi.tac==257)
		{
			m_list.InsertItem(0,"-");
			m_list.SetItemText(0,1,"-");
			m_list.SetItemText(0,2,"-");
			m_list.SetItemText(0,3,"-");
			m_list.SetItemText(0,4,"-");
			m_list.SetItemText(0,5,"-");
			m_list.SetItemText(0,6,"-");
			return;
			
		}

		strPLMN.Format("%d",prd->td.lmi.plmn);
		strLAC.Format("%d",prd->td.lmi.tac);
		strCid.Format("%d",prd->td.lmi.global_ci);
		strArfcn.Format("%d",prd->td.lmi.earfcn);
		if(prd->td.lmi.rscp-115<=-110)
			strRssi.Format("%d",0);
		else
			strRssi.Format("%d",prd->td.lmi.rscp-115);
	



		strNb="";
		int nb_cnt=0;
		for(int i=0;i<6;i++)
		{
			if(prd->td.lni[i].tac!=0 && prd->td.lni[i].tac!=257)
			{
				strTmp.Format("%d (%d)",prd->td.lni[i].earfcn,prd->td.lni[i].rscp);
				strNb+=strTmp;
				nb_cnt+=1;
			
			}
			
		}

		strNum.Format("%d",nb_cnt);




		
		
		m_list.InsertItem(0,strPLMN);
		m_list.SetItemText(0,1,strLAC);
		m_list.SetItemText(0,2,strCid);
		m_list.SetItemText(0,3,strArfcn);
		m_list.SetItemText(0,4,strRssi);
		m_list.SetItemText(0,5,strNum);
		m_list.SetItemText(0,6,strNb);




	}
	else if(m_nIndex==5)
	{

		if(prd->lte1.lmi.tac==257)
		{
			m_list.InsertItem(0,"-");
			m_list.SetItemText(0,1,"-");
			m_list.SetItemText(0,2,"-");
			m_list.SetItemText(0,3,"-");
			m_list.SetItemText(0,4,"-");
			m_list.SetItemText(0,5,"-");
			m_list.SetItemText(0,6,"-");
			return;
			
		}


		strPLMN.Format("%d",prd->lte1.lmi.plmn);
		strLAC.Format("%d",prd->lte1.lmi.tac);
		strCid.Format("%d",prd->lte1.lmi.global_ci);
		strArfcn.Format("%d",prd->lte1.lmi.earfcn);

		if(prd->lte1.lmi.rscp-140 <= -115)
			strRssi.Format("%d",0);

		else
			strRssi.Format("%d",prd->lte1.lmi.rscp-140);
		
		
		strNb="";int nb_cnt=0;
		for(int i=0;i<6;i++)
		{
			if(prd->lte1.lni[i].tac!=0 && prd->lte1.lni[i].tac!=257)
			{
				
				
				strTmp.Format("%d (%d)",prd->lte1.lni[i].earfcn,prd->lte1.lni[i].rscp);
				strNb+=strTmp;
				nb_cnt+=1;
				
			}
			
		}
		
		strNum.Format("%d",nb_cnt);
		m_list.InsertItem(0,strPLMN);
		m_list.SetItemText(0,1,strLAC);
		m_list.SetItemText(0,2,strCid);
		m_list.SetItemText(0,3,strArfcn);
		m_list.SetItemText(0,4,strRssi);
		m_list.SetItemText(0,5,strNum);
		m_list.SetItemText(0,6,strNb);


	}
	else if(m_nIndex==6)
	{

		if(prd->lte2.lmi.tac==257)
		{
			m_list.InsertItem(0,"-");
			m_list.SetItemText(0,1,"-");
			m_list.SetItemText(0,2,"-");
			m_list.SetItemText(0,3,"-");
			m_list.SetItemText(0,4,"-");
			m_list.SetItemText(0,5,"-");
			m_list.SetItemText(0,6,"-");
			return;
			
		}

		strPLMN.Format("%d",prd->lte2.lmi.plmn);
		strLAC.Format("%d",prd->lte2.lmi.tac);
		strCid.Format("%d",prd->lte2.lmi.global_ci);
		strArfcn.Format("%d",prd->lte2.lmi.earfcn);
		if(prd->lte2.lmi.rscp-140 <= -115)
			strRssi.Format("%d",0);
		
		else
			strRssi.Format("%d",prd->lte2.lmi.rscp-140);
		
		strNb="";int nb_cnt=0;
		for(int i=0;i<6;i++)
		{
			if(prd->lte2.lni[i].tac!=0 && prd->lte2.lni[i].tac!=257)
			{
				
				
				strTmp.Format("%d (%d)",prd->lte2.lni[i].earfcn,prd->lte2.lni[i].rscp);
				strNb+=strTmp;
				nb_cnt+=1;
			}
			
		}
		
		strNum.Format("%d",nb_cnt);
		m_list.InsertItem(0,strPLMN);
		m_list.SetItemText(0,1,strLAC);
		m_list.SetItemText(0,2,strCid);
		m_list.SetItemText(0,3,strArfcn);
		m_list.SetItemText(0,4,strRssi);
		m_list.SetItemText(0,5,strNum);
		m_list.SetItemText(0,6,strNb);
	}
	else if(m_nIndex==7)
	{

		if(prd->lte3.lmi.tac==257)
		{
			m_list.InsertItem(0,"-");
			m_list.SetItemText(0,1,"-");
			m_list.SetItemText(0,2,"-");
			m_list.SetItemText(0,3,"-");
			m_list.SetItemText(0,4,"-");
			m_list.SetItemText(0,5,"-");
			m_list.SetItemText(0,6,"-");
			return;
			
		}

		strPLMN.Format("%d",prd->lte3.lmi.plmn);
		strLAC.Format("%d",prd->lte3.lmi.tac);
		strCid.Format("%d",prd->lte3.lmi.global_ci);
		strArfcn.Format("%d",prd->lte3.lmi.earfcn);
		if(prd->lte3.lmi.rscp-140 <= -115)
			strRssi.Format("%d",0);
		
		else
			strRssi.Format("%d",prd->lte3.lmi.rscp-140);
		
		
		strNb="";int nb_cnt=0;
		for(int i=0;i<6;i++)
		{
			if(prd->lte3.lni[i].tac!=0 && prd->lte3.lni[i].tac!=257)
			{
				
				strTmp.Format("%d (%d)",prd->lte3.lni[i].earfcn,prd->lte3.lni[i].rscp);
				strNb+=strTmp;
				nb_cnt+=1;
			}
			
		}
		
		
		strNum.Format("%d",nb_cnt);
		m_list.InsertItem(0,strPLMN);
		m_list.SetItemText(0,1,strLAC);
		m_list.SetItemText(0,2,strCid);
		m_list.SetItemText(0,3,strArfcn);
		m_list.SetItemText(0,4,strRssi);
		m_list.SetItemText(0,5,strNum);
		m_list.SetItemText(0,6,strNb);
	}

	CString strlong2,strlat2;
	double long1 = pMainDlg->m_curRealData.base_long;
	strlong2.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
	double lat1 = pMainDlg->m_curRealData.base_lat;
	strlat2.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
	
	
	Point * p =  wgs_gcj_encrypts(atof(strlat2), atof(strlong2));
	
	strlong2.Format("%f", p->lng );
	strlat2.Format("%f", p->lat );
	
	
	pMainDlg->m_webPage.CallJScript("MarkToPoint",strlong2,strlat2,strIcon[2]);
	

}*/
/*
void CDlgReal::OnButton1() 
{
	// TODO: Add your control notification handler code here

	if(pMainDlg->m_curRealData.time1==0)
	{
		AfxMessageBox("无效数据");
	//	return;
	}

	UpdateData(TRUE);

	pMainDlg->m_bCanShow=FALSE;


	CFileDialog fdlg(FALSE,".txt","",OFN_OVERWRITEPROMPT,"(*.txt)|*.txt|");
	CString filename="";
	if(fdlg.DoModal()==IDOK)
	{
		filename=fdlg.GetPathName();
	}
	else
		return;


//	CDlgMark dlgMark;
//	if(dlgMark.DoModal()==IDOK)
	{
//		UpdateData(FALSE);
		pMainDlg->SaveMark(filename);
//		pMainDlg->SaveMark(m_strMk);
	}
	pMainDlg->m_bCanShow=TRUE;
}
*/
/*void CDlgReal::OnButton11() 
{
	// TODO: Add your control notification handler code here
	if(pMainDlg->m_curRealData.time1==0)
	{
		AfxMessageBox("无效数据");
		return;
	}

	CString strlong2,strlat2;
	double long1 = pMainDlg->m_curRealData.base_long;
	strlong2.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
	double lat1 = pMainDlg->m_curRealData.base_lat;
	strlat2.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
	
	
	Point * p =  wgs_gcj_encrypts(atof(strlat2), atof(strlong2));
	
	strlong2.Format("%f", p->lng );
	strlat2.Format("%f", p->lat );

	pMainDlg->m_webPage.CallJScript("CenterToPoint",strlong2,strlat2,strIcon[2]);


}*/

HBRUSH CDlgReal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
/*
	
	if(pWnd->GetDlgCtrlID()==IDC_RADIO1)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs2.GetSafeHandle();
	}
	if(pWnd->GetDlgCtrlID()==IDC_RADIO4)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs3.GetSafeHandle();
	}
	if(pWnd->GetDlgCtrlID()==IDC_RADIO5)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs4.GetSafeHandle();
	}
	if(pWnd->GetDlgCtrlID()==IDC_RADIO6)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs5.GetSafeHandle();
	}
	
	if(pWnd->GetDlgCtrlID()==IDC_RADIO7)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs6.GetSafeHandle();
	}
	
	if(pWnd->GetDlgCtrlID()==IDC_RADIO8)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs7.GetSafeHandle();
	}
	
	if(pWnd->GetDlgCtrlID()==IDC_RADIO9)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs8.GetSafeHandle();
	}
	
	if(pWnd->GetDlgCtrlID()==IDC_RADIO10)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs9.GetSafeHandle();
	}*/
	return hbr;
}



void CDlgReal::SetControlPostion()
{/*
	int nTmp,nTmp2;
	CRect rtClient; 
	
	pMainDlg->m_stBtm.GetWindowRect(&rtClient);
	int nW = rtClient.Width();
	int nH = rtClient.Height();
	
	GetClientRect(&rtClient);             //获得对话框客户区屏幕坐标   
    ClientToScreen(&rtClient); //映射为屏幕坐标   
	//	MoveWindow(CRect(0,0,nW,nH)); 
	
	CRect rtCaption;   
	CWnd *pWnd;
	int lx,ly,dx,dy;
	
	pWnd=&m_list;   
    pWnd->GetWindowRect(&rtCaption);          //获得button屏幕坐标   
	lx=rtCaption.Width();                 //button长度   
	ly=rtCaption.Height();   //button高度   
	
	dx = rtCaption.left;
	dy = rtCaption.top;
	

	pWnd->MoveWindow(CRect(dx-rtClient.left,dy-rtClient.top,nW,dy-rtClient.top+126));
	*/
	
}
/*
void CDlgReal::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems ();
	m_list.SetColumnHeader(_T("网络代码, 100;  LAC, 100;CID,100;信道,100;信号强度,100;邻区数,100;邻区信道（强度）,500"));
		
}

void CDlgReal::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems ();
	m_list.SetColumnHeader(_T("网络代码, 100;  LAC, 100;CID,100;信道,100;信号强度,100;邻区数,100;邻区信道（强度）,500"));
		
}

void CDlgReal::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems ();
	m_list.SetColumnHeader(_T("网络代码, 100;  SID, 100;NID,100;BSID,100;频点,100;PN,100;邻区PN,500"));

}

void CDlgReal::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems ();
	m_list.SetColumnHeader(_T("网络代码, 100;  LAC, 100;CID,100;频点,100;扰码,100;信号强度,100;邻区数,100;邻区频点（扰码）,500"));
	
}

void CDlgReal::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	
	m_list.DeleteAllItems ();
	m_list.SetColumnHeader(_T("网络代码, 100;  LAC, 100;CID,100;频点,100;信号强度,100;邻区数,100;邻区频点（扰码）,500"));
	
}

void CDlgReal::OnRadio8() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems ();
	m_list.SetColumnHeader(_T("网络代码, 100;  TAC, 100;CID,100;频点,100;信号强度,100;邻区数,100;邻区频点（扰码）,500"));
	
}

void CDlgReal::OnRadio9() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems ();
	m_list.SetColumnHeader(_T("网络代码, 100;  TAC, 100;CID,100;频点,100;信号强度,100;邻区数,100;邻区频点（扰码）,500"));
	
}

void CDlgReal::OnRadio10() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems ();
	m_list.SetColumnHeader(_T("网络代码, 100;  TAC, 100;CID,100;频点,100;信号强度,100;邻区数,100;邻区频点（扰码）,500"));
	
}
*/

void CDlgReal::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem != -1)
	{
		
		m_nSelectItem = pNMListView->iItem;
		
		LPPOINT lpoint=new tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置
		CMenu *menu;
		CMenu PopMenu;
		if(m_nSelectItem==m_nLockItem)
			PopMenu.LoadMenu(IDR_MENU6);
		else
			PopMenu.LoadMenu(IDR_MENU5);
		menu = PopMenu.GetSubMenu (0);
		

		menu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
		//资源回收
		HMENU hmenu=menu->Detach();
		menu->DestroyMenu();
		delete lpoint;
		
	}
	*pResult = 0;
}

void CDlgReal::OnLockModule() 
{
	// TODO: Add your command handler code here
	if(m_nSelectItem!=-1)
	{
		m_nLockItem = m_nSelectItem;
		CString strSN = m_listModule.GetItemText(m_nLockItem,0);

		m_nLockSN = atoi(strSN);
	}
}

void CDlgReal::OnReleaseLock() 
{
	// TODO: Add your command handler code here
	m_nLockSN = 0;
	m_nLockItem = -1;
	m_nSelectItem=-1;
}
