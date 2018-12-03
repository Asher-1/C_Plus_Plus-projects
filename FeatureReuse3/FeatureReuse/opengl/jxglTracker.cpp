///////////////////////////////////////////////////////////////////////
// jxglTracker.cpp: implementation file
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

#include "..\stdafx.h"
#include <gl/gl.h>
#include "jxglTracker.h"

jxglTracker::jxglTracker(void)
{
    m_bErased = FALSE;
}

jxglTracker::~jxglTracker(void)
{
}

CRect jxglTracker::GetTrackRect()const
{   // m_rect is not normalized
    // meaning left coordinate may be greater than right coordinate
    return m_rect;
}


void jxglTracker::DrawTrackRect(int x1, int y1, int x2, int y2)
{
    CRect rectClient;
    m_pWnd->GetClientRect(&rectClient);
    
    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);
    // drawing different rubber-banding rectangle depending on the mouse movement x-direction
    if(x1 < x2)
    {
        glColor4f(0.0, 0.0, 1.0, 0.0);
    }
    else
    {
        glColor4f(1.0, 0.0, 0.0, 0.0);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // OpenGL window coordinates are different from GDI's
    glRecti(x1, rectClient.Height() - y1, x2, rectClient.Height() - y2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glFlush();  // must flush here
    glDisable(GL_COLOR_LOGIC_OP);
} 

void jxglTracker::DrawTrackRect(const CRect& rect)
{
    DrawTrackRect(rect.left, rect.top, rect.right, rect.bottom);
}

BOOL jxglTracker::Track(CWnd* pWnd, CPoint point)
{
    m_pWnd = pWnd;
    ASSERT(m_pWnd != 0);
    CRect rectClient;
    m_pWnd->GetClientRect(&rectClient);
    
    // set drawing mode to front-buffer
    glDrawBuffer(GL_FRONT);
    
    // set up a convenient projection matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, rectClient.Width(), 0, rectClient.Height(), -1, 1);
    glViewport(-1, -1, rectClient.Width() + 2, rectClient.Height() + 2);
    
    if (::GetCapture() != NULL)
    {  
        return FALSE;
    }
    
    // set mouse capture because we are going to work on this window
    pWnd->SetCapture();
    ASSERT(pWnd == CWnd::GetCapture());
    pWnd->UpdateWindow();
    
    BOOL bMoved = FALSE;
    CPoint ptOld = point;
    CRect rectOld = CRect(ptOld, ptOld);
    CPoint ptNew;
    
    BOOL bStop = FALSE;
    for (;;)
    {  // loop forever until LButtonUp, RButtonDown or ESC keyDown
        MSG msg;
        VERIFY(::GetMessage(&msg, NULL, 0, 0));
        
        if (CWnd::GetCapture() != pWnd)
        {
            break;
        }
        
        if(msg.message == WM_LBUTTONUP || msg.message == WM_MOUSEMOVE)
        {
            ptNew.x = (int)(short)LOWORD(msg.lParam);
            ptNew.y = (int)(short)HIWORD(msg.lParam);
            m_rect = CRect(ptOld, ptNew);
            
            if (bMoved)
            {
                m_bErased = TRUE;
                DrawTrackRect(&rectOld);
            }
            rectOld = m_rect;
            if (msg.message != WM_LBUTTONUP)
            {
                bMoved = TRUE;
            }
            
            if (msg.message == WM_MOUSEMOVE)
            {
                m_bErased = FALSE;
                DrawTrackRect(&m_rect);
            }
            else
            {
                bStop = TRUE;
                ASSERT(msg.message == WM_LBUTTONUP);
            }
        }
        else if(msg.message == WM_KEYDOWN)
        {
            if (msg.wParam == VK_ESCAPE)
            {
                bStop = TRUE;
            }
        }
        else if(msg.message == WM_RBUTTONDOWN)
        { 
            bStop = TRUE;
        }
        else
        {
            DispatchMessage(&msg);
        }
        
        if(bStop)
        {
            break;
        }
        
    }  // for (;;)
    
    // release mouse capture
    ReleaseCapture();
    
    if(!m_bErased)
    {  // do a final erase if needed
        DrawTrackRect(m_rect);
    }
    
    glPopMatrix();
    glDrawBuffer(GL_BACK);  // restore drawing mode to back-buffer
    
    return TRUE;
}