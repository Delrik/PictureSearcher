
// PictureSearcherDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PictureSearcher.h"
#include "PictureSearcherDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPictureSearcherDlg dialog



CPictureSearcherDlg::CPictureSearcherDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PICTURESEARCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPictureSearcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SEARCH, editBox);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, button);
	DDX_Control(pDX, IDC_PICTURE, pictureBox);
}

BEGIN_MESSAGE_MAP(CPictureSearcherDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CPictureSearcherDlg::OnBnClickedButtonSearch)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_GETMINMAXINFO()
	ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()


// CPictureSearcherDlg message handlers

BOOL CPictureSearcherDlg::OnInitDialog()
{
	loaded = false;
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CPictureSearcherDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			MessageBox(L"NICE");
			return 1;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPictureSearcherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPictureSearcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
std::string url_encode(const std::string& value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
		std::string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char)c);
		escaped << std::nouppercase;
	}

	return escaped.str();
}
void CPictureSearcherDlg::OnBnClickedButtonSearch()
{
	button.ShowWindow(false);
	editBox.EnableWindow(false);
	CString queryBuf;
	editBox.GetWindowTextW(queryBuf);
	if (queryBuf.IsEmpty()) {
		button.ShowWindow(true);
		editBox.EnableWindow(true);
		return;
	}
	CT2CA ct2caBuf(queryBuf);
	std::string query(ct2caBuf), result;
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::string offset = std::to_string(mersenne() % 101);
	query = url_encode(query);
	query = "https://bing-image-search1.p.rapidapi.com/images/search?offset=" + offset + "&count=1&q=" + query;


	CURL* hnd = curl_easy_init();
	CURLcode curlResult;
	if (hnd) {
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "x-rapidapi-host: bing-image-search1.p.rapidapi.com");
		headers = curl_slist_append(headers, "x-rapidapi-key: bf049f7a77msh37fa3b217d866e0p174151jsn87b3b5dfa07e");

		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(hnd, CURLOPT_URL, query.c_str());
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &result);

		curlResult = curl_easy_perform(hnd);
		curl_easy_cleanup(hnd);
		if (result == "") return;
		int start = result.find("contentUrl") + 14;
		int end = result.find("\"", start);
		
		std::string url = result.substr(start, end - start);
		url.erase(remove(url.begin(), url.end(), '\\'), url.end());

		hnd = curl_easy_init();
		if (hnd) {
			FILE* file;
			file = fopen("1.jpg", "wb");
			if (file == NULL) {	
				exit(1);
			}
			curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
			curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, NULL);
			curl_easy_setopt(hnd, CURLOPT_WRITEDATA, file);
			curlResult = curl_easy_perform(hnd);
			curl_easy_cleanup(hnd);
			if (curlResult) {
				exit(1);
			}
			fclose(file);
			Sleep(1000);

			CImage img;
			img.Load(L"1.jpg");
			if (!img) {
				MessageBox(L"Incorrect request", L"Error", MB_OK);
				Sleep(2000);
				editBox.EnableWindow(true);
				button.ShowWindow(true);
				return;
			}
			//Resize
			CImage sm_img;

			CDC* screenDC = GetDC();
			CDC* pMDC = new CDC;
			pMDC->CreateCompatibleDC(screenDC);

			CBitmap* pb = new CBitmap;
			RECT rc;
			pictureBox.GetWindowRect(&rc);
			int iNewWidth = rc.right - rc.left;
			int iNewHeight = rc.bottom - rc.top;
			pb->CreateCompatibleBitmap(pMDC, iNewWidth, iNewHeight);
			img.StretchBlt(pMDC->m_hDC, 0, 0, iNewWidth, iNewHeight, SRCCOPY);
			// Visualize
			HDC hdcpic = ::GetDC(pictureBox.m_hWnd);
			img.StretchBlt(hdcpic, 0, 0, iNewWidth, iNewHeight, SRCCOPY);

			img.Detach();
			ReleaseDC(screenDC);
		}
		Sleep(2000);
		loaded = true;
		editBox.EnableWindow(true);
		button.ShowWindow(true);
	}

	// TODO: Add your control notification handler code here
}


void CPictureSearcherDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	if (loaded) {
		CImage img;
		img.Load(L"1.jpg");

		//Resize
		CImage sm_img;

		CDC* screenDC = GetDC();
		CDC* pMDC = new CDC;
		pMDC->CreateCompatibleDC(screenDC);

		CBitmap* pb = new CBitmap;
		RECT rc;
		//pictureBox.GetWindowRect(&rc);
		pictureBox.GetClientRect(&rc);
		int iNewWidth = rc.right - rc.left;
		int iNewHeight = rc.bottom - rc.top;
		pb->CreateCompatibleBitmap(pMDC, iNewWidth, iNewHeight);
		img.StretchBlt(pMDC->m_hDC, 0, 0, iNewWidth, iNewHeight, SRCCOPY);
		// Visualize
		HDC hdcpic = ::GetDC(pictureBox.m_hWnd);
		img.StretchBlt(hdcpic, 0, 0, iNewWidth, iNewHeight, SRCCOPY);

		img.Detach();
		ReleaseDC(screenDC);
	}
	// TODO: Add your message handler code here
}


void CPictureSearcherDlg::OnSize(UINT nType, int cx, int cy)
{
	
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
}


void CPictureSearcherDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);
	// TODO: Add your message handler code here
}


void CPictureSearcherDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 990;
	lpMMI->ptMinTrackSize.y = 700;
	
}


void CPictureSearcherDlg::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnExitSizeMove();
	if (loaded) {
		CImage img;
		img.Load(L"1.jpg");

		//Resize
		CImage sm_img;

		CDC* screenDC = GetDC();
		CDC* pMDC = new CDC;
		pMDC->CreateCompatibleDC(screenDC);

		CBitmap* pb = new CBitmap;
		RECT rc;
		//pictureBox.GetWindowRect(&rc);
		pictureBox.GetClientRect(&rc);
		int iNewWidth = rc.right - rc.left;
		int iNewHeight = rc.bottom - rc.top;
		pb->CreateCompatibleBitmap(pMDC, iNewWidth, iNewHeight);
		img.StretchBlt(pMDC->m_hDC, 0, 0, iNewWidth, iNewHeight, SRCCOPY);
		// Visualize
		HDC hdcpic = ::GetDC(pictureBox.m_hWnd);
		img.StretchBlt(hdcpic, 0, 0, iNewWidth, iNewHeight, SRCCOPY);

		img.Detach();
		ReleaseDC(screenDC);
	}
}
