#pragma once


// CTCPCLIENTDLGDlg 
class CTCPCLIENTDLGDlg : public CDialogEx
{
public:
	CTCPCLIENTDLGDlg(CWnd* pParent = nullptr);	


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPCLIENTDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	



protected:
	HICON m_hIcon;


	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnSocket(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSend();
	CEdit m_server_ip;
	CEdit m_server_port;
	CEdit m_edit_text;
	CEdit m_edit_text_sent;
	CButton m_btn_connect;
	CButton m_btn_send;


public:
	SOCKET s;
	sockaddr_in addr;
};
