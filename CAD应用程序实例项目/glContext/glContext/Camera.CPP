#include "stdafx.h"

#include <gl/gl.h>

#include <gl/glu.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include <gl\glut.h>

#include "camera.h"

//-------------------------------------------------------------
// For Camera class
//-------------------------------------------------------------
//-------------------------------------------------------------
// For Camera class
//-------------------------------------------------------------
GCamera::GCamera(void)
{
}

GCamera::~GCamera()
{
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

	CVector3D s(2,3,4);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_ref.x,m_ref.y,m_ref.z,m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);	
}

void GCamera::selection(int xPos,int yPos) // 参数x，y分别是视景窗口中鼠标点的位置
{
	GLint	vp[4];

	// 获取当前视口信息
	glGetIntegerv(GL_VIEWPORT,vp); // 获取当前视口坐标

	glMatrixMode(GL_PROJECTION);// 设置投影变换

	glLoadIdentity(); // 初始化投影变换矩阵

	glRenderMode(GL_SELECT); // 切换到选择模式

	// 根据鼠标位置，定义选择投影矩阵，这里拾取范围的宽度和高度分别设为
	// 固定值1，也可以在类中将这个拾取范围设为参数可以调节的
	gluPickMatrix(xPos, vp[3] - yPos, 1, 1, vp);

	//apply projective matrix
	// 乘以投影矩阵，这个矩阵应该和函数projection()中的投影矩阵相一致
	double left		=  - m_width/2.0;
	double right	=  m_width/2.0;
	double bottom	=  - m_height/2.0;
	double top		=  m_height/2.0;

	glOrtho(left,right,bottom,top,m_near,m_far);
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_ref.x,m_ref.y,m_ref.z,
		m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);
}

void GCamera::init()
{
	m_eye = CPoint3D(0,0,1000);
	m_ref = CPoint3D(0,0,0);
	m_far = 10000;
	m_near= 1;

	m_width = 2400.0;
	m_height = 2400.0;

	m_vecUp = CVector3D(0,1,0);
	
	m_screen[0] = 400;
	m_screen[1] = 400;
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
	ASSERT(scale > 0.0); // 缩放尺寸必须大于0
    m_width *= scale; // 缩放视景体的宽
    m_height *= scale; //缩放视景体的高
}

void GCamera::zoom_all(double x0,double y0,double z0,double x1,double y1,double z1)
{
	double width,height;
	double	xl, yl, zl;

	// 模型包容盒的长宽高
	xl = x1-x0;
	yl = y1-y0;
	zl = z1-z0;

	// 计算能够包含模型包容盒的视景体的宽和高
	width = max(max(xl,yl),zl);
	height= max(max(xl,yl),zl);

	// 重新设置视景体的宽和高
	set_view_rect(width,height);

	// 移动视点和参考点
	CVector3D vec = m_eye - m_ref;
	m_ref.x = (x0+x1)/2;
	m_ref.y = (y0+y1)/2;
	m_ref.z = (z0+z1)/2;
	m_eye = m_ref + vec;
}

void GCamera::set_view_type( int type )
{
	double r;
	CVector3D vec;

	vec = m_ref - m_eye; // 矢量vec表示视线方向
	r = vec.GetLength(); // 视点与参照点的距离

	if(IS_ZERO(r)) r = 50.0; // 防止视点与参照点重合
	if( r > 10000)  r = 10000; // 防止视点距离参照点太远

	switch(type){
	case VIEW_FRONT: // 前视图
		m_eye = m_ref + CVector3D(0,-r,0); // 移动视点位置
		m_vecUp = CVector3D(0,0,1);
		break;
	case VIEW_BACK:  // 后视图
		m_eye = m_ref + CVector3D(0,r,0); // 移动视点位置
		m_vecUp = CVector3D(0,0,1);
		break;
	case VIEW_TOP:   // 俯视图
		m_eye = m_ref + CVector3D(0,0,r); // 移动视点位置
		m_vecUp = CVector3D(0,1,0);
		break;
	case VIEW_BOTTOM: // 顶视图
		m_eye = m_ref + CVector3D(0,0,-r); // 移动视点位置
		m_vecUp = CVector3D(0,1,0);
		break;
	case VIEW_RIGHT: // 右视图
		m_eye = m_ref + CVector3D(r,0,0); // 移动视点位置
		m_vecUp = CVector3D(0,0,1);
		break;
	case VIEW_LEFT: // 左视图
		m_eye = m_ref + CVector3D(-r,0,0); // 移动视点位置
		m_vecUp = CVector3D(0,0,1);
		break;
	case VIEW_SW_ISOMETRIC: // SW 轴侧图
		m_eye = m_ref + CVector3D(-1,-1,1).GetNormal()*r; // 移动视点位置
		update_upVec(); // 更新视线上方向矢量 		
		break;
	case VIEW_SE_ISOMETRIC: // SE 轴侧图
		m_eye = m_ref + CVector3D(1,-1,1).GetNormal()*r; // 移动视点位置
		update_upVec(); // 更新视线上方向矢量 		
		break;
	case VIEW_NE_ISOMETRIC: // NE 轴侧图
		m_eye = m_ref + CVector3D(1,1,1).GetNormal()*r; // 移动视点位置
		update_upVec(); // 更新视线上方向矢量 
		break;
	case VIEW_NW_ISOMETRIC: //  NW 轴侧图
		m_eye = m_ref + CVector3D(-1,1,1).GetNormal()*r; // 移动视点位置
		update_upVec(); // 更新视线上方向矢量 
		break;

	case 10:
		m_eye = m_ref + CVector3D(-1, 1, 1).GetNormal()*r; // 移动视点位置
		update_upVec(); // 更新视线上方向矢量 
		break;

	}
}

void GCamera::move_view(double dpx, double dpy)
{
	CVector3D vec;
	CVector3D xUp, yUp;

	vec = m_ref - m_eye; // 视线方向矢量

	vec.Normalize(); // 单位化视线方向矢量

	xUp = vec * m_vecUp; // xUp：事物窗口的x轴对应于OpenGL用户坐标系的矢量

	yUp = xUp * vec; // yUp：事物窗口的y轴对应于OpenGL用户坐标系的矢量

	m_eye -= xUp*m_width*dpx + yUp*m_height*dpy; // 移动视点位置
	m_ref -= xUp*m_width*dpx + yUp*m_height*dpy; // 移动参考点位置 
}

void GCamera::update_upVec()
{
	CVector3D vec = m_ref - m_eye; // 视线方向矢量
	CVector3D zVec(0,0,1);
	CVector3D vec0;

	vec.Normalize(); // 矢量单位化
	vec0 = vec * zVec; 
	m_vecUp = vec0 * vec; // 矢量m_vecUp与视线方向垂直
}

CPoint3D GCamera::getEye()
{
	return m_eye;
}


CPoint3D GCamera::getRef()
{
	return m_ref;
}
