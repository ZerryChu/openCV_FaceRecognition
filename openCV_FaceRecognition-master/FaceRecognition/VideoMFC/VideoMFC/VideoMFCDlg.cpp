
// VideoMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoMFC.h"
#include "VideoMFCDlg.h"
#include "afxdialogex.h"
#include "map_and_path.h"
#include "socket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CvCapture* capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
CvVideoWriter* writer = 0;
IplImage *resizeRes;//��ż�⵽������
IplImage* faceGray;//��ż�⵽������ �Ҷ�ͼ��
bool bool_cameOpen = false;//ȫ�ֱ��� ��־����ͷ�Ƿ��
bool bool_picNum = false;//ȫ�ֱ��� ��־ѵ��ͼƬ�Ƿ�Ϊ��
bool bool_detec_reco = false;//ȫ�ֱ��� 
double dConfidence;//���Ŷ�

CvMemStorage* storage = 0;
CvHaarClassifierCascade* cascade = 0;
CvHaarClassifierCascade* nested_cascade = 0;
int use_nested_cascade = 0;
const char* cascade_name =
    "../data/haarcascades/haarcascade_frontalface_alt.xml";
const char* nested_cascade_name =
    "../data/haarcascade_eye_tree_eyeglasses.xml";
double scale = 1;
int num_components = 9;
double facethreshold = 9.0;
//cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
//cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer();//LBP����������ڵ���������֤����Ч�����
//cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
vector<Mat> images;
vector<int> labels;

IplImage *frame, *frame_copy = 0;
IplImage *image = 0;
const char* scale_opt = "--scale="; // ������ѡ��ָʾ���� 
int scale_opt_len = (int)strlen(scale_opt);
const char* cascade_opt = "--cascade=";
int cascade_opt_len = (int)strlen(cascade_opt);
const char* nested_cascade_opt = "--nested-cascade";
int nested_cascade_opt_len = (int)strlen(nested_cascade_opt);
int i;
const char* input_name = 0;

// CAboutDlg dialog used for App About
CString strConfidence = "4000";
CEdit* pEdtConfidence;
CString strTip = "";
CEdit* pTip;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoMFCDlg dialog




CVideoMFCDlg::CVideoMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CVideoMFCDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CVideoMFCDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EdtConfidence, &CVideoMFCDlg::OnEnChangeEdtconfidence)
	ON_BN_CLICKED(IDC_Photograph, &CVideoMFCDlg::OnBnClickedPhotograph)
	ON_BN_CLICKED(IDC_Recognize, &CVideoMFCDlg::OnBnClickedRecognize)
	ON_BN_CLICKED(IDC_ClearPictures, &CVideoMFCDlg::OnBnClickedOpenServer)
END_MESSAGE_MAP()


// CVideoMFCDlg message handlers
BOOL CVideoMFCDlg::OnDestroy()
{
	cvReleaseImage( &resizeRes );
	cvReleaseImage( &faceGray );
	return TRUE; 
}
BOOL CVideoMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	pwnd = GetDlgItem(IDC_ShowImage);
	//pwnd->MoveWindow(35,30,352,288);
    pDC =pwnd->GetDC();
	//pDC =GetDC();
    hDC= pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_Photograph)->EnableWindow(false);
	GetDlgItem(IDC_Recognize)->EnableWindow(false);
	pEdtConfidence = (CEdit*) GetDlgItem(IDC_EdtConfidence);
	pTip = (CEdit*) GetDlgItem(IDC_Tip);
	pEdtConfidence->SetWindowText(strConfidence);
	// pEdtConfidence->GetWindowText(strConfidence);
	pTip->SetWindowText( strTip );
	if(read_img_number()>0)
		bool_picNum = true;
	else
		bool_picNum = false;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoMFCDlg::OnPaint()
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
HCURSOR CVideoMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*****************************************������ͷ*******************************************/
void CVideoMFCDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
	}

	if (!capture)
	{
		AfxMessageBox("�޷�������ͷ");
		return;
	}

	//writer=cvCreateVideoWriter("MyVideo.avi",CV_FOURCC('x','v','I','D'),25,cvSize(640,480));
	IplImage* m_Frame;
	m_Frame=cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame,1);	
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
	}

	// ��ȡѵ�����ݣ���ȡimages�����Ӧ��labels
	read_trainningFiles();

	// ���ü�ʱ��,ÿ10ms����һ���¼�
	SetTimer(1,10,NULL);
	cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 ); // ���ط����� 
    if( !cascade ) 
    {
        MessageBox("�޷����ط������ļ�����ȷ�ϣ�");
    }
    storage = cvCreateMemStorage(0); // �����ڴ�洢�� 
    
    //if( !input_name || (isdigit(input_name[0]) && input_name[1] == '\0') ) // �ж������������Ƶ��ţ������ļ� 
     //   capture = cvCaptureFromCAM( !input_name ? 0 : input_name[0] - '0' ); // ������Ƶ��ȡ�ṹ 
	/*
    else if( input_name )
    {
        image = cvLoadImage( input_name, 1 ); // �����ͼ������� 
        if( !image )
		{
			capture = cvCaptureFromAVI(input_name); // ����ͼ��������Ƶ��ȡ, input_name ��Ƶ�ļ���
			cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640); 
			cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480); ////////////////////////////////////////////////////////////////////
		}
    }*/
    //else
     //   image = cvLoadImage( "timg.jpg", 1 ); //��û������ó�������Ŀ¼��lena.jpgͼƬ 
    //cvNamedWindow( "result", 1 );
	
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_Photograph)->EnableWindow(true);
	GetDlgItem(IDC_Recognize)->EnableWindow(true);
	bool_detec_reco = false;
	bool_cameOpen = true;
}


/********************************************���ö�ʱ��*********************************************/
void CVideoMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	extern map<int, Person > mp;

	int person_number = -1;
	int isRight = 1;
	int notRight[30];

	//��ʾ����ͷ
	IplImage* m_Frame;

	//������ͷ�����ļ�ץȡ1֡
	m_Frame=cvQueryFrame(capture);
	//AllocConsole();
	//�ж��Ǽ�⻹��ʶ������
	if (bool_cameOpen)
	{
		if (!bool_detec_reco) //falseֻΪ���
		{
			detect_and_draw(m_Frame);
		}
		else if (bool_picNum) { //false����ѵ��ͼƬΪ��
			person_number = recog_and_draw(m_Frame);//����ʶ������
			
			if (1 == isRight) {
				isRight = -1;
				memset(notRight, 0, sizeof(notRight));
			}
			if (person_number != -1 && person_number != 0 && notRight[person_number] != 1) {
				bool_detec_reco = false;

				// mp[person_number].info.c_str();
				// mp[person_number].name.c_str();
				if (1 == attendence[person_number]) {
					string result = mp[person_number].name + "������˱���ǩ��";
					::MessageBox(NULL, result.c_str(), "��ʾ", MB_OK);
				}
				else {
					string result = "��֤���Ϊ: " + mp[person_number].name;
					if (::MessageBox(NULL, result.c_str(), "��֤���", MB_YESNO) == IDYES){
						attendence[person_number] = 1;
						// ����notRight����
						isRight = 1;

						char* tipPhoto;
						if (person_number != -1 && person_number != 0) {
							pTip->SetWindowText(mp[person_number].info.c_str());
						}
						else {
							pTip->SetWindowText("");
						}
						// ��̨�ύǩ������
						// attend(person_number);
						//AfxMessageBox(mp[person_number].name.c_str());
					}
					else {
						notRight[person_number] = 1;
					}
				}
			}
			// ��֤��Ƭ
			//person_number = recog_and_draw2("C:\\Users\\Administrator\\Desktop\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\sourcePic\\target.jpg");
			
		}
	}
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame,1);	
	m_CvvImage.DrawToHDC(hDC, &rect);
	//cvWriteFrame(writer,m_Frame); //��֡ͼ��ͨ��writerд���ļ�
	//cvWaitKey(10);

	CDialogEx::OnTimer(nIDEvent);
}

//�ر�����ͷ��ť
void CVideoMFCDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	cvReleaseVideoWriter(&writer);
	cvReleaseCapture(&capture);
	CDC MemDC; 
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1); 
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
	GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_Photograph)->EnableWindow(false);
	GetDlgItem(IDC_Recognize)->EnableWindow(false);
	bool_cameOpen = false;
}

//�رմ���
void CVideoMFCDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	cvReleaseCapture(&capture);
	CDialogEx::OnClose();
}



void CVideoMFCDlg::OnEnChangeEdtconfidence()
{
}

bool write_img_info(int number, int img_number) {
	FILE *fp = fopen(FILEPATH, "a+");
	if (fp == 0) {
	return false;
	}
	fseek(fp, 0, SEEK_END);
	stringstream s, ss;
	ss << number;
	s << img_number;
	string str = PICTUREPATH + s.str() + ".jpg;" + ss.str() + "\n";
	fwrite(str.c_str(), str.size(), 1, fp);
	fclose(fp);
	return true;
}

//���հ�ť(�����ջ�ȡѵ�����ݣ���������֤)
void CVideoMFCDlg::OnBnClickedPhotograph()
{	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(capture);
	if (!faceGray)
	{
		pTip->GetWindowText(strTip);
		CString tipPhoto = strTip + "\r\n����ʧ�ܣ��뽫����ͷ��׼����";
		pTip->SetWindowText( tipPhoto );
		return;
	}
	Mat img(faceGray,0);
	stringstream ss;
	int img_number = read_img_number() + 1;
	ss << img_number;
	string faceImgName = "..//einfacedata//trainingdata//" + ss.str() + ".jpg";
	imwrite(faceImgName,img);

	//pTip->GetWindowText(strTip);
	CString tipPhoto = strTip + "\r\n���ճɹ����Ѵ�Ϊ" + (ss.str() + ".jpg").c_str();
	int number;
	if ( 0 < (number = recog_and_draw(m_Frame)) ) {
		if (write_img_info(number, img_number)) {
			tipPhoto += "  at.txt�Ѹ���.";
			pTip->SetWindowText(tipPhoto);
		}
		else {
			tipPhoto += "  at.txt���´���.";
			pTip->SetWindowText(tipPhoto);
		}
	}
	else {
		pTip->SetWindowText(tipPhoto);
	}
}

//��ʼor�ر���֤��ť
void CVideoMFCDlg::OnBnClickedRecognize()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	pEdtConfidence->GetWindowText(strConfidence);
	try
	{
		dConfidence = atoi((const char *)strConfidence);
	}
	catch (cv::Exception &e)
	{
		MessageBox("���Ŷ�������������");
		return;
	}

	if (bool_detec_reco == false)
		bool_detec_reco = true;
	else
		bool_detec_reco = false;
	bool_picNum = true;
}



// �ֻ�����չ����
//��ʼ����ͼƬԴ������
void CVideoMFCDlg::OnBnClickedOpenServer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ������Ҫ������չΪ���½���ִ��
	server();
}
