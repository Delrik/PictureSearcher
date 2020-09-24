
// PictureSearcherDlg.h : header file
//

#pragma once


// CPictureSearcherDlg dialog
class CPictureSearcherDlg : public CDialogEx
{
// Construction
public:
	CPictureSearcherDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICTURESEARCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();
	CEdit editBox;
	CButton button;
	CStatic pictureBox;
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	//My vars
private:
	bool loaded;
	int minX;
	int minY;
public:
	afx_msg void OnExitSizeMove();
};
