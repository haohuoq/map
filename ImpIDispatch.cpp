#include "stdafx.h"
#include "ImpIDispatch.h"
#include "CellLocatiom.h"
#include "CellLocatiomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCellLocatiomDlg * pMainDlg;

CString cszCB_IsOurCustomBrowser		= "CB_IsOurCustomBrowser";
CString cszCB_Close						= "CB_Close";
CString cszCB_CustomFunction			= "CB_CustomFunction";
CString cszCB_CustomFunctionWithParams	= "CB_CustomFunctionWithParams";
CString cszCB_CustomFunctionLatLng		= "CB_CustomFunctionLatLng";
CString cszCB_ShowModalDialog			= "CB_ShowModalDialog";
CString cszCB_ShowModelessDialog		= "CB_ShowModelessDialog";
CString cszCB_GetCurrentLevel			= "CB_GetCurrentLevel";
#define	DISPID_CB_IsOurCustomBrowser		1
#define DISPID_CB_Close						2
#define DISPID_CB_CustomFunction			3
#define DISPID_CB_CustomFunctionWithParams	4
#define	DISPID_CB_CustomFunctionLatLng		5
#define DISPID_CB_ShowModalDialog			6
#define DISPID_CB_ShowModelessDialog		7
#define DISPID_CB_GetCurrentLevel			8
CImpIDispatch::CImpIDispatch(void)
{
	m_cRef = 0;
}

CImpIDispatch::~CImpIDispatch(void)
{
	ASSERT(m_cRef==0);
}

STDMETHODIMP CImpIDispatch::QueryInterface(REFIID riid, void** ppv)
{
	*ppv = NULL;
	
	if(IID_IDispatch == riid)
	{
        *ppv = this;
	}
	
	if(NULL != *ppv)
    {
		((LPUNKNOWN)*ppv)->AddRef();
		return NOERROR;
	}
	
	return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CImpIDispatch::AddRef(void)
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CImpIDispatch::Release(void)
{
	return --m_cRef;
}

STDMETHODIMP CImpIDispatch::GetTypeInfoCount(UINT* /*pctinfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetTypeInfo(
			/* [in] */ UINT /*iTInfo*/,
            /* [in] */ LCID /*lcid*/,
            /* [out] */ ITypeInfo** /*ppTInfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetIDsOfNames(
			/* [in] */ REFIID riid,
            /* [size_is][in] */ OLECHAR** rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID* rgDispId)
{
	HRESULT		hr		= NOERROR;
	UINT		i		= 0;
	CString		cszName(rgszNames[i]);

	for(i=0; i<cNames; ++i)
	{
		if(cszName == cszCB_IsOurCustomBrowser)
		{
			rgDispId[i] = DISPID_CB_IsOurCustomBrowser;
		}
		else if(cszName == cszCB_Close)
		{
			rgDispId[i] = DISPID_CB_Close;
		}
		else if(cszName == cszCB_CustomFunction)
		{
			rgDispId[i] = DISPID_CB_CustomFunction;
		}
		else if(cszName == cszCB_CustomFunctionWithParams)
		{
			rgDispId[i] = DISPID_CB_CustomFunctionWithParams;
		}
		else if(cszName == cszCB_CustomFunctionLatLng)
		{
			rgDispId[i] = DISPID_CB_CustomFunctionLatLng;
		}
		else if(cszName == cszCB_ShowModalDialog)
		{
			rgDispId[i] = DISPID_CB_ShowModalDialog;
		}
		else if(cszName == cszCB_ShowModelessDialog)
		{
			rgDispId[i] = DISPID_CB_ShowModelessDialog;
		}

		else if(cszName == cszCB_GetCurrentLevel)
		{
			rgDispId[i] = DISPID_CB_GetCurrentLevel;
		}
		else
		{
			// One or more are unknown so set the return code accordingly
			hr = ResultFromScode(DISP_E_UNKNOWNNAME);
			rgDispId[i] = DISPID_UNKNOWN;
		}
	}
	return hr;
}

STDMETHODIMP CImpIDispatch::Invoke(
			/* [in] */ DISPID dispIdMember,
			/* [in] */ REFIID /*riid*/,
			/* [in] */ LCID /*lcid*/,
			/* [in] */ WORD wFlags,
			/* [out][in] */ DISPPARAMS* pDispParams,
			/* [out] */ VARIANT* pVarResult,
			/* [out] */ EXCEPINFO* /*pExcepInfo*/,
			/* [out] */ UINT* puArgErr)
{
	CCellLocatiomDlg * pDlg = pMainDlg;//(CCellLocatiomDlg*)theApp.m_pMainWnd;
	if(dispIdMember == DISPID_CB_IsOurCustomBrowser)
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			VariantInit(pVarResult);
			V_VT(pVarResult) = VT_BOOL;
			V_BOOL(pVarResult) = true;
		}
	}
	
	if(dispIdMember == DISPID_CB_Close) 
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			pDlg->EndDialog(0);
		}
	}
	
	// 调用应用程序指令(无参数)
	if(dispIdMember == DISPID_CB_CustomFunction) 
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
 			pDlg->InvokeFunc();
		}
	}
	
	if(dispIdMember == DISPID_CB_CustomFunctionWithParams) 
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			// arguments come in reverse order
			// for some reason
			CString	strArg1	= CString(pDispParams->rgvarg[1].bstrVal);	// in case you want a CString copy
			int		iArg2	= pDispParams->rgvarg[0].intVal;
			pDlg->InvokeFuncWithParams(strArg1, iArg2);
		}
	}

	if(dispIdMember == DISPID_CB_CustomFunctionLatLng) 
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			// arguments come in reverse order
			// for some reason
			double dbLng = pDispParams->rgvarg[1].dblVal;
			double dbLat = pDispParams->rgvarg[0].dblVal;
			
			pDlg->ShowLatLng(dbLng, dbLat);
		}
	}
	
	if(dispIdMember == DISPID_CB_ShowModelessDialog)
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			// arguments come in reverse order
			// for some reason
		//	CString cszArg1= CString(pDispParams->rgvarg[4].bstrVal);	// in case you want a CString copy
			double d1= pDispParams->rgvarg[0].dblVal;
			double d2= pDispParams->rgvarg[1].dblVal;
			double d3= pDispParams->rgvarg[2].dblVal;
			double d4= pDispParams->rgvarg[3].dblVal;
			pDlg->ShowMapData(d1,d2,d3,d4);
		//	CString strstt;
		//	strstt.Format("%.5f %.5f %.5f %.5f",d1,d2,d3,d4);
		//	AfxMessageBox(strstt);
		}
	}
	
	if(dispIdMember == DISPID_CB_ShowModalDialog) 
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			// arguments come in reverse order
			// for some reason

		//	CString cszArg1= CString(pDispParams->rgvarg[2].bstrVal);	// in case you want a CString copy
			double dbLng = pDispParams->rgvarg[0].dblVal;
			double dbLat = pDispParams->rgvarg[1].dblVal;
			pDlg->ShowModalDlg( dbLng, dbLat);

		}
	}

	if(dispIdMember == DISPID_CB_GetCurrentLevel) 
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			// arguments come in reverse order
			// for some reason
			
		//	CString cszArg1= CString(pDispParams->rgvarg[2].bstrVal);	// in case you want a CString copy
		//	double dbLng = pDispParams->rgvarg[0].dblVal;
			int lv = pDispParams->rgvarg[0].intVal;
			pDlg->ShowCurLevel(lv);
			
		}
	}



	return S_OK;
}
