
// TESTDlg.cpp : ��@��
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
// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
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


// CTESTDlg ��ܤ��



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


// CTESTDlg �T���B�z�`��

BOOL CTESTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �N [����...] �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
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

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w

	CWnd *pWSrc = GetDlgItem(IDC_STATIC1);
	pdcSrc = new CClientDC(pWSrc);

	CWnd *pWSrc2 = GetDlgItem(IDC_STATIC2);
	pdcSrc2 = new CClientDC(pWSrc2);
	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
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

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CTESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTESTDlg::OnBnClickedOpen()
{
	// TODO: �b���[�J����i���B�z�`���{���X
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

	//�Ϥ���Ƕ�
	int x, y;
	for (x = 0; x < height; x++) {
		for (y = 0; y < width; y++) {
			int G = pic[y*pitch + x * 3 + 1];
			pic[y*pitch + x * 3] = G;
			pic[y*pitch + x * 3 + 2] = G;
		}
	}
	img.Draw(*pdcSrc, 0, 0);

	//�����o�i
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
