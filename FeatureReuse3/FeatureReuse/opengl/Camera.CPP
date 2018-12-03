//#include "\ShoePatternDesign\ShoePatternDesign\stdafx.h"
#include "..\stdafx.h"
#include "gl/gl.h"
#include "gl/glu.h"

#include "camera.h"

//-------------------------------------------------------------
// For Camera class
//-------------------------------------------------------------
GCamera::GCamera(void)
{
}

GCamera::~GCamera()
{
}
//
void GCamera::ProjectionToScreen()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glRenderMode(GL_SELECT);

	//apply projective matrix
	double left		=  - m_width/2.0;
	double right	=  m_width/2.0;
	double bottom	=  - m_height/2.0;
	double top		=  m_height/2.0;

	glOrtho(left,right,bottom,top,m_near,m_far);
	//
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_ref.x,m_ref.y,m_ref.z,m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);	
	
}

void GCamera::projection2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glRenderMode(GL_RENDER);

	//apply projective matrix
	double left		=  - m_width/2.0;
	double right	=  m_width/2.0;
	double bottom	=  - m_height/2.0;
	double top		=  m_height/2.0;

	gluOrtho2D(left,right,bottom,top);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}

void GCamera::projection() 
{
	//switch to projection
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();

	glRenderMode(GL_RENDER);

	//apply projective matrix
	double left		=  - m_width/2.0;
	double right	=  m_width/2.0;
	double bottom	=  - m_height/2.0;
	double top		=  m_height/2.0;

	glOrtho(left,right,bottom,top,m_near,m_far);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_ref.x,m_ref.y,m_ref.z,m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);	
}
//-----------------------------------------------//
void GCamera::Getjuzhen(GLint Viewport[4], GLdouble ModelMatrix[16], GLdouble ProjMatrix[16])
{
	for (int i=0;i<16;i++)
	{
		ModelMatrix[i]=CurrentModelMatrix[i];
		ProjMatrix[i]=CurrentProjMatrix[i];
	}

	for (int i=0;i<4;i++)
	{
		Viewport[i]=CurrentViewport[i];
	}
}
//--------------------------------------------------------------------------//
void GCamera::BeginGetMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glRenderMode(GL_SELECT);

	//apply projective matrix
	double left		=  - m_width/2.0;
	double right	=  m_width/2.0;
	double bottom	=  - m_height/2.0;
	double top		=  m_height/2.0;

	glOrtho(left,right,bottom,top,m_near,m_far);

	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);

	glGetDoublev(GL_MODELVIEW_MATRIX, ModelMatrix);		//获得模型变换矩阵
	glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);		//获得投影变换矩阵

	//--------------------------------------------------------------------------
	for (int i=0;i<16;i++)
	{
		CurrentModelMatrix[i]=ModelMatrix[i];
		CurrentProjMatrix[i]=ProjMatrix[i];
	}

	for (int  i=0;i<4;i++)
	{
		CurrentViewport[i]=Viewport[i];
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_ref.x,m_ref.y,m_ref.z,	m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);

}
//---------------------------------------------------------------------------
void GCamera::selection(int xPos,int yPos)
{
	GLint	vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);

   	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glRenderMode(GL_SELECT);
 	gluPickMatrix(xPos,vp[3]-yPos, 1, 1, vp );

	//apply projective matrix
	double left		=  - m_width/2.0;
	double right	=  m_width/2.0;
	double bottom	=  - m_height/2.0;
	double top		=  m_height/2.0;

	glOrtho(left,right,bottom,top,m_near,m_far);

	//-----------------将屏幕坐标（mouseX, MouseY）转化为模型空间坐标-------------
	//------操作的时候应该点击屏幕中模型显示的地方，否则转化后的结果不可预测------
	ox =0; oy =0; oz=0;
	GLubyte mouseColor[3] = {0};

	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);
	
	float fZValue = 0;
	glReadPixels(xPos,Viewport[3]-yPos,							// 屏幕坐标
		1,1,																							//宽高都为1，即只读一个象素
		GL_DEPTH_COMPONENT,											//希望获得深度信息
		GL_FLOAT,																			//浮点型
		&fZValue );																			//获得值保存在fZValue中  得到一个0～1之间的数

	
	gluUnProject(xPos, Viewport[3]-yPos,
		fZValue, ModelMatrix, ProjMatrix, Viewport,	&ox, &oy, &oz);
//--------------------------------------------------------------
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_ref.x,m_ref.y,m_ref.z, m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);
}

void GCamera::init()
{
	m_eye = CPoint3D(0,0,5000);
	m_ref = CPoint3D(0,0,0);
	m_far = 8000;
	m_near = -8000;

	m_width = 800.0;
	m_height = 600.0;

	m_vecUp = CVector3D(0,1,0);
	
	m_screen[0] = 20;
	m_screen[1] = 20;
}

void GCamera::set_screen( int x, int y) 
{ 
	glViewport(0,0,x,y);
	if(y==0) y=1;
	double ratio = (double)x/(double)y;
	m_width *= (double)x/m_screen[0];
	m_height *= (double)y/m_screen[1];
	m_width =  m_height*ratio;
	m_screen[0] = x;
	m_screen[1] = y; 
}

void GCamera::set_eye(double eye_x,double eye_y,double eye_z)
{
	m_eye.x = eye_x;
	m_eye.y = eye_y;
	m_eye.z = eye_z;
}

void GCamera::set_ref(double ref_x,double ref_y,double ref_z)
{
	m_ref.x = ref_x;
	m_ref.y = ref_y;
	m_ref.z = ref_z;
}

void GCamera::set_vecUp(double up_dx,double up_dy,double up_dz)
{
	m_vecUp.dx = up_dx;
	m_vecUp.dy = up_dy;
	m_vecUp.dz = up_dz;
}

void GCamera::set_view_rect(double width,double height)
{
	m_width = width;
	m_height = height;
	double aspect = m_screen[0]/m_screen[1];
	m_width =  m_height*aspect;
}

void GCamera::get_view_rect(double& width,double& height)
{
	width = m_width;
	height = m_height;
}

void GCamera::zoom(double scale)
{
	ASSERT(scale > 0.0);
    m_width *= scale;
    m_height *= scale;
}

void GCamera::zoom_all(double x0,double y0,double z0,double x1,double y1,double z1)
{
	double width,height;
	double	xl, yl, zl;
	xl = x1-x0;
	yl = y1-y0;
	zl = z1-z0;

	width = max(max(xl,yl),zl);
	height= max(max(xl,yl),zl);

	set_view_rect(width,height);

	CVector3D vec = m_eye - m_ref;
	m_ref.x = (x0+x1)/2;
	m_ref.y = (y0+y1)/2;
	m_ref.z = (z0+z1)/2;
	m_eye = m_ref + vec;

	m_refCenter = m_ref;
}

void GCamera::set_view_type( int type )
{
	double r;
	CVector3D vec;

	vec = m_ref - m_eye;
	r = vec.GetLength();

	if(IS_ZERO(r)) r = 50.0;
	if( r > 10000)  r = 10000;

	switch(type){
	case VIEW_FRONT:
		m_eye = m_ref + CVector3D(0,-r,0);
		m_vecUp = CVector3D(0,0,1);
		break;
	case VIEW_BACK:
		m_eye = m_ref + CVector3D(0,r,0);
		m_vecUp = CVector3D(0,0,1);
		break;
	case VIEW_TOP:
		m_eye = m_ref + CVector3D(0,0,r);
		m_vecUp = CVector3D(0,1,0);
		break;
	case VIEW_BOTTOM:
		m_eye = m_ref + CVector3D(0,0,-r);
		m_vecUp = CVector3D(0,1,0);
		break;
	case VIEW_RIGHT:
		m_eye = m_ref + CVector3D(r,0,0);
		m_vecUp = CVector3D(0,0,1);
		break;
	case VIEW_LEFT:
		m_eye = m_ref + CVector3D(-r,0,0);
		m_vecUp = CVector3D(0,0,1);
		break;
	case VIEW_SW_ISOMETRIC:
		m_eye = m_ref + CVector3D(-1,-1,1).GetNormal()*r;
		update_upVec();		
		break;
	case VIEW_SE_ISOMETRIC:
		m_eye = m_ref + CVector3D(1,-1,1).GetNormal()*r;
		update_upVec();		
		break;
	case VIEW_NE_ISOMETRIC:
		m_eye = m_ref + CVector3D(1,1,1).GetNormal()*r;
		update_upVec();
		break;
	case VIEW_NW_ISOMETRIC:
		m_eye = m_ref + CVector3D(-1,1,1).GetNormal()*r;
		update_upVec();
		break;
	}
}

void GCamera::View_To_Plane(CVector3D nor)
{
	double r;
	CVector3D vec;

	vec = m_ref - m_eye;
	r = vec.GetLength();

	if(IS_ZERO(r)) r = 50.0;
	if( r > 10000)  r = 10000;
	m_eye = m_ref + nor.GetNormal()*r;
	update_upVec();
}


void GCamera::move_view(double dpx, double dpy)
{
	CVector3D vec;
	CVector3D xUp, yUp;

	vec = m_ref - m_eye;
	vec.Normalize();
	xUp = vec*m_vecUp;
	yUp = xUp*vec;

	m_eye -= xUp*m_width*dpx + yUp*m_height*dpy;
	m_ref -= xUp*m_width*dpx + yUp*m_height*dpy;
}

void GCamera::update_upVec()
{
	CVector3D vec = m_ref - m_eye;
	CVector3D zVec(0,0,1);
	CVector3D vec0;

	vec.Normalize();
	vec0 = vec*zVec; 
	m_vecUp = vec0*vec;
}

//绕X轴旋转
void GCamera::rotate_x_view(double dx)
{
	CVector3D rotVec(1,0,0);
	CVector3D origVec;
	CMatrix3D rotMtrx;

	rotMtrx = rotMtrx.CreateRotateMatrix(dx,rotVec);
	origVec = m_eye - m_ref;
	origVec *= rotMtrx;
	m_eye = m_ref + origVec;

	//update_upVec();

	m_vecUp *= rotMtrx;

}
//绕Y轴旋转
void GCamera::rotate_y_view(double dy)
{
	CVector3D rotVec(0,1,0);
	CVector3D origVec;
	CMatrix3D rotMtrx;

	rotMtrx = rotMtrx.CreateRotateMatrix(dy,rotVec);
	origVec = m_eye - m_ref;
	origVec *= rotMtrx;
	m_eye = m_ref + origVec;

	//update_upVec();

}
//绕Z轴旋转
void GCamera::rotate_z_view(double dz)
{
	CVector3D rotVec(0,0,1);
	CVector3D origVec;
	CMatrix3D rotMtrx;

	rotMtrx = rotMtrx.CreateRotateMatrix(dz,rotVec);
	m_vecUp *= rotMtrx;

}
//绕给定矢量旋转
void GCamera::rotate_view(double dRot,VECTOR3D vect)
{
}
//绕屏幕给定轴旋转
void GCamera::rotate_view(double dRot,short snXyz)
{
	CVector3D rotVec;
	CVector3D origVec;
	CMatrix3D rotMtrx;

	CVector3D	mVec;

	/*mVec = m_ref - m_refCenter;

	m_ref = m_refCenter;
	m_eye = m_eye - mVec;*/

	origVec = m_eye - m_ref;

	switch(snXyz)
	{
	case 0:			//绕屏幕X轴方向旋转
		rotVec = m_vecUp * origVec;			//旋转轴
		rotMtrx = rotMtrx.CreateRotateMatrix(dRot,rotVec);

		//修正m_vecUp
		m_vecUp *= rotMtrx;		
		//修正视点位置
		origVec *= rotMtrx;
		m_eye = m_ref + origVec;
		break;
	case 1:			//绕屏幕Y轴方向旋转
		//旋转轴为m_vecUp
		rotMtrx = rotMtrx.CreateRotateMatrix(dRot,m_vecUp);
		//不需要修正m_vecUp
		//修正视点位置
		origVec *= rotMtrx;
		m_eye = m_ref + origVec;

		break;
	case 2:			//绕屏幕Z轴方向旋转
		//旋转轴为origVec
		rotMtrx = rotMtrx.CreateRotateMatrix(dRot,origVec);
		//修正m_vecUp
		m_vecUp *= rotMtrx;
		//不需修正视点位置
	    break;
	default:
	    break;
	}

}

void GCamera::view_to_camera(double &x,double &y)
{
	x = x - m_screen[0]/2;
	x = x * m_width / m_screen[0];
	y = m_screen[1]/2 - y;
	y = y * m_height / m_screen[1];
}
void GCamera::GetWxyz(double &x, double &y, double &z)
{
	x =ox;
	y =oy;
	z =oz;
}
//
void GCamera::GetCurrentView(CPoint3D &eye, CPoint3D &refp, CVector3D &vecUp)
{
	eye=m_eye;
	refp=m_ref;
	vecUp=m_vecUp;
}
void GCamera::SetBackView(CPoint3D eye, CPoint3D refp, CVector3D vecUp)
{
	m_eye=eye;
	m_ref=refp;
	m_vecUp=vecUp;
}