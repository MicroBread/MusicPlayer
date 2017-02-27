
// mengMusicDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CmengMusicDlg 对话框
class CmengMusicDlg : public CDialogEx
{
	// 构造
public:
	CmengMusicDlg(CWnd* pParent = NULL);	// 标准构造函数

												// 对话框数据
	enum { IDD = IDD_MENGMUSIC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


														// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_int;
	int m_int2;
	int cm;//分钟
	int cs;//秒
	int totaltime;//总共时间
	int currenttime;//当前时间
	CString loctime;//当前时间
	CString alltime;//总共时间
	CString outtime;//输出时间
	CListBox m_list;
	CSliderCtrl m_slider;
	CSliderCtrl m_slider2;
	void Save();//自动保存歌曲列表
	void check();//检查播放模式函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);//定时器，用于触发设备
	afx_msg void OnBnClickedAdd();//添加音乐按钮的控制
	afx_msg void OnLbnSelchangeList();//歌曲列表操作
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);//音量滑动条的控制
	afx_msg void OnBnClickedDelete();//删除歌曲操作
	afx_msg void OnBnClickedPlay();//播放音乐按钮设置
	afx_msg void OnBnClickedPause();//暂停播放按钮设置
	afx_msg void OnBnClickedPrevious();//上一首按钮设置
	afx_msg void OnBnClickedNext();//下一首按钮设置
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
};