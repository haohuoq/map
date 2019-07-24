// DlgRst.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgRst.h"
#include "CellLocatiomDlg.h"
#include "myexcel.h"
#include <fstream>
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRst dialog
extern int mark_flag;
extern CCellLocatiomDlg * pMainDlg;
extern Point * wgs_gcj_encrypts(double wgLat, double wgLon) ;
extern CString strIcon[10];
CDlgRst::CDlgRst(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRst::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRst)
	m_nIdx = 0;
	m_nOldIndex = 0;
	//}}AFX_DATA_INIT
}


void CDlgRst::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRst)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Radio(pDX, IDC_RADIO1, m_nIdx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRst, CDialog)
	//{{AFX_MSG_MAP(CDlgRst)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio9)
	ON_BN_CLICKED(IDC_RADIO10, OnRadio10)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_COMMAND(ID_MENU_DEL_ALL, OnMenuDelAll)
	ON_COMMAND(ID_MENU_DEL_CUR, OnMenuDelCur)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRst message handlers

BOOL CDlgRst::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_font.CreateFont(20, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "微软雅黑");
	

	GetDlgItem(IDC_LIST1)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);

	GetDlgItem(IDC_RADIO1)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO4)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO5)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO6)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO7)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO8)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO9)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO10)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO12)->SetFont(&m_font);

	m_brs.CreateSolidBrush(RGB(180,180,180));
	m_brs2.CreateSolidBrush(RGB(156,235,123));
	m_brs3.CreateSolidBrush(RGB(156,123,235));
	m_brs4.CreateSolidBrush(RGB(255,190,125));
	m_brs5.CreateSolidBrush(RGB(255,255,149));

	m_brs6.CreateSolidBrush(RGB(255,149,255));
	m_brs7.CreateSolidBrush(RGB(123,156,235));
	m_brs8.CreateSolidBrush(RGB(125,255,190));
	m_brs9.CreateSolidBrush(RGB(149,255,255));
	
	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list.SetColumnHeader(_T("时间, 150;经度, 150;纬度,150;移动G_LAC,100;移动G_CID,100;联通G_LAC,100;联通G_CID,100;电信C_SID,100;电信C_BSID,100;联通W_LAC,100;联通W_CID,100;移动T_LAC,100;移动T_CID,100;移动4G_RAC,110;移动4G_CID,110;联通4G_RAC,110;联通4G_CID,110;电信4G_RAC,110;电信4G_CID,110"));
	m_list.SetGridLines(TRUE);
	m_list.SetSortable(FALSE);


	for(int i=0;i<7;i++)
	{
		m_arrCID[i].SetSize(0,1);
	}
	m_nArrAdd = 0;
//	SetControlPostion();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRst::SetControlPostion()
{

//	AfxMessageBox("SetControlPostion");
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

	


	
}
const double pi = 3.14159265358979324;
const double a = 6378245.0;
const double ee = 0.00669342162296594323;
const double x_pi = 3.14159265358979324 * 3000.0 / 180.0;
 
struct Loc
{
	double Lon;			//坐标的经度
	double Lat;			//坐标的纬度
};
 
extern double transformLat(double x,double y);		//将GPS坐标转换为google纬度坐标辅助函数
extern double transformLon(double x,double y);		//将GPS坐标转换为google经度坐标辅助函数
extern Loc bd_encrypt(Loc gg);//将谷歌坐标转换为百度坐标
extern Loc transform(Loc gps);//将GPS坐标转换为google地图
 

void CDlgRst:: LoadToList(CString s)
{


	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	
	strDBPath += "\\database.mdb";
	

	


	
	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
		
	}
	else
	{
		AfxMessageBox("数据库连接失败!");
		delete m_pAdoRecordset;
		delete m_pAdoConn;
		return;
	}
	
	m_pAdoConn->BeginTrans();

	CString strSQL = "SELECT * FROM gather WHERE savetime='"+s+"'";
	m_pAdoRecordset->Open(strSQL);
	
	int num = m_pAdoRecordset->GetRecordCount();
	int nItemCount = 0;
	CString stt;
	stt.Format("共找到记录%d个",num);

	int mod = num/150;
//	AfxMessageBox(stt);
	CString strData;
	CString strZs,str1,str2,str3,str4,str5,str6;
	CString strLat,strLng;
	if(num<1)
	{
		AfxMessageBox("路测数据库中没找到数据");


		m_pAdoRecordset->Close();
		m_pAdoConn->Close();
		
		delete m_pAdoRecordset;
		delete m_pAdoConn;
		return;
	}
	else
	{

		pMainDlg->m_webPage.CallJScript("ClearLocations");
		m_pAdoRecordset->MoveFirst();

		while(!m_pAdoRecordset->IsEOF())
		{

			m_pAdoRecordset->GetCollect((LPCSTR)"lat", strLat);
			m_pAdoRecordset->GetCollect((LPCSTR)"lng", strLng);
//		
			m_pAdoRecordset->GetCollect((LPCSTR)"plmn", strZs);
			m_pAdoRecordset->GetCollect((LPCSTR)"rssi", str3);
			m_pAdoRecordset->GetCollect((LPCSTR)"mode", str6);
			m_pAdoRecordset->GetCollect((LPCSTR)"lac", str1);
			m_pAdoRecordset->GetCollect((LPCSTR)"cid", str2);
			if(atof(strLat)==0.0)
			{
			
				m_pAdoRecordset->GetCollect((LPCSTR)"lat2", strLat);
				m_pAdoRecordset->GetCollect((LPCSTR)"lng2", strLng);

			}
			
			
			//}
			
			//	strData = "118.773,32.043,a#b#c#d#e_1#2#3#4#5_v#v#v#v#v";
			
		//	else
			
				if(atof(strLat)!=0.0)
				{




 
		

	//4546.40891/100=45.4640891可以直接读出45度, 4546.40891–45*100=46.40891, 可以直接读出46分

			double long1 = atof(strLng);
			double lat1 = atof(strLat);
		
		Point * p =  wgs_gcj_encrypts(lat1, long1);
		
		str4.Format("%f", p->lng );
		str5.Format("%f", p->lat );


		CString strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(TAC)</th><th>CID</th> <th>RSSI</th> </tr>";
		strContent+=	"<tr align=\"center\">  <td>"+strZs+" "+str6+"</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>"+str3+"</td> </tr> </table>";

				//
					if(mod>0 && (nItemCount%mod==1))
					{
						/*Loc demo,bd;
						demo.Lat = atof(strLat);
						demo.Lon = atof(strLng);
						
							//bd = bd_encrypt(transform(demo));
							//strLng.Format("%.5f",bd.Lon);
							//strLat.Format("%.5f",bd.Lat);
						strLng.Format("%.5f",demo.Lon);
						strLat.Format("%.5f",demo.Lat);
						strData = strLng+","+strLat;
*/
						//	if(nItemCount==0)
							//	pMainDlg->m_webPage.CallJScript("setMapCenter",strLng,strLat);



						strData = str4+","+str5+","+strContent+","+strIcon[0];
			//m_webPage.CallJScript("addMarker",str4,str5);AddLocation
						pMainDlg->m_webPage.CallJScript("AddLocation",strData);



						//	pMainDlg->m_webPage.CallJScript("AddLocation",strData);
						
					}

					else if(mod==0)
					{
						strData = str4+","+str5+","+strContent+","+strIcon[0];
	
						pMainDlg->m_webPage.CallJScript("AddLocation",strData);

					}
					//	total_count++;	
				
					nItemCount++; 
				}
			

			m_pAdoRecordset->MoveNext ();
		}
	}

	if(nItemCount>0)
	{
		pMainDlg->m_webPage.CallJScript("ShowLocations");
		pMainDlg->m_webPage.CallJScript("MoveTo",str4,str5);
	}

	m_pAdoRecordset->Close();
	
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;
	/*pMainDlg->m_webPage.CallJScript("ClearLocations");
	m_nIdx = 0;
	UpdateData(FALSE);
	

	CString strLng,strLat;
	m_list.DeleteAllItems();
	s.Replace(":","_");
	
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	
	
	CString temp_file1 = strAppPath+"\\temp\\"+s+".db";


	CString str1,str2,str3,str4,str5,str6="0",str7,strSQL;
	CString str11,str12,str13;
	CString str21,str22,str23;
	CString str31,str32,str33;
	CString str41,str42,str43;
	CString str51,str52,str53;
	CString str61,str62,str63;
	CString str71,str72,str73;
	CString str81,str82,str83;

	int nfilesize;    //文件大小
	int nNumCnt = 0;						
	int i = 0;
	int count = 0;

	int total_count=0;
	FILE*fp;
	TSaveData sd;
		
	if ((fp=fopen(temp_file1,"rb"))==NULL)
	{
		AfxMessageBox("打开文件失败!");
					
		return ;
				
	}
				           
	fseek(fp,0L,SEEK_END);
	nfilesize=ftell(fp);
	fseek(fp,0L,SEEK_SET);

	if(nfilesize<=0)//已经加了SN，原为54
	{
			AfxMessageBox("数据文件内无数据!");
		
			fclose(fp);
			fp=NULL;
		
			return ;
		}

		count = nfilesize/sizeof(TSaveData);
		CString strCount;
		strCount.Format("有数据%d条",count);
		AfxMessageBox(strCount);*/
/*		BYTE buf[256];

		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;
		int cnt = 0;
		for(cnt=0;cnt<nfilesize/sizeof(TSaveData);cnt++)
		{
			count = fread( buf, 1,sizeof(TSaveData), fp );

			if(count== sizeof(TSaveData))
			{
				memset(&sd,0,sizeof(TSaveData));
				memcpy(&sd,buf, sizeof(TSaveData));
				str11.Format("%d",sd.ni[0].lac);
				str12.Format("%d",sd.ni[0].cid);
				str13.Format("%d",sd.ni[0].bsid);

				str21.Format("%d",sd.ni[1].lac);
				str22.Format("%d",sd.ni[1].cid);
				str23.Format("%d",sd.ni[1].bsid);

				str31.Format("%d",sd.ni[2].lac);
				str32.Format("%d",sd.ni[2].cid);
				str33.Format("%d",sd.ni[2].bsid);

				str41.Format("%d",sd.ni[3].lac);
				str42.Format("%d",sd.ni[3].cid);
				str43.Format("%d",sd.ni[3].bsid);

				str51.Format("%d",sd.ni[4].lac);
				str52.Format("%d",sd.ni[4].cid);
				str53.Format("%d",sd.ni[4].bsid);

				str61.Format("%d",sd.ni[5].lac);
				str62.Format("%d",(sd.ni[5].bsid*0x10000+sd.ni[5].cid));
				str63.Format("%d",sd.ni[5].bsid);

				str71.Format("%d",sd.ni[6].lac);
				str72.Format("%d",(sd.ni[6].bsid*0x10000+sd.ni[6].cid));
				str73.Format("%d",sd.ni[6].bsid);

				str81.Format("%d",sd.ni[7].lac);
				str82.Format("%d",(sd.ni[7].bsid*0x10000+sd.ni[7].cid));
				str83.Format("%d",sd.ni[7].bsid);


				CTime tmm(sd.time1);
				
				str7 = tmm.Format("%y-%m-%d %H:%M:%S");
//				if(str7.Left(2)!="15")
//					continue;
				double long1 = sd.base_long;
				str4.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
				double lat1 = sd.base_lat;
				str5.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);
		
				m_list.InsertItem(total_count,"");
				m_list.SetItemText(total_count,0,str7);
				m_list.SetItemText(total_count,1,str4);
				m_list.SetItemText(total_count,2,str5);
				m_list.SetItemText(total_count,3,str11);
				m_list.SetItemText(total_count,4,str12);
				m_list.SetItemText(total_count,5,str21);
				m_list.SetItemText(total_count,6,str22);
				m_list.SetItemText(total_count,7,str31);
				m_list.SetItemText(total_count,8,str33);
				m_list.SetItemText(total_count,9,str41);
				m_list.SetItemText(total_count,10,str42);
				m_list.SetItemText(total_count,11,str51);
				m_list.SetItemText(total_count,12,str52);
				m_list.SetItemText(total_count,13,str61);
				m_list.SetItemText(total_count,14,str62);
				m_list.SetItemText(total_count,15,str71);
				m_list.SetItemText(total_count,16,str72);
				m_list.SetItemText(total_count,17,str81);
				m_list.SetItemText(total_count,18,str82);

				mark_flag = 0;


				g1 = "<tr align=\"center\">  <td>移动GSM</td> <td>"+str11+"</td> <td>"+str12+"</td> <td>#</td> </tr>";
				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td>#</td><td>"+str33+"</td> </tr>";
				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
				//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+g2+g3+g4+g5+g6+g7+g8+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));

				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[0];
				

				if(total_count == 0)
				{
					strLng = strlong2;
					strLat = strlat2;
				}
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;		
			}
			else
			{
				break;

			}
		}

		if(fp)
		{
			
			fclose(fp);fp=NULL;
		}
	
		pMainDlg->m_webPage.CallJScript("MoveTo", strLng,strLat );
		if(total_count>0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");
*/


	
}

void CDlgRst::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		
		/*CString g1 = "<tr align=\"center\">  <td>移动GSM</td> <td>"+m_list.GetItemText(pNMListView->iItem,3)+"</td> <td>"+m_list.GetItemText(pNMListView->iItem,4)+"</td> <td>#</td> </tr>";
		CString g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+m_list.GetItemText(pNMListView->iItem,5)+"</td> <td>"+m_list.GetItemText(pNMListView->iItem,6)+"</td> <td>#</td> </tr>";
		CString g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+m_list.GetItemText(pNMListView->iItem,7)+"</td> <td>"+m_list.GetItemText(pNMListView->iItem,8)+"</td> <td>#</td> </tr>";
		CString g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+m_list.GetItemText(pNMListView->iItem,9)+"</td> <td>"+m_list.GetItemText(pNMListView->iItem,10)+"</td> <td>#</td> </tr>";
		CString g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+m_list.GetItemText(pNMListView->iItem,11)+"</td> <td>"+m_list.GetItemText(pNMListView->iItem,12)+"</td> <td>#</td> </tr>";
		CString g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+m_list.GetItemText(pNMListView->iItem,13)+"</td> <td>#</td> <td>#</td> </tr>";
		CString g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+m_list.GetItemText(pNMListView->iItem,14)+"</td> <td>#</td> <td>#</td> </tr>";
		CString g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+m_list.GetItemText(pNMListView->iItem,15)+"</td> <td>#</td> <td>#</td> </tr>";
		CString g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+m_list.GetItemText(pNMListView->iItem,0)+"</td><td> </td> <td> </td> </tr>";
		CString strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
		strContent+=g1+g2+g3+g4+g5+g6+g7+g8+"</table>";
		strContent+="<h1 align=\"right\">      时间:"+m_list.GetItemText(pNMListView->iItem,0)+"</h1>";
		*/
		Point * p =  wgs_gcj_encrypts(atof(m_list.GetItemText(pNMListView->iItem,2)), atof(m_list.GetItemText(pNMListView->iItem,1)));
				
		CString strlong2,strlat2;
		
		strlong2.Format("%f", p->lng );
		strlat2.Format("%f", p->lat );
	
	//	AfxMessageBox(strlong2+" "+strlat2);
	//	CString strData = strlong2+","+strlat2+","+strContent+","+strIcon[0];

		pMainDlg->m_webPage.CallJScript("ShowCurPoint",strlong2,strlat2);
	}
	*pResult = 0;
}

HBRUSH CDlgRst::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_RADIO1)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs.GetSafeHandle();
	}
	
	if(pWnd->GetDlgCtrlID()==IDC_RADIO4)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs2.GetSafeHandle();
	}
	if(pWnd->GetDlgCtrlID()==IDC_RADIO5)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs3.GetSafeHandle();
	}
	if(pWnd->GetDlgCtrlID()==IDC_RADIO6)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs4.GetSafeHandle();
	}
	if(pWnd->GetDlgCtrlID()==IDC_RADIO7)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs5.GetSafeHandle();
	}

	if(pWnd->GetDlgCtrlID()==IDC_RADIO8)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs6.GetSafeHandle();
	}

	if(pWnd->GetDlgCtrlID()==IDC_RADIO9)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs7.GetSafeHandle();
	}

	if(pWnd->GetDlgCtrlID()==IDC_RADIO10)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs8.GetSafeHandle();
	}

	if(pWnd->GetDlgCtrlID()==IDC_RADIO12)
	{
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)m_brs9.GetSafeHandle();
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


int CDlgRst::IsCidInArr(CString s)
{
	int m;
	for(int a=0;a<7;a++)
	{

		m = m_arrCID[a].GetSize();
		if(m>0)
		{
			for(int b=0;b<m;b++)
			{
				if(m_arrCID[a].GetAt(b) == s)
				{
					return a;
				}
			}
		}
	}
	return 7;
	
}

int CDlgRst::AddCidToArr(CString s)
{
	m_arrCID[m_nArrAdd].Add(s);
	m_nArrAdd=(m_nArrAdd+1)%7;
	return  (m_nArrAdd+7 -1)%7;

}
void CDlgRst::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int n = m_list.GetItemCount();


	if(n>0)
	{
		
		if(m_nOldIndex!=0)
		{
			
			m_list.SetItemBkColor(-1,m_nOldIndex*2+1,RGB(255,255,255));
			m_list.SetItemBkColor(-1,m_nOldIndex*2+2,RGB(255,255,255));
		}
		m_list.SetItemBkColor(-1,3,RGB(180,180,180));
		m_list.SetItemBkColor(-1,4,RGB(180,180,180));


		pMainDlg->m_webPage.CallJScript("ClearLocations");
	

		for(int z=0;z<7;z++)
			m_arrCID[z].RemoveAll();
		m_nArrAdd = 0;
		CString str1,str2,str3,str4,str5,str6="0",str7;
		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;

		int total_count=0;
		for(int i=0;i<n;i++)
		{
			str4 = m_list.GetItemText(i,1);
			str5 = m_list.GetItemText(i,2);
			str7 = m_list.GetItemText(i,0);

			str1 = m_list.GetItemText(i,3);
			str2 = m_list.GetItemText(i,4);
			
			if(str2!="" && str2!="0")
			{

			
				int cid_pos=7;
				if((cid_pos = IsCidInArr(str2))>=7) //no
					cid_pos = AddCidToArr(str2);
				

				g1 = "<tr align=\"center\">  <td>移动GSM</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>#</td> </tr>";
				
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
				
				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[cid_pos+3];
				
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;
			
			}
		}
		
		
//				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
//				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td></td>#<td>"+str33+"</td> </tr>";
//				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
//				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
//				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
//				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
//				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
				//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
	
		if(total_count>0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");
	}
	m_nOldIndex = m_nIdx;

}

void CDlgRst::OnRadio5() 
{UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	int n = m_list.GetItemCount();
	if(n>0)
	{
		if(m_nOldIndex!=0)
		{
			
			m_list.SetItemBkColor(-1,m_nOldIndex*2+1,RGB(255,255,255));
			m_list.SetItemBkColor(-1,m_nOldIndex*2+2,RGB(255,255,255));
		}
		
		m_list.SetItemBkColor(-1,5,RGB(180,180,180));
		m_list.SetItemBkColor(-1,6,RGB(180,180,180));

		pMainDlg->m_webPage.CallJScript("ClearLocations");
		
		
		for(int z=0;z<7;z++)
			m_arrCID[z].RemoveAll();
		m_nArrAdd = 0;
		CString str1,str2,str3,str4,str5,str6="0",str7;
		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;
		
		int total_count=0;
		for(int i=0;i<n;i++)
		{
			str4 = m_list.GetItemText(i,1);
			str5 = m_list.GetItemText(i,2);
			str7 = m_list.GetItemText(i,0);
			
			str1 = m_list.GetItemText(i,5);
			str2 = m_list.GetItemText(i,6);
			
			if(str2!="" && str2!="0")
			{
				
				
				int cid_pos=7;
				if((cid_pos = IsCidInArr(str2))>=7) //no
					cid_pos = AddCidToArr(str2);
				
				
				g1 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>#</td> </tr>";
				
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
				
				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[cid_pos+3];
				
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;
				
			}
		}
		
		
		//				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		//				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td></td>#<td>"+str33+"</td> </tr>";
		//				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		//				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		//				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		//				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		//				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
		
		if(total_count>0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");


	}
	m_nOldIndex = m_nIdx;
}

void CDlgRst::OnRadio6() 
{UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	int n = m_list.GetItemCount();
	if(n>0)
	{
		
		if(m_nOldIndex!=0)
		{
			
			m_list.SetItemBkColor(-1,m_nOldIndex*2+1,RGB(255,255,255));
			m_list.SetItemBkColor(-1,m_nOldIndex*2+2,RGB(255,255,255));
		}
		
		
		m_list.SetItemBkColor(-1,7,RGB(180,180,180));
		m_list.SetItemBkColor(-1,8,RGB(180,180,180));


		pMainDlg->m_webPage.CallJScript("ClearLocations");
		
		
		for(int z=0;z<7;z++)
			m_arrCID[z].RemoveAll();
		m_nArrAdd = 0;
		CString str1,str2,str3,str4,str5,str6="0",str7;
		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;
		
		int total_count=0;
		for(int i=0;i<n;i++)
		{
			str4 = m_list.GetItemText(i,1);
			str5 = m_list.GetItemText(i,2);
			str7 = m_list.GetItemText(i,0);
			
			str1 = m_list.GetItemText(i,7);
			str2 = m_list.GetItemText(i,8);
			
			if(str2!="" && str2!="0")
			{
				
				
				int cid_pos=7;
				if((cid_pos = IsCidInArr(str2))>=7) //no
					cid_pos = AddCidToArr(str2);
				
				
				g1 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str1+"</td> <td>#</td> <td>"+str2+"</td> </tr>";
				
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
				
				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[cid_pos+3];
				
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;
				
			}
		}
		
		
		//				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		//				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td></td>#<td>"+str33+"</td> </tr>";
		//				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		//				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		//				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		//				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		//				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
		
		if(total_count>0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");


	}m_nOldIndex = m_nIdx;
}

void CDlgRst::OnRadio7() 
{UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	int n = m_list.GetItemCount();
	if(n>0)
	{
		
		if(m_nOldIndex!=0)
		{
			
			m_list.SetItemBkColor(-1,m_nOldIndex*2+1,RGB(255,255,255));
			m_list.SetItemBkColor(-1,m_nOldIndex*2+2,RGB(255,255,255));
		}
		
		
		m_list.SetItemBkColor(-1,9,RGB(180,180,180));
		m_list.SetItemBkColor(-1,10,RGB(180,180,180));


		pMainDlg->m_webPage.CallJScript("ClearLocations");
		
		
		for(int z=0;z<7;z++)
			m_arrCID[z].RemoveAll();
		m_nArrAdd = 0;
		CString str1,str2,str3,str4,str5,str6="0",str7;
		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;
		
		int total_count=0;
		for(int i=0;i<n;i++)
		{
			str4 = m_list.GetItemText(i,1);
			str5 = m_list.GetItemText(i,2);
			str7 = m_list.GetItemText(i,0);
			
			str1 = m_list.GetItemText(i,9);
			str2 = m_list.GetItemText(i,10);
			
			if(str2!="" && str2!="0")
			{
				
				
				int cid_pos=7;
				if((cid_pos = IsCidInArr(str2))>=7) //no
					cid_pos = AddCidToArr(str2);
				
				
				g1 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str1+"</td><td>"+str2+"</td> <td>#</td> </tr>";
				
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
				
				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[cid_pos+3];
				
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;
				
			}
		}
		
		
		//				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		//				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td></td>#<td>"+str33+"</td> </tr>";
		//				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		//				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		//				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		//				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		//				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
		
		if(total_count>0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");

	}m_nOldIndex = m_nIdx;
}

void CDlgRst::OnRadio8() 
{UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	int n = m_list.GetItemCount();
	if(n>0)
	{
		
		if(m_nOldIndex!=0)
		{
			
			m_list.SetItemBkColor(-1,m_nOldIndex*2+1,RGB(255,255,255));
			m_list.SetItemBkColor(-1,m_nOldIndex*2+2,RGB(255,255,255));
		}
		
		
		m_list.SetItemBkColor(-1,11,RGB(180,180,180));
		m_list.SetItemBkColor(-1,12,RGB(180,180,180));

		pMainDlg->m_webPage.CallJScript("ClearLocations");
		
		
		for(int z=0;z<7;z++)
			m_arrCID[z].RemoveAll();
		m_nArrAdd = 0;
		CString str1,str2,str3,str4,str5,str6="0",str7;
		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;
		
		int total_count=0;
		for(int i=0;i<n;i++)
		{
			str4 = m_list.GetItemText(i,1);
			str5 = m_list.GetItemText(i,2);
			str7 = m_list.GetItemText(i,0);
			
			str1 = m_list.GetItemText(i,11);
			str2 = m_list.GetItemText(i,12);
			
			if(str2!="" && str2!="0")
			{
				
				
				int cid_pos=7;
				if((cid_pos = IsCidInArr(str2))>=7) //no
					cid_pos = AddCidToArr(str2);
				
				
				g1 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>#</td> </tr>";
				
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
				
				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[cid_pos+3];
				
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;
				
			}
		}
		
		
		//				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		//				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td></td>#<td>"+str33+"</td> </tr>";
		//				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		//				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		//				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		//				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		//				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
		
		if(total_count>0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");

	}m_nOldIndex = m_nIdx;
}

void CDlgRst::OnRadio9() 
{UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	int n = m_list.GetItemCount();
	if(n>0)
	{
		
		if(m_nOldIndex!=0)
		{
			
			m_list.SetItemBkColor(-1,m_nOldIndex*2+1,RGB(255,255,255));
			m_list.SetItemBkColor(-1,m_nOldIndex*2+2,RGB(255,255,255));
		}
		
		
		m_list.SetItemBkColor(-1,13,RGB(180,180,180));
		m_list.SetItemBkColor(-1,14,RGB(180,180,180));


		pMainDlg->m_webPage.CallJScript("ClearLocations");
		
		
		for(int z=0;z<7;z++)
			m_arrCID[z].RemoveAll();
		m_nArrAdd = 0;
		CString str1,str2,str3,str4,str5,str6="0",str7;
		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;
		
		int total_count=0;
		for(int i=0;i<n;i++)
		{
			str4 = m_list.GetItemText(i,1);
			str5 = m_list.GetItemText(i,2);
			str7 = m_list.GetItemText(i,0);
			
			str1 = m_list.GetItemText(i,13);
			str2 = m_list.GetItemText(i,14);
			
			if(str2!="" && str2!="0")
			{
				
				
				int cid_pos=7;
				if((cid_pos = IsCidInArr(str2))>=7) //no
					cid_pos = AddCidToArr(str2);
				
				
				g1 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>#</td> </tr>";
				
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
				
				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[cid_pos+3];
				
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;
				
			}
		}
		
		
		//				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		//				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td></td>#<td>"+str33+"</td> </tr>";
		//				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		//				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		//				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		//				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		//				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
		
		if(total_count>0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");



	}m_nOldIndex = m_nIdx;
}

void CDlgRst::OnRadio10() 
{UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	int n = m_list.GetItemCount();
	if(n>0)
	{
		
		if(m_nOldIndex!=0)
		{
			
			m_list.SetItemBkColor(-1,m_nOldIndex*2+1,RGB(255,255,255));
			m_list.SetItemBkColor(-1,m_nOldIndex*2+2,RGB(255,255,255));
		}
		
		
		m_list.SetItemBkColor(-1,15,RGB(180,180,180));
		m_list.SetItemBkColor(-1,16,RGB(180,180,180));

		pMainDlg->m_webPage.CallJScript("ClearLocations");
		
		
		for(int z=0;z<7;z++)
			m_arrCID[z].RemoveAll();
		m_nArrAdd = 0;
		CString str1,str2,str3,str4,str5,str6="0",str7;
		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;
		
		int total_count=0;
		for(int i=0;i<n;i++)
		{
			str4 = m_list.GetItemText(i,1);
			str5 = m_list.GetItemText(i,2);
			str7 = m_list.GetItemText(i,0);
			
			str1 = m_list.GetItemText(i,15);
			str2 = m_list.GetItemText(i,16);
			
			if(str2!="" && str2!="0")
			{
				
				
				int cid_pos=7;
				if((cid_pos = IsCidInArr(str2))>=7) //no
					cid_pos = AddCidToArr(str2);
				
				
				g1 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>#</td> </tr>";
				
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
				
				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[cid_pos+3];
				
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;
				
			}
		}
		
		
		//				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		//				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td></td>#<td>"+str33+"</td> </tr>";
		//				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		//				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		//				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		//				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		//				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
		
		if(total_count>0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");



	}m_nOldIndex = m_nIdx;
}

void CDlgRst::OnRadio12() 
{UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	int n = m_list.GetItemCount();
	if(n>0)
	{
		
		if(m_nOldIndex!=0)
		{
			
			m_list.SetItemBkColor(-1,m_nOldIndex*2+1,RGB(255,255,255));
			m_list.SetItemBkColor(-1,m_nOldIndex*2+2,RGB(255,255,255));
		}
		
		
		m_list.SetItemBkColor(-1,17,RGB(180,180,180));
		m_list.SetItemBkColor(-1,18,RGB(180,180,180));


		pMainDlg->m_webPage.CallJScript("ClearLocations");
		
		
		for(int z=0;z<7;z++)
			m_arrCID[z].RemoveAll();
		m_nArrAdd = 0;
		CString str1,str2,str3,str4,str5,str6="0",str7;
		CString strData="";
		CString g1,g2,g3,g4,g5,g6,g7,g8,g9,strContent;
		CString strlong2,strlat2;
		
		int total_count=0;
		for(int i=0;i<n;i++)
		{
			str4 = m_list.GetItemText(i,1);
			str5 = m_list.GetItemText(i,2);
			str7 = m_list.GetItemText(i,0);
			
			str1 = m_list.GetItemText(i,17);
			str2 = m_list.GetItemText(i,18);
			
			if(str2!="" && str2!="0")
			{
				
				
				int cid_pos=7;
				if((cid_pos = IsCidInArr(str2))>=7) //no
					cid_pos = AddCidToArr(str2);
				
				
				g1 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str1+"</td> <td>"+str2+"</td> <td>#</td> </tr>";
				
				strContent = "<table width=\"300\" align=\"center\" border=\"1\" cellspacing=\"0\"> <tr bgcolor=\"#A7DCE0\"> <th>网络制式</th><th>LAC(SID)</th><th>CID(NID)</th> <th>(BSID)</th> </tr>";
				strContent+=g1+"</table>";
				strContent+="<h1 align=\"right\">      时间:"+str7+"</h1>";
				
				Point * p =  wgs_gcj_encrypts(atof(str5), atof(str4));
				
				strlong2.Format("%f", p->lng );
				strlat2.Format("%f", p->lat );
				strData = strlong2+","+strlat2+","+strContent+","+strIcon[cid_pos+3];
				
				pMainDlg->m_webPage.CallJScript("AddLocation",strData);
				total_count++;
				
			}
		}
		
		
		//				g2 = "<tr align=\"center\">  <td>联通GSM</td> <td>"+str21+"</td> <td>"+str22+"</td> <td>#</td> </tr>";
		//				g3 = "<tr align=\"center\">  <td>电信CDMA</td> <td>"+str31+"</td> <td></td>#<td>"+str33+"</td> </tr>";
		//				g4 = "<tr align=\"center\">  <td>联通WCDMA</td> <td>"+str41+"</td> <td>"+str42+"</td> <td>#</td> </tr>";
		//				g5 = "<tr align=\"center\">  <td>移动TD</td> <td>"+str51+"</td> <td>"+str52+"</td> <td>#</td> </tr>";
		//				g6 = "<tr align=\"center\">  <td>移动LTE</td> <td>"+str61+"</td> <td>"+str62+"</td> <td>#</td> </tr>";
		//				g7 = "<tr align=\"center\">  <td>联通LTE</td> <td>"+str71+"</td> <td>"+str72+"</td> <td>#</td> </tr>";
		//				g8 = "<tr align=\"center\">  <td>电信LTE</td> <td>"+str81+"</td> <td>"+str82+"</td> <td>#</td> </tr>";
		//g9 = "<tr bgcolor=\"#D0A0A0\ align=\"center\">  <td>时间</td> <td>"+str7+"</td><td> </td> <td> </td> </tr>";
		
		if(total_count>=0)
			pMainDlg->m_webPage.CallJScript("ShowLocations");



	}
	
	m_nOldIndex = m_nIdx;
}

void CDlgRst::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fdlg(FALSE,".xls","result_data",OFN_OVERWRITEPROMPT,"Worksheet Files (*.xls)|*.xls|Chart Files (*.xlc)|*.xlc|Data Files (*.xlc;*.xls)|*.xlc; *.xls|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CString filename="";
	if(fdlg.DoModal()==IDOK)
	{
		filename=fdlg.GetPathName();
	}
	else
		return;
	
	//	MessageBox(filename);
	if(filename.Find(".xls")>0||filename.Find(".xlc")>0||filename.Find(".xlsx")>0)
	{
		if(ExportToExcel(filename))
		{
			
			AfxMessageBox("导出Excel成功!");
		}
	}
	else if(filename.Find(".txt")>0)
	{
		if(ExportToTxt(filename))
			AfxMessageBox("导出Txt成功!");
		}
}


bool CDlgRst::ExportToExcel(CString filename)
{
	CMyExcel my;
	int i=0;
	int j=0;
	my.Open();
	my.AddSheet("cell_data");
	//m_listOpRst.GetHeaderText();
	my.SetColumnWidth(150);
	
	int columnCount=m_list.GetColumnCount();
	
	
	MyNumberFormat Xnumber;
	Xnumber.GetText();
	my.SetNumberFormat(Xnumber);
	for(i=1;i<=columnCount;i++)
	{
		my.SetItemText(1,i,m_list.GetHeaderText(i-1));
	}
	
	int countRows=	m_list.GetItemCount();
	
	for(i=0;i<countRows;i++)
	{
		
			for(j=0;j<columnCount;j++)
			{
				CString tmp=m_list.GetItemText(i,j);
				my.SetItemText(i+2,j+1,tmp);
			}
			
		
		
	}
	


	my.SaveAs(filename);
	return true;
}

bool CDlgRst::ExportToTxt(CString filename)
{
	
	
	int i=0;
	int j=0;
	
	ofstream fs;
	fs.open(filename);
	
	int columnCount=m_list.GetColumnCount();
	
	char* seperator="		";
	for(i=1;i<=columnCount;i++)
	{
		fs<<m_list.GetHeaderText(i-1).GetBuffer(0)<<seperator;
	}
	
	fs<<endl;
	int countRows=	m_list.GetItemCount();
	for(i=0;i<countRows;i++)
	{
		if(m_list.GetCheck(i)==TRUE)
		{
			
			for(j=0;j<columnCount;j++)
			{
				fs<<m_list.GetItemText(i,j).GetBuffer(0)<<seperator;
			}
			
			fs<<endl;
		}
	}
	
	fs.close();
	
	return true;
}

void CDlgRst::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(mark_flag==1)
	{

		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		
		if(pNMListView->iItem != -1)
		{
			
			m_nDelItem = pNMListView->iItem;
			
			LPPOINT lpoint=new tagPOINT;
			::GetCursorPos(lpoint);//得到鼠标位置
			CMenu *menu;
			CMenu PopMenu;
			PopMenu.LoadMenu(IDR_MENU3);
			menu = PopMenu.GetSubMenu (0);
			
			menu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
			//资源回收
			HMENU hmenu=menu->Detach();
			menu->DestroyMenu();
			delete lpoint;
			
		}
	}
	*pResult = 0;
}

void CDlgRst::OnMenuDelAll() 
{
	// TODO: Add your command handler code here
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	
	strDBPath += "\\database.mdb";
	
	
	
	
	
	
	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
		
	}
	else
	{
		AfxMessageBox("数据库连接失败!");
		delete m_pAdoRecordset;
		delete m_pAdoConn;
		return;
	}
	
	m_pAdoConn->BeginTrans();
	
	CString strSQL = "DELETE * FROM mark";
	m_pAdoRecordset->Open(strSQL);
	
	
	m_pAdoRecordset->Update();
	m_pAdoRecordset->Close();
	
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;
	
	
	m_list.DeleteAllItems();
	
	pMainDlg->m_webPage.CallJScript("ClearLocations");
}

void CDlgRst::OnMenuDelCur() 
{
	// TODO: Add your command handler code here
	CString str7 = m_list.GetItemText(m_nDelItem,0);
	
	int y = 2000+atoi(str7.Left(2));
				int m = atoi(str7.Mid(3,2));
				int d = atoi(str7.Mid(6,2));
				int H = atoi(str7.Mid(9,2));
				int M = atoi(str7.Mid(12,2));
				int S = atoi(str7.Mid(15,2));
				CTime tmm(y,m,d,H,M,S);
				
				str7.Format("%d",tmm.GetTime());

	
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	
	strDBPath += "\\database.mdb";
	
	
	CAdoConnection * m_pAdoConn;
	CAdoRecordSet * m_pAdoRecordset;
	m_pAdoConn = new CAdoConnection;
	m_pAdoRecordset = new CAdoRecordSet;
	if (m_pAdoConn->ConnectAccess(strDBPath))
	{
		m_pAdoRecordset->SetAdoConnection(m_pAdoConn);
		m_pAdoRecordset->SetCursorLocation();
		
	}
	else
	{
		AfxMessageBox("数据库连接失败!");
		delete m_pAdoRecordset;
		delete m_pAdoConn;
		return;
	}
	
	m_pAdoConn->BeginTrans();
	
	CString strSQL = "DELETE * FROM mark WHERE time="+str7;
//	AfxMessageBox(strSQL);
	m_pAdoRecordset->Open(strSQL);
		
	
	m_pAdoRecordset->Update();
	m_pAdoRecordset->Close();
	
	m_pAdoConn->CommitTrans();
	m_pAdoConn->Close();
	delete m_pAdoRecordset;
	delete m_pAdoConn;

	
	m_list.DeleteItem(m_nDelItem);

}
