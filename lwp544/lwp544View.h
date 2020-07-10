// lwp544View.h : interface of the CLwp544View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LWP544VIEW_H__E10CF3DE_780C_45F2_9E9A_EE8965BA263B__INCLUDED_)
#define AFX_LWP544VIEW_H__E10CF3DE_780C_45F2_9E9A_EE8965BA263B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLwp544View : public CView
{
protected: // create from serialization only
	CLwp544View();
	DECLARE_DYNCREATE(CLwp544View)

// Attributes
public:
	CLwp544Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLwp544View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLwp544View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLwp544View)
	afx_msg void OnGray();
	afx_msg void OnUpdateGray(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHestogram();
	afx_msg void OnUpdateHestogram(CCmdUI* pCmdUI);
	afx_msg void OnEqualize();
	afx_msg void OnUpdateEqualize(CCmdUI* pCmdUI);
	afx_msg void OnLinetrans();
	afx_msg void OnUpdateLinetrans(CCmdUI* pCmdUI);
	afx_msg void OnFourier();
	afx_msg void OnUpdateFourier(CCmdUI* pCmdUI);
	afx_msg void OnIfourier();
	afx_msg void OnUpdateIfourier(CCmdUI* pCmdUI);
	afx_msg void OnIffourier();
	afx_msg void OnUpdateIffourier(CCmdUI* pCmdUI);
	afx_msg void OnFfourier();
	afx_msg void OnUpdateFfourier(CCmdUI* pCmdUI);
	afx_msg void OnAve();
	afx_msg void OnUpdateAve(CCmdUI* pCmdUI);
	afx_msg void OnMid();
	afx_msg void OnUpdateMid(CCmdUI* pCmdUI);
	afx_msg void OnStair();
	afx_msg void OnUpdateStair(CCmdUI* pCmdUI);
	afx_msg void OnRaplas();
	afx_msg void OnUpdateRaplas(CCmdUI* pCmdUI);
	afx_msg void OnGoss();
	afx_msg void OnUpdateGoss(CCmdUI* pCmdUI);
	afx_msg void OnHl();
	afx_msg void OnUpdateHl(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in lwp544View.cpp
inline CLwp544Doc* CLwp544View::GetDocument()
   { return (CLwp544Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LWP544VIEW_H__E10CF3DE_780C_45F2_9E9A_EE8965BA263B__INCLUDED_)
