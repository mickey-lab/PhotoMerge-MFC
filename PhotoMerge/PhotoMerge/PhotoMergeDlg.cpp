#include "stdafx.h"
#include "PhotoMerge.h"
#include "PhotoMergeDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
//#pragma warning(disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
};


CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()


void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	ShellExecute(NULL, NULL, _T("http://nancom.tistory.com/"), NULL, NULL, SW_SHOWNORMAL);
}


// CPhotoMergeDlg 대화 상자
CPhotoMergeDlg::CPhotoMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PHOTOMERGE_DIALOG, pParent)
	, mergeSizeX(_T("640"))
	, mergeSizeY(_T("480"))
	, arrangemNumX(_T(""))
	, arrangemNumY(_T(""))
	, arrangemSpace(_T("15"))
	, saveFileName(_T("Output"))
	, windowTopMost(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CPhotoMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHOTO_CNT, dropFilesNumCtrl);

	DDX_Control(pDX, IDC_SIZE_EDIT1, mergeSizeCtrlX);
	DDX_Text(pDX, IDC_SIZE_EDIT1, mergeSizeX);
	DDV_MaxChars(pDX, mergeSizeX, 5);
	DDX_Control(pDX, IDC_SIZE_EDIT2, mergeSizeCtrlY);
	DDX_Text(pDX, IDC_SIZE_EDIT2, mergeSizeY);
	DDV_MaxChars(pDX, mergeSizeY, 5);
	DDX_Control(pDX, IDC_SIZE_COMBO1, mergeSizeComboCtrl);

	DDX_Control(pDX, IDC_SIZE_EDIT3, arrangemNumCtrlX);
	DDX_Text(pDX, IDC_SIZE_EDIT3, arrangemNumX);
	DDV_MaxChars(pDX, arrangemNumX, 3);
	DDX_Control(pDX, IDC_SIZE_EDIT4, arrangemNumCtrlY);
	DDX_Text(pDX, IDC_SIZE_EDIT4, arrangemNumY);
	DDV_MaxChars(pDX, arrangemNumY, 3);

	DDX_Control(pDX, IDC_EDIT1, arrangemSpaceCtrl);
	DDX_Text(pDX, IDC_EDIT1, arrangemSpace);
	DDV_MaxChars(pDX, arrangemSpace, 4);

	DDX_Check(pDX, IDC_WINDOW_CHECK1, windowTopMost);
	DDX_Control(pDX, IDC_WINDOW_TRNS_SLIDE, transSliderCtrl);

	DDX_Control(pDX, IDC_SAVE_EDIT1, saveFileNameCtrl);
	DDX_Text(pDX, IDC_SAVE_EDIT1, saveFileName);
	DDV_MaxChars(pDX, saveFileName, 20);
	DDX_Control(pDX, IDC_SAVE_RADIO1, saveDefFolderRadioCtrl);
	DDX_Control(pDX, IDC_SAVE_RADIO2, saveDifFolderRadioCtrl);
	DDX_Control(pDX, IDC_SAVE_EDIT2, saveDifFolderCtrl);
	DDX_Control(pDX, IDC_CHECK2, saveFileOption1);
}

BEGIN_MESSAGE_MAP(CPhotoMergeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_WINDOW_CHECK1, &CPhotoMergeDlg::OnClickWindowTopMostCheck)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WINDOW_TRNS_SLIDE, &CPhotoMergeDlg::OnNMCustomdrawTransSlider)
	ON_CBN_SELCHANGE(IDC_SIZE_COMBO1, &CPhotoMergeDlg::OnSelectMergeSizeCombo)
	ON_BN_CLICKED(IDC_SAVE_SET_BUTTON2, &CPhotoMergeDlg::OnSelectSaveDifFolder)
	ON_BN_CLICKED(IDC_SAVE_OPEN_BUTTON1, &CPhotoMergeDlg::OnOpenSaveDefFolder)
	ON_BN_CLICKED(IDC_SAVE_OPEN_BUTTON2, &CPhotoMergeDlg::OnOpenSaveDifFolder)
	ON_BN_CLICKED(IDOK, &CPhotoMergeDlg::OnMergePhotos)
	ON_EN_CHANGE(IDC_SAVE_EDIT1, &CPhotoMergeDlg::OnEnChangeSaveEdit1)
	ON_BN_CLICKED(IDABOUT, &CPhotoMergeDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_SAVE_CHECK1, &CPhotoMergeDlg::OnBnClickSaveFileOption1)
END_MESSAGE_MAP()


// CPhotoMergeDlg 메시지 처리기
BOOL CPhotoMergeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// 크기 조절에 콤보 박스 셋팅
	mergeSizeComboCtrl.AddString(_T("사용자 지정"));
	mergeSizeComboCtrl.AddString(_T("640 x 480(4:3)"));
	mergeSizeComboCtrl.AddString(_T("800 x 600(4:3)"));
	mergeSizeComboCtrl.AddString(_T("1024 x 768(4:3)"));
	mergeSizeComboCtrl.AddString(_T("1280 x 960(4:3)"));
	mergeSizeComboCtrl.AddString(_T("1600 x 1200(4:3)"));
	mergeSizeComboCtrl.AddString(_T("1920 x 1440(4:3)"));
	mergeSizeComboCtrl.AddString(_T("2560 x 1920(4:3)"));
	mergeSizeComboCtrl.AddString(_T("3200 x 2400(4:3)"));
	mergeSizeComboCtrl.AddString(_T("4096 x 3072(4:3)"));
	mergeSizeComboCtrl.AddString(_T("6400 x 4800(4:3)"));
	mergeSizeComboCtrl.AddString(_T("640 x 360(16:9)"));
	mergeSizeComboCtrl.AddString(_T("1024 x 576(16:9)"));
	mergeSizeComboCtrl.AddString(_T("1280 x 720(16:9)"));
	mergeSizeComboCtrl.AddString(_T("1600 x 900(16:9)"));
	mergeSizeComboCtrl.AddString(_T("2048 x 1152(16:9)"));
	mergeSizeComboCtrl.AddString(_T("2560 x 1440(16:9)"));
	mergeSizeComboCtrl.AddString(_T("3200 x 1800(16:9)"));
	mergeSizeComboCtrl.AddString(_T("3840 x 2160(16:9)"));
	mergeSizeComboCtrl.AddString(_T("5120 x 2880(16:9)"));
	mergeSizeComboCtrl.AddString(_T("7680 x 4320(16:9)"));
	mergeSizeComboCtrl.AddString(_T("8192 x 4608(16:9)"));
	mergeSizeComboCtrl.SetCurSel(1);

	// 항상 위 체크
	if (windowTopMost)
	{
		//AfxMessageBox(_T("Checked"));
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	// 다이얼로그 투명화를 위한 초기 셋팅
	transExtendedStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, transExtendedStyle | WS_EX_LAYERED);

	// 투명도 슬라이더 셋팅
	transSliderCtrl.SetRange(80, 255);	// 투명도 슬라이더 범위
	transSliderCtrl.SetPos(255);
	transSliderCtrl.SetLineSize(2);
	::SetLayeredWindowAttributes(GetSafeHwnd(), 0, transSliderCtrl.GetPos(), LWA_ALPHA);

	// 저장폴더 Radio버튼 설정
	saveDefFolderRadioCtrl.SetCheck(BST_CHECKED);
	saveDifFolderRadioCtrl.SetCheck(BST_UNCHECKED);

	// 다른폴더 저장 초기 경로 셋팅
	TCHAR desktopPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, (LPWSTR)desktopPath, CSIDL_DESKTOP, FALSE);
	saveDifFolder = desktopPath;
	saveDifFolder += _T("\\");
	saveDifFolderCtrl.SetWindowTextW(saveDifFolder);
	//AfxMessageBox(saveDifFolder);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CPhotoMergeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 아래 코드가 필요합니다.
// 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는 프레임워크에서 이 작업을 자동으로 수행합니다.
void CPhotoMergeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서 이 함수를 호출합니다.
HCURSOR CPhotoMergeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPhotoMergeDlg::checkIntToCString(CString str, int w)
{
	CString temp;
	temp.Format(_T("%d"), w);
	AfxMessageBox(str + temp);
}

void CPhotoMergeDlg::checkIntToCString(CString str, int w, int x)
{
	CString temp;
	temp.Format(_T("%d, %d"), w, x);
	AfxMessageBox(str + temp);
}

void CPhotoMergeDlg::checkIntToCString(CString str, int w, int x, int y)
{
	CString temp;
	temp.Format(_T("%d, %d, %d"), w, x, y);
	AfxMessageBox(str + temp);
}

void CPhotoMergeDlg::checkIntToCString(CString str, int w, int x, int y, int z)
{
	CString temp;
	temp.Format(_T("%d, %d, %d, %d"), w, x, y, z);
	AfxMessageBox(str + temp);
}

void CPhotoMergeDlg::OnDropFiles(HDROP hDropInfo)
{
	CString strFilePath;
	DWORD nBuffer = 0;

	CString strFilesCnt;

	for (int i = 0; i < 1000; i++)
		dropFilesPath[i] = "";

	// 드래그 드롭된 파일의 갯수
	dropFilesNum = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (dropFilesNum < 1)
	{
		AfxMessageBox(_T("1개 이상의 사진이 필요합니다."));
		return;
	}

	for (int i = 0; i < dropFilesNum; i++)
	{
		nBuffer = DragQueryFile(hDropInfo, i, NULL, 0);

		// 파일의 경로 얻어옴
		DragQueryFile(hDropInfo, i, strFilePath.GetBuffer(nBuffer + 1), nBuffer + 1);
		dropFilesPath[i] = strFilePath;
		strFilePath.ReleaseBuffer();
		//AfxMessageBox(dropFilesPath[i]);
	}
	DragFinish(hDropInfo);

	// 드래그 드롭된 파일의 갯수 Static Text Box에 표시
	strFilesCnt.Format(_T("첨부된 사진: %d"), dropFilesNum);
	dropFilesNumCtrl.SetWindowTextW(strFilesCnt);

	// 사진 배열 값 Edit Text Box에 표시
	if ((dropFilesNum % 2) == 0)
	{
		// 세로 사진 배열 갯수를 1/2로 맞춤
		CString numY;
		numY.Format(_T("%d"), dropFilesNum / 2);

		arrangemNumCtrlX.SetWindowTextW(_T("2"));
		arrangemNumCtrlY.SetWindowTextW(numY);
	}
	else
	{
		// 세로 사진 배열 갯수를 1/2로 맞춤
		CString numY;
		numY.Format(_T("%d"), dropFilesNum / 2 + 1);
	
		if (dropFilesNum != 1)
			arrangemNumCtrlX.SetWindowTextW(_T("2"));
		else
			arrangemNumCtrlX.SetWindowTextW(_T("1"));

		arrangemNumCtrlY.SetWindowTextW(numY);
	}
	

	// 저장폴더 셋팅
	int i = dropFilesPath[0].ReverseFind('\\');			// 파일 이름을 지우기 위해서 오른쪽 마지막 '/'를 찾는다.
	saveDefFolder = dropFilesPath[0].Left(i) + _T("\\");	// i칸까지 경로가 확보된다(뒤에 있는 파일 이름이 지워짐)
	//AfxMessageBox(saveDefFolder);

	//AfxMessageBox(strFilesCnt + _T("개의 사진을 불러왔습니다."));

	CDialogEx::OnDropFiles(hDropInfo);
}


// 항상 위 함수
void CPhotoMergeDlg::OnClickWindowTopMostCheck()
{
	if (windowTopMost)
	{
		//AfxMessageBox(_T("UnChecked"));
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		windowTopMost = false;
	}
	else if (windowTopMost == false)
	{
		//AfxMessageBox(_T("Checked"));
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		windowTopMost = true;
	}
}


// 투명도 슬라이더 조절 함수
void CPhotoMergeDlg::OnNMCustomdrawTransSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	::SetLayeredWindowAttributes(GetSafeHwnd(), 0, transSliderCtrl.GetPos(), LWA_ALPHA);

	*pResult = 0;
}


// 사이즈 선택 함수
void CPhotoMergeDlg::OnSelectMergeSizeCombo()
{
	switch (mergeSizeComboCtrl.GetCurSel())
	{
	case 1:
		mergeSizeCtrlX.SetWindowTextW(_T("640"));
		mergeSizeCtrlY.SetWindowTextW(_T("480"));
		break;
	case 2:
		mergeSizeCtrlX.SetWindowTextW(_T("800"));
		mergeSizeCtrlY.SetWindowTextW(_T("600"));
		break;
	case 3:
		mergeSizeCtrlX.SetWindowTextW(_T("1024"));
		mergeSizeCtrlY.SetWindowTextW(_T("768"));
		break;
	case 4:
		mergeSizeCtrlX.SetWindowTextW(_T("1280"));
		mergeSizeCtrlY.SetWindowTextW(_T("960"));
		break;
	case 5:
		mergeSizeCtrlX.SetWindowTextW(_T("1600"));
		mergeSizeCtrlY.SetWindowTextW(_T("1200"));
		break;
	case 6:
		mergeSizeCtrlX.SetWindowTextW(_T("1920"));
		mergeSizeCtrlY.SetWindowTextW(_T("1440"));
		break;
	case 7:
		mergeSizeCtrlX.SetWindowTextW(_T("2560"));
		mergeSizeCtrlY.SetWindowTextW(_T("1920"));
		break;
	case 8:
		mergeSizeCtrlX.SetWindowTextW(_T("3200"));
		mergeSizeCtrlY.SetWindowTextW(_T("2400"));
		break;
	case 9:
		mergeSizeCtrlX.SetWindowTextW(_T("4096"));
		mergeSizeCtrlY.SetWindowTextW(_T("3072"));
		break;
	case 10:
		mergeSizeCtrlX.SetWindowTextW(_T("6400"));
		mergeSizeCtrlY.SetWindowTextW(_T("4800"));
		break;
	case 11:
		mergeSizeCtrlX.SetWindowTextW(_T("640"));
		mergeSizeCtrlY.SetWindowTextW(_T("360"));
		break;
	case 12:
		mergeSizeCtrlX.SetWindowTextW(_T("1024"));
		mergeSizeCtrlY.SetWindowTextW(_T("480"));
		break;
	case 13:
		mergeSizeCtrlX.SetWindowTextW(_T("1280"));
		mergeSizeCtrlY.SetWindowTextW(_T("720"));
		break;
	case 14:
		mergeSizeCtrlX.SetWindowTextW(_T("1600"));
		mergeSizeCtrlY.SetWindowTextW(_T("900"));
		break;
	case 15:
		mergeSizeCtrlX.SetWindowTextW(_T("2048"));
		mergeSizeCtrlY.SetWindowTextW(_T("1152"));
		break;
	case 16:
		mergeSizeCtrlX.SetWindowTextW(_T("2560"));
		mergeSizeCtrlY.SetWindowTextW(_T("1440"));
		break;
	case 17:
		mergeSizeCtrlX.SetWindowTextW(_T("3200"));
		mergeSizeCtrlY.SetWindowTextW(_T("1800"));
		break;
	case 18:
		mergeSizeCtrlX.SetWindowTextW(_T("3840"));
		mergeSizeCtrlY.SetWindowTextW(_T("2160"));
		break;
	case 19:
		mergeSizeCtrlX.SetWindowTextW(_T("5120"));
		mergeSizeCtrlY.SetWindowTextW(_T("2880"));
		break;
	case 20:
		mergeSizeCtrlX.SetWindowTextW(_T("7680"));
		mergeSizeCtrlY.SetWindowTextW(_T("4320"));
		break;
	case 21:
		mergeSizeCtrlX.SetWindowTextW(_T("8192"));
		mergeSizeCtrlY.SetWindowTextW(_T("4608"));
		break;
	default:
		break;
	}
}


// 기본 저장 폴더 열기 
void CPhotoMergeDlg::OnOpenSaveDefFolder()
{
	if (saveDefFolder != "")
	{
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), saveDefFolder, NULL, SW_SHOW);
	}
	else
	{
		AfxMessageBox(_T("사진을 첨부해주세요."));
	}
}


// 지정된 저장 폴더 열기 
void CPhotoMergeDlg::OnOpenSaveDifFolder()
{
	ShellExecute(NULL, _T("open"), _T("explorer.exe"), saveDifFolder, NULL, SW_SHOW);
}


// 파일 이름 변경시 이름 변수 변경 함수
void CPhotoMergeDlg::OnEnChangeSaveEdit1()
{
	saveFileNameCtrl.GetWindowTextW(saveFileName);
}


// 저장 폴더 지정하는 브라우저
void CPhotoMergeDlg::OnSelectSaveDifFolder()
{
	BROWSEINFO bi;
	TCHAR szBuffer[MAX_PATH];
	::ZeroMemory(&bi, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, MAX_PATH);

	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = _T("파일이 저장될 폴더를 선택해주세요.");
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST itemIdList = ::SHBrowseForFolder(&bi);
	if (::SHGetPathFromIDList(itemIdList, szBuffer))
	{
		// Radio버튼 설정
		saveDefFolderRadioCtrl.SetCheck(BST_UNCHECKED);
		saveDifFolderRadioCtrl.SetCheck(BST_CHECKED);

		saveDifFolder = szBuffer;
		saveDifFolder += _T("\\");
		saveDifFolderCtrl.SetWindowTextW(saveDifFolder);
	}
}


// 합성 단추 함수
void CPhotoMergeDlg::OnMergePhotos()
{
	if (dropFilesNum < 1)
	{
		AfxMessageBox(_T("1개 이상의 사진이 필요합니다."));
		return;
	}

	if (saveDefFolderRadioCtrl.GetCheck())
	{
		saveFolder = saveDefFolder + saveFileName + _T(".jpg");
	}
	else
	{
		saveFolder = saveDifFolder + saveFileName + _T(".jpg");
	}

	CFileFind pFind;
	BOOL bRet = pFind.FindFile(saveFolder);
	if (bRet)
	{
		int nResult;
		nResult = AfxMessageBox(saveFolder + _T(" 파일이 이미 존재 합니다.\n덮어 쓰시겠습니까?"), MB_YESNO | MB_ICONWARNING);
		
		if (nResult == IDNO)
			return;
	}

	if (GdiplusStartup(&gdiplustToken, &gdiplusStartupInput, NULL) == Ok) {
		// Editbox에서 정보 가져오기
		CString tempMergeX, tempMergeY;
		mergeSizeCtrlX.GetWindowTextW(tempMergeX);
		mergeSizeCtrlY.GetWindowTextW(tempMergeY);
		int mergeX = _ttoi(tempMergeX);
		int mergeY = _ttoi(tempMergeY);

		CString tempArrangemNumX, tempArrangemNumY;
		arrangemNumCtrlX.GetWindowTextW(tempArrangemNumX);
		arrangemNumCtrlY.GetWindowTextW(tempArrangemNumY);
		int arrX = _ttoi(tempArrangemNumX);
		int arrY = _ttoi(tempArrangemNumY);

		CString tempArrangemSpace;
		arrangemSpaceCtrl.GetWindowTextW(tempArrangemSpace);
		int arrSpace = _ttoi(tempArrangemSpace);

		// 각각의 사진이 가져야할 크기
		int eaPhotoSizeX = (mergeX - (arrX + 1) * arrSpace) / arrX;
		int eaPhotoSizeY = (mergeY - (arrY + 1) * arrSpace) / arrY;
		int posX = arrSpace;
		int posY = arrSpace;

		// 그리기 작업을 위한 객체 선언
		CDC *dc = GetDC();
		CDC memDC;
		memDC.CreateCompatibleDC(dc);

		CBitmap bmpbuffer;
		bmpbuffer.CreateCompatibleBitmap(dc, mergeX, mergeY); // 호환성 조절
		CBitmap *oldBitmap = (CBitmap *)memDC.SelectObject(&bmpbuffer);

		CBrush brush;
		memDC.FillSolidRect(0, 0, mergeX, mergeY, RGB(255, 255, 255));

		Graphics graphics(memDC);
		CImage	image;
		CRect	rc;
		HRESULT hResult = NULL;

		// ArrangemNumY 개수
		int num = 0;
		for (int i = 0; i < arrY; i++, posX = arrSpace, posY += (eaPhotoSizeY + arrSpace))
		{
			// ArrangemNumX 개수
			for (int j = 0; (j < arrX) && (num < dropFilesNum); j++, posX += (eaPhotoSizeX + arrSpace))
			{
				hResult = image.Load(dropFilesPath[num++]);

				if (FAILED(hResult))
				{
					AfxMessageBox(_T("사진파일을 인식할 수 없습니다.\n다시 확인해주세요."));
					return;
				}

				if ((num == dropFilesNum) && (dropFilesNum % 2 == 1))
				{
					// 남은 여백 구하는 공식
					int tmpWidth = posX + eaPhotoSizeX * (arrX - j) + arrSpace * (arrX - j - 1);

					rc.SetRect(posX, posY, tmpWidth, posY + eaPhotoSizeY);
					image.Draw(graphics.GetHDC(), rc, InterpolationModeDefault);
					image.Destroy();
					graphics.ReleaseHDC(memDC);

					continue;
				}

				//checkIntToCString(_T("i, arrX, j, arrY "), i, arrX, j, arrY);
				//checkIntToCString(_T("좌표 "), posX, posY, eaPhotoSizeX, eaPhotoSizeY);
				rc.SetRect(posX, posY, posX + eaPhotoSizeX, posY + eaPhotoSizeY);
				image.Draw(graphics.GetHDC(), rc, InterpolationModeDefault);
				image.Destroy();
				graphics.ReleaseHDC(memDC);
			}
		}

		HPALETTE hpal;
		hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

		//Gdiplus::Bitmap을 만든다!!!  
		Bitmap bitmap((HBITMAP)bmpbuffer, hpal);
		int width = bitmap.GetWidth();
		int height = bitmap.GetHeight();

		/* GDI plus로 저장하기 !! */
		CLSID				encoderClsid;
		EncoderParameters	encoderParameters;
		ULONG				quality;

		encoderParameters.Count = 1;
		encoderParameters.Parameter[0].Guid = EncoderQuality;
		encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
		encoderParameters.Parameter[0].NumberOfValues = 1;

		// 이미지 jpg 저장시 Quality level 100.
		quality = 100;
		encoderParameters.Parameter[0].Value = &quality;

		GetEncoderClsid(_T("image/jpeg"), &encoderClsid);
		bitmap.Save(saveFolder, &encoderClsid, &encoderParameters);

		ReleaseDC(&memDC);
	}
	else
	{
		AfxMessageBox(_T("GDI+ 클래스를 초기화 하지 못했습니다.\n제작자에게 문의하세요."));
		return;
	}

	GdiplusShutdown(gdiplustToken);


	// 사진 파일 제거 옵션이 활성화 되어 있을 경우 사진 파일 제거
	if (saveFileOption1.GetCheck())
	{
		if (GetWindowsType() <= 7)	// Windows XP 이하
		{
			// SHFILEOPSTRUCT에서 마지막 파일 구분이 NULL 두개인데 아래 for문에서
			// NULL을 한개씩만 더해주므로 1부터 시작
			int totallyPathLen = 1;
			for (int i = 0; i < dropFilesNum; i++)
			{
				// 파일 경로 길이 + NULL값 공간
				totallyPathLen += dropFilesPath[i].GetLength() + 1;
			}

			int p = 0;
			TCHAR *pFrom = new TCHAR[totallyPathLen];	// FileOp.pFrom에 사용할 변수
			for (int i = 0; i < dropFilesNum; i++)
			{
				TCHAR *tempDropFilesPath = (TCHAR*)(LPCTSTR)dropFilesPath[i];

				for (int len = 0; len < dropFilesPath[i].GetLength(); len++)
				{
					pFrom[p++] = tempDropFilesPath[len];
				}
				pFrom[p++] = '\0';
			}
			pFrom[p] = '\0';

			// 사진 파일을 휴지통으로 보내려 할때 사용하는 옵션
			SHFILEOPSTRUCT FileOp;
			FileOp.hwnd = NULL;
			FileOp.wFunc = FO_DELETE;
			FileOp.pFrom = pFrom;
			FileOp.pTo = NULL;
			FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI; // 확인메시지가 안뜨도록 설정 
			FileOp.fAnyOperationsAborted = false;
			FileOp.hNameMappings = NULL;
			FileOp.lpszProgressTitle = NULL;
			
			// SHFileOperation() return codes
			// https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb762164%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
			if (int i = SHFileOperation(&FileOp) != 0)	// 0 이외에는 정상적인 성공이 아님
			{
				AfxMessageBox(_T("파일을 지우지 못했습니다."));
			}

			delete[] pFrom;
		}	
		else	// Vista 이상
		{

		}
	}

	AfxMessageBox(_T("합성한 사진이 저장되었습니다.\n경로: \"") + saveFolder + _T("\""));
}


// 이미지 저장에 필요한 함수
int CPhotoMergeDlg::GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo * pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo *)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}


// About Box 여는 함수
void CPhotoMergeDlg::OnBnClickedAbout()
{
	static CAboutDlg Dlg;

	if (Dlg.GetSafeHwnd() == NULL)
		Dlg.Create(IDD_ABOUTBOX);

	Dlg.ShowWindow(SW_SHOW);
}


void CPhotoMergeDlg::OnBnClickSaveFileOption1()
{
	// TODO: Add your control notification handler code here
	if (saveFileOption1.GetCheck())
	{
		int nResult;
		nResult = AfxMessageBox(_T("합성에 사용된 사진들이 제거됩니다.\n해당 기능을 사용하시겠습니까?"), MB_YESNO | MB_ICONWARNING);

		if (nResult == IDNO)
			saveFileOption1.SetCheck(FALSE);
	}
}

int CPhotoMergeDlg::GetWindowsType(void)
{
	/*
	return value :
	-1 : 버전얻기 실패
	1 : Windows 95,
	2 : Windows 98,
	3 : Windows ME,
	4 : Windows NT,
	5 : Windows 2000,
	6 : Windows XP,
	7 : Windows XP Professional x64 Edition
		Windows XP Home Server
		Windows Server 2003
		Windows Server 2003 R2
	8 : Windows Vista
		Windows Server 2008
	9 : Windows 7
		winodws 2008 R2
	10: Windows 8
		Windows Server 2012
	11: Windows 8.1
		Windows Server 2012 R2
	12: Windows 10
		Windows Server 2016 Technical Preview

	버전 정보 확인 사이트(OSVERSIONINFOEX structure)
	https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms724833(v=vs.85).aspx
	*/

	int nVersion = -1;
	OSVERSIONINFOEX osvi = { 0, };
	BOOL version_ex_flag = 0;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if (!(version_ex_flag = GetVersionEx((OSVERSIONINFO *)&osvi)))
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&osvi))
			return -1;
	}

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_WINDOWS:    // 윈도우즈 9x 기반의 운영체제인 경우
	{
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			// Windows 95
			nVersion = 1;
		}
		else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			// Windows 98
			nVersion = 2;
		}
		else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			// Windows ME
			nVersion = 3;
		}
	}
	break;
	case VER_PLATFORM_WIN32_NT: // NT 기술 기반의 운영체제인 경우
	{
		if (osvi.dwMajorVersion <= 4)
		{
			// Windows NT
			nVersion = 4;
		}
		else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
		{
			// Windows 2000
			nVersion = 5;
		}
		else if (version_ex_flag)
		{
			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
			{
				// Windows XP
				nVersion = 6;
			}
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
			{
				// Windows XP Professional x64 Edition
				// Windows XP Home Server
				// Windows Server 2003
				// Windows Server 2003 R2
				nVersion = 7;
			}
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
			{
				// Windows Vista
				// Windows Server 2008
				nVersion = 8;
			}
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
			{
				// Windows 7
				// winodws 2008 R2
				nVersion = 9;
			}
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
			{
				// Windows 8
				// Windows Server 2012
				nVersion = 10;
			}
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3)
			{
				// Windows 8.1
				// Windows Server 2012 R2
				nVersion = 11;
			}
			else if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0)
			{
				// Windows 10
				// Windows Server 2016 Technical Preview
				nVersion = 12;
			}
		}
	}
	break;
	}
	
	// 자신의 운영체제 버전 확인
	//checkIntToCString(_T("osvi: "), osvi.dwMajorVersion, osvi.dwMinorVersion);

	return nVersion;
}
