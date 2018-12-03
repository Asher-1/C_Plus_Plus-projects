//GLfont.cpp: implementation of the GLfont class.
//
//////////////////////////////////////////////////////////////////////

#include "GLfont.h"
#include "gl/gl.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLfont::GLfont()
{              
	LOGFONT lf;
	lf.lfHeight = 12;
	lf.lfWidth = 0;       
	lf.lfEscapement = 0;  
	lf.lfOrientation = 0; 
	lf.lfWeight = FW_DONTCARE;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_TT_PRECIS; 
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lf.lfQuality = PROOF_QUALITY; 
	lf.lfPitchAndFamily = VARIABLE_PITCH | TMPF_TRUETYPE | FF_MODERN; 
	strcpy(lf.lfFaceName,"宋体");
	
	// 创建字体
	m_hFont = CreateFontIndirect(&lf);
}

GLfont::~GLfont()
{
	if(m_hFont) DeleteObject(m_hFont);  
	glDeleteLists(base, 96);
} 

void GLfont::BuildEnglishFont(HDC hDC,int size)			// Build Our Bitmap Font
{
	HFONT	font;	 				// Windows Font ID
	HFONT	oldfont; 				// Used For Good House Keeping

	base = glGenLists(96); 				// Storage For 96 Characters

	font = ::CreateFont(-size,        		// Height Of Font
                0,					// Width Of Font
                0,					// Angle Of Escapement
                0,					// Orientation Angle
                FW_DONTCARE,				// Font Weight
                FALSE,					// Italic
                FALSE,					// Underline
                FALSE,					// Strikeout
                ANSI_CHARSET,				// Character Set Identifier
                OUT_TT_PRECIS,				// Output Precision
                CLIP_DEFAULT_PRECIS,			// Clipping Precision
                ANTIALIASED_QUALITY,			// Output Quality
                FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
                "Arial");				// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void GLfont::glPrintEnglish(float x,float y,float r,float g,float b,const char *fmt, ...)					// Custom GL "Print" Routine
{                    
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
	char		text[256];  				// Holds Our String
	va_list		ap;					// Pointer To List Of Arguments

	if (fmt == NULL)	        			// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);	     				// Parses The String For Variables
	    vsprintf(text, fmt, ap);  				// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);    				// Pushes The Display List Bits
	glListBase(base - 32);	      				// Sets The Base Character to 32
        
        glTranslatef(0.0f,0.0f,-1.0f);
        glColor3f(r,g,b);
	glRasterPos2f(x,y);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib(); 
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();   
        glEnable(GL_TEXTURE);
        glEnable(GL_LIGHTING);
}

void GLfont::CreateFont(char *facename, int height, int weight, bool italic,bool underline,bool strikeout)
{
	LOGFONT lf;
	
	lf.lfHeight = height;
	lf.lfWidth = 0;       
	lf.lfEscapement = 0;  
	lf.lfOrientation = 0; 
	lf.lfWeight = weight; 
	lf.lfItalic = italic; 
	lf.lfUnderline = underline; 
	lf.lfStrikeOut = strikeout;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_TT_PRECIS; 
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lf.lfQuality = PROOF_QUALITY; 
	lf.lfPitchAndFamily = VARIABLE_PITCH | TMPF_TRUETYPE | FF_MODERN; 
	strcpy(lf.lfFaceName,facename); 
	
	// 创建字体
	m_hFont = CreateFontIndirect(&lf);
	
}


void GLfont::ShowText(float x, float y, LPCTSTR lpszText,float r,float g,float b)
{
	//取消光照和纹理
        //glDisable(GL_LIGHTING);
        //glDisable(GL_TEXTURE);
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity(); 
        glColor3f(r,g,b);
	glTranslatef(0, 0, -1);
        
	HBITMAP hbitmap;
	BITMAP bm;
	SIZE size;
	UCHAR* pBmpBits;
	HFONT hOldFont;
	HDC hdc = wglGetCurrentDC(); 	
	
	hOldFont = (HFONT)SelectObject(hdc, m_hFont);
	::GetTextExtentPoint32(hdc, lpszText, strlen(lpszText), &size);
	hbitmap = CreateBitmap(size.cx, size.cy,1, 1, NULL);
	
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	if(hMemDC)      //向内存设备环境中写入文字，形成一个内存位图
	{
		HBITMAP hPrevBmp = (HBITMAP)SelectObject(hMemDC,hbitmap);
		HFONT hPrevFont = (HFONT)SelectObject(hMemDC, m_hFont);

		SetBkColor(hMemDC, RGB(0, 0, 0));
		SetTextColor(hMemDC, RGB(255, 255, 255));
		SetBkMode(hMemDC, OPAQUE);
		TextOut(hMemDC, 0, 0, lpszText, strlen(lpszText));
		
		// copy GDI bitmap to DIB
		SelectObject(hdc,hbitmap); 
		GetObject(hbitmap, sizeof(bm), &bm);
		size.cx = (bm.bmWidth + 31) & (~31);
		size.cy = bm.bmHeight;
		int bufsize = size.cy * (((bm.bmWidth + 31) & (~31)) /8);
		pBmpBits = new UCHAR[bufsize];
		memset(pBmpBits, 0, sizeof(UCHAR)*bufsize);
		
		struct {
			BITMAPINFOHEADER bih;
			RGBQUAD col[2];
		}bic;
		BITMAPINFO *binf = (BITMAPINFO *)&bic;
		
		binf->bmiHeader.biSize = sizeof(binf->bmiHeader);
		binf->bmiHeader.biWidth = bm.bmWidth;
		binf->bmiHeader.biHeight = bm.bmHeight;
		binf->bmiHeader.biPlanes = 1;
		binf->bmiHeader.biBitCount = 1;
		binf->bmiHeader.biCompression = BI_RGB;
		binf->bmiHeader.biSizeImage = bufsize;
		binf->bmiHeader.biXPelsPerMeter = 1;
		binf->bmiHeader.biYPelsPerMeter = 1;
		binf->bmiHeader.biClrUsed = 0;
		binf->bmiHeader.biClrImportant = 0;
		
		::GetDIBits(hdc, hbitmap, 0, bm.bmHeight, pBmpBits, binf,DIB_RGB_COLORS);
                               
		SelectObject(hMemDC, hPrevFont);
		SelectObject(hMemDC, hPrevBmp);
	}
	::DeleteDC(hMemDC);
	
	// delete font from DC
	SelectObject(hdc, hOldFont);
	// display text
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glRasterPos2f(x,y);
	glBitmap(size.cx, size.cy, 0.0, 0.0, 0.0, 0.0, pBmpBits);
	delete pBmpBits;
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
        //glEnable(GL_TEXTURE);
        //glEnable(GL_LIGHTING);
}

