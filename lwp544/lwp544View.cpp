// lwp544View.cpp : implementation of the CLwp544View class
//

#include "stdafx.h"
#include "lwp544.h"
#include "HistogramDlg.h"
#include "lwp544Doc.h"
#include "lwp544View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLwp544View

IMPLEMENT_DYNCREATE(CLwp544View, CView)

BEGIN_MESSAGE_MAP(CLwp544View, CView)
	//{{AFX_MSG_MAP(CLwp544View)
	ON_COMMAND(ID_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(ID_GRAY, OnUpdateGray)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HESTOGRAM, OnHestogram)
	ON_UPDATE_COMMAND_UI(ID_HESTOGRAM, OnUpdateHestogram)
	ON_COMMAND(ID_EQUALIZE, OnEqualize)
	ON_UPDATE_COMMAND_UI(ID_EQUALIZE, OnUpdateEqualize)
	ON_COMMAND(ID_LINETRANS, OnLinetrans)
	ON_UPDATE_COMMAND_UI(ID_LINETRANS, OnUpdateLinetrans)
	ON_COMMAND(ID_FOURIER, OnFourier)
	ON_UPDATE_COMMAND_UI(ID_FOURIER, OnUpdateFourier)
	ON_COMMAND(ID_IFOURIER, OnIfourier)
	ON_UPDATE_COMMAND_UI(ID_IFOURIER, OnUpdateIfourier)
	ON_COMMAND(ID_IFFOURIER, OnIffourier)
	ON_UPDATE_COMMAND_UI(ID_IFFOURIER, OnUpdateIffourier)
	ON_COMMAND(ID_FFOURIER, OnFfourier)
	ON_UPDATE_COMMAND_UI(ID_FFOURIER, OnUpdateFfourier)
	ON_COMMAND(ID_AVE, OnAve)
	ON_UPDATE_COMMAND_UI(ID_AVE, OnUpdateAve)
	ON_COMMAND(ID_MID, OnMid)
	ON_UPDATE_COMMAND_UI(ID_MID, OnUpdateMid)
	ON_COMMAND(ID_STAIR, OnStair)
	ON_UPDATE_COMMAND_UI(ID_STAIR, OnUpdateStair)
	ON_COMMAND(ID_RAPLAS, OnRaplas)
	ON_UPDATE_COMMAND_UI(ID_RAPLAS, OnUpdateRaplas)
	ON_COMMAND(ID_GOSS, OnGoss)
	ON_UPDATE_COMMAND_UI(ID_GOSS, OnUpdateGoss)
	ON_COMMAND(ID_HL, OnHl)
	ON_UPDATE_COMMAND_UI(ID_HL, OnUpdateHl)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLwp544View construction/destruction

CLwp544View::CLwp544View()
{
	// TODO: add construction code here

}

CLwp544View::~CLwp544View()
{
}

BOOL CLwp544View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLwp544View drawing
extern BITMAPINFO *lpBitsInfo;
extern BITMAPINFO *lpDIB_FFT;
extern BITMAPINFO *lpDIB_IFFT;
void CLwp544View::OnDraw(CDC* pDC)
{
	CLwp544Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if (NULL == lpBitsInfo)
		return;

	LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	StretchDIBits(
		pDC->GetSafeHdc(),
		0,0,
		lpBitsInfo->bmiHeader.biWidth,
		lpBitsInfo->bmiHeader.biHeight,
		0,0,
		lpBitsInfo->bmiHeader.biWidth,
		lpBitsInfo->bmiHeader.biHeight,
		lpBits,
		lpBitsInfo,
		DIB_RGB_COLORS,
		SRCCOPY);


	if(lpDIB_FFT)
	{
		lpBits=(LPVOID)&lpDIB_FFT->bmiColors[256];
		StretchDIBits(
			pDC->GetSafeHdc(),
			600,0,
			lpDIB_FFT->bmiHeader.biWidth,
			lpDIB_FFT->bmiHeader.biHeight,
			0,0,
			lpDIB_FFT->bmiHeader.biWidth,
			lpDIB_FFT->bmiHeader.biHeight,
			lpBits,
			lpDIB_FFT,
			DIB_RGB_COLORS,
			SRCCOPY);
	}
	if (lpDIB_IFFT)
	{
		lpBits = (LPVOID)&lpDIB_IFFT->bmiColors[256];
		StretchDIBits( 
			pDC->GetSafeHdc(),
			0,0,
			lpDIB_IFFT->bmiHeader.biWidth,
			lpDIB_IFFT->bmiHeader.biHeight,
			0,0,
			lpDIB_IFFT->bmiHeader.biWidth,
			lpDIB_IFFT->bmiHeader.biHeight,
			lpBits,
			lpDIB_IFFT, // bitmap data 
			DIB_RGB_COLORS,
			SRCCOPY);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CLwp544View printing

BOOL CLwp544View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLwp544View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLwp544View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CLwp544View diagnostics

#ifdef _DEBUG
void CLwp544View::AssertValid() const
{
	CView::AssertValid();
}

void CLwp544View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLwp544Doc* CLwp544View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLwp544Doc)));
	return (CLwp544Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLwp544View message handlers
void gray();
void CLwp544View::OnGray() 
{
	// TODO: Add your command handler code here
	gray();
	Invalidate();
}

void CLwp544View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

void pixel(int i,int j,char*);
void CLwp544View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//鼠标点的位置信息
	char xy[100];
	memset(xy,0,100);//清空字符串，100个字节置0
	sprintf(xy, "x:%d y:%d     ", point.x, point.y);

	//记录坐标点对应像素的值，即颜色值
	char rgb[100];
	memset(rgb,0,100);
	pixel(point.y, point.x, rgb);    //通过rgb读出（rgb用于输出用）
	
	//坐标与颜色值合并
	strcat(xy, rgb);

	//在状态栏中显示
	((CFrameWnd*)GetParent())->SetMessageText(xy);

//	CScrollView::OnMouseMove(nFlags, point);
}

void CLwp544View::OnHestogram() 
{
	// TODO: Add your command handler code here
	CHistogramDlg dlg;
	dlg.DoModal();
}

BOOL IsGray();
void CLwp544View::OnUpdateHestogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());//没有这一段的话可以将所有的图像都画出直方图，有这一段的话只能够画灰度图像
}

void Equalize();
void CLwp544View::OnEqualize() 
{
	// TODO: Add your command handler code here
	Equalize();
	Invalidate();
}

void CLwp544View::OnUpdateEqualize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

void Linetrans(float a,float b);
void CLwp544View::OnLinetrans() 
{
	// TODO: Add your command handler code here
	Linetrans(2,10);
	Invalidate();
}

void CLwp544View::OnUpdateLinetrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}


void Fourier();
void CLwp544View::OnFourier() 
{
	// TODO: Add your command handler code here
	Fourier();
	Invalidate();
}

void CLwp544View::OnUpdateFourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

void IFourier();
void CLwp544View::OnIfourier() 
{
	// TODO: Add your command handler code here
	IFourier();
	Invalidate();
}

BOOL globalFDvalid();
void CLwp544View::OnUpdateIfourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}




void FFourier();
void CLwp544View::OnFfourier() 
{
	// TODO: Add your command handler code here
	FFourier();
	Invalidate();
}

void CLwp544View::OnUpdateFfourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

void IFFourier();
void CLwp544View::OnIffourier() 
{
	// TODO: Add your command handler code here
	if (lpDIB_IFFT)
		free(lpDIB_IFFT);

	IFFourier();
	Invalidate();
}

void CLwp544View::OnUpdateIffourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(globalFDvalid());
}


void AverageFilter();
void CLwp544View::OnAve() 
{
	// TODO: Add your command handler code here
	AverageFilter();
	Invalidate();
}

void CLwp544View::OnUpdateAve(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && (IsGray()));
}

void MedianFilter();
void CLwp544View::OnMid() 
{
	// TODO: Add your command handler code here
	MedianFilter();
	Invalidate();
}

void CLwp544View::OnUpdateMid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && (IsGray()));
}

void GradSharp();
void CLwp544View::OnStair() 
{
	// TODO: Add your command handler code here
	GradSharp();
	Invalidate();
}

void CLwp544View::OnUpdateStair(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && (IsGray()));
}

void RaplasSharpen();
void CLwp544View::OnRaplas() 
{
	// TODO: Add your command handler code here
	RaplasSharpen();
	Invalidate();
}

void CLwp544View::OnUpdateRaplas(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && (IsGray()));
}

void GossFilter();
void CLwp544View::OnGoss() 
{
	// TODO: Add your command handler code here
	GossFilter();
	Invalidate();
}

void CLwp544View::OnUpdateGoss(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && (IsGray()));
}

void Ideal_Filter_FFT(int D);
void CLwp544View::OnHl() 
{
	// TODO: Add your command handler code here
	Ideal_Filter_FFT(50);
	Invalidate();
}

void CLwp544View::OnUpdateHl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(globalFDvalid()&&lpDIB_FFT != NULL);
}
