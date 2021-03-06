#include "stdafx.h"
#include "TCPCLIENTDLG.h"
#include "TCPCLIENTDLGDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTCPCLIENTDLGApp

BEGIN_MESSAGE_MAP(CTCPCLIENTDLGApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()




CTCPCLIENTDLGApp::CTCPCLIENTDLGApp()
{

	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;


}



CTCPCLIENTDLGApp theApp;




BOOL CTCPCLIENTDLGApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;


	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));


	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CTCPCLIENTDLGDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "warning: Failed to create dlg，app terminate unexpectedly。\n");
		TRACE(traceAppMsg, 0, "warning:If you use MFC controls on the dlg，then you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}


	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif


	return FALSE;
}

