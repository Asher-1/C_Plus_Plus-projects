// Entity.h: interface for the CEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENTITY_H__BE769D85_6C72_43F4_88FB_5521CA66FC8E__INCLUDED_)
#define AFX_ENTITY_H__BE769D85_6C72_43F4_88FB_5521CA66FC8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\inc\GeomCalc\cadbase.h"

#include "gl/gl.h"

#include "gl/glu.h"

#define GLUT_DISABLE_ATEXIT_HACK
#include <gl\glut.h>

#include "gl/glaux.h"
#pragma comment(lib, "glaux")// 必须加的声明，放置出现无法识别的外部符号
#pragma comment(lib, "legacy_stdio_definitions") // 强制兼容旧的代码，编译通过

#include "tetgen.h"
#include <vector>
#include <stdlib.h>

#define COND_READ(cond, where, len) do { if ((cond) && !fread((void *)&(where), (len), 1, f)) return false; } while (0)

using namespace std;

class COpenGLDC;
class AFX_EXT_CLASS CEntity : public CObject  
{
	// 几何对象的基本属性变量
protected:
	BOOL		m_bModified; // 是否被修改过
	CBox3D*		m_pBox; // 指向一个最小包容盒
	UINT		m_id; // 几何体的ID号
	CString		m_name;  // 几何体的字符标识信息
	COLORREF	m_color; // 几何体的颜色
	BOOL		m_bVisible;
	BOOL		m_bHighlight;

public:
	//constructor and destructor
	CEntity();
	virtual ~CEntity();

	//display
	virtual void Draw(COpenGLDC* pDC)=0; // 显示几何体（纯虚函数，用于接口）

	//serialize
	virtual	void Serialize(CArchive& ar); // 串行化存取	

	//attrib accessing 
	BOOL		GetBox(CBox3D& box);	// 获取包容盒
	
	void		SetID(UINT nID);		// 设置ID号
	UINT		GetID();				// 获取ID号
	
	void		SetName(LPCTSTR name);	// 设置字符标识信息
	CString		GetName();				// 获取字符标识信息
	
	void		SetColor(COLORREF color); // 设置颜色
	COLORREF	GetColor();				  // 获取颜色

	void		SetVisible(BOOL bVisible);
	BOOL		IsVisible();

	void		SetHighlight(BOOL bHighlight);
	BOOL		IsHighlight();

protected:
	virtual		void UpdateBox()=0; // 重新计算包容盒的尺寸（纯虚拟函数）

};


//triangle chip
class AFX_EXT_CLASS CTriChip:public CObject
{

	DECLARE_SERIAL(CTriChip) // 串行化声明
public:
	//attribs
	CPoint3D  vex[3]; // 默认STL顶点数据 
	CVector3D normal; // 默认STL三角面法失数据

	CPoint3D  tri[3]; // 三角面片顶点数据 
	CPoint3D  fourFace[4]; //四面体四个角点数据 

public:
	//constructor && destructor
	CTriChip(); // 构造函数

	CTriChip::CTriChip(const CPoint3D& v0, const CPoint3D& v1, const CPoint3D& v2);

	CTriChip(const CPoint3D& v0,const CPoint3D& v1,
			 const CPoint3D& v2,const CVector3D& norm);

	CTriChip::CTriChip(const CPoint3D & v0, const CPoint3D & v1, const CPoint3D & v2, const CPoint3D & v3,
		const CPoint3D & v4, const CPoint3D & v5, const CPoint3D & v6);

	virtual ~CTriChip(); // 析构函数

	//display
	virtual void Draw(COpenGLDC* pDC,int b); // 显示函数


	//serialize
	virtual void Serialize(CArchive& ar); // 串行化存取
	
	//operator
	const CTriChip& operator=(const CTriChip&); // 操作符重载

};


//tessellated model
class AFX_EXT_CLASS CSTLModel: public CEntity
{
	DECLARE_SERIAL(CSTLModel)  // 串行化声明
public:
	tetgenio v_tetIN;
	tetgenio v_tetOUT;

	int m_Mode; // 绘制模式状态变量，可设置为0，1，2


	vector<CPoint3D> vertices;
	vector<CPoint3D>  faces;

	CTriChip* tem_tri;  // 用于暂时指向CTriChip对象

	CTypedPtrArray<CObArray,CTriChip*>	m_TriList; // 三角片面数据链信息

	// .node文件
	vector<CTriChip*> temp_vertex; // 用于顶点数据缓存容器，在析构函数中对其内的对象一一析构

	vector<CPoint3D> node; // 结点数据
	// .face文件
	vector<CPoint3D> triface; // 三角面数据
	vector<int> trifacemarker; // 三角面标记
	//.ele文件 
	vector<int> tetrahedronattribute; // 四面体属性
	vector<CPoint4D> tetrahedron; // 用于存储四面体四个角点的索引数据
	// .edge文件
	vector<int> edge; // 四面体的边
	vector<int> edgemarker; // 四面体的边标记

	GLfloat m_xAngle = 0.0f;
	GLfloat m_yAngle = 0.0f;
	float m_Scale = 1.0f; // 缩放比例

	void SetScale(float scale); // 用于设置缩放比例
	void SetMouseRo(GLfloat xAngle, GLfloat yAngle); // 用于设置旋转角度

	void SetMode(int mode); // 设置绘制模式函数

	bool Dissection();  // 剖分函数
	bool CSTLModel::Input_to_CTriChip(); // 导入剖分数据到CTriChip对象中

public:
	//constructor && destructor
	CSTLModel(); // 构造函数
	virtual ~CSTLModel(); // 析构函数

	//display
	void	Draw(COpenGLDC* pDC); // 显示函数

	//serialize
	void	Serialize(CArchive& ar); // 串行化存取

	//		operation
	const   CSTLModel& operator=(const CSTLModel&); // “=”操作符重载
	
	void	Add(CTriChip* tri);
	bool IsTextFile(const char * strFileName);
	// 增加三角片面
	BOOL	LoadSTLFile(LPCTSTR file,int mode); // 读入STL文件



	void	Clear(); // 清除几何数据

	//attrib accessing
	BOOL	IsEmpty(); // 判断模型是否为空

protected:
	virtual void UpdateBox(); // 重新计算包容盒尺寸

public:
	bool inputStl_to_Tetgen(); // 将读取的stl文件数据传给v_tetIN对象，用于剖分四面体的输入数据
	bool Load_Stl_Points(LPCTSTR m_stlfile); // 自定义读取stl数据点函数

};

class AFX_EXT_CLASS CPart : public CEntity // 由CEntity派生
{
//attribs

protected:
	CTypedPtrArray<CObArray,CEntity*> m_EntList; // 几何元素对象指针链

public:
	//constructor && destructor
	CPart();
	virtual ~CPart();

	//draw
	virtual void Draw(COpenGLDC* pDC); // 显示函数

	//serialize
	virtual void Serialize(CArchive& ar); // 串行化存取函数

	//operation
	void	AddEntity(CEntity* ent); // 添加几何体函数
	void	RemoveEntity(CEntity* ent); // 释放单一几何体的存储空间函数
	void	RemoveAllEntity(); // 释放所有几何体的存储空间函数

	//attrib accessing
	BOOL	IsEmpty(); // 判断模型是否为空的函数

	UINT		GetEntitySize(); // 获取模型内几何体具体数量的函数
	CEntity*	GetEntity(UINT i); // 获取模型集合内序号为i的几何体的函数

protected:
	virtual void	UpdateBox(); // 重新计算所有几何模型整体包容盒的函数
};
#endif // !defined(AFX_ENTITY_H__BE769D85_6C72_43F4_88FB_5521CA66FC8E__INCLUDED_)
