#include "stdafx.h"
#include "TCPCLIENTDLG.h"
#include "TCPCLIENTDLGDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#pragma comment(lib,"ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SOCKET WM_USER+99


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    


protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)


END_MESSAGE_MAP()






CTCPCLIENTDLGDlg::CTCPCLIENTDLGDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPCLIENTDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPCLIENTDLGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDR, m_server_ip);
	DDX_Control(pDX, IDC_PORT, m_server_port);
	DDX_Control(pDX, IDC_TEXT, m_edit_text);
	DDX_Control(pDX, IDC_SENDTEXT, m_edit_text_sent);
	DDX_Control(pDX, IDC_CONNECT, m_btn_connect);
	DDX_Control(pDX, IDC_SEND, m_btn_send);
}

BEGIN_MESSAGE_MAP(CTCPCLIENTDLGDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, &CTCPCLIENTDLGDlg::OnSocket)
	ON_BN_CLICKED(IDC_CONNECT, &CTCPCLIENTDLGDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_SEND, &CTCPCLIENTDLGDlg::OnBnClickedSend)
END_MESSAGE_MAP()




BOOL CTCPCLIENTDLGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	//2018.12.4
	m_edit_text.EnableWindow(false);
	m_edit_text_sent.EnableWindow(false);
	m_btn_send.EnableWindow(false);

	//create a socket
	s = ::socket(AF_INET,SOCK_STREAM,0);

	//Set the socket as asyncronous mode
	//WSAAsyncSelect(s,this->m_hWnd,WM_SOCKET,FD_READ);

	return TRUE;  
}

void CTCPCLIENTDLGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CTCPCLIENTDLGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);


		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;


		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CTCPCLIENTDLGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//
LRESULT CTCPCLIENTDLGDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	char cs[1000] = "";
	CString str_text;
	if (lParam == FD_READ)
	{
		//RECEIVE DATAS
		recv(s,cs,1000,NULL);

		m_edit_text.GetWindowTextW(str_text);
		str_text += L"\r\n SERVER SAID: ";
		str_text += CString(cs);
		m_edit_text.SetWindowTextW(str_text);
	}


	return 0;
}

char* UsrCstringToChar(CString str)
{
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *cs_text = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, cs_text, len, NULL, NULL);
	return cs_text;
}

//COONECT TO SERVER OF TCP
void CTCPCLIENTDLGDlg::OnBnClickedConnect()
{
	CString str_port, str_ip,str_text;
	m_server_ip.GetWindowTextW(str_ip);
	m_server_port.GetWindowTextW(str_port);
	if (str_ip.IsEmpty() || str_port.IsEmpty())
	{
		MessageBox(L"IP AND PORT IS Empty.");
		return;
	}
	else
	{
		char *cs_ip = NULL;
		cs_ip = UsrCstringToChar(str_ip);
		int port = _wtoi(str_port);
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr(cs_ip);
		addr.sin_port = htons(port);

		m_edit_text.SetWindowTextW(L"connecting to server... ...\r\n");

		//Maybe the first time of connection failed,try again ,it succeed,why?
		if (::connect(s, (sockaddr*)&addr, sizeof(addr)))
		{
			m_edit_text.GetWindowTextW(str_text);
			str_text += L"\r\n";
			str_text += L"Successfully connect to server.\r\n";
			m_edit_text.SetWindowTextW(str_text);

			m_btn_send.EnableWindow(true);
			m_edit_text_sent.EnableWindow(true);

			//send msg to server
			char sent_msg[1024] = "it's client...";
			::send(s,sent_msg,1024,0);
		}
		else
		{
			m_edit_text.GetWindowTextW(str_text);
			str_text += L"\r\n";
			str_text += L"Failed to connect,try again...\r\n";
			m_edit_text.SetWindowTextW(str_text);
		}
	}
}



//SEND MSG TO SERVER
void CTCPCLIENTDLGDlg::OnBnClickedSend()
{
	CString str_text,str_text_send;
	m_edit_text_sent.GetWindowTextW(str_text_send);

	if(str_text_send.IsEmpty())
	{
		m_edit_text.GetWindowTextW(str_text);
		str_text += L"\r\n";
		str_text += L"MSG SHOULD NEOT BE EMPTY\r\n";
		m_edit_text.SetWindowTextW(str_text);
	}
	else
	{
		char *cs_text = NULL;
		cs_text = UsrCstringToChar(str_text_send);
		::send(s, cs_text,str_text_send.GetLength(),0);
		delete cs_text;


		m_edit_text.GetWindowTextW(str_text);
		str_text += L"\r\n";
		str_text += str_text_send;
		m_edit_text.SetWindowTextW(str_text);
	}
}
