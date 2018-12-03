/*************************************************************************
* This head file is for 3D scene script. We will define a scene class    *
* with some light and camera and models.                                 *
*************************************************************************/
#ifndef _CAMERA_H__
#define _CAMERA_H__

#include "..\inc\GeomCalc\cadbase.h"

// 宏定义， 用于选择典型的观察视图
#define	VIEW_FRONT		0
#define	VIEW_BACK		1
#define VIEW_TOP		2
#define	VIEW_BOTTOM		3
#define VIEW_RIGHT		4
#define	VIEW_LEFT		5
#define VIEW_SW_ISOMETRIC	6
#define VIEW_SE_ISOMETRIC	7
#define VIEW_NE_ISOMETRIC	8
#define VIEW_NW_ISOMETRIC	9
// 用于显示缩放的宏定义
#define ZOOM_ALL		9
#define ZOOM_IN			10
#define ZOOM_OUT		11

class AFX_EXT_CLASS GCamera
{

public:
	//eye coordinator
	// 视点位置和方向
    CPoint3D	m_eye;
	CPoint3D	m_ref;
	CVector3D	m_vecUp;

	// 计算视线上方向的矢量
	void update_upVec();

protected:
	//viewing volume
	// 定义视景体的参数
    double		m_far, m_near;
	double	 	m_width,m_height;

	//viewport 视口参数
	double			m_screen[2];

public:
	// 构造与析构函数
    GCamera();
	~GCamera();	

	//initailizing 初始化函数
	void init();

	// 取景定义，设置绘图模式下的投影变换
    void projection();
	// 选择定义，设置选择模式下的投影变换
	void selection(int xPos,int yPos);

	//zooming 景物缩放
    void zoom(double scale); 
	void zoom_all(double x0,double y0,double z0,double x1,double y1,double z1);

	//switch into a classical view
	// 景物平移
    void set_view_type(int type);

	//move view with precent
	// 选择典型视图
	void move_view(double dpx, double dpy);

	//set viewport acoording to window
	// 设置视口尺寸
	void set_screen( int x, int y);

	//set eye coordinate 设置视点位置和方向
	void set_eye(double eye_x,double eye_y,double eye_z);
	void set_ref(double ref_x,double ref_y,double ref_z);
	void set_vecUp(double up_dx,double up_dy,double up_dz);
	
	//set viewing volume 设置视景体
	void set_view_rect(double width,double height);
	void get_view_rect(double& width,double& height);

	CPoint3D getEye();
	CPoint3D getRef();

};
#endif 

/////////////////
