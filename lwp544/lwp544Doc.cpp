// lwp544Doc.cpp : implementation of the CLwp544Doc class
//

#include "stdafx.h"
#include "lwp544.h"

#include "lwp544Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLwp544Doc

IMPLEMENT_DYNCREATE(CLwp544Doc, CDocument)

BEGIN_MESSAGE_MAP(CLwp544Doc, CDocument)
	//{{AFX_MSG_MAP(CLwp544Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLwp544Doc construction/destruction

CLwp544Doc::CLwp544Doc()
{
	// TODO: add one-time construction code here

}

CLwp544Doc::~CLwp544Doc()
{
}

BOOL CLwp544Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLwp544Doc serialization

void CLwp544Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLwp544Doc diagnostics

#ifdef _DEBUG
void CLwp544Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLwp544Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLwp544Doc commands
BOOL LoadBmpFile(char* BmpFileName);
BOOL CLwp544Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	LoadBmpFile((char*) lpszPathName);
	return TRUE;
}
