// DlgQry.cpp : implementation file
//

#include "stdafx.h"
#include "CellLocatiom.h"
#include "DlgQry.h"
#include "CellLocatiomDlg.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQry dialog

static double x_pi = 3.14159265358979324 * 3000.0 / 180.0;
static double pi = 3.14159265358979324;
static double a = 6378245.0;
static double ee = 0.00669342162296594323;
extern CString strIcon[10];
extern int link_flag;
BOOL outOfChina(double lat, double lon) {
	if (lon < 72.004 || lon > 137.8347)
		return TRUE;
	if (lat < 0.8293 || lat > 55.8271)
		return TRUE;
	return FALSE;
}

double transformLat(double x, double y) {
	double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
	ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
	return ret;
}

double transformLon(double x, double y) {
	double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
	ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
	return ret;
}


Point * wgs_gcj_encrypts(double wgLat, double wgLon) {
	Point *point=new Point();
	if (outOfChina(wgLat, wgLon)) {
		point->setLat(wgLat);
		point->setLng(wgLon);
		return point;
	}
	double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
	double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
	double radLat = wgLat / 180.0 * pi;
	double magic = sin(radLat);
	magic = 1 - ee * magic * magic;
	double sqrtMagic = sqrt(magic);
	dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
	dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
	double lat = wgLat + dLat;
	double lon = wgLon + dLon;
	point->setLat(lat);
	point->setLng(lon);
	return point;
}

Point * wgs_gcj_encrypts_fan(double wgLat, double wgLon) {
	Point *point=new Point();
	if (outOfChina(wgLat, wgLon)) {
		point->setLat(wgLat);
		point->setLng(wgLon);
		return point;
	}
	double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
	double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
	double radLat = wgLat / 180.0 * pi;
	double magic = sin(radLat);
	magic = 1 - ee * magic * magic;
	double sqrtMagic = sqrt(magic);
	dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
	dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
	double lat = wgLat + dLat;
	double lon = wgLon + dLon;
	point->setLat(lat);
	point->setLng(lon);
	return point;
}


Point *  gcj_wgs_decrypt(double gcjLat, double  gcjLon) {

						CString dy;

						double initDelta = 0.01;
						double threshold = 0.000000001;
						double dLat = initDelta;
						double dLon = initDelta;
						double mLat = gcjLat - dLat;
						double mLon = gcjLon - dLon;
						double pLat = gcjLat + dLat;
						double pLon = gcjLon + dLon;
						double wgsLat, wgsLon;
						int i = 0;
						Point *point=new Point();
						while (1) {
							wgsLat = (mLat + pLat) / 2;
							wgsLon = (mLon + pLon) / 2;
							Point* tmp = wgs_gcj_encrypts(wgsLat, wgsLon);
							dLat = tmp->getLat() - gcjLat;
							dLon = tmp->getLng() - gcjLon;
							//dy.Format("%f  %f",fabs(dLat),fabs(dLon));
							//AfxMessageBox(dy);

							if ((fabs(dLat) < threshold) && (fabs(dLon) < threshold))
							{
								//AfxMessageBox("xx");
								break;
							}
							
							if (dLat > 0) pLat = wgsLat; else mLat = wgsLat;
							if (dLon > 0) pLon = wgsLon; else mLon = wgsLon;
							
							if (++i > 10000) break;
						}
						//console.log(i);

						point->setLat(wgsLat);
						point->setLng(wgsLon);
						return point;

						//return {'lat': wgsLat, 'lon': wgsLon};
    }


struct Loc
{
	double Lon;			//坐标的经度
	double Lat;			//坐标的纬度
};
 
Loc bd_encrypt(Loc gg)//将谷歌坐标转换为百度坐标
{
	Loc bd;
    double x = gg.Lon, y = gg.Lat;
	double z = sqrt(x * x + y * y) + 0.00002 * sin(y * x_pi);
	double theta = atan2(y, x) + 0.000003 * cos(x * x_pi);
	bd.Lon = z * cos(theta) + 0.0065;
	bd.Lat = z * sin(theta) + 0.006;
	return bd;
}
 
Loc transform(Loc gps)
{
	Loc gg;
    double dLat = transformLat(gps.Lon - 105.0, gps.Lat - 35.0);
    double dLon = transformLon(gps.Lon - 105.0, gps.Lat - 35.0);
    double radLat = gps.Lat / 180.0 * pi;
    double magic = sin(radLat);
    magic = 1 - ee * magic * magic;
    double sqrtMagic = sqrt(magic);
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
    dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
    gg.Lat = gps.Lat + dLat;
    gg.Lon = gps.Lon + dLon;
	return gg;
}

extern CCellLocatiomDlg * pMainDlg;
CDlgQry::CDlgQry(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQry::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgQry)
	m_nIndex = 1;
	m_nComboSel = 0;
	m_nlac = 0;
	m_ncid = 0;
	m_dLng = 0.0;
	m_nQid = -1;
	m_dLat = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgQry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgQry)
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Control(pDX, IDC_BUTTON10, m_btnExp);
	DDX_Control(pDX, IDC_EDIT3, m_edtRst);
	DDX_Control(pDX, IDC_COMBO1, m_cbSel);
	DDX_Control(pDX, IDC_STATIC_CUR, m_stCur);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_tmPk2);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_tmPk1);
	DDX_Radio(pDX, IDC_RADIO1, m_nIndex);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nComboSel);
	DDX_Text(pDX, IDC_EDIT1, m_nlac);
	DDX_Text(pDX, IDC_EDIT2, m_ncid);
	DDX_Text(pDX, IDC_EDIT4, m_dLng);
	DDX_Text(pDX, IDC_EDIT5, m_dLat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQry, CDialog)
	//{{AFX_MSG_MAP(CDlgQry)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON2, OnBtnQry)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON7, OnBtnGetSurnd)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON10, OnBtnExport)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, OnRclickList2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_COMMAND(ID_MENU_DEL_ALL, OnMenuDelAll)
	ON_COMMAND(ID_MENU_DEL_CUR, OnMenuDelCur)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQry message handlers

BOOL CDlgQry::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_font.CreateFont(20, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "微软雅黑");

	GetDlgItem(IDC_STATIC_1)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_2)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_3)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_4)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_5)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_6)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_7)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_HIS)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_GPS)->SetFont(&m_font);
	GetDlgItem(IDC_LIST2)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_CUR)->SetFont(&m_font);

	GetDlgItem(IDC_RADIO1)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO2)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO3)->SetFont(&m_font);
	GetDlgItem(IDC_RADIO11)->SetFont(&m_font);

	GetDlgItem(IDC_EDIT1)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT2)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT3)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT4)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT5)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON10)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON16)->SetFont(&m_font);

	GetDlgItem(IDC_COMBO1)->SetFont(&m_font);

	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON16)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	

	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE  );
	m_list.SetColumnHeader(_T("时间, 120;内容,130;remark,0;类型,73"));
	m_list.SetGridLines(TRUE);
	m_list.SetSortable(FALSE);


	m_tmPk1.SetFont(&m_font);
	m_tmPk2.SetFont(&m_font);
	m_tmPk1.SetFormat("yyyy-MM-dd HH:mm:ss");
	m_tmPk2.SetFormat("yyyy-MM-dd HH:mm:ss");

	LoadQryRecord();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgQry::OnButton3() 
{

	CString stt;
	stt.Format("%d",sizeof(TRealtimeData));
	AfxMessageBox(stt);

	return;
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strDBPath = strAppPath;
	int id; 
	CString tmp_file = strAppPath+"\\temp\\data.temp";
	CString temp_file1 = strAppPath+"\\temp\\data_save.temp";
	
	
	MoveFileEx(tmp_file,temp_file1,MOVEFILE_REPLACE_EXISTING);
	DeleteFile(tmp_file);

	return;
	CString strlong2,strlat2;
	strlong2.Format("%f", 117.04 );
	strlat2.Format("%f",  36.634);
	
	pMainDlg->m_webPage.CallJScript("CenterToPoint",strlong2,strlat2,strIcon[2]);


	// TODO: Add your control notification handler code here
	//pMainDlg->m_webPage.CallJScript("drawline");
//pMainDlg->m_webPage.CallJScript("GetLngLat");//GetLngLat
//	pMainDlg->m_webPage.CallJScript("FindPoint", "新街口");

//	pMainDlg->m_webPage.CallJScript("GetCurZoom");

//	pMainDlg->m_webPage.CallJScript("ClearMarkerEvent");
}


void CDlgQry::OnButton4() 
{
	// TODO: Add your control notification handler code here
	double m_dbLng,m_dbLat;

	CString str4 = "11843.6145";
	CString str5 = "3200.1138";
	double long1 = atof(str4);
	str4.Format("%f",((int)long1)/100+  (long1-((int)long1)/100*100)/60.0);
	double lat1 = atof(str5);
	str5.Format("%f",((int)lat1)/100+  (lat1-((int)lat1)/100*100)/60.0);

	AfxMessageBox(str4 + " " + str5);
//	return;

//	CString strLat = "32.001266",strLng="118.436145";


	CString strLat = "32.0000211",strLng="118.726908";
	//strLng.Format("%lf",m_dbLng);
	//strLat.Format("%lf",m_dbLat);
	double lng = atof(str4);//118.726908;
	double lat = atof(str5);//32.00211;
	Point * p =  wgs_gcj_encrypts(lat, lng);
	CString str;
	str.Format("%f   %f", p->lng, p->lat);
//	AfxMessageBox(str);
	strLng.Format("%f", p->lng );
	strLat.Format("%f", p->lat );
	pMainDlg->m_webPage.CallJScript("MoveTo", strLng,strLat );
}

void CDlgQry::OnButton5() 
{


	CString strLat = "31.5989",strLng="118.4409";
	






	double lng = atof(strLng);//118.726908;
	double lat = atof(strLat);//32.00211;
	Point * p =  wgs_gcj_encrypts(lat, lng);
	CString str,str1;
	str.Format("%f   %f", p->lng, p->lat);
	AfxMessageBox(str);


	Point*pp = gcj_wgs_decrypt(p->lat,p->lng);

	str1.Format("%f   %f", pp->lng, pp->lat);
	AfxMessageBox(str1);


//	pMainDlg->m_webPage.CallJScript("MarkerIcon");//GetLngLat
	
}

void CDlgQry::OnButton6() 
{
	// TODO: Add your control notification handler code here
	pMainDlg->m_webPage.CallJScript("ClearLocations");
}

void CDlgQry::OnBtnQry() 
{
	// TODO: Add your control notification handler code here

//	pMainDlg->m_webPage.CallJScript("ClearLocations");

	//if(link_flag==0)
//	{
//		AfxMessageBox("此功能需要联网获取百度地图支持");
//		return;
//	}
	
	UpdateData(TRUE);
	if(m_ncid==0)
	{
		AfxMessageBox("CID参数不能为0");
		return;
	}
	pMainDlg->QryData(m_nComboSel,m_nlac,m_ncid);
	m_nQid=1;

	CString strContent;
	strContent.Format("%d,%d,%d",m_nComboSel+1,m_nlac,m_ncid);
	InsertToList(0,0,strContent);

}

void CDlgQry::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON16)->EnableWindow(FALSE);
}

void CDlgQry::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON16)->EnableWindow(FALSE);
}

void CDlgQry::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON16)->EnableWindow(FALSE);
}

void CDlgQry::OnButton8() 
{
	// TODO: Add your control notification handler code here
	pMainDlg->m_webPage.CallJScript("ClearLocations");
	
	
	UpdateData(TRUE);
	CTime tm1,tm2;
	  m_tmPk1.GetTime(tm1);
	  m_tmPk2.GetTime(tm2);

	if(tm1.GetTime()>=tm2.GetTime())
	{
		AfxMessageBox("时间参数错误");
		return;
	}
	
	pMainDlg->QryData2(tm1.GetTime(),tm2.GetTime());
}

void CDlgQry::OnBtnGetSurnd() 
{
	// TODO: Add your control notification handler code here
	pMainDlg->m_webPage.CallJScript("GetMapRang");
}

void CDlgQry::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	int sel = m_cbSel.GetCurSel();
	if(sel==2)
	{
		GetDlgItem(IDC_STATIC_2)->SetWindowText("SID");
		GetDlgItem(IDC_STATIC_3)->SetWindowText("BSID");
	}
	else if(sel==5 ||sel==6 ||sel==7  )
	{

		GetDlgItem(IDC_STATIC_2)->SetWindowText("TAC");
		GetDlgItem(IDC_STATIC_3)->SetWindowText("CID");
	}
	else
	{
		GetDlgItem(IDC_STATIC_2)->SetWindowText("LAC");
		GetDlgItem(IDC_STATIC_3)->SetWindowText("CID");

	}
}

void CDlgQry::OnBtnExport() 
{
	// TODO: Add your control notification handler code here
/*	CFileDialog fdlg(FALSE,".xls","data",OFN_OVERWRITEPROMPT,"Worksheet Files (*.xls)|*.xls|Chart Files (*.xlc)|*.xlc|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||");
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
			//CString showTxt;
			//showTxt.Format("导出到E成功!",filename)
			AfxMessageBox("导出Excel成功!");
		}
	}*/
	
}

void CDlgQry::OnRadio11() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
}

void CDlgQry::OnButton16() 
{
	pMainDlg->m_webPage.CallJScript("ClearLocations");
	
	
	UpdateData(TRUE);


	double lng = m_dLng;//118.726908;
	double lat = m_dLat;//32.00211;
	Point * p =  wgs_gcj_encrypts(lat, lng);
	CString strLng,strLat;
	strLng.Format("%f", p->lng );
	strLat.Format("%f", p->lat );
	pMainDlg->m_webPage.CallJScript("CenterToPoint",strLng,strLat,strIcon[1]);
//	pMainDlg->m_webPage.CallJScript("MarkerIcon");MarkToPoint CenterToPoint


	CString strContent;
	strContent.Format("%g,%g",m_dLng,m_dLat);
	InsertToList(1,0,strContent);


	
}


void CDlgQry::InsertToList(int m, int i, CString strContent)
{
	CTime tmNow = CTime::GetCurrentTime();
	CString strTime = tmNow.Format("%m_%d %H:%M:%S");
	CString strRmk,strType;
	strRmk.Format("%d",m);
	if(m==0)strType="基站";
	else strType="经纬度";

	int n = m_list.GetItemCount();
	if(n<10)
	{

		m_list.InsertItem(0,"");
		m_list.SetItemText(0,0,strTime);
		m_list.SetItemText(0,1,strContent);
		m_list.SetItemText(0,2,strRmk);
		m_list.SetItemText(0,3,strType);
		n++;
	}
	else
	{
		m_list.DeleteItem(9);
		m_list.InsertItem(0,"");
		m_list.SetItemText(0,0,strTime);
		m_list.SetItemText(0,1,strContent);
		m_list.SetItemText(0,2,strRmk);
		m_list.SetItemText(0,3,strType);
		n=10;
	}

	for(int a=0;a<n;a++)
	{
		if(a%2==0)
			m_list.SetItemBkColor(a,-1,RGB(239,239,239),TRUE);
		else
			m_list.SetItemBkColor(a,-1,RGB(255,255,255),TRUE);

	}
}
void CDlgQry::LoadQryRecord()
{
	m_list.DeleteAllItems();
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strIniPath = strAppPath;
	strIniPath += "\\query_info.ini";
	CString strTime,strInfo,strRmk;
	CString strTmp0,strTmp1,strTmp2;

	for(int i=0;i<10;i++)
	{
		strTmp0.Format("time%d",i);
		strTmp1.Format("content%d",i);
		strTmp2.Format("remark%d",i);
		
		GetPrivateProfileString("record",strTmp0,NULL,strTime.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
		GetPrivateProfileString("record",strTmp1,NULL,strInfo.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
		GetPrivateProfileString("record",strTmp2,NULL,strRmk.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
		
		if(strTime!=""&&strInfo!="")
		{
			m_list.InsertItem(i,"");
			m_list.SetItemText(i,0,strTime);
			m_list.SetItemText(i,1,strInfo);
			m_list.SetItemText(i,2,strRmk);
			if(strRmk=="0")
				m_list.SetItemText(i,3,"基站");
			else
				m_list.SetItemText(i,3,"经纬度");


			
			if(i%2==0)
				m_list.SetItemBkColor(i,-1,RGB(239,239,239),TRUE);
			else
				m_list.SetItemBkColor(i,-1,RGB(255,255,255),TRUE);
	
			
		}
		
	}

}
void CDlgQry::SaveQryRecord()
{

	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strIniPath = strAppPath;
	strIniPath += "\\query_info.ini";
	CString strTime,strInfo,strRmk;
	CString strTmp0,strTmp1,strTmp2;
	int i=0;
	int n=m_list.GetItemCount();
	for(i=0;i<n;i++)
	{
		
		strTime = m_list.GetItemText(i,0);
		strInfo = m_list.GetItemText(i,1);
		strRmk = m_list.GetItemText(i,2);
		
		strTmp0.Format("time%d",i);
		strTmp1.Format("content%d",i);
		strTmp2.Format("remark%d",i);
		WritePrivateProfileString("record",strTmp0,strTime,strIniPath);
		WritePrivateProfileString("record",strTmp1,strInfo,strIniPath);
		WritePrivateProfileString("record",strTmp2,strRmk,strIniPath);
	
		
	}
	for(;i<10;i++)
	{
		strTmp0.Format("time%d",i);
		strTmp1.Format("content%d",i);
		strTmp2.Format("remark%d",i);
		WritePrivateProfileString("record",strTmp0,"",strIniPath);
		WritePrivateProfileString("record",strTmp1,"",strIniPath);
		WritePrivateProfileString("record",strTmp2,"",strIniPath);
	
	}
}


void CDlgQry::OnDestroy() 
{

	SaveQryRecord();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDlgQry::OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
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
	*pResult = 0;
}

void CDlgQry::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem != -1)
	{
		
		int count = 0;
		m_nSelItem = pNMListView->iItem;
		CString strContent = m_list.GetItemText(m_nSelItem,1);
		int n= strContent.Find(',');
		CString strTmp2,strTmp3;
		CString strTmp1 = strContent.Left(n);
		strContent.Delete(0,n+1);

		n= strContent.Find(',');
		if(n>=0)
		{

		
			strTmp2 = strContent.Left(n);
			strContent.Delete(0,n+1);
			strTmp3 = strContent;

			int a = atoi(strTmp1)-1;
			m_cbSel.SetCurSel(a);
			GetDlgItem(IDC_EDIT1)->SetWindowText(strTmp2);
			GetDlgItem(IDC_EDIT2)->SetWindowText(strTmp3);

		}
		else
		{
			strTmp2 = strContent;
			GetDlgItem(IDC_EDIT4)->SetWindowText(strTmp1);
			GetDlgItem(IDC_EDIT5)->SetWindowText(strTmp2);
		}





		
		
	}
	*pResult = 0;
}

void CDlgQry::OnMenuDelAll() 
{
	// TODO: Add your command handler code here
	m_list.DeleteAllItems();
}

void CDlgQry::OnMenuDelCur() 
{
	// TODO: Add your command handler code here
	m_list.DeleteItem(m_nDelItem);
}
