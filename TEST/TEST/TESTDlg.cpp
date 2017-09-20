
// TESTDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "TEST.h"
#include "TESTDlg.h"
#include "afxdialogex.h"
#include <atlimage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClientDC *pdcSrc;
CClientDC *pdcSrc2;
// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
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


// CTESTDlg 對話方塊



CTESTDlg::CTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTESTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CTESTDlg::OnBnClickedOpen)
END_MESSAGE_MAP()


// CTESTDlg 訊息處理常式

BOOL CTESTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定

	CWnd *pWSrc = GetDlgItem(IDC_STATIC1);
	pdcSrc = new CClientDC(pWSrc);

	CWnd *pWSrc2 = GetDlgItem(IDC_STATIC2);
	pdcSrc2 = new CClientDC(pWSrc2);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CTESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTESTDlg::OnBnClickedOpen()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CFileDialog DlgSelect(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "Color BMP Files(*.bmp)|*.bmp|", NULL);
	if (DlgSelect.DoModal() != IDOK) {
		return;
	}

	CString FilePath = DlgSelect.GetPathName();
	CImage img;
	CImage img2;
	img.Load(FilePath);

	int width = img.GetWidth();
	int height = img.GetHeight();
	int pitch = img.GetPitch(); //pitch=width*3
	byte *pic = (byte*)img.GetBits();

	//圖片轉灰階
	int x, y;
	for (x = 0; x < height; x++) {
		for (y = 0; y < width; y++) {
			int G = pic[y*pitch + x * 3 + 1];
			pic[y*pitch + x * 3] = G;
			pic[y*pitch + x * 3 + 2] = G;
		}
	}
	img.Draw(*pdcSrc, 0, 0);

	//平滑濾波
	byte *nimg = (byte*)img2.GetBits();
	memcpy(nimg, pic, width*height * sizeof(byte));

	int i, j;
	for (i = 4; i < 255 * 3 + 1; i += 3) {
		for (j = 1; j < 255; j++) {
			nimg[j*pitch + i] = (pic[(j - 1)*pitch + (i - 3)] + pic[j*pitch + (i - 3)] + pic[(j + 1)*pitch + (i - 3)]
								+ pic[(j - 1)*pitch + i] + pic[j*pitch + i] + pic[(j + 1)*pitch + i]
								+ pic[(j - 1)*pitch + (i + 3)] + pic[j*pitch + (i + 3)] + pic[(j + 1)*pitch + (i + 3)]) / 9;
		}
	}
	
	img2.Draw(*pdcSrc2, 0, 0);
}
