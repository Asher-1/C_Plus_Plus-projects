// EastDrawDoc.cpp : implementation of the CEastDrawDoc class
//

#include "stdafx.h"
#include "EastDraw.h"

#include "EastDrawDoc.h"
#include"EastDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEastDrawDoc

IMPLEMENT_DYNCREATE(CEastDrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CEastDrawDoc, CDocument)
	//{{AFX_MSG_MAP(CEastDrawDoc)
	ON_COMMAND(ID_FILE_SAVE_AS_Bmp, OnFILESAVEASBmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEastDrawDoc construction/destruction

CEastDrawDoc::CEastDrawDoc()
{
	// TODO: add one-time construction code here
m_bChangeTitle=false;
m_TotalSize=CSize(2000,2000);
m_CenterPoint=CPoint(-1000,-1000);
}

CEastDrawDoc::~CEastDrawDoc()
{
}

BOOL CEastDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
    
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEastDrawDoc serialization

void CEastDrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	 ar<<m_TotalSize;
	 ar<<m_CenterPoint;
		// TODO: add storing code here
	}
	else
	{
	ar>>this->m_TotalSize;
	ar>>m_CenterPoint;
	// TODO: add loading code here
	}

	m_DLineArray.Serialize(ar);
	m_CurveArray.Serialize(ar);
	m_RectangleArray.Serialize(ar);
    m_PolygonArray.Serialize(ar);
	m_RoundRectangleArray.Serialize(ar);
	m_RoundArray.Serialize(ar);
	m_EllipseArray.Serialize(ar);
    m_LiEllipseArray.Serialize(ar);
	m_RoundArcArray.Serialize(ar);
	m_PolyBezierArray.Serialize(ar);
	m_TextArray.Serialize(ar);
	m_EllipseArcArray.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CEastDrawDoc diagnostics

#ifdef _DEBUG
void CEastDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEastDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEastDrawDoc commands

BOOL CEastDrawDoc::SaveBitMap(CFile&file)
{

	CEastDrawView*pView;
	POSITION pos=GetFirstViewPosition();
	pView=(CEastDrawView*)GetNextView(pos);
    return	pView->SaveDIB(NULL,file);

}



void CEastDrawDoc::AddTempFile()
{
    m_bChangeTitle=true;
	HANDLE hFind;
	WIN32_FIND_DATA dataFind;
	BOOL bMoreFiles=TRUE;
	CString strTitle;
    strTitle=GetTitle();
    char    tmpPathc[_MAX_PATH+1];
    GetTempPath(_MAX_PATH,tmpPathc);
	CString tmpPath=tmpPathc;
	hFind=FindFirstFile(tmpPath+"\\EdTemp",&dataFind);

   
	if(hFind==INVALID_HANDLE_VALUE)
	{
	   	FindClose(hFind);
        ::CreateDirectory(tmpPath+"\\EdTemp",NULL);
		::CreateDirectory(tmpPath+"\\EdTemp\\"+strTitle,NULL);
	  
	}
	else
	{   
		FindClose(hFind);
        hFind=FindFirstFile(tmpPath+"\\EdTemp\\"+strTitle,&dataFind);
        if(hFind==INVALID_HANDLE_VALUE)
		{
			::CreateDirectory(tmpPath+"\\EdTemp\\"+strTitle,NULL);
		}
	}
   	 	CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"),CFile::modeReadWrite|CFile::modeCreate);
		file1.Close();
		CFile file2(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"),CFile::modeReadWrite|CFile::modeCreate);
		file2.Close();
		CFile file3(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"),CFile::modeReadWrite|CFile::modeCreate);
	    file3.Close();
		CFile file4(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),CFile::modeReadWrite|CFile::modeCreate);
	    file4.Close();
		CFile file5(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),CFile::modeReadWrite|CFile::modeCreate);
	    file5.Close();
		CFile file6(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),CFile::modeReadWrite|CFile::modeCreate);
	    file6.Close();
		FindClose(hFind);
   
}



void CEastDrawDoc::AutoSave()
{
   
	CString strTitle;
    strTitle=GetTitle();
    char    tmpPathc[_MAX_PATH+1];
    GetTempPath(_MAX_PATH,tmpPathc);
	CString tmpPath=tmpPathc;

    CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"),CFile::modeReadWrite|CFile::modeCreate);
	CArchive ar(&file1,CArchive::store);
    Serialize(ar);
    //file1.Close();
	
	CFile file2(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"),CFile::modeReadWrite|CFile::modeCreate);
	CArchive ar2(&file2,CArchive::store);
    Serialize(ar2);	
   ///	file2.Close();
	
	CFile file3(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"),CFile::modeReadWrite|CFile::modeCreate);
	CArchive ar3(&file3,CArchive::store);
    Serialize(ar3);	

    CFile file4(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),CFile::modeReadWrite|CFile::modeCreate);
	CArchive ar4(&file4,CArchive::store);
    Serialize(ar4);

	CFile file5(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),CFile::modeReadWrite|CFile::modeCreate);
	CArchive ar5(&file5,CArchive::store);
    Serialize(ar5);
    
	CFile file6(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),CFile::modeReadWrite|CFile::modeCreate);
	CArchive ar6(&file6,CArchive::store);
    Serialize(ar6);






  //file3.Close();
}

BOOL CEastDrawDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

void CEastDrawDoc::ExchangeSave(int index,int&rtimes)
{
	//Requeue(index,rtimes);

	CString strTitle;
    strTitle=GetTitle();
    char    tmpPathc[_MAX_PATH+1];
    GetTempPath(_MAX_PATH,tmpPathc);
	CString tmpPath=tmpPathc;
	
  
	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed")); 
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"));
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"));
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
   
	   
	CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"),CFile::modeReadWrite|CFile::modeCreate);
	CArchive ar(&file1,CArchive::store);
    Serialize(ar);

   //OPENFILENAME ofn;  // older version

   //memset(&ofn, 0, sizeof(ofn));

   //ofn.lStructSize = sizeof(ofn);

   //int nResult = ::GetOpenFileName(&ofn);



}

void CEastDrawDoc::OnBUTTON32858OnDo(int index)
{
    CString strTitle;
    strTitle=GetTitle();
    char    tmpPathc[_MAX_PATH+1];
    GetTempPath(_MAX_PATH,tmpPathc);
	CString tmpPath=tmpPathc;
// CString str;
// str.Format("%d",index);
 //AfxMessageBox(str);
  if(index==0)
	{
        CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"),CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	    CArchive ar(&file1,CArchive::load);
        Serialize(ar); 
	}		
	
 if(index==1)
 {
	    CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"),CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	    CArchive ar(&file1,CArchive::load);
        Serialize(ar); 
 }  
        
 if(index==2)
 {
        CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"),CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	    CArchive ar(&file1,CArchive::load);
        Serialize(ar); 
 }
 if(index==3)
 {
      CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	  CArchive ar(&file1,CArchive::load);
      Serialize(ar); 
}

if(index==4)
 {
      CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	  CArchive ar(&file1,CArchive::load);
      Serialize(ar); 
} 
if(index==5)
 {
      CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	  CArchive ar(&file1,CArchive::load);
      Serialize(ar); 
} 


}


void CEastDrawDoc::OnFILESAVEASBmp() 
{


	// TODO: Add your command handler code here
 	
 	TCHAR szFilters[]=_T("BMP 文件(*.bmp)|*.bmp|所有文件(*.*)|*.*||");
 
 	CFileDialog dlg(false,_T("bmp"),_T("*.bmp"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilters);
 	if(dlg.DoModal()==IDOK)
 {
 	
 	CFile file;
 	CFileException fe;
  
   if (!file.Open(dlg.GetPathName(), CFile::modeCreate |
 	  CFile::modeWrite ,&fe))
 	{
 	
 	    fe.ReportError();
 		fe.Delete();
 		return  ;
 	}
   
 
 	BOOL bSuccess = FALSE;
 	TRY
 	{
 		BeginWaitCursor();
 		bSuccess = SaveBitMap(file);
 		file.Close();
 	}
 	CATCH (CException, eSave)
 	{
 		file.Abort();
 		EndWaitCursor();
 		
 		eSave->ReportError();
 		eSave->Delete();
 		return ;
 	}
 	END_CATCH
 
 	EndWaitCursor();
    
 if (!bSuccess)
 	{
 		
 		CString strMsg;
 		strMsg.Format("保存失败，请稍后重试");
 		AfxMessageBox(strMsg);
 	}
 
 }
}

void CEastDrawDoc::ReMoveTemp()
{   
	CString strTitle;
    strTitle=GetTitle();
    char    tmpPathc[_MAX_PATH+1];
    GetTempPath(_MAX_PATH,tmpPathc);
	CString tmpPath=tmpPathc;
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"));
	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
    
	::RemoveDirectory(_T(tmpPath+"\\EdTemp\\"+strTitle));
    ::RemoveDirectory(_T(tmpPath+"\\EdTemp\\"));
}

void CEastDrawDoc::Requeue(int &index, int &rtimes)
{
	CString strTitle;
    strTitle=GetTitle();
    char    tmpPathc[_MAX_PATH+1];
    GetTempPath(_MAX_PATH,tmpPathc);
	CString tmpPath=tmpPathc;
   if(index==0)
   {
    rtimes=rtimes<6?rtimes:6;
   }
   if(index==1)
   {
   	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed")); 
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"));
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"));
    CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),CFile::modeReadWrite|CFile::modeCreate);
    rtimes=rtimes<5?rtimes:5;
   }

   if(index==2)
   {
   	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"));
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"));
    CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file2(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),CFile::modeReadWrite|CFile::modeCreate);
    rtimes=rtimes<4?rtimes:4;
   }
   if(index==3)
   {
   	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"));
    CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file2(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file3(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),CFile::modeReadWrite|CFile::modeCreate);
    rtimes=rtimes<3?rtimes:3;
   }

   if(index==4)
   {
   	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"));

    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
	CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
    CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file2(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file3(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),CFile::modeReadWrite|CFile::modeCreate);
     CFile file4(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"),CFile::modeReadWrite|CFile::modeCreate);
	 rtimes=rtimes<2?rtimes:2;
   }
 if(index==5)
   {
   	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
	CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"));
    CFile::Remove(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"));
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),_T(tmpPath+"\\EdTemp\\"+strTitle+"\\First.ed"));
    CFile file1(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Six.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file2(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Fifth.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file3(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Forth.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file4(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Third.ed"),CFile::modeReadWrite|CFile::modeCreate);
    CFile file5(_T(tmpPath+"\\EdTemp\\"+strTitle+"\\Second.ed"),CFile::modeReadWrite|CFile::modeCreate);
    rtimes=rtimes<1?rtimes:1;
 }

}

void CEastDrawDoc::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
  
   if(m_bChangeTitle)
	{
	CString strTitle;
    strTitle=GetTitle();

    char    tmpPathc[_MAX_PATH+1];
    GetTempPath(_MAX_PATH,tmpPathc);
	CString tmpPath=tmpPathc;
	
    CFile::Rename(_T(tmpPath+"\\EdTemp\\"+strTitle),_T(tmpPath+"\\EdTemp\\"+lpszTitle));

    }
	
	CDocument::SetTitle(lpszTitle);
	
}
