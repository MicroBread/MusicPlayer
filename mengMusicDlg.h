
// mengMusicDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CmengMusicDlg �Ի���
class CmengMusicDlg : public CDialogEx
{
	// ����
public:
	CmengMusicDlg(CWnd* pParent = NULL);	// ��׼���캯��

												// �Ի�������
	enum { IDD = IDD_MENGMUSIC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


														// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_int;
	int m_int2;
	int cm;//����
	int cs;//��
	int totaltime;//�ܹ�ʱ��
	int currenttime;//��ǰʱ��
	CString loctime;//��ǰʱ��
	CString alltime;//�ܹ�ʱ��
	CString outtime;//���ʱ��
	CListBox m_list;
	CSliderCtrl m_slider;
	CSliderCtrl m_slider2;
	void Save();//�Զ���������б�
	void check();//��鲥��ģʽ����
	afx_msg void OnTimer(UINT_PTR nIDEvent);//��ʱ�������ڴ����豸
	afx_msg void OnBnClickedAdd();//������ְ�ť�Ŀ���
	afx_msg void OnLbnSelchangeList();//�����б����
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);//�����������Ŀ���
	afx_msg void OnBnClickedDelete();//ɾ����������
	afx_msg void OnBnClickedPlay();//�������ְ�ť����
	afx_msg void OnBnClickedPause();//��ͣ���Ű�ť����
	afx_msg void OnBnClickedPrevious();//��һ�װ�ť����
	afx_msg void OnBnClickedNext();//��һ�װ�ť����
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
};