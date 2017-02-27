
// mengMusicDlg.cpp : 实现文件
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
//全局变量
int loction;//标记当前播放歌曲的位置
HWND m_hwnd;//句柄
DWORD DeviceID;//双字长度
MCI_OPEN_PARMS MciOpenParms;//当前播放的歌曲
CString m_pathname;//当前歌曲名
TCHAR *pszFilename = _T("list.txt");//歌曲名存储路径

void play()//播放音频文件操作
{
	MCI_PLAY_PARMS MciPlayParms;
	MciPlayParms.dwCallback = (DWORD)m_hwnd;
	MciPlayParms.dwFrom = 0;//每次播放都是从0开始
	mciSendCommand(DeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)(LPVOID)&MciPlayParms);
}

void pause()//暂停音乐
{
	mciSendCommand(DeviceID, MCI_PAUSE, 0, 0);//调用系统本身的函数
}

void resume()//继续播放音乐
{
	mciSendCommand(DeviceID, MCI_RESUME, 0, 0);//调用系统本身的函数
}

void CmengMusicDlg::Save()//自动保存歌曲列表
{
	TCHAR* old_locale = _tcsdup(_tsetlocale(LC_CTYPE, NULL));
	_tsetlocale(LC_CTYPE, _T("chs"));
	CStdioFile file;//文件流操作
	file.Open(pszFilename, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText);//打开保存歌曲路径的文件
	for (int i = 0; i < m_list.GetCount(); i++)//按歌曲列表逐个存储
	{
		CString strtemp;//定义一个字符串
		m_list.GetText(i, strtemp);//获取音乐文件路径
		strtemp += (CString)"\n";//加入换行符
		file.WriteString(strtemp);//写入文件中
	}
	setlocale(LC_CTYPE, (char*)old_locale);
	free(old_locale);//还原区域设定
	file.Close();//关闭文件
}

DWORD setvolume(DWORD vol)//设置音量函数
{
	MCI_DGV_SETAUDIO_PARMS setVolume;//这是设置音量的参数数据结构
	setVolume.dwCallback = NULL;
	setVolume.dwItem = MCI_DGV_SETAUDIO_VOLUME;//设置音量
	setVolume.dwValue = vol;//音量值是vol
	mciSendCommand(DeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&setVolume);
	return 0;
}

DWORD info(DWORD item)
{
	MCI_STATUS_PARMS mcistatusparms;
	mcistatusparms.dwItem = item;
	mcistatusparms.dwReturn = 0;
	mciSendCommand(DeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcistatusparms);//获取歌曲长度
	return mcistatusparms.dwReturn;//返回歌曲长度
}

void setvolume2(int vol)//设置歌曲进度
{
	int cun = info(MCI_STATUS_POSITION);//获取歌曲当前播放长度;
	if (abs(vol - cun)>5000)//进度条位置偏移明显
	{
		MCI_PLAY_PARMS sp;
		sp.dwFrom = vol;//指定播放时间
		mciSendCommand(DeviceID, MCI_PLAY, MCI_FROM, (DWORD)&sp);//根据进度条播放
	}
}

void Settime(CString &alltime, int totaltime, int cm, int cs)
{
	totaltime = totaltime / 1000;//转换成秒为单位的数据
	cm = totaltime / 60;//获得分数据
	cs = totaltime % 60;//获得秒数据
	if (cm < 10 && cs < 10)
		alltime.Format(_T("%d%d:%d%d"), 0, cm, 0, cs);//int数据转换成CString
	else
		if (cm<10)
			alltime.Format(_T("%d%d:%d"), 0, cm, cs);//int数据转换成CString
		else
			if (cs < 10)
				alltime.Format(_T("%d:%d%d"), cm, 0, cs);//int数据转换成CString
			else
				alltime.Format(_T("%d:%d"), cm, cs);//int数据转换成CString
}

void Load(HWND hwnd, CString strfilepath)//置位函数
{
	m_hwnd = hwnd;
	mciSendCommand(DeviceID, MCI_CLOSE, 0, 0);//在加载文件前先清空上一次的播放记录；
	MciOpenParms.lpstrElementName = strfilepath;//将音乐文件路径传给设备
	DWORD dwReturn;
	dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)(LPVOID)&MciOpenParms);
	if (dwReturn != 0)
	{
		//如果打开文件失败，则将出错信息储存在buffer，并显示出错警告
		char buffer[256];
		mciGetErrorString(dwReturn, (LPWSTR)buffer, 256);
		MessageBox(hwnd, (LPCWSTR)buffer, (LPCWSTR)"出错警告!", MB_ICONHAND | MB_ICONERROR | MB_ICONSTOP);
	}
	DeviceID = MciOpenParms.wDeviceID;
	//打开文件成功就关联到设备
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

														// 实现
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


// CmengMusicDlg 对话框



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
	CString count;//歌曲总数
	count.Format(_T("%d"), m_list.GetCount());//int数据转换成CString
	DDX_Text(pDX, IDC_COUNT, count);
}

BEGIN_MESSAGE_MAP(CmengMusicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()//定时器映射
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


// CmengMusicDlg 消息处理程序

BOOL CmengMusicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_slider.SetRange(0, 1000);//滑块的移动范围；
	m_slider.SetPos(500);//滑块指针的初始位置设为500代表初始音量为50
	m_slider2.SetRange(0, 1000);//滑块的移动范围；
	m_slider2.SetPos(0);//滑块指针的初始位置设为0代表歌曲未播放
	m_list.SetHorizontalExtent(100);//设置列表水平宽度
									//为读入中文正常
	TCHAR* old_locale = _tcsdup(_tsetlocale(LC_CTYPE, NULL));
	_tsetlocale(LC_CTYPE, _T("chs"));
	CStdioFile *fsp = new CStdioFile(pszFilename, CFile::modeRead);//定义一个文件流操作
	CString str;//定义一个字符串
	if (fsp->ReadString(str) == NULL)
	{
		GetDlgItem(IDC_PLAY)->EnableWindow(false);//设置按钮不可用
		GetDlgItem(IDC_NEXT)->EnableWindow(false);//设置按钮不可用
		GetDlgItem(IDC_PAUSE)->EnableWindow(false);//设置按钮不可用
		GetDlgItem(IDC_DELETE)->EnableWindow(false);//设置按钮不可用
		GetDlgItem(IDC_PREVIOUS)->EnableWindow(false);//设置按钮不可用
	}
	else
	{
		while (fsp->ReadString(str))//每当从文件中读到字符串
		{
			m_list.AddString(str);//将字符串添加到列表中
		}
		m_list.SetCurSel(0);//默认从第一首开始
		m_list.GetText(m_list.GetCurSel(), m_pathname);//获取播放的歌曲名
		GetDlgItem(IDC_PLAY)->EnableWindow(true);//设置按钮可用
		GetDlgItem(IDC_NEXT)->EnableWindow(true);//设置按钮可用
		GetDlgItem(IDC_PAUSE)->EnableWindow(true);//设置按钮可用
		GetDlgItem(IDC_DELETE)->EnableWindow(true);//设置按钮可用
		GetDlgItem(IDC_PREVIOUS)->EnableWindow(true);//设置按钮可用
	}
	setlocale(LC_CTYPE, (char*)old_locale);//字符转换
	free(old_locale);//还原区域设定
	fsp->Close();//关闭文件
	delete fsp;//释放空间
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(true);//默认播放模式为随机播放
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmengMusicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmengMusicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmengMusicDlg::OnBnClickedAdd()//添加音乐按钮的控制
{
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT, (LPTSTR)_T("(*.avi;*.mp3;*.wmv)|*.avi;*.mp3;*.wmv||"));//设置可打开文件格式
	POSITION pos = dlg.GetStartPosition();//获取起始位置
	if (dlg.DoModal() == IDOK)
	{
		while (pos != NULL)
		{
			m_pathname = dlg.GetNextPathName(pos);//读取文件名
			m_list.AddString(m_pathname);//加入到文件列表中
		}
	}
	Save();//自动保存音乐列表
	CString count;//歌曲总数
	count.Format(_T("%d"), m_list.GetCount());//int数据转换成CString
	SetDlgItemText(IDC_COUNT, count);//输出歌曲总数
}


void CmengMusicDlg::OnLbnSelchangeList()//歌曲列表操作
{
	CString strFilepath;//定义一个字符串
	int index = m_list.GetCurSel();
	m_list.GetText(index, strFilepath);//获取路径
	m_pathname = strFilepath;
	GetDlgItem(IDC_PLAY)->EnableWindow(true);//设置按钮可用
	GetDlgItem(IDC_NEXT)->EnableWindow(true);//设置按钮可用
	GetDlgItem(IDC_PAUSE)->EnableWindow(true);//设置按钮可用
	GetDlgItem(IDC_DELETE)->EnableWindow(true);//设置按钮可用
	GetDlgItem(IDC_PREVIOUS)->EnableWindow(true);//设置按钮可用
}


void CmengMusicDlg::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)//音量滑动条的控制
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(true);
	m_int = m_slider.GetPos() / 10;//由于范围设置为0~1000，故需除以10
	setvolume(m_slider.GetPos());//设置音量
	UpdateData(false);
	*pResult = 0;
}


void CmengMusicDlg::OnBnClickedDelete()//删除歌曲操作
{
	m_list.DeleteString(m_list.GetCurSel());//删除当前选中的行；
	Save();//自动保存音乐列表
	CString count;//歌曲总数
	count.Format(_T("%d"), m_list.GetCount());//int数据转换成CString
	SetDlgItemText(IDC_COUNT, count);//输出歌曲总数
}


void CmengMusicDlg::OnBnClickedPlay()//播放音乐按钮设置
{
	int index = m_list.GetCurSel();//获取播放的位置
	loction = index;//获取播放的位置
	m_list.GetText(index, m_pathname);//获取播放的歌曲名
	Load(this->m_hWnd, m_pathname);//置位
	play();//调用播放音乐函数
	SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
	SetTimer(1, 1000, NULL);//设置定时器
	SetDlgItemText(IDC_PAUSE, (CString)"Pause");//开始播放时，将ID为IDC_PAUSE的按钮上的文字改为“Pause”
}


void CmengMusicDlg::OnBnClickedPause()//暂停播放按钮设置
{
	CString strtemp;//创建一个字符串
	GetDlgItemText(IDC_PAUSE, strtemp);//获取按钮的字符
	if (strtemp.Compare((CString)"Pause") == 0)//判断当前状态
	{
		pause();//暂停播放音乐
		SetDlgItemText(IDC_PAUSE, (CString)"Continue");//将显示字符改为“Continue”
	}
	if (strtemp.Compare((CString)"Continue") == 0)
	{
		resume();//继续播放音乐
		SetDlgItemText(IDC_PAUSE, (CString)"Pause");//将显示字符改为“Pause”
	}
}


void CmengMusicDlg::OnBnClickedPrevious()//上一首按钮设置
{
         if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())//选择随机播放模式
		{
			srand(time(0));
			m_list.SetCurSel(rand() % m_list.GetCount());//随即跳转光标
			m_list.GetText(m_list.GetCurSel(), m_pathname);//获取播放的歌曲名
			Load(this->m_hWnd, m_pathname);//置位
			play();//播放歌曲
			SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
		}
		else
		{
			int index = m_list.GetCurSel() - 1;//获取播放的位置
			if (index < 0)
				index = m_list.GetCount() - 1;//若index<0，则播放列表的最后一首歌
			m_list.GetText(index, m_pathname);//获取播放的歌曲名
			Load(this->m_hWnd, m_pathname);//置位
			play();//调用播放函数
			SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
			m_list.SetCurSel(index);//置光标
		}
}


void CmengMusicDlg::OnBnClickedNext()//下一首按钮设置
{
         if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())//选择随机播放模式
		{
			srand(time(0));
			m_list.SetCurSel(rand() % m_list.GetCount());//随即跳转光标
			m_list.GetText(m_list.GetCurSel(), m_pathname);//获取播放的歌曲名
			Load(this->m_hWnd, m_pathname);//置位
			play();//播放歌曲
			SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
		}
		else
		{
			int index = m_list.GetCurSel() + 1;//获取播放的位置
			if (index >= m_list.GetCount())
				index = 0;//若index>歌曲数量-1，则播放列表的第一首歌
			m_list.GetText(index, m_pathname);//获取播放的歌曲名
			Load(this->m_hWnd, m_pathname);//置位
			play();//调用播放函数
			SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
			m_list.SetCurSel(index);//置光标
		}
}


void CmengMusicDlg::check()//检查播放模式函数
{
		if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())//选择单曲循环模式
		{
			m_list.GetText(m_list.GetCurSel(), m_pathname);//获取播放的歌曲名
			Load(this->m_hWnd, m_pathname);//置位
			play();//播放歌曲
			SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
		}
		if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())//选择列表循环模式
		{
			int index = m_list.GetCurSel() + 1;//获取播放的位置
			loction = index;//获取播放的位置
			if (index >= m_list.GetCount())//播放至末尾则从头开始
				index = 0;
			m_list.GetText(index, m_pathname);//获取播放的歌曲名
			Load(this->m_hWnd, m_pathname);//置位
			m_list.SetCurSel(index);//置光标
			play();//播放歌曲
			SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
		}
		if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())//选择顺序播放模式
		{
			int index = m_list.GetCurSel() + 1;//获取播放的位置
			loction = index;//获取播放的位置
			if (index >= m_list.GetCount())//播放至末尾
				mciSendCommand(DeviceID, MCI_STOP, 0, 0);//停止播放音乐
			else
			{
				m_list.GetText(index, m_pathname);//获取播放的歌曲名
				Load(this->m_hWnd, m_pathname);//置位
				m_list.SetCurSel(index);//置光标
				play();//播放歌曲
				SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
			}
		}
		if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())//选择随机播放模式
		{
			srand(time(0));
			int index = rand() % m_list.GetCount();//获取播放的位置
			loction = index;//获取播放的位置
			m_list.SetCurSel(index);//随即跳转光标
			m_list.GetText(m_list.GetCurSel(), m_pathname);//获取播放的歌曲名
			Load(this->m_hWnd, m_pathname);//置位
			play();//播放歌曲
			SetDlgItemText(IDC_CURR, m_pathname);//输出歌曲名
		}
}


void CmengMusicDlg::OnTimer(UINT_PTR nIDEvent)//触发器函数
{
	MCI_STATUS_PARMS MciStatusParms;
	MciStatusParms.dwItem = MCI_STATUS_MODE;
	totaltime = info(MCI_STATUS_LENGTH);//获取歌曲总时长
	currenttime = info(MCI_STATUS_POSITION);//获取歌曲当前播放长度
	m_slider2.SetPos(currenttime * 1000 / totaltime);//设置进度条位置
	DWORD result = mciSendCommand(DeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&MciStatusParms);
	if (currenttime >= totaltime)//判断当前音乐是否已播放完
		check();//调用检查播放模式函数
	Settime(alltime, totaltime, cm, cs);//设置总该时长
	Settime(loctime, currenttime, cm, cs);//设置当前时长
	outtime = loctime + _T("/") + alltime;//设置输出格式
	SetDlgItemText(IDC_vol2, outtime);//输出时间
	CDialogEx::OnTimer(nIDEvent);//调用触发器函数
}


void CmengMusicDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)//播放进度控件设置
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(true);
	int cun;
	cun = info(MCI_STATUS_LENGTH);//获取歌曲总时长
	m_int2 = m_slider2.GetPos() / 10;//由于范围设置为0~1000，故需除以10
	cun = cun*m_int2 / 100;//获取进度条对应的播放时间
	setvolume2(cun);//设置歌曲播放进度
	UpdateData(false);
	*pResult = 0;
}
