// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__630F69E8_D9CD_427D_883B_5EAEC0C7BF90__INCLUDED_)
#define AFX_STDAFX_H__630F69E8_D9CD_427D_883B_5EAEC0C7BF90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
#pragma warning(disable : 4786)	// too long identifiers.
#include <SHLWAPI.H>

#include "TabCtrl\TabCtrl.h"
#include "winsock.h"
#include "log_out.h"
//#include "mysql.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__630F69E8_D9CD_427D_883B_5EAEC0C7BF90__INCLUDED_)
