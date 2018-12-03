#ifndef JXGL_TRACKER
#define JXGL_TRACKER

///////////////////////////////////////////////////////////////////////
// jxglTracker.h: header file
//
//
// Written by Junlin Xu <junlin.xu@gmail.com>
// Copyright (c) 2006-2009. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. 
// This file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
//
//////////////////////////////////////////////////////////////////////

class jxglTracker
{
public:
   jxglTracker(void);
public:
   ~jxglTracker(void);
public:
   BOOL Track(CWnd* pWnd, CPoint point);
   CRect GetTrackRect()const; // un-normalized
private:
   void DrawTrackRect(int x1, int y1, int x2, int y2);
   void DrawTrackRect(const CRect& rect);
   CRect m_rect;
   BOOL m_bErased;
   CWnd* m_pWnd;
};


#endif