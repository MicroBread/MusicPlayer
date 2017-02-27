
// mengMusicDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mengMusic.h"
#include "mengMusicDlg.h"
#include "afxdialogex.h"
#include "Mmsystem.h"
#include "Digitalv.h"
#include <locale>
#include <cstdlib>
#pragma comment(lib,"Winmm.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//ȫ�ֱ���
int loction;//��ǵ�ǰ���Ÿ�����λ��
HWND m_hwnd;//���
DWORD DeviceID;//˫�ֳ���
MCI_OPEN_PARMS MciOpenParms;//��ǰ���ŵĸ���
CString m_pathname;//��ǰ������
TCHAR *pszFilename = _T("list.txt");//�������洢·��

void play()//������Ƶ�ļ�����
{
	MCI_PLAY_PARMS MciPlayParms;
	MciPlayParms.dwCallback = (DWORD)m_hwnd;
	MciPlayParms.dwFrom = 0;//ÿ�β��Ŷ��Ǵ�0��ʼ
	mciSendCommand(DeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)(LPVOID)&MciPlayParms);
}

void pause()//��ͣ����
{
	mciSendCommand(DeviceID, MCI_PAUSE, 0, 0);//����ϵͳ����ĺ���
}

void resume()//������������
{
	mciSendCommand(DeviceID, MCI_RESUME, 0, 0);//����ϵͳ����ĺ���
}

void CmengMusicDlg::Save()//�Զ���������б�
{
	TCHAR* old_locale = _tcsdup(_tsetlocale(LC_CTYPE, NULL));
	_tsetlocale(LC_CTYPE, _T("chs"));
	CStdioFile file;//�ļ�������
	file.Open(pszFilename, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText);//�򿪱������·�����ļ�
	for (int i = 0; i < m_list.GetCount(); i++)//�������б�����洢
	{
		CString strtemp;//����һ���ַ���
		m_list.GetText(i, strtemp);//��ȡ�����ļ�·��
		strtemp += (CString)"\n";//���뻻�з�
		file.WriteString(strtemp);//д���ļ���
	}
	setlocale(LC_CTYPE, (char*)old_locale);
	free(old_locale);//��ԭ�����趨
	file.Close();//�ر��ļ�
}

DWORD setvolume(DWORD vol)//������������
{
	MCI_DGV_SETAUDIO_PARMS setVolume;//�������������Ĳ������ݽṹ
	setVolume.dwCallback = NULL;
	setVolume.dwItem = MCI_DGV_SETAUDIO_VOLUME;//��������
	setVolume.dwValue = vol;//����ֵ��vol
	mciSendCommand(DeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&setVolume);
	return 0;
}

DWORD info(DWORD item)
{
	MCI_STATUS_PARMS mcistatusparms;
	mcistatusparms.dwItem = item;
	mcistatusparms.dwReturn = 0;
	mciSendCommand(DeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcistatusparms);//��ȡ��������
	return mcistatusparms.dwReturn;//���ظ�������
}

void setvolume2(int vol)//���ø�������
{
	int cun = info(MCI_STATUS_POSITION);//��ȡ������ǰ���ų���;
	if (abs(vol - cun)>5000)//������λ��ƫ������
	{
		MCI_PLAY_PARMS sp;
		sp.dwFrom = vol;//ָ������ʱ��
		mciSendCommand(DeviceID, MCI_PLAY, MCI_FROM, (DWORD)&sp);//���ݽ���������
	}
}

void Settime(CString &alltime, int totaltime, int cm, int cs)
{
	totaltime = totaltime / 1000;//ת������Ϊ��λ������
	cm = totaltime / 60;//��÷�����
	cs = totaltime % 60;//���������
	if (cm < 10 && cs < 10)
		alltime.Format(_T("%d%d:%d%d"), 0, cm, 0, cs);//int����ת����CString
	else
		if (cm<10)
			alltime.Format(_T("%d%d:%d"), 0, cm, cs);//int����ת����CString
		else
			if (cs < 10)
				alltime.Format(_T("%d:%d%d"), cm, 0, cs);//int����ת����CString
			else
				alltime.Format(_T("%d:%d"), cm, cs);//int����ת����CString
}

void Load(HWND hwnd, CString strfilepath)//��λ����
{
	m_hwnd = hwnd;
	mciSendCommand(DeviceID, MCI_CLOSE, 0, 0);//�ڼ����ļ�ǰ�������һ�εĲ��ż�¼��
	MciOpenParms.lpstrElementName = strfilepath;//�������ļ�·�������豸
	DWORD dwReturn;
	dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)(LPVOID)&MciOpenParms);
	if (dwReturn != 0)
	{
		//������ļ�ʧ�ܣ��򽫳�����Ϣ������buffer������ʾ������
		char buffer[256];
		mciGetErrorString(dwReturn, (LPWSTR)buffer, 256);
		MessageBox(hwnd, (LPCWSTR)buffer, (LPCWSTR)"������!", MB_ICONHAND | MB_ICONERROR | MB_ICONSTOP);
	}
	DeviceID = MciOpenParms.wDeviceID;
	//���ļ��ɹ��͹������豸
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

														// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmengMusicDlg �Ի���



CmengMusicDlg::CmengMusicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmengMusicDlg::IDD, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmengMusicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Text(pDX, IDC_vol, m_int);
	DDX_Control(pDX, IDC_LIST, m_list);
	CString count;//��������
	count.Format(_T("%d"), m_list.GetCount());//int����ת����CString
	DDX_Text(pDX, IDC_COUNT, count);
}

BEGIN_MESSAGE_MAP(CmengMusicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()//��ʱ��ӳ��
	ON_BN_CLICKED(IDC_ADD, &CmengMusicDlg::OnBnClickedAdd)
	ON_LBN_SELCHANGE(IDC_LIST, &CmengMusicDlg::OnLbnSelchangeList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER, &CmengMusicDlg::OnNMCustomdrawSlider)
	ON_BN_CLICKED(IDC_DELETE, &CmengMusicDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_PLAY, &CmengMusicDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_PAUSE, &CmengMusicDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_PREVIOUS, &CmengMusicDlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_NEXT, &CmengMusicDlg::OnBnClickedNext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CmengMusicDlg::OnNMCustomdrawSlider2)
END_MESSAGE_MAP()


// CmengMusicDlg ��Ϣ�������

BOOL CmengMusicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	m_slider.SetRange(0, 1000);//������ƶ���Χ��
	m_slider.SetPos(500);//����ָ��ĳ�ʼλ����Ϊ500�����ʼ����Ϊ50
	m_slider2.SetRange(0, 1000);//������ƶ���Χ��
	m_slider2.SetPos(0);//����ָ��ĳ�ʼλ����Ϊ0�������δ����
	m_list.SetHorizontalExtent(100);//�����б�ˮƽ���
									//Ϊ������������
	TCHAR* old_locale = _tcsdup(_tsetlocale(LC_CTYPE, NULL));
	_tsetlocale(LC_CTYPE, _T("chs"));
	CStdioFile *fsp = new CStdioFile(pszFilename, CFile::modeRead);//����һ���ļ�������
	CString str;//����һ���ַ���
	if (fsp->ReadString(str) == NULL)
	{
		GetDlgItem(IDC_PLAY)->EnableWindow(false);//���ð�ť������
		GetDlgItem(IDC_NEXT)->EnableWindow(false);//���ð�ť������
		GetDlgItem(IDC_PAUSE)->EnableWindow(false);//���ð�ť������
		GetDlgItem(IDC_DELETE)->EnableWindow(false);//���ð�ť������
		GetDlgItem(IDC_PREVIOUS)->EnableWindow(false);//���ð�ť������
	}
	else
	{
		while (fsp->ReadString(str))//ÿ�����ļ��ж����ַ���
		{
			m_list.AddString(str);//���ַ�����ӵ��б���
		}
		m_list.SetCurSel(0);//Ĭ�ϴӵ�һ�׿�ʼ
		m_list.GetText(m_list.GetCurSel(), m_pathname);//��ȡ���ŵĸ�����
		GetDlgItem(IDC_PLAY)->EnableWindow(true);//���ð�ť����
		GetDlgItem(IDC_NEXT)->EnableWindow(true);//���ð�ť����
		GetDlgItem(IDC_PAUSE)->EnableWindow(true);//���ð�ť����
		GetDlgItem(IDC_DELETE)->EnableWindow(true);//���ð�ť����
		GetDlgItem(IDC_PREVIOUS)->EnableWindow(true);//���ð�ť����
	}
	setlocale(LC_CTYPE, (char*)old_locale);//�ַ�ת��
	free(old_locale);//��ԭ�����趨
	fsp->Close();//�ر��ļ�
	delete fsp;//�ͷſռ�
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(true);//Ĭ�ϲ���ģʽΪ�������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CmengMusicDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmengMusicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CmengMusicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmengMusicDlg::OnBnClickedAdd()//������ְ�ť�Ŀ���
{
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT, (LPTSTR)_T("(*.avi;*.mp3;*.wmv)|*.avi;*.mp3;*.wmv||"));//���ÿɴ��ļ���ʽ
	POSITION pos = dlg.GetStartPosition();//��ȡ��ʼλ��
	if (dlg.DoModal() == IDOK)
	{
		while (pos != NULL)
		{
			m_pathname = dlg.GetNextPathName(pos);//��ȡ�ļ���
			m_list.AddString(m_pathname);//���뵽�ļ��б���
		}
	}
	Save();//�Զ����������б�
	CString count;//��������
	count.Format(_T("%d"), m_list.GetCount());//int����ת����CString
	SetDlgItemText(IDC_COUNT, count);//�����������
}


void CmengMusicDlg::OnLbnSelchangeList()//�����б����
{
	CString strFilepath;//����һ���ַ���
	int index = m_list.GetCurSel();
	m_list.GetText(index, strFilepath);//��ȡ·��
	m_pathname = strFilepath;
	GetDlgItem(IDC_PLAY)->EnableWindow(true);//���ð�ť����
	GetDlgItem(IDC_NEXT)->EnableWindow(true);//���ð�ť����
	GetDlgItem(IDC_PAUSE)->EnableWindow(true);//���ð�ť����
	GetDlgItem(IDC_DELETE)->EnableWindow(true);//���ð�ť����
	GetDlgItem(IDC_PREVIOUS)->EnableWindow(true);//���ð�ť����
}


void CmengMusicDlg::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)//�����������Ŀ���
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(true);
	m_int = m_slider.GetPos() / 10;//���ڷ�Χ����Ϊ0~1000���������10
	setvolume(m_slider.GetPos());//��������
	UpdateData(false);
	*pResult = 0;
}


void CmengMusicDlg::OnBnClickedDelete()//ɾ����������
{
	m_list.DeleteString(m_list.GetCurSel());//ɾ����ǰѡ�е��У�
	Save();//�Զ����������б�
	CString count;//��������
	count.Format(_T("%d"), m_list.GetCount());//int����ת����CString
	SetDlgItemText(IDC_COUNT, count);//�����������
}


void CmengMusicDlg::OnBnClickedPlay()//�������ְ�ť����
{
	int index = m_list.GetCurSel();//��ȡ���ŵ�λ��
	loction = index;//��ȡ���ŵ�λ��
	m_list.GetText(index, m_pathname);//��ȡ���ŵĸ�����
	Load(this->m_hWnd, m_pathname);//��λ
	play();//���ò������ֺ���
	SetDlgItemText(IDC_CURR, m_pathname);//���������
	SetTimer(1, 1000, NULL);//���ö�ʱ��
	SetDlgItemText(IDC_PAUSE, (CString)"Pause");//��ʼ����ʱ����IDΪIDC_PAUSE�İ�ť�ϵ����ָ�Ϊ��Pause��
}


void CmengMusicDlg::OnBnClickedPause()//��ͣ���Ű�ť����
{
	CString strtemp;//����һ���ַ���
	GetDlgItemText(IDC_PAUSE, strtemp);//��ȡ��ť���ַ�
	if (strtemp.Compare((CString)"Pause") == 0)//�жϵ�ǰ״̬
	{
		pause();//��ͣ��������
		SetDlgItemText(IDC_PAUSE, (CString)"Continue");//����ʾ�ַ���Ϊ��Continue��
	}
	if (strtemp.Compare((CString)"Continue") == 0)
	{
		resume();//������������
		SetDlgItemText(IDC_PAUSE, (CString)"Pause");//����ʾ�ַ���Ϊ��Pause��
	}
}


void CmengMusicDlg::OnBnClickedPrevious()//��һ�װ�ť����
{
         if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())//ѡ���������ģʽ
		{
			srand(time(0));
			m_list.SetCurSel(rand() % m_list.GetCount());//�漴��ת���
			m_list.GetText(m_list.GetCurSel(), m_pathname);//��ȡ���ŵĸ�����
			Load(this->m_hWnd, m_pathname);//��λ
			play();//���Ÿ���
			SetDlgItemText(IDC_CURR, m_pathname);//���������
		}
		else
		{
			int index = m_list.GetCurSel() - 1;//��ȡ���ŵ�λ��
			if (index < 0)
				index = m_list.GetCount() - 1;//��index<0���򲥷��б�����һ�׸�
			m_list.GetText(index, m_pathname);//��ȡ���ŵĸ�����
			Load(this->m_hWnd, m_pathname);//��λ
			play();//���ò��ź���
			SetDlgItemText(IDC_CURR, m_pathname);//���������
			m_list.SetCurSel(index);//�ù��
		}
}


void CmengMusicDlg::OnBnClickedNext()//��һ�װ�ť����
{
         if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())//ѡ���������ģʽ
		{
			srand(time(0));
			m_list.SetCurSel(rand() % m_list.GetCount());//�漴��ת���
			m_list.GetText(m_list.GetCurSel(), m_pathname);//��ȡ���ŵĸ�����
			Load(this->m_hWnd, m_pathname);//��λ
			play();//���Ÿ���
			SetDlgItemText(IDC_CURR, m_pathname);//���������
		}
		else
		{
			int index = m_list.GetCurSel() + 1;//��ȡ���ŵ�λ��
			if (index >= m_list.GetCount())
				index = 0;//��index>��������-1���򲥷��б�ĵ�һ�׸�
			m_list.GetText(index, m_pathname);//��ȡ���ŵĸ�����
			Load(this->m_hWnd, m_pathname);//��λ
			play();//���ò��ź���
			SetDlgItemText(IDC_CURR, m_pathname);//���������
			m_list.SetCurSel(index);//�ù��
		}
}


void CmengMusicDlg::check()//��鲥��ģʽ����
{
		if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())//ѡ����ѭ��ģʽ
		{
			m_list.GetText(m_list.GetCurSel(), m_pathname);//��ȡ���ŵĸ�����
			Load(this->m_hWnd, m_pathname);//��λ
			play();//���Ÿ���
			SetDlgItemText(IDC_CURR, m_pathname);//���������
		}
		if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())//ѡ���б�ѭ��ģʽ
		{
			int index = m_list.GetCurSel() + 1;//��ȡ���ŵ�λ��
			loction = index;//��ȡ���ŵ�λ��
			if (index >= m_list.GetCount())//������ĩβ���ͷ��ʼ
				index = 0;
			m_list.GetText(index, m_pathname);//��ȡ���ŵĸ�����
			Load(this->m_hWnd, m_pathname);//��λ
			m_list.SetCurSel(index);//�ù��
			play();//���Ÿ���
			SetDlgItemText(IDC_CURR, m_pathname);//���������
		}
		if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())//ѡ��˳�򲥷�ģʽ
		{
			int index = m_list.GetCurSel() + 1;//��ȡ���ŵ�λ��
			loction = index;//��ȡ���ŵ�λ��
			if (index >= m_list.GetCount())//������ĩβ
				mciSendCommand(DeviceID, MCI_STOP, 0, 0);//ֹͣ��������
			else
			{
				m_list.GetText(index, m_pathname);//��ȡ���ŵĸ�����
				Load(this->m_hWnd, m_pathname);//��λ
				m_list.SetCurSel(index);//�ù��
				play();//���Ÿ���
				SetDlgItemText(IDC_CURR, m_pathname);//���������
			}
		}
		if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())//ѡ���������ģʽ
		{
			srand(time(0));
			int index = rand() % m_list.GetCount();//��ȡ���ŵ�λ��
			loction = index;//��ȡ���ŵ�λ��
			m_list.SetCurSel(index);//�漴��ת���
			m_list.GetText(m_list.GetCurSel(), m_pathname);//��ȡ���ŵĸ�����
			Load(this->m_hWnd, m_pathname);//��λ
			play();//���Ÿ���
			SetDlgItemText(IDC_CURR, m_pathname);//���������
		}
}


void CmengMusicDlg::OnTimer(UINT_PTR nIDEvent)//����������
{
	MCI_STATUS_PARMS MciStatusParms;
	MciStatusParms.dwItem = MCI_STATUS_MODE;
	totaltime = info(MCI_STATUS_LENGTH);//��ȡ������ʱ��
	currenttime = info(MCI_STATUS_POSITION);//��ȡ������ǰ���ų���
	m_slider2.SetPos(currenttime * 1000 / totaltime);//���ý�����λ��
	DWORD result = mciSendCommand(DeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&MciStatusParms);
	if (currenttime >= totaltime)//�жϵ�ǰ�����Ƿ��Ѳ�����
		check();//���ü�鲥��ģʽ����
	Settime(alltime, totaltime, cm, cs);//�����ܸ�ʱ��
	Settime(loctime, currenttime, cm, cs);//���õ�ǰʱ��
	outtime = loctime + _T("/") + alltime;//���������ʽ
	SetDlgItemText(IDC_vol2, outtime);//���ʱ��
	CDialogEx::OnTimer(nIDEvent);//���ô���������
}


void CmengMusicDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)//���Ž��ȿؼ�����
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(true);
	int cun;
	cun = info(MCI_STATUS_LENGTH);//��ȡ������ʱ��
	m_int2 = m_slider2.GetPos() / 10;//���ڷ�Χ����Ϊ0~1000���������10
	cun = cun*m_int2 / 100;//��ȡ��������Ӧ�Ĳ���ʱ��
	setvolume2(cun);//���ø������Ž���
	UpdateData(false);
	*pResult = 0;
}
