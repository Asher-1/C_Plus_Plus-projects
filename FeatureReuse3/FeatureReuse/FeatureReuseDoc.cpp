// FeatureReuseDoc.cpp : CFeatureReuseDoc 类的实现
#include "stdafx.h"
#include "FeatureReuse.h"
#include "ParserVrml.h"
#include "sys/stat.h"
#include "FeatureReuseDoc.h"
#include <conio.h>
#include <winuser.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define new DEBUG_NEW
#endif
// CFeatureReuseDoc
IMPLEMENT_DYNCREATE(CFeatureReuseDoc, CDocument)
BEGIN_MESSAGE_MAP(CFeatureReuseDoc, CDocument)
END_MESSAGE_MAP()

int CFeatureReuseDoc::steps = 0; // 工件受力变形阶段总数
// CFeatureReuseDoc 构造/析构
CFeatureReuseDoc::CFeatureReuseDoc()
{
	// 初始化数据
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_Scale = 1.0f;

	steps = 0;
	m_Mode = 0; // 以默认模式绘制
	is_PrintArrows = false;
	is_Draw_cloudPicture = false;
	stage = 0;
	delta = 0;
	counts = 1;

	Current_mesh=-1;
	meshes.clear();
	sections.clear();
}

CFeatureReuseDoc::~CFeatureReuseDoc()
{
	// Force vector to relinquish its memory (Look up "STL swap trick").
	vector<TriMesh*>().swap(meshes);

	vector<int>().swap(edgemarker);
	vector<int>().swap(trifacemarker);
	vector<CPoint3D>().swap(node);
	vector<CPoint3D>().swap(triface);
	vector<int>().swap(tetrahedronattribute);

	vector<CTriChip>().swap(m_TriList);
	vector<CTriChip>().swap(temp_vertex);
	vector<Section>().swap(sections);

}


BOOL CFeatureReuseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	Current_mesh=-1;
	// Force vector to relinquish its memory (Look up "STL swap trick").
	vector<TriMesh*>	 ().swap(meshes);

	vector<int>().swap(edgemarker);
	vector<int>().swap(trifacemarker);
	vector<CPoint3D>().swap(node);
	vector<CPoint3D>().swap(triface);
	vector<int>().swap(tetrahedronattribute);

	vector<CTriChip> ().swap(m_TriList);
	vector<CTriChip>().swap(temp_vertex);


	steps = 0;
	m_Mode = 0; // 以默认模式绘制
	is_PrintArrows = false;
	is_Draw_cloudPicture = false;
	stage = 0;
	delta = 0;
	vector<Section>().swap(sections);

	return TRUE;
}

// CFeatureReuseDoc 序列化
void CFeatureReuseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

// CFeatureReuseDoc 诊断
#ifdef _DEBUG
void CFeatureReuseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFeatureReuseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG


// 动态设置缩放比例函数
void CFeatureReuseDoc::SetScale(float scale)
{
	m_Scale = scale;
}

// 动态设置旋转角度
void CFeatureReuseDoc::SetMouseRotate(GLfloat xAngle, GLfloat yAngle)
{
	m_xAngle = xAngle;
	m_yAngle = yAngle;

}

// 将读取的stl文件数据传给v_tetIN对象，用于剖分四面体的输入数据
bool CFeatureReuseDoc::inputStl_to_Tetgen()
{

	tetgenmesh::arraypool *plist;
	tetgenio::facet *f;
	tetgenio::polygon *p;
	double *coord;
	int nverts = 0, iverts = 0;
	int nfaces = 0;
	int  i;

	// STL file has no number of points available. Use a list to read points.
	plist = new tetgenmesh::arraypool(sizeof(double) * 3, 10);

	// 遍历容器，即遍历每一个三角片面对象
	for (auto iter = temp_vertex.cbegin(); iter != temp_vertex.cend(); iter++)
	{
		for (int j = 0; j < 3; j++)// 行循环
		{
			// 依次将一个三维点的x,y,z坐标加入coord中
			plist->newindex((void **)&coord);
			coord[0] = (REAL)iter->vex[j].x;
			coord[1] = (REAL)iter->vex[j].y;
			coord[2] = (REAL)iter->vex[j].z;
		}
	}

	nverts = (int)plist->objects;
	// nverts should be an integer times 3 (every 3 vertices denote a face).
	if (nverts == 0 || (nverts % 3 != 0)) {
		printf("Error:  Wrong number of vertices in file.\n");
		delete plist;
		return false;
	}

	v_tetIN.numberofpoints = nverts;
	v_tetIN.pointlist = new REAL[nverts * 3];

	for (i = 0; i < nverts; i++) {
		coord = (double *)fastlookup(plist, i);
		iverts = i * 3;
		v_tetIN.pointlist[iverts] = (REAL)coord[0];
		v_tetIN.pointlist[iverts + 1] = (REAL)coord[1];
		v_tetIN.pointlist[iverts + 2] = (REAL)coord[2];
	}

	nfaces = (int)(nverts / 3);
	v_tetIN.numberoffacets = nfaces;
	v_tetIN.facetlist = new tetgenio::facet[nfaces];

	// Default use '1' as the array starting index.
	v_tetIN.firstnumber = 1;
	iverts = v_tetIN.firstnumber;
	for (i = 0; i < nfaces; i++) {
		f = &v_tetIN.facetlist[i];
		v_tetIN.init(f);
		// In .stl format, each facet has one polygon, no hole.
		f->numberofpolygons = 1;
		f->polygonlist = new tetgenio::polygon[1]; // 剖分出错点，内存泄漏
		p = &f->polygonlist[0];
		v_tetIN.init(p);
		// Each polygon has three vertices.
		p->numberofvertices = 3;

		p->vertexlist = new int[p->numberofvertices]; // 剖分出错点2，，内存泄漏
		p->vertexlist[0] = iverts;
		p->vertexlist[1] = iverts + 1;
		p->vertexlist[2] = iverts + 2;
		iverts += 3;
	}
	delete plist;
	return true;
}

// 剖分函数
bool CFeatureReuseDoc::Dissection()
{
	//if (m_Mode != 0) // 判断是否开启了四面体剖分功能，若模式m_Mode不为0则开启剖分功能
	//{
	// 关于命令参数，参照tetgenbehavior.parse_commandline(int argc, char **argv)函数
	tetgenbehavior b;
	//-P,输入文件是一个PLC(约束德洛内四面体剖分)
	b.plc = 1;
	//-q,为了提高网格质量，设置质量界限（在-q选项后加上一个值较小的数，默认是2.0）
	b.quality = 1;
	b.minratio = 1.2;
	// -a,对所有的四面体强加一个有限元界限（-a选项后带一个较小的值）
	//b.fixedvolume = 1;
	//b.maxvolume = 1.0;
	v_tetIN.initialize(); // 初始化数据输入对象

	//v_tetIN.load_stl();
	//  首先读取stl点及法失数据，作为tetgenio对象的stl输入数据
	if (!inputStl_to_Tetgen()) {
		TRACE(_T("调试信息：本地stl数据转换给tetgen库的io对象失败!"));
		return FALSE;
	}

	v_tetOUT.initialize(); // 输出对象初始化
	tetrahedralize(&b, &v_tetIN, &v_tetOUT); // 四面体剖分函数

											 // Output mesh to files ’barout(q1.2).node’, ’barout(q1.2).ele’ and ’barout(q1.2).face’.
	v_tetOUT.save_nodes("..\\FeatureReuse\\剖分数据\\barout(q1.2)");
	v_tetOUT.save_elements("..\\FeatureReuse\\剖分数据\\barout(q1.2)");
	v_tetOUT.save_faces("..\\FeatureReuse\\剖分数据\\barout(q1.2)");
	v_tetOUT.save_edges("..\\FeatureReuse\\剖分数据\\barout(q1.2)");

	// 存储node结点数据  即PLC点集
	for (int i = 0; i < v_tetOUT.numberofpoints; i++)
	{
		// 读取四面体剖分后的结点数据信息
		CPoint3D tempPoint = CPoint3D(v_tetOUT.pointlist[i * 3],
			v_tetOUT.pointlist[i * 3 + 1], v_tetOUT.pointlist[i * 3 + 2]);

		node.push_back(tempPoint); // 存储结点坐标

	}

	// 保存三角面数据
	for (int i = 0; i < v_tetOUT.numberoftrifaces; i++) {

		CPoint3D tempFace = CPoint3D(v_tetOUT.trifacelist[i * 3],
			v_tetOUT.trifacelist[i * 3 + 1], v_tetOUT.trifacelist[i * 3 + 2]);

		triface.push_back(tempFace);  //  存储三角面三个角点所对应结点的索引

		if (v_tetOUT.trifacemarkerlist != NULL) {
			trifacemarker.push_back(v_tetOUT.trifacemarkerlist[i]);
		}
	}

	// 存储四面体列表
	for (int i = 0; i < v_tetOUT.numberoftetrahedra; i++) {
		CPoint4D cpoint4d = CPoint4D(v_tetOUT.tetrahedronlist[i *  v_tetOUT.numberofcorners],
			v_tetOUT.tetrahedronlist[i *  v_tetOUT.numberofcorners + 1],
			v_tetOUT.tetrahedronlist[i *  v_tetOUT.numberofcorners + 2],
			v_tetOUT.tetrahedronlist[i *  v_tetOUT.numberofcorners + 3]);

		tetrahedron.push_back(cpoint4d);  //  存储四面体四个角点所对应结点的索引

	}

	return true;
}

// 剖分数据 导入CTriChip对象
bool CFeatureReuseDoc::Input_to_CTriChip()
{
	// 保存默认STL数据
	for (int i = 0; i < temp_vertex.size(); i++) {

		// 添加三角面数据，用于OpenGL绘制
		m_TriList.push_back(temp_vertex[i]);
	}

	// 保存三角面数据
	for (int i = 0; i < triface.size(); i++) {
		tem_tri = CTriChip();

		tem_tri.tri[0].x = node.at((int)triface[i].x - 1).x;
		tem_tri.tri[0].y = node.at((int)triface[i].x - 1).y;
		tem_tri.tri[0].z = node.at((int)triface[i].x - 1).z;

		tem_tri.tri[1].x = node.at((int)triface[i].y - 1).x;
		tem_tri.tri[1].y = node.at((int)triface[i].y - 1).y;
		tem_tri.tri[1].z = node.at((int)triface[i].y - 1).z;

		tem_tri.tri[2].x = node.at((int)triface[i].z - 1).x;
		tem_tri.tri[2].y = node.at((int)triface[i].z - 1).y;
		tem_tri.tri[2].z = node.at((int)triface[i].z - 1).z;

		// 添加三角面数据，用于OpenGL绘制
		m_TriList.push_back(tem_tri);
	}

	// 保存四面体数据
	for (int i = 0; i < tetrahedron.size(); i++) {
		tem_tri = CTriChip();

		tem_tri.fourFace[0].x = node.at((int)tetrahedron[i].x - 1).x;
		tem_tri.fourFace[0].y = node.at((int)tetrahedron[i].x - 1).y;
		tem_tri.fourFace[0].z = node.at((int)tetrahedron[i].x - 1).z;

		tem_tri.fourFace[1].x = node.at((int)tetrahedron[i].y - 1).x;
		tem_tri.fourFace[1].y = node.at((int)tetrahedron[i].y - 1).y;
		tem_tri.fourFace[1].z = node.at((int)tetrahedron[i].y - 1).z;

		tem_tri.fourFace[2].x = node.at((int)tetrahedron[i].z - 1).x;
		tem_tri.fourFace[2].y = node.at((int)tetrahedron[i].z - 1).y;
		tem_tri.fourFace[2].z = node.at((int)tetrahedron[i].z - 1).z;

		tem_tri.fourFace[3].x = node.at((int)tetrahedron[i].t - 1).x;
		tem_tri.fourFace[3].y = node.at((int)tetrahedron[i].t - 1).y;
		tem_tri.fourFace[3].z = node.at((int)tetrahedron[i].t - 1).z;

		// 添加三角面数据，用于OpenGL绘制
		m_TriList.push_back(tem_tri);
	}
	return true;
}

// 自定义读取stl数据点函数
bool CFeatureReuseDoc::Load_Stl_Points(LPCTSTR m_stlfile, TriMesh *themesh,int fn)
{
	// 因为使用的是Unicode字符集，则char* 和 LPTSTR属于不同的类型
	// 为了使用这些宏，必须在函数的开始处用USES_CONVERSION来初始化某些局部变量
	USES_CONVERSION;
	for (int j = 0; j < fn; j++)
	{
		tem_tri = CTriChip(); // 内存泄漏点
		int a, b, c;
		a = themesh->faces[j].v[0];
		b = themesh->faces[j].v[1];
		c = themesh->faces[j].v[2];

		// 保存法失
		tem_tri.normal.dx = themesh->FaceNormal[j][0];
		tem_tri.normal.dy = themesh->FaceNormal[j][1];
		tem_tri.normal.dz = themesh->FaceNormal[j][2];

		tem_tri.vex[0].x = themesh->vertices[a][0];
		tem_tri.vex[0].y = themesh->vertices[a][1];
		tem_tri.vex[0].z = themesh->vertices[a][2];

		tem_tri.vex[1].x = themesh->vertices[b][0];
		tem_tri.vex[1].y = themesh->vertices[b][1];
		tem_tri.vex[1].z = themesh->vertices[b][2];

		tem_tri.vex[2].x = themesh->vertices[c][0];
		tem_tri.vex[2].y = themesh->vertices[c][1];
		tem_tri.vex[2].z = themesh->vertices[c][2];

		temp_vertex.push_back(tem_tri);
	}
	return true;
}

void CFeatureReuseDoc::SetMode(int mode)
{
	m_Mode = mode;
}

void CFeatureReuseDoc::SetArrows(bool b)
{
	is_PrintArrows = b;
}

void CFeatureReuseDoc::Set_cloudPicture(bool b)
{
	is_Draw_cloudPicture = b;
}

void CFeatureReuseDoc::SetDelta(int m_delta)// 设置工件模型动态显示循环变量
{
	delta = m_delta;
}

void CFeatureReuseDoc::SetStage(int m_stage)
{
	stage = m_stage;
}

void CFeatureReuseDoc::SetCounter(int counter) {
	counts = counter;
}



// CFeatureReuseDoc 命令
//读取文件STL、PLY、WRL、PTS和ASC等，可以读取二进制的STL文件
BOOL CFeatureReuseDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CString file = lpszPathName;
	SetTitle(lpszPathName);
	// Extension
	CString extension = lpszPathName;
	extension = extension.Right(4);
	extension.MakeLower();

	const char *ffile = file;	
	if(extension == ".stl") //三角网格模型的STL格式文件
	{
		if(IsTextFile(ffile))          //文本格式
		{
			TriMesh *themesh = TriMesh::read_stl(file.GetBuffer(0));
			if (!themesh)
				return false;
			themesh->need_bbox();
			
			themesh->need_normals();

			// 调用自定义节点读取函数
			Load_Stl_Points(lpszPathName, themesh, themesh->faces.size());

			// 自定义剖分函数
			if (!Dissection())
			{
				TRACE(_T("调试信息：自定义剖分STL函数剖分失败!"));
			}

			// 自定义导入剖分数据函数
			if (!Input_to_CTriChip())
			{
				TRACE(_T("调试信息：自定义导入剖分数据函数读取失败!"));

			}
		
			meshes.push_back(themesh);

			Current_mesh=meshes.size()-1;
			themesh->My_ID=Current_mesh;

		}
		else                               //二进制格式
		{
			TriMesh *themesh = TriMesh::load_stl(ffile);
			if (!themesh)
				return false;
			themesh->need_bbox();
			
			themesh->need_Facenormals();
			themesh->need_normals();

			// 自定义读取stl数据点函数
			if (!Load_Stl_Points(lpszPathName, themesh, themesh->faces.size()))
			{
				TRACE(_T("调试信息：// 自定义读取stl数据点函数调用失败!"));
			}
			

			// 自定义剖分函数
			if (!Dissection())
			{
				TRACE(_T("调试信息：自定义剖分STL函数剖分失败!"));
			}

			// 自定义导入剖分数据函数
			if (!Input_to_CTriChip())
			{
				TRACE(_T("调试信息：自定义导入剖分数据函数读取失败!"));

			}

			meshes.push_back(themesh);
			Current_mesh=meshes.size()-1;
			themesh->My_ID=Current_mesh;
		}
	}

	else if (extension == ".txt") //三角网格模型的STL格式文件
	{
		if (IsTextFile(ffile))          //文本格式
		{
			if (!read_txt(file.GetBuffer(0)))
				return FALSE;
		}
	}

	else if(extension == ".ply")
	{
		const char *ff=file;
		TriMesh *themesh = TriMesh::read(ff);
		if (!themesh)
			return false;
		themesh->need_bbox();
		themesh->need_Facenormals();
		themesh->need_normals();
		meshes.push_back(themesh);
		Current_mesh = meshes.size()-1;
		themesh->My_ID=Current_mesh;
	}
	else if(extension == ".wrl")
	{
		TriMesh *mesh = new TriMesh();
		CParserVrml parser;
		if (parser.Run((char *)lpszPathName,mesh))
		{
			mesh->need_bbox();
			mesh->need_Facenormals();
			mesh->need_normals();
			meshes.push_back(mesh);
			Current_mesh=meshes.size()-1;
			mesh->My_ID=Current_mesh;
		}
	}
	else if(extension == ".off")
	{
		const char *ff=file;
		TriMesh *themesh = TriMesh::load_off(ff);
		if (!themesh)
			return false;
		themesh->need_bbox();
		themesh->need_Facenormals();
		themesh->need_normals();
		meshes.push_back(themesh);
		Current_mesh = meshes.size()-1;
		themesh->My_ID=Current_mesh;
	}
	else
	{
		AfxMessageBox("暂时无法读取该种类型的文件！");
		return FALSE;
	}
	return TRUE;
}

bool CFeatureReuseDoc::IsTextFile(const char *strFileName)
{
	if(strFileName==NULL) return false;
	struct stat f_stat; 
	if(stat(strFileName,&f_stat)==-1)
	{ 
		return false;
	} 
	long lngFilesize=0;
	lngFilesize=(long)f_stat.st_size;
	if(lngFilesize<=0) return false;
	FILE *ft=fopen(strFileName, "rb");
	if(ft==NULL)
	{
		return false;
	}
	char btData[1];
	long lngReadPos;
	long lngMaxRead = 40960;
	if(lngMaxRead>lngFilesize) lngMaxRead=lngFilesize;
	for(lngReadPos=1;lngReadPos<=lngMaxRead;lngReadPos++)
	{
		fread(&btData,1,1,ft);
		if(btData[0]=='\0')
		{
			fclose(ft);
			return false; 
		}
	}
	fclose(ft);
	return true;
}

void CFeatureReuseDoc::split(char **dst, char *str, const char *delim)//字符分割函数的简单定义和实现
{
	char *s = NULL;

	s = strtok(str, delim);
	while (s != NULL)
	{
		*dst++ = s;
		s = strtok(NULL, delim);
	}
}




// 寻找未用过的边缘三角面的个数
int CFeatureReuseDoc::findNumOfTrigon_Edge(vector<Trigon> vec)
{
	int counts = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec.at(i).is_edge)
		{
			if (!vec.at(i).is_used)
			{
				counts++;
			}
		}
	}
	return counts;
}

// 判断三角面集合vec所有的三角面是否都已经用过了
bool CFeatureReuseDoc::isAllUsed(vector<Trigon> vec)
{
	int counts = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec.at(i).is_used)
		{
			counts++;
		}
	}
	if (counts == vec.size())
	{
		return true;
	}
	return false;
}

// 寻找是否有相同的三角面片
bool CFeatureReuseDoc::findSame(vector<Dcontour> Dcontour_Set, Trigon m_triangle)
{
	// 获取当前三角面片三个顶点的节点坐标值
	CPoint2D p2,p3, p4;
	p2 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
	p3 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
	p4 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);

	for (int i = 0; i < Dcontour_Set.size(); i++)
	{
		Dcontour d = Dcontour_Set.at(i);
		CPoint2D p0 = d.m_lines.at(0);
		CPoint2D p1 = d.m_lines.at(1);
		if ((p0 == p2 || p0 == p3 || p0 == p4) && (p1 == p2 || p1 == p3 || p1 == p4) )
		{
			return true;
		}

	}
	return false;
}

// 读取工件的坐标点及应力应变和矢量方向
bool CFeatureReuseDoc::read_txt(string f)
{
	ifstream fin(f, ios::in);
	char* temp[50] = { 0 };
	string feature;
	double dx1, dy1, dx2, dy2, dx3, dy3;
	double Vx1, Vy1, m_strain1, m_stress1, Vx2, Vy2, m_strain2, m_stress2, Vx3, Vy3, m_strain3, m_stress3;
	int m_index1, m_index2, m_index3;
	while (!fin.eof())
	{
		getline(fin, feature); // 一次读取一行数据

		if (feature.find("step id") != string::npos)
		{
			Section m_Section = Section();
			while (1)
			{				
				getline(fin, feature); // 一次读取一行数据
				
				if (feature.length() == 0) // 判断空行
				{
					break;
				}
				split(temp, (char*)feature.data(), " ");
				// 获取索引号
				m_index1 = atof(temp[0]);
				m_index2 = atof(temp[1]);
				m_index3 = atof(temp[2]);
				// 获取其他信息
				dx1 = atof(temp[3]);
				dy1 = atof(temp[4]);
				dx2 = atof(temp[5]);
				dy2 = atof(temp[6]);
				dx3 = atof(temp[7]);
				dy3 = atof(temp[8]);
				Vx1 = atof(temp[9]);
				Vy1 = atof(temp[10]);
				Vx2 = atof(temp[11]);
				Vy2 = atof(temp[12]);
				Vx3 = atof(temp[13]);
				Vy3 = atof(temp[14]);
				m_strain1 = atof(temp[15]);
				m_stress1 = atof(temp[16]);
				m_strain2 = atof(temp[17]);
				m_stress2 = atof(temp[18]);
				m_strain3 = atof(temp[19]);
				m_stress3 = atof(temp[20]);

				m_Section.Index.push_back(m_index1);
				m_Section.Index.push_back(m_index2);
				m_Section.Index.push_back(m_index3);

				CPoint2D m_point1 = CPoint2D(dx1, dy1);
				CPoint2D m_point2 = CPoint2D(dx2, dy2);
				CPoint2D m_point3 = CPoint2D(dx3, dy3);
				m_Section.point_2D.push_back(m_point1);
				m_Section.point_2D.push_back(m_point2);
				m_Section.point_2D.push_back(m_point3);

				CPoint2D m_speed1 = CPoint2D(Vx1, Vy1);
				CPoint2D m_speed2 = CPoint2D(Vx2, Vy2);
				CPoint2D m_speed3 = CPoint2D(Vx3, Vy3);
				m_Section.speed_2D.push_back(m_speed1);
				m_Section.speed_2D.push_back(m_speed2);
				m_Section.speed_2D.push_back(m_speed3);

				m_Section.strain.push_back(m_strain1);
				m_Section.strain.push_back(m_strain2);
				m_Section.strain.push_back(m_strain3);

				m_Section.stress.push_back(m_stress1);
				m_Section.stress.push_back(m_stress2);
				m_Section.stress.push_back(m_stress3);
			}

			vector<Trigon> Trigon_Set;
			// 遍历m_Section查找含有等值点的三角网格
			for (int i = 0; i < m_Section.point_2D.size(); i = i + 3)
			{
				Trigon trigon;
				DPoint p1,p2,p3;
				p1.x = m_Section.point_2D.at(i).x;
				p1.y = m_Section.point_2D.at(i).y;
				p1.index = m_Section.Index.at(i);
				p1.strain = m_Section.strain.at(i);
				p1.stress = m_Section.stress.at(i);
				p2.x = m_Section.point_2D.at(i+1).x;
				p2.y = m_Section.point_2D.at(i+1).y;
				p2.index = m_Section.Index.at(i+1);
				p2.strain = m_Section.strain.at(i+1);
				p2.stress = m_Section.stress.at(i+1);
				p3.x = m_Section.point_2D.at(i+2).x;
				p3.y = m_Section.point_2D.at(i+2).y;
				p3.index = m_Section.Index.at(i+2);
				p3.strain = m_Section.strain.at(i+2);
				p3.stress = m_Section.stress.at(i+2);
				// 获取三角面片
				trigon.is_used = false;
				trigon.point[0] = p1;
				trigon.point[1] = p2;
				trigon.point[2] = p3;
				Trigon_Set.push_back(trigon);
			}
			ContourTriArr.push_back(Trigon_Set);
			sections.push_back(m_Section);
		}
	}
	steps = sections.size(); // 获取工件变形的总阶段数

	// 搜寻边缘三角面片
	for (int k = 0; k < ContourTriArr.size(); k++)
	{
		vector<Trigon> triangle_Edge; // 用于保存边缘三角面片
		vector<Trigon> triangle_Set;  // 用于保存当前三角面片集合
		triangle_Set = ContourTriArr.at(k); // 获取第k步的工件的三角面片集合

		// 遍历第k步的三角面片集合
		for (int i = 0; i < triangle_Set.size(); i = i + 1)
		{
			Trigon m_triangle;
			// 获取第k阶段的第i个三角面片
			m_triangle = triangle_Set.at(i);

			bool flag1 = true; // 用于标记三角面片1，2点所在边是否为边界边
			bool flag2 = true; // 用于标记三角面片2，3点所在边是否为边界边
			bool flag3 = true; // 用于标记三角面片3，1点所在边是否为边界边

			int ind1, ind2, ind3;
			ind1 = m_triangle.point[0].index;
			ind2 = m_triangle.point[1].index;
			ind3 = m_triangle.point[2].index;
			// 遍历三角面集合，标记当前三角面片的共边
			for (int j = 0; j < triangle_Set.size(); j++)
			{
				// 跳过当前待检测三角面片
				if (i == j)
				{
					continue;
				}

				Trigon m_Trigon;
				m_Trigon = triangle_Set.at(j);  // 获取三角面

				//可能1，2和1，3点共边
				if (m_Trigon.point[0].index == ind1)
				{
					if (m_Trigon.point[1].index == ind2)
						flag1 = false;
					else if (m_Trigon.point[2].index == ind2)
						flag1 = false;
					if (m_Trigon.point[1].index == ind3)
						flag3 = false;
					else if (m_Trigon.point[2].index == ind3)
						flag3 = false;
				}
				else if (m_Trigon.point[1].index == ind1)
				{
					if (m_Trigon.point[0].index == ind2)
						flag1 = false;
					else if (m_Trigon.point[2].index == ind2)
						flag1 = false;
					if (m_Trigon.point[0].index == ind3)
						flag3 = false;
					else if (m_Trigon.point[2].index == ind3)
						flag3 = false;
				}
				else if (m_Trigon.point[2].index == ind1)
				{
					if (m_Trigon.point[0].index == ind2)
						flag1 = false;
					else if (m_Trigon.point[1].index == ind2)
						flag1 = false;
					if (m_Trigon.point[0].index == ind3)
						flag3 = false;
					else if (m_Trigon.point[1].index == ind3)
						flag3 = false;
				}

				//可能2，1和2，3点共边
				if (m_Trigon.point[0].index == ind2)
				{
					if (m_Trigon.point[1].index == ind1)
						flag1 = false;
					else if (m_Trigon.point[2].index == ind1)
						flag1 = false;
					if (m_Trigon.point[1].index == ind3)
						flag2 = false;
					else if (m_Trigon.point[2].index == ind3)
						flag2 = false;
				}
				else if (m_Trigon.point[1].index == ind2)
				{
					if (m_Trigon.point[0].index == ind1)
						flag1 = false;
					else if (m_Trigon.point[2].index == ind1)
						flag1 = false;
					if (m_Trigon.point[0].index == ind3)
						flag2 = false;
					else if (m_Trigon.point[2].index == ind3)
						flag2 = false;
				}
				else if (m_Trigon.point[2].index == ind2)
				{
					if (m_Trigon.point[0].index == ind1)
						flag1 = false;
					else if (m_Trigon.point[1].index == ind1)
						flag1 = false;
					if (m_Trigon.point[0].index == ind3)
						flag2 = false;
					else if (m_Trigon.point[1].index == ind3)
						flag2 = false;
				}

				// 可能3，1和3，2点共边
				if (m_Trigon.point[0].index == ind3)
				{
					if (m_Trigon.point[1].index == ind1)
						flag3 = false;
					else if (m_Trigon.point[2].index == ind1)
						flag3 = false;
					if (m_Trigon.point[1].index == ind2)
						flag2 = false;
					else if (m_Trigon.point[2].index == ind2)
						flag2 = false;
				}
				else if (m_Trigon.point[1].index == ind3)
				{
					if (m_Trigon.point[0].index == ind1)
						flag3 = false;
					else if (m_Trigon.point[2].index == ind1)
						flag3 = false;
					if (m_Trigon.point[0].index == ind2)
						flag2 = false;
					else if (m_Trigon.point[2].index == ind2)
						flag2 = false;
				}
				else if (m_Trigon.point[2].index == ind3)
				{
					if (m_Trigon.point[1].index == ind1)
						flag3 = false;
					else if (m_Trigon.point[3].index == ind1)
						flag3 = false;
					if (m_Trigon.point[1].index == ind2)
						flag2 = false;
					else if (m_Trigon.point[3].index == ind2)
						flag2 = false;
				}
			}

			// 判断当前三角面片是否有边界边（即有不共边的边）来判断是否为边界三角面片
			if (flag1) // 1,2点所在的边为边界边
			{
					m_triangle.is_edge = true;	 // 标记当前三角面片为边界三角面片
					m_triangle.flag1 = true;		// 设置当前三角面片的1，2点为边界边
			}
			if (flag2) // 2, 3点所在的边为边界边
			{
				m_triangle.is_edge = true;	 // 标记当前三角面片为边界三角面片
				m_triangle.flag2 = true;		// 设置当前三角面片的2，3点为边界边
			}
			if (flag3) //3, 1点所在的边为边界边
			{
				m_triangle.is_edge = true;	 // 标记当前三角面片为边界三角面片
				m_triangle.flag3 = true;		// 设置当前三角面片的3，1点为边界边
			}

			if (flag1 || flag2 || flag3) // 若当前三角面片为边缘三角面片则加入边缘三角面片集合
			{
				
				triangle_Edge.push_back(m_triangle); // 保存边界三角面片
			}

			ContourTriArr.at(k).at(i) = m_triangle;   // 保存已修改过属性的三角面片
		}

		// 保存第k阶段的边缘三角面片
		Trigon_Edge_Set.push_back(triangle_Edge); 
	}


	// 循环边界三角面片，建立边缘曲线
	for (int k = 0; k < Trigon_Edge_Set.size(); k++)
	{
		vector<CPoint2D> edgeCurve;   // 用于保存第k阶段的边缘曲线
		vector<Trigon> triangle_Set;  // 用于保存当前边界三角面片集合
		triangle_Set = Trigon_Edge_Set.at(k); // 获取第k阶段的工件的边界三角面片集合

		// 遍历第k阶段的边界三角面片集合
		int next_Tri = 0; // 记录下一个邻接三角面片所在容器中的位置
		int P_ind = 0;		// 记录下一个邻接三角面片中与上一个三角面片共点所在的边界边的另一端的点下标
		while (true)
		{
			Trigon m_triangle;
			// 获取第k阶段的第next_Tri个三角面片
			m_triangle = triangle_Set.at(next_Tri);

			if (next_Tri != 0)
			{
				// 有两条边界边的三角面片
				if ((m_triangle.flag1 && m_triangle.flag2) || (m_triangle.flag1 && m_triangle.flag3) || (m_triangle.flag2 && m_triangle.flag3))
				{
					for (int m = 0; m < 3; m++)
					{
						if (m == P_ind)
							continue;
						double temp_x = edgeCurve.back().x;
						double temp_y = edgeCurve.back().y;
						if (temp_x != m_triangle.point[m].x || temp_y != m_triangle.point[m].y)
						{
							CPoint2D temp = CPoint2D(m_triangle.point[P_ind].x, m_triangle.point[P_ind].y);
							edgeCurve.push_back(temp);
							P_ind = m;
							break;
						}
					}
				}

				// 加入第next_Tri个三角面片的第P_ind个点坐标
				CPoint2D temp = CPoint2D(m_triangle.point[P_ind].x, m_triangle.point[P_ind].y);
				edgeCurve.push_back(temp);
				int ind = m_triangle.point[P_ind].index;
				// 获取下一个邻接三角面片
				int j ;
				for ( j = 0; j < triangle_Set.size(); j++)
				{
					if (next_Tri == j)
						continue;
					Trigon temp_triangle = triangle_Set.at(j);
					int ind4, ind5, ind6;
					ind4 = temp_triangle.point[0].index;
					ind5 = temp_triangle.point[1].index;
					ind6 = temp_triangle.point[2].index;
					if (ind4 == ind) // 1，2边或者1，3边所在边为边界边
					{
						if (temp_triangle.flag1) // 1，2边所在边为边界边 
						{
							next_Tri = j;
							P_ind = 1;
						}
						else if (temp_triangle.flag3) //1，3边所在边为边界边
						{
							next_Tri = j;
							P_ind = 2;
						}
						break;
					}
					else if (ind5 == ind)  // 2，1边或者2，3边所在边为边界边
					{
						if (temp_triangle.flag1) // 1，2边为边界边
						{
							next_Tri = j;
							P_ind = 0;
						}
						else if (temp_triangle.flag2) // 2，3边为边界边
						{
							next_Tri = j;
							P_ind = 2;
						}
						break;
					}
					else if (ind6 == ind)  // 3，1边或者3，2边所在边为边界边
					{
						if (temp_triangle.flag2) // 3，2边为边界边
						{
							next_Tri = j;
							P_ind = 1;
						}
						else if (temp_triangle.flag3) // 3，1边为边界边
						{
							next_Tri = j;
							P_ind = 0;
						}
						break;
					}

				}

				//// 搜索结束跳出循环
				//if (j == triangle_Set.size())
				//	break;

				// 回到边缘曲线起点
				if (next_Tri == 0)
				{
					//Trigon m_tri;
					//// 获取第k阶段的第next_Tri个三角面片
					//m_tri = triangle_Set.at(next_Tri);
					//// 有两条边界边的三角面片
					//if ((m_tri.flag1 && m_tri.flag2) || (m_tri.flag2 && m_tri.flag3) || (m_tri.flag1 && m_tri.flag3))
					//{
					//	CPoint2D tep1 = edgeCurve.at(0);
					//	CPoint2D tep3 = edgeCurve.at(1);
					//	for (int m = 0; m < 3; m++)
					//	{
					//		CPoint2D tep2 = CPoint2D(m_triangle.point[m].x, m_triangle.point[m].y);
					//		if ((tep1 == tep2) || (tep3 == tep2))
					//		{
					//			continue;
					//		}
					//		else
					//		{
					//			CPoint2D temp = CPoint2D(m_triangle.point[P_ind].x, m_triangle.point[P_ind].y);
					//			edgeCurve.push_back(temp);
					//			P_ind = m;
					//			break;
					//		}
					//	}
					//}

					break; // 跳出循环，搜索下一阶段的边缘曲线
				}

			}
			// 建立边缘曲线起点
			else
			{
				int ind1, ind2, ind3;
				ind1 = m_triangle.point[0].index;
				ind2 = m_triangle.point[1].index;
				ind3 = m_triangle.point[2].index;
				bool flag1 = m_triangle.flag1; // 获取1，2边界边标志
				bool flag2 = m_triangle.flag2; // 获取2，3边界边标志
				bool flag3 = m_triangle.flag3; // 获取3，1边界边标志


				if (flag1) // 1,2点所在的边为边界边
				{
					if ((flag1 && flag2) || (flag1 && flag3))
					{
						if (flag1 && flag3)
						{
							CPoint2D temp1 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
							CPoint2D temp2 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
							edgeCurve.push_back(temp1);
							edgeCurve.push_back(temp2);
						}
						else
						{
							CPoint2D temp1 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
							CPoint2D temp2 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
							edgeCurve.push_back(temp1);
							edgeCurve.push_back(temp2);
						}

					}
					else
					{
						CPoint2D temp = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
						edgeCurve.push_back(temp);
					}

					// 寻找下一个邻接三角面片
					for (int j = 0; j < triangle_Set.size(); j++)
					{
						if (next_Tri == j)
							continue;
						Trigon temp_triangle = triangle_Set.at(j);
						int ind4, ind5, ind6;
						ind4 = temp_triangle.point[0].index;
						ind5 = temp_triangle.point[1].index;
						ind6 = temp_triangle.point[2].index;
						if (ind4 == ind1)   // 1，2边为边界边或者1，3边为边界边
						{
							if (temp_triangle.flag1)
							{
								next_Tri = j;
								P_ind = 1;
							}
							else if (temp_triangle.flag3)
							{
								next_Tri = j;
								P_ind = 2;
							}
							break;
						}
						else if (ind5 == ind1)  // 2，1边为边界边或者2，3边为边界边
						{

							if (temp_triangle.flag1)
							{
								next_Tri = j;
								P_ind = 0;
							}
							else if (temp_triangle.flag2)
							{
								next_Tri = j;
								P_ind = 2;
							}
							break;
						}
						else if (ind6 == ind1)  // 3，1边为边界边或者3，2边为边界边
						{
							if (temp_triangle.flag2)
							{
								next_Tri = j;
								P_ind = 1;
							}
							else if (temp_triangle.flag3)
							{
								next_Tri = j;
								P_ind = 0;
							}
							break;
						}
						else if (ind4 == ind2)  // 1，2边为边界边或者1，3边为边界边
						{

							if (temp_triangle.flag1)
							{
								next_Tri = j;
								P_ind = 1;
							}
							else if (temp_triangle.flag3)
							{
								next_Tri = j;
								P_ind = 2;
							}
							break;
						}
						else if (ind5 == ind2) // 2，1边为边界边或者2，3边为边界边
						{

							if (temp_triangle.flag1)
							{
								next_Tri = j;
								P_ind = 0;
							}
							else if (temp_triangle.flag2)
							{
								next_Tri = j;
								P_ind = 2;
							}
							break;
						}
						else if (ind6 == ind2)  // 3，1边为边界边或者3，2边为边界边
						{
							if (temp_triangle.flag2)
							{
								next_Tri = j;
								P_ind = 1;
							}
							else if (temp_triangle.flag3)
							{
								next_Tri = j;
								P_ind = 0;
							}
							break;
						}

					}
				}
				else if (flag2) // 2, 3点所在的边为边界边
				{
					if ((flag2 && flag1) || (flag2 && flag3))
					{
						if (flag2 && flag3)
						{
							CPoint2D temp1 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
							CPoint2D temp2 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
							edgeCurve.push_back(temp1);
							edgeCurve.push_back(temp2);
						}
						else
						{
							CPoint2D temp1 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
							CPoint2D temp2 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
							edgeCurve.push_back(temp1);
							edgeCurve.push_back(temp2);
						}

					}
					else
					{
						CPoint2D temp = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
						edgeCurve.push_back(temp);
					}

					// 寻找下一个邻接三角面片
					for (int j = 0; j < triangle_Set.size(); j++)
					{
						if (next_Tri == j)
							continue;
						Trigon temp_triangle = triangle_Set.at(j);
						int ind4, ind5, ind6;
						ind4 = temp_triangle.point[0].index;
						ind5 = temp_triangle.point[1].index;
						ind6 = temp_triangle.point[2].index;
						if (ind4 == ind2)   // 1，2边为边界边或者1，3边为边界边
						{
							if (temp_triangle.flag1)
							{
								next_Tri = j;
								P_ind = 1;
							}
							else if (temp_triangle.flag3)
							{
								next_Tri = j;
								P_ind = 2;
							}
							break;
						}
						else if (ind5 == ind2)  // 2，1边为边界边或者2，3边为边界边
						{

							if (temp_triangle.flag1)
							{
								next_Tri = j;
								P_ind = 0;
							}
							else if (temp_triangle.flag2)
							{
								next_Tri = j;
								P_ind = 2;
							}
							break;
						}
						else if (ind6 == ind2)  // 3，1边为边界边或者3，2边为边界边
						{
							if (temp_triangle.flag2)
							{
								next_Tri = j;
								P_ind = 1;
							}
							else if (temp_triangle.flag3)
							{
								next_Tri = j;
								P_ind = 0;
							}
							break;
						}
						else if (ind4 == ind3)  // 1，2边为边界边或者1，3边为边界边
						{

							if (temp_triangle.flag1)
							{
								next_Tri = j;
								P_ind = 1;
							}
							else if (temp_triangle.flag3)
							{
								next_Tri = j;
								P_ind = 2;
							}
							break;
						}
						else if (ind5 == ind3) // 2，1边为边界边或者2，3边为边界边
						{

							if (temp_triangle.flag1)
							{
								next_Tri = j;
								P_ind = 0;
							}
							else if (temp_triangle.flag2)
							{
								next_Tri = j;
								P_ind = 2;
							}
							break;
						}
						else if (ind6 == ind3)  // 3，1边为边界边或者3，2边为边界边
						{
							if (temp_triangle.flag2)
							{
								next_Tri = j;
								P_ind = 1;
							}
							else if (temp_triangle.flag3)
							{
								next_Tri = j;
								P_ind = 0;
							}
							break;
						}
					}
				}
				else if (flag3) //3, 1点所在的边为边界边
				{

					if ((flag3 && flag1) || (flag3 && flag2))
					{
						if (flag2 && flag3)
						{
							CPoint2D temp1 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
							CPoint2D temp2 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
							edgeCurve.push_back(temp1);
							edgeCurve.push_back(temp2);
						}
						else
						{
							CPoint2D temp1 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
							CPoint2D temp2 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
							edgeCurve.push_back(temp1);
							edgeCurve.push_back(temp2);
						}

					}
					else
					{
						CPoint2D temp = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
						edgeCurve.push_back(temp);
					}

					// 寻找下一个邻接三角面片
					for (int j = 0; j < triangle_Set.size(); j++)
					{
						if (next_Tri == j)
							continue;
						Trigon temp_triangle = triangle_Set.at(j);
						int ind4, ind5, ind6;
						ind4 = temp_triangle.point[0].index;
						ind5 = temp_triangle.point[1].index;
						ind6 = temp_triangle.point[2].index;

					}
				}
			}
		}

		// 加入第k阶段边缘曲线
		BorderLine_Set.push_back(edgeCurve);
	}


	// 获取各个阶段的高程值集合
	for (int i = 0; i < ContourTriArr.size(); i++)
	{
		// 定义第i阶段的高程值集合
		vector<double> stress_Set;
		// 初始化最大值和最小值变量

		double stress_Temp = 0, m_stress_Max, m_stress_Min;
		m_stress_Max = ContourTriArr.at(i).at(0).point[0].stress;
		m_stress_Min = ContourTriArr.at(i).at(0).point[0].stress;

		double temp_x,temp_y, m_Max_x, m_Min_x, m_Max_y, m_Min_y;
		m_Max_x = ContourTriArr.at(i).at(0).point[0].x;
		m_Min_x = ContourTriArr.at(i).at(0).point[0].x;
		m_Max_y = ContourTriArr.at(i).at(0).point[0].y;
		m_Min_y = ContourTriArr.at(i).at(0).point[0].y;

		// 求解应力和横纵坐标的最大值和最小值
		for (int j = 0; j < ContourTriArr.at(i).size(); j++)
		{
			for (int m = 0; m<3; m++)
			{
				// 寻找横纵坐标最大值和最小值
				temp_x = ContourTriArr.at(i).at(j).point[m].x;
				temp_y = ContourTriArr.at(i).at(j).point[m].y;
				if (temp_x > m_Max_x)
				{
					m_Max_x = temp_x;
				}
				if (temp_x < m_Min_x)
				{
					m_Min_x = temp_x;
				}
				if (temp_y > m_Max_y)
				{
					m_Max_y = temp_y;
				}
				if (temp_y < m_Min_y)
				{
					m_Min_y = temp_y;
				}

				// 寻找应力最大值和最小值
				stress_Temp = ContourTriArr.at(i).at(j).point[m].stress;
					if (stress_Temp > m_stress_Max)
					{
						m_stress_Max = stress_Temp;
					}
					if (stress_Temp < m_stress_Min)
					{
						m_stress_Min = stress_Temp;
					}
			}
		}

		Max_x.push_back(m_Max_x);
		Max_y.push_back(m_Max_y);
		Min_x.push_back(m_Min_x);
		Min_y.push_back(m_Min_y);


		Ncolor = 10.0;
		m_stress_Max = (int(m_stress_Max / 10.0) + 1) * 10;
		m_stress_Min = (int(m_stress_Min / 10.0)) * 10;
		stress_Max.push_back(m_stress_Max);
		stress_Min.push_back(m_stress_Min);

		Dc.push_back((int)((m_stress_Max - m_stress_Min) / Ncolor));  // 保存每个阶段的Dc值
		double m_delta = (m_stress_Max - m_stress_Min + 1) / Ncolor;
		delta_Set.push_back(m_delta); //  保存delta值
		// 保存每个阶段的高程值分级
		for (int n = 1; n < int(Ncolor); n++)
		{
			stress_Set.push_back(m_stress_Min + n*m_delta);
		}
		stress_TotalSet.push_back(stress_Set);
	}


	// 基于delaunay等值线搜索算法构建等值线
	vector<vector<Trigon>> Trigon_Set_Stage = ContourTriArr;  
	//先循环阶段，再遍历高程值，再根据高程值循环当前阶段的三角面片数
	for (int k= 0; k < Trigon_Set_Stage.size(); k++)
	{
		// 创建等值线集合对象
		vector<vector<Dcontour>> Dcontour_SetM;  

		// 遍历高程值
		for (int g = 0; g < stress_TotalSet.at(k).size(); g++)
		{
	
			vector<Trigon> triangle_Temp;         // 创建三角面片结合对象 
			double m_stress = stress_TotalSet.at(k).at(g); // 获取高程值

			// 根据高程值解析出含有该高程值的三角面片集合
			for (int v = 0; v < Trigon_Set_Stage.at(k).size(); v++)
			{
				Trigon m_triangle_Temp;
				m_triangle_Temp = Trigon_Set_Stage.at(k).at(v);

				double analy1 = (m_stress - m_triangle_Temp.point[0].stress)*(m_stress - m_triangle_Temp.point[1].stress);
				double analy2 = (m_stress - m_triangle_Temp.point[0].stress)*(m_stress - m_triangle_Temp.point[2].stress);
				double analy3 = (m_stress - m_triangle_Temp.point[1].stress)*(m_stress - m_triangle_Temp.point[2].stress);
				// 将含有高程值的三角面片加入triangle_Temp中
				if (analy1 < 0 || analy2 < 0 || analy3 < 0)
				{
					// 计入含高程值的三角面片
					triangle_Temp.push_back(m_triangle_Temp); 
				}
			}

			// 不断搜索具有高程值的等值线，直到triangle_Temp全部搜索完毕
			while (!isAllUsed(triangle_Temp))
			{
				int counter = 0;
				// 记录当前三角面片的输出边（由index1和index2决定）
				double index1 = 0;
				double index2 = 0;
				double index3 = 0;
				vector<Dcontour> Dcontour_Set;     // 创建等值线对象
				// 搜索含有高程值的等值线集合
				for (int i = 0; i < triangle_Temp.size(); i ++)
				{
					double x0, y0, H;
					double stress1, stress2, stress3;
					Trigon m_triangle;
					m_triangle = triangle_Temp.at(i);

					// 判断triangle_Temp集合中是否还有未使用的边缘三角面片
					if (findNumOfTrigon_Edge(triangle_Temp) != 0)
					{
						// 当前三角面片不是边缘三角面片
						if (!m_triangle.is_edge)
						{
							continue;
						}
						// 当前三角面片是边缘三角面片
						else
						{
							if (m_triangle.is_used) // 当前三角面片用过了
							{
								continue;
							}
						}
					}

					// 当前三角面片是非边缘三角面片
					if (!m_triangle.is_edge)
					{
						if (m_triangle.is_used)
						{
							continue;
						}
						else
						{
							m_triangle.is_used = true;
							triangle_Temp.at(i) = m_triangle; // 重新放回三角面片集合中
						}
					}

					// 当前三角面片是边缘三角面片
					else
					{
						if (m_triangle.is_used)
						{
							continue;
						}
						else
						{
							m_triangle.is_used = true;
							triangle_Temp.at(i) = m_triangle; // 重新放回三角面片集合中
						}
					}

					// 寻找一条闭合等值线或者非闭合等值线
					if (counter == 0) // 寻找等值线的起点
					{
						stress1 = m_triangle.point[0].stress;
						stress2 = m_triangle.point[1].stress;
						stress3 = m_triangle.point[2].stress;
						double analy1 = (m_stress - stress1)*(m_stress - stress2);
						double analy2 = (m_stress - stress1)*(m_stress - stress3);
						double analy3 = (m_stress - stress2)*(m_stress - stress3);
						if (analy1 < 0) // 1，2坐标点所在的边存在高程值
						{
							counter = 1; // 修改状态
							Dcontour contour1;
							x0 = m_triangle.point[0].x + (m_triangle.point[1].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress2 - stress1);
							y0 = m_triangle.point[0].y + (m_triangle.point[1].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress2 - stress1);
							H = m_stress;
							contour1.x = x0;
							contour1.y = y0;
							contour1.H = H;

							contour1.m_lines.push_back(CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y));
							contour1.m_lines.push_back(CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y));
							Dcontour_Set.push_back(contour1);
							//m_triangle.is_used = true; // 标记以使用过的三角面片
							m_triangle.is_Original = true; // 标记起始三角面片
							triangle_Temp.at(i) = m_triangle; // 重新放回三角面片集合中

							// 1，3坐标点所在的边存在高程值
							if (analy2 < 0)
							{
								Dcontour contour2;
								x0 = m_triangle.point[0].x + (m_triangle.point[2].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress3 - stress1);
								y0 = m_triangle.point[0].y + (m_triangle.point[2].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress3 - stress1);
								H = m_stress;
								contour2.x = x0;
								contour2.y = y0;
								contour2.H = H;

								contour2.m_lines.push_back(CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y));
								contour2.m_lines.push_back(CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y));
								Dcontour_Set.push_back(contour2);

								Dcontour tep1,tep2;
								// 非闭合等值线
								if (m_triangle.is_edge)
								{
									// 1,3点所在的边为边界边
									if (m_triangle.flag3)
									{
										tep1 = Dcontour_Set.at(0); // 缓存非边界边
										tep2 = Dcontour_Set.at(1); // 边界边
										// 获取等值线线头所在边界边的两个端点坐标
										CPoint2D p1, p2;
										p1 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
										p2 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
										tep2.m_lines.clear();
										tep2.m_lines.push_back(p1);
										tep2.m_lines.push_back(p2);
										// 标记当前点为等值线线头
										tep2.is_begin = true;
										Dcontour_Set.clear();
										Dcontour_Set.push_back(tep2);
										Dcontour_Set.push_back(tep1);
										// 记录当前三角面片的输出边（由index1和index2决定）
										index1 = m_triangle.point[0].index;
										index2 = m_triangle.point[1].index;
										index3 = m_triangle.point[2].index;

									}
									// 1,2点所在的边为边界边
									else 
									{
										// 记录当前三角面片的输出边（由index1和index2决定）
										index1 = m_triangle.point[0].index;
										index2 = m_triangle.point[2].index;
										index3 = m_triangle.point[1].index;
										Dcontour_Set.at(0).is_begin = true; // 缓存边界边
									}
								}
								// 闭合等值线
								else
								{

									index1 = m_triangle.point[0].index;
									index2 = m_triangle.point[2].index;
									index3 = m_triangle.point[1].index;

								}
							}
							// 2，3坐标点所在的边存在高程值
							else if (analy3 < 0)
							{
								Dcontour contour2;
								x0 = m_triangle.point[1].x + (m_triangle.point[2].x - m_triangle.point[1].x)*(m_stress - stress2) / (stress3 - stress2);
								y0 = m_triangle.point[1].y + (m_triangle.point[2].y - m_triangle.point[1].y)*(m_stress - stress2) / (stress3 - stress2);
								H = m_stress;
								contour2.x = x0;
								contour2.y = y0;
								contour2.H = H;
								contour2.m_lines.push_back(CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y));
								contour2.m_lines.push_back(CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y));
								Dcontour_Set.push_back(contour2);

								Dcontour tep1,tep2;
								if (m_triangle.is_edge)
								{
									// 2,3点所在的边为边界边
									if (m_triangle.flag2)
									{
										tep1 = Dcontour_Set.at(0); // 缓存非边界边
										tep2 = Dcontour_Set.at(1); // 边界边
										// 获取等值线线头所在边界边的两个端点坐标
										CPoint2D p1, p2;
										p1 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
										p2 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
										tep2.m_lines.clear();
										tep2.m_lines.push_back(p1);
										tep2.m_lines.push_back(p2);
										// 标记当前点为等值线线头
										tep2.is_begin = true;
										Dcontour_Set.clear();
										Dcontour_Set.push_back(tep2);
										Dcontour_Set.push_back(tep1);

										index1 = m_triangle.point[0].index;
										index2 = m_triangle.point[1].index;
										index3 = m_triangle.point[2].index;
				
									}
									// 1,2点所在的边为边界边
									else
									{
										// 记录当前三角面片的输出边（由index1和index2决定）
										index1 = m_triangle.point[1].index;
										index2 = m_triangle.point[2].index;
										index3 = m_triangle.point[0].index;
										Dcontour_Set.at(0).is_begin = true; // 缓存边界边
									}
								}
								else
								{
									index2 = m_triangle.point[1].index;
									index1 = m_triangle.point[2].index;
									index3 = m_triangle.point[0].index;
								}

							}
						}
						else if (analy2 < 0) // 1，3坐标点所在的边存在高程值
						{
							counter = 1; // 修改状态
							Dcontour contour2;
							x0 = m_triangle.point[0].x + (m_triangle.point[2].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress3 - stress1);
							y0 = m_triangle.point[0].y + (m_triangle.point[2].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress3 - stress1);
							H = m_stress;
							contour2.x = x0;
							contour2.y = y0;
							contour2.H = H;
							contour2.m_lines.push_back(CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y));
							contour2.m_lines.push_back(CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y));
							Dcontour_Set.push_back(contour2);
							//m_triangle.is_used = true; // 标记以使用过的三角面片
							m_triangle.is_Original = true; // 标记起始三角面片
							triangle_Temp.at(i) = m_triangle; // 重新放回三角面片集合中

							// 1，2坐标点所在的边存在高程值
							if (analy1 < 0)
							{
								Dcontour contour1;
								x0 = m_triangle.point[0].x + (m_triangle.point[1].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress2 - stress1);
								y0 = m_triangle.point[0].y + (m_triangle.point[1].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress2 - stress1);
								H = m_stress;
								contour1.x = x0;
								contour1.y = y0;
								contour1.H = H;
								contour1.m_lines.push_back(CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y));
								contour1.m_lines.push_back(CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y));
								Dcontour_Set.push_back(contour1);

								Dcontour tep1,tep2;
								if (m_triangle.is_edge)
								{
									// 1,2点所在的边为边界边
									if (m_triangle.flag1)
									{
										tep1 = Dcontour_Set.at(0); // 缓存非边界边
										tep2 = Dcontour_Set.at(1); // 边界边
										// 获取等值线线头所在边界边的两个端点坐标
										CPoint2D p1, p2;
										p1 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
										p2 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
										tep2.m_lines.clear();
										tep2.m_lines.push_back(p1);
										tep2.m_lines.push_back(p2);
										// 标记当前点为等值线线头
										tep2.is_begin = true;
										Dcontour_Set.clear();
										Dcontour_Set.push_back(tep2);
										Dcontour_Set.push_back(tep1);

										index1 = m_triangle.point[0].index;
										index2 = m_triangle.point[2].index;
										index3 = m_triangle.point[1].index;

									}
									// 1,3点所在的边为边界边
									else
									{
										// 记录当前三角面片的输出边（由index1和index2决定）
										index1 = m_triangle.point[0].index;
										index2 = m_triangle.point[1].index;
										index3 = m_triangle.point[2].index;
										Dcontour_Set.at(0).is_begin = true; // 缓存边界边
									}
								}
								else
								{
									index1 = m_triangle.point[0].index;
									index2 = m_triangle.point[1].index;
									index3 = m_triangle.point[2].index;

								}

							}
							// 2，3坐标点所在的边存在高程值
							else if (analy3 < 0)
							{
								Dcontour contour1;
								x0 = m_triangle.point[1].x + (m_triangle.point[2].x - m_triangle.point[1].x)*(m_stress - stress2) / (stress3 - stress2);
								y0 = m_triangle.point[1].y + (m_triangle.point[2].y - m_triangle.point[1].y)*(m_stress - stress2) / (stress3 - stress2);
								H = m_stress;
								contour1.x = x0;
								contour1.y = y0;
								contour1.H = H;
								contour1.m_lines.push_back(CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y));
								contour1.m_lines.push_back(CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y));
								Dcontour_Set.push_back(contour1);

								Dcontour tep1,tep2;
								if (m_triangle.is_edge)
								{
									// 2,3点所在的边为边界边
									if (m_triangle.flag2)
									{
										tep1 = Dcontour_Set.at(0); // 缓存非边界边
										tep2 = Dcontour_Set.at(1); // 边界边
										// 获取等值线线头所在边界边的两个端点坐标
										CPoint2D p1, p2;
										p1 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
										p2 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
										tep2.m_lines.clear();
										tep2.m_lines.push_back(p1);
										tep2.m_lines.push_back(p2);
										// 标记当前点为等值线线头
										tep2.is_begin = true;
										Dcontour_Set.clear();
										Dcontour_Set.push_back(tep2);
										Dcontour_Set.push_back(tep1);

										index1 = m_triangle.point[0].index;
										index2 = m_triangle.point[2].index;
										index3 = m_triangle.point[1].index;

									}
									// 1,3点所在的边为边界边
									else
									{
										// 记录当前三角面片的输出边（由index1和index2决定）
										index1 = m_triangle.point[1].index;
										index2 = m_triangle.point[2].index;
										index3 = m_triangle.point[0].index;
										Dcontour_Set.at(0).is_begin = true; // 缓存边界边
									}
								}
								else
								{
									index2 = m_triangle.point[1].index;
									index1 = m_triangle.point[2].index;
									index3 = m_triangle.point[0].index;

								}

							}
						}
						else if (analy3 < 0) // 2，3坐标点所在的边存在高程值
						{
							counter = 1; // 修改状态
							Dcontour contour1;
							x0 = m_triangle.point[1].x + (m_triangle.point[2].x - m_triangle.point[1].x)*(m_stress - stress2) / (stress3 - stress2);
							y0 = m_triangle.point[1].y + (m_triangle.point[2].y - m_triangle.point[1].y)*(m_stress - stress2) / (stress3 - stress2);
							H = m_stress;
							contour1.x = x0;
							contour1.y = y0;
							contour1.H = H;
							contour1.m_lines.push_back(CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y));
							contour1.m_lines.push_back(CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y));
							Dcontour_Set.push_back(contour1);
							//m_triangle.is_used = true; // 标记以使用过的三角面片
							m_triangle.is_Original = true; // 标记起始三角面片
							triangle_Temp.at(i) = m_triangle; // 重新放回三角面片集合中

							// 1，2坐标点所在的边存在高程值
							if (analy1 < 0)
							{
								Dcontour contour2;
								x0 = m_triangle.point[0].x + (m_triangle.point[1].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress2 - stress1);
								y0 = m_triangle.point[0].y + (m_triangle.point[1].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress2 - stress1);
								H = m_stress;
								contour2.x = x0;
								contour2.y = y0;
								contour2.H = H;
								contour2.m_lines.push_back(CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y));
								contour2.m_lines.push_back(CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y));
								Dcontour_Set.push_back(contour2);

								Dcontour tep1, tep2;
								if (m_triangle.is_edge)
								{
									// 1,2点所在的边为边界边
									if (m_triangle.flag1)
									{
										tep1 = Dcontour_Set.at(0); // 缓存非边界边
										tep2 = Dcontour_Set.at(1); // 边界边
										// 获取等值线线头所在边界边的两个端点坐标
										CPoint2D p1, p2;
										p1 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
										p2 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
										tep2.m_lines.clear();
										tep2.m_lines.push_back(p1);
										tep2.m_lines.push_back(p2);
										// 标记当前点为等值线线头
										tep2.is_begin = true;
										Dcontour_Set.clear();
										Dcontour_Set.push_back(tep2);
										Dcontour_Set.push_back(tep1);

										index1 = m_triangle.point[1].index;
										index2 = m_triangle.point[2].index;
										index3 = m_triangle.point[0].index;

									}
									// 2,3点所在的边为边界边
									else
									{
										// 记录当前三角面片的输出边（由index1和index2决定）
										index1 = m_triangle.point[0].index;
										index2 = m_triangle.point[1].index;
										index3 = m_triangle.point[2].index;
										Dcontour_Set.at(0).is_begin = true; // 缓存边界边
									}
								}
								else
								{
									index1 = m_triangle.point[0].index;
									index2 = m_triangle.point[1].index;
									index3 = m_triangle.point[2].index;

								}

							}
							// 1，3坐标点所在的边存在高程值
							else if (analy2 < 0)
							{
								Dcontour contour2;
								x0 = m_triangle.point[0].x + (m_triangle.point[2].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress3 - stress1);
								y0 = m_triangle.point[0].y + (m_triangle.point[2].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress3 - stress1);
								H = m_stress;
								contour2.x = x0;
								contour2.y = y0;
								contour2.H = H;
								contour2.m_lines.push_back(CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y));
								contour2.m_lines.push_back(CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y));
								Dcontour_Set.push_back(contour2);

								Dcontour tep1,tep2;
								if (m_triangle.is_edge)
								{
									// 1,3点所在的边为边界边
									if (m_triangle.flag3)
									{
										tep1 = Dcontour_Set.at(0); // 缓存非边界边
										tep2 = Dcontour_Set.at(1); // 边界边
										// 获取等值线线头所在边界边的两个端点坐标
										CPoint2D p1, p2;
										p1 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
										p2 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
										tep2.m_lines.clear();
										tep2.m_lines.push_back(p1);
										tep2.m_lines.push_back(p2);
										// 标记当前点为等值线线头
										tep2.is_begin = true;
										Dcontour_Set.clear();
										Dcontour_Set.push_back(tep2);
										Dcontour_Set.push_back(tep1);

										index1 = m_triangle.point[1].index;
										index2 = m_triangle.point[2].index;
										index3 = m_triangle.point[0].index;

									}
									// 2,3点所在的边为边界边
									else
									{
										// 记录当前三角面片的输出边（由index1和index2决定）
										index1 = m_triangle.point[0].index;
										index2 = m_triangle.point[2].index;
										index3 = m_triangle.point[1].index;
										Dcontour_Set.at(0).is_begin = true; // 缓存边界边
									}
								}
								else
								{
									index1 = m_triangle.point[0].index;
									index2 = m_triangle.point[2].index;
									index3 = m_triangle.point[1].index;

								}

							}
						}


						// 从上边搜到的起始点开始继续往下搜索
						bool is_end = false;
						int flag = -1;
						while (1)
						{
							// 判断寻找是否结束
							if (is_end) break;

							int j = 0;
							// 从起始点往下搜索符合条件的等值线
							for (j = 0; j < triangle_Temp.size(); j++)
							{
								double x0, y0, H;
								double stress1, stress2, stress3;
								Trigon  m_triangle;
								m_triangle = triangle_Temp.at(j);

								if (flag == j)
								{
									continue;
								}

								// 防止闭等值线无限搜寻
								if (m_triangle.is_Original)
								{
									if (m_triangle.is_used)
									{
										continue;
									}
								}

								stress1 = m_triangle.point[0].stress;
								stress2 = m_triangle.point[1].stress;
								stress3 = m_triangle.point[2].stress;
								double analy1 = (m_stress - stress1)*(m_stress - stress2);
								double analy2 = (m_stress - stress1)*(m_stress - stress3);
								double analy3 = (m_stress - stress2)*(m_stress - stress3);

								if (m_triangle.point[0].index == index1) // 判断三角面片顶点下标是否是搜索目标
								{
									if (m_triangle.point[1].index == index2) // 找到了与当前三角面片共边的三角面片
									{
										// 搜寻当前三角面片的另一条含有高程值的边
										if (analy2 < 0) // 1，3坐标点所在的边存在高程值
										{
											Dcontour contour2;
											x0 = m_triangle.point[0].x + (m_triangle.point[2].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress3 - stress1);
											y0 = m_triangle.point[0].y + (m_triangle.point[2].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress3 - stress1);
											H = m_stress;
											contour2.x = x0;
											contour2.y = y0;
											contour2.H = H;

											// 获取当前高程值所在边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
											p4 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
											contour2.m_lines.push_back(p3);
											contour2.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour2);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);

											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中

											index1 = m_triangle.point[0].index;
											index2 = m_triangle.point[2].index;
											index3 = m_triangle.point[1].index;
											flag = j;
											break;
										}
										else if (analy3 < 0) // 2，3坐标点所在的边存在高程值
										{
											counter = 1; // 修改状态
											Dcontour contour1;
											x0 = m_triangle.point[1].x + (m_triangle.point[2].x - m_triangle.point[1].x)*(m_stress - stress2) / (stress3 - stress2);
											y0 = m_triangle.point[1].y + (m_triangle.point[2].y - m_triangle.point[1].y)*(m_stress - stress2) / (stress3 - stress2);
											H = m_stress;
											contour1.x = x0;
											contour1.y = y0;
											contour1.H = H;

											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
											p4 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
											contour1.m_lines.push_back(p3);
											contour1.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour1);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中
											index1 = m_triangle.point[1].index;
											index2 = m_triangle.point[2].index;
											index3 = m_triangle.point[0].index;
											flag = j;
											break;
										}
									}

									else if (m_triangle.point[2].index == index2)
									{
										// 搜寻当前三角面片的另一条含有高程值的边
										if (analy1 < 0) // 1，2坐标点所在的边存在高程值
										{
											Dcontour contour2;
											x0 = m_triangle.point[0].x + (m_triangle.point[1].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress2 - stress1);
											y0 = m_triangle.point[0].y + (m_triangle.point[1].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress2 - stress1);
											H = m_stress;
											contour2.x = x0;
											contour2.y = y0;
											contour2.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
											p4 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
											contour2.m_lines.push_back(p3);
											contour2.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour2);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中

											index1 = m_triangle.point[0].index;
											index2 = m_triangle.point[1].index;
											index3 = m_triangle.point[2].index;
											flag = j;
											break;
										}
										else if (analy3 < 0) // 2，3坐标点所在的边存在高程值
										{
											counter = 1; // 修改状态
											Dcontour contour1;
											x0 = m_triangle.point[1].x + (m_triangle.point[2].x - m_triangle.point[1].x)*(m_stress - stress2) / (stress3 - stress2);
											y0 = m_triangle.point[1].y + (m_triangle.point[2].y - m_triangle.point[1].y)*(m_stress - stress2) / (stress3 - stress2);
											H = m_stress;
											contour1.x = x0;
											contour1.y = y0;
											contour1.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
											p4 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
											contour1.m_lines.push_back(p3);
											contour1.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour1);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中
											index1 = m_triangle.point[1].index;
											index2 = m_triangle.point[2].index;
											index3 = m_triangle.point[0].index;
											flag = j;
											break;
										}
									}
								}

								else if (m_triangle.point[1].index == index1)
								{
									if (m_triangle.point[2].index == index2)
									{

										// 搜寻当前三角面片的另一条含有高程值的边
										if (analy1 < 0) // 1，2坐标点所在的边存在高程值
										{
											Dcontour contour2;
											x0 = m_triangle.point[0].x + (m_triangle.point[1].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress2 - stress1);
											y0 = m_triangle.point[0].y + (m_triangle.point[1].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress2 - stress1);
											H = m_stress;
											contour2.x = x0;
											contour2.y = y0;
											contour2.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
											p4 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
											contour2.m_lines.push_back(p3);
											contour2.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour2);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中

											index1 = m_triangle.point[0].index;
											index2 = m_triangle.point[1].index;
											flag = j;
											break;
										}
										else if (analy2 < 0) // 1，3坐标点所在的边存在高程值
										{
											Dcontour contour2;
											x0 = m_triangle.point[0].x + (m_triangle.point[2].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress3 - stress1);
											y0 = m_triangle.point[0].y + (m_triangle.point[2].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress3 - stress1);
											H = m_stress;
											contour2.x = x0;
											contour2.y = y0;
											contour2.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
											p4 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
											contour2.m_lines.push_back(p3);
											contour2.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour2);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中

											index1 = m_triangle.point[0].index;
											index2 = m_triangle.point[2].index;
											flag = j;
											break;
										}
									}
									else if (m_triangle.point[0].index == index2)
									{

										// 搜寻当前三角面片的另一条含有高程值的边
										if (analy2 < 0) // 1，3坐标点所在的边存在高程值
										{
											Dcontour contour2;
											x0 = m_triangle.point[0].x + (m_triangle.point[2].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress3 - stress1);
											y0 = m_triangle.point[0].y + (m_triangle.point[2].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress3 - stress1);
											H = m_stress;
											contour2.x = x0;
											contour2.y = y0;
											contour2.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
											p4 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
											contour2.m_lines.push_back(p3);
											contour2.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour2);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中

											index1 = m_triangle.point[0].index;
											index2 = m_triangle.point[2].index;
											flag = j;
											break;
										}
										else if (analy3 < 0) // 2，3坐标点所在的边存在高程值
										{
											counter = 1; // 修改状态
											Dcontour contour1;
											x0 = m_triangle.point[1].x + (m_triangle.point[2].x - m_triangle.point[1].x)*(m_stress - stress2) / (stress3 - stress2);
											y0 = m_triangle.point[1].y + (m_triangle.point[2].y - m_triangle.point[1].y)*(m_stress - stress2) / (stress3 - stress2);
											H = m_stress;
											contour1.x = x0;
											contour1.y = y0;
											contour1.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
											p4 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
											contour1.m_lines.push_back(p3);
											contour1.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour1);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中
											index1 = m_triangle.point[1].index;
											index2 = m_triangle.point[2].index;
											flag = j;
											break;
										}
									}
								}

								else if (m_triangle.point[2].index == index1)
								{
									if (m_triangle.point[1].index == index2)
									{
										// 搜寻当前三角面片的另一条含有高程值的边
										if (analy1 < 0) // 1，2坐标点所在的边存在高程值
										{
											Dcontour contour2;
											x0 = m_triangle.point[0].x + (m_triangle.point[1].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress2 - stress1);
											y0 = m_triangle.point[0].y + (m_triangle.point[1].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress2 - stress1);
											H = m_stress;
											contour2.x = x0;
											contour2.y = y0;
											contour2.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
											p4 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
											contour2.m_lines.push_back(p3);
											contour2.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour2);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中

											index1 = m_triangle.point[0].index;
											index2 = m_triangle.point[1].index;
											flag = j;
											break;
										}
										else if (analy2 < 0) // 1，3坐标点所在的边存在高程值
										{
											Dcontour contour2;
											x0 = m_triangle.point[0].x + (m_triangle.point[2].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress3 - stress1);
											y0 = m_triangle.point[0].y + (m_triangle.point[2].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress3 - stress1);
											H = m_stress;
											contour2.x = x0;
											contour2.y = y0;
											contour2.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
											p4 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
											contour2.m_lines.push_back(p3);
											contour2.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour2);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中

											index1 = m_triangle.point[0].index;
											index2 = m_triangle.point[2].index;
											flag = j;
											break;
										}
									}
									else if (m_triangle.point[0].index == index2)
									{

										// 搜寻当前三角面片的另一条含有高程值的边
										if (analy1 < 0) // 1，2坐标点所在的边存在高程值
										{
											Dcontour contour2;
											x0 = m_triangle.point[0].x + (m_triangle.point[1].x - m_triangle.point[0].x)*(m_stress - stress1) / (stress2 - stress1);
											y0 = m_triangle.point[0].y + (m_triangle.point[1].y - m_triangle.point[0].y)*(m_stress - stress1) / (stress2 - stress1);
											H = m_stress;
											contour2.x = x0;
											contour2.y = y0;
											contour2.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
											p4 = CPoint2D(m_triangle.point[0].x, m_triangle.point[0].y);
											contour2.m_lines.push_back(p3);
											contour2.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour2);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中

											index1 = m_triangle.point[0].index;
											index2 = m_triangle.point[1].index;
											flag = j;
											break;
										}
										else if (analy3 < 0) // 2，3坐标点所在的边存在高程值
										{
											counter = 1; // 修改状态
											Dcontour contour1;
											x0 = m_triangle.point[1].x + (m_triangle.point[2].x - m_triangle.point[1].x)*(m_stress - stress2) / (stress3 - stress2);
											y0 = m_triangle.point[1].y + (m_triangle.point[2].y - m_triangle.point[1].y)*(m_stress - stress2) / (stress3 - stress2);
											H = m_stress;
											contour1.x = x0;
											contour1.y = y0;
											contour1.H = H;
											// 获取等值线线头所在边界边的两个端点坐标
											CPoint2D p3, p4;
											p3 = CPoint2D(m_triangle.point[1].x, m_triangle.point[1].y);
											p4 = CPoint2D(m_triangle.point[2].x, m_triangle.point[2].y);
											contour1.m_lines.push_back(p3);
											contour1.m_lines.push_back(p4);

											Dcontour_Set.push_back(contour1);
											// 删除使用过的三角面片
											//triangle_Temp.erase(triangle_Temp.begin() + j);
											m_triangle.is_used = true; // 标记以使用过的三角面片
											triangle_Temp.at(j) = m_triangle; // 重新放回三角面片集合中
											index1 = m_triangle.point[1].index;
											index2 = m_triangle.point[2].index;
											flag = j;
											break;
										}
									}
								}

							}

							// 判断是否搜索到底，若是则跳出循环，搜索下一条含有该高程值的等值线
							if (j == triangle_Temp.size())
							{
								// 判断此等值线的第一个元素是否为线头元素，若是这说明这是条非封闭等值线，必存在线尾
								if (Dcontour_Set.at(0).is_begin)
								{
									Dcontour tep = Dcontour_Set.back();
									
									// 标记当前点为等值线线尾
									tep.is_end = true;
									Dcontour_Set.pop_back();
									Dcontour_Set.push_back(tep);
								}
								// 结束循环
								is_end = true;
							}
						}
					}
					else // 
					{
						break;
					}

				}

				// 保存第k阶段的所有等值线
				if (Dcontour_Set.size() != 0)
				{
					Dcontour_SetM.push_back(Dcontour_Set);
				}
				
			}
		}
		// 保存所有阶段的等值线
		Dcontour_TotalSet.push_back(Dcontour_SetM);
	}




	 //扫描边缘线，获取封闭区域和非封闭区域
	for (int k = 0; k < BorderLine_Set.size(); k++)
	{
		vector<CPoint2D> edgeCurve = BorderLine_Set.at(k);   // 用于获取第k阶段的边缘曲线
		vector<vector<CPoint3D>> NonClosedRegion_Set_Temp;
		vector<vector<CPoint3D>> ClosedRegion_Set_Temp;
		// 加入非封闭区域，沿着一个固定方向搜索，在当前边界线上的非封闭等值线
		for (int i = 1; i < edgeCurve.size(); i++)
		{
			CPoint2D p1 = edgeCurve.at(i - 1);
			CPoint2D p2 = edgeCurve.at(i);
			// 循环所有的等值线，搜索符合条件的等值线
			for (int j = 0; j < Dcontour_TotalSet.at(k).size(); j++)
			{
				vector<Dcontour> Dcontour_Set;     // 创建等值线对象
				Dcontour_Set = Dcontour_TotalSet.at(k).at(j);
				if (Dcontour_Set.size() == 0)
				{
					continue;
				}
				// 非封闭等值线
				if (Dcontour_Set.at(0).is_begin)
				{
					CPoint2D p3 = Dcontour_Set.at(0).m_lines.at(0);
					CPoint2D p4 = Dcontour_Set.at(0).m_lines.at(1);
					if ((p1 == p3 && p2 == p4) || (p1 == p4 && p2 == p3))
					{
						// 创建非封闭区域对象
						vector<CPoint3D>  NonClosedRegion; 
						// 循环加入当前非封闭等值线的坐标点和H值
						int m_capacity = Dcontour_Set.size();
						for (int m = 0; m < Dcontour_Set.size(); m++)
						{
							CPoint3D temp = CPoint3D(Dcontour_Set.at(m).x, Dcontour_Set.at(m).y, Dcontour_Set.at(m).H);
							NonClosedRegion.push_back(temp);
						}
						// 确定线尾所在边缘线的位置并加入当前等值线与边缘线的公共部分
						CPoint2D tep1 = Dcontour_Set.back().m_lines.at(0);
						CPoint2D tep2 = Dcontour_Set.back().m_lines.at(1);
						for (int m = 1; m < edgeCurve.size(); m++)
						{
							CPoint2D tep3 = edgeCurve.at(m - 1);
							CPoint2D tep4 = edgeCurve.at(m);
							CPoint2D tep5 = CPoint2D(0.00001, 0.00001);
							if (m == edgeCurve.size()-1)
							{
								CPoint2D tep5 = edgeCurve.at(0);
							}
							// 找到了线尾
							if ((tep1 == tep3 && tep2 == tep4) || (tep1 == tep4 && tep2 == tep3))
							{
								// 整个非封闭区域在一个三角面片单边界边上
								if ((p1 == tep1)&&(p2 == tep2) || (p1 == tep2) && (p2 == tep1))
								{
									break;
								}
								if (Dcontour_Set.size() == 2)
								{
									if (m>i)
									{
										CPoint3D temp = CPoint3D(edgeCurve.at(i).x, edgeCurve.at(i).y, Dcontour_Set.at(0).H);
										NonClosedRegion.push_back(temp);
									}
									else
									{
										CPoint3D temp = CPoint3D(edgeCurve.at(m).x, edgeCurve.at(m).y, Dcontour_Set.at(0).H);
										NonClosedRegion.push_back(temp);
									}

									break;
								}

								// 获取凸多边形区域（排除凹多边形的干扰）
								// 第一种情况
								if (m > i)
								{
									// 线头和线尾跨整个边缘线的距离小于1/2
									if ((m-i)*1.0/ edgeCurve.size() < 0.5)
									{
										// 直接获取与非封闭区域公共部分的边缘曲线
										vector< int> many;// 初始化
										int capacity_Child = 0;
										bool err_Dire = false;
										bool startPoc = false;
										for (int n = m; n > i; n--)
										{
											CPoint2D p1 = edgeCurve.at(n);
											CPoint2D p2 = edgeCurve.at(n-1);
											// 循环所有的等值线，搜索符合条件的等值线
											int g = 0;
											for (g = 0; g < Dcontour_TotalSet.at(k).size(); g++)
											{
												if (g == j)  // 如果当前搜索出的等值线是前面刚扫描到的第j条等值线则直接跳过
												{
													continue;
												}
												int w = 0;
												for (w = 0; w < many.size(); w++)
												{
													if (g == many.at(w))
													{
														break;
													}
												}
												if (w != many.size())
												{
													continue;
												}

												vector<Dcontour> Dcontour_Set;     // 创建等值线对象
												Dcontour_Set = Dcontour_TotalSet.at(k).at(g);
												// 判断搜索方向，若错误则跳过当前
												if (m_capacity < Dcontour_Set.size())
												{
													continue;
												}
												// 非封闭等值线
												if (Dcontour_Set.at(0).is_begin)
												{
													CPoint2D p3 = Dcontour_Set.at(0).m_lines.at(0);
													CPoint2D p4 = Dcontour_Set.at(0).m_lines.at(1);
													CPoint2D p5 = Dcontour_Set.back().m_lines.at(0);
													CPoint2D p6 = Dcontour_Set.back().m_lines.at(1);
													// 遇到线头
													if ((p1 == p3 && p2 == p4) || (p1 == p4 && p2 == p3))
													{
														// 判断搜索方向，若错误，则跳出

														//if (startPoc)
														//{
														//	if (capacity_Child >= Dcontour_Set.size())
														//	{
														//		err_Dire = true;
														//	}
														//	else
														//	{
														//		err_Dire = false;
														//	}
														//	if (err_Dire)
														//	{
														//		continue;
														//	}
														//}

														// 正向加入
														for (int h = 0; h < Dcontour_Set.size(); h++)
														{
															CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
															NonClosedRegion.push_back(temp);
														}
														// 搜索线尾
														for (int e = 1; e < edgeCurve.size(); e++)
														{
															CPoint2D tep3 = edgeCurve.at(e - 1);
															CPoint2D tep4 = edgeCurve.at(e);
															if ((p5 == tep3 && p6 == tep4) || (p5 == tep4 && p6 == tep3))
															{
																if (e > i)
																{
																	n = e+1; // 更新边缘线集合下标搜索点
																	many.push_back(g); //要判断当前边界边是否还存在其他等值线

																	capacity_Child = Dcontour_Set.size();
																	startPoc = true;
																
																}
																else if (e == i)
																{
																	n = i- 1;
																}
																break;
															}
														}
														break;
													}
													// 遇到线尾
													else if ((p1 == p5 && p2 == p6) || (p1 == p6 && p2 == p5))
													{

														// 判断搜索方向，若错误，则跳出

														//if (startPoc)
														//{
														//	if (capacity_Child >= Dcontour_Set.size())
														//	{
														//		err_Dire = true;
														//	}
														//	else
														//	{
														//		err_Dire = false;
														//	}
														//	if (err_Dire)
														//	{
														//		continue;
														//	}
														//}

														// 反向加入
														for (int h = Dcontour_Set.size()-1; h >= 0; h--)
														{
															CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
															NonClosedRegion.push_back(temp);
														}
														// 搜索线头
														for (int e = 1; e < edgeCurve.size(); e++)
														{
															CPoint2D tep3 = edgeCurve.at(e - 1);
															CPoint2D tep4 = edgeCurve.at(e);
															if ((p3 == tep3 && p4 == tep4) || (p3 == tep4 && p4 == tep3))
															{
																if (e > i)
																{
																	n = e + 1; // 更新边缘线集合下标搜索点，
																	many.push_back(g); //要判断当前边界边是否还存在其他等值线

																	capacity_Child = Dcontour_Set.size();
																	startPoc = true;

																}
																else if (e == i)
																{
																	n = i - 1;
																}
																break;
															}
														}
														break;
													}
													else
													{
														continue;
													}
												}
											}

											// 
											if (n < i)
											{
												break;
											}
											// 未遇到等值线
											if (g == Dcontour_TotalSet.at(k).size())
											{
												if (n == i)
												{
													continue;
												}
												else if (n >i)
												{
													CPoint3D temp = CPoint3D(edgeCurve.at(n - 1).x, edgeCurve.at(n - 1).y, Dcontour_Set.back().H);
													NonClosedRegion.push_back(temp);
												}

											}
											else
											{
												continue;
											}
										}
									}
									// 线头和线尾跨整个边缘线的距离大于1/2
									else
									{
										int r_index = i +1;
										// 反向获取与非封闭区域公共部分的边缘曲线
										vector<int> many; // 初始化
										for (int n = m; n < edgeCurve.size(); n++)
										{
											CPoint2D p1 = edgeCurve.at(n);
											CPoint2D p2 = edgeCurve.at(n - 1);
											CPoint2D p7 = CPoint2D(0.00001, 0.00001);
											if (n == edgeCurve.size() - 1)
											{
												CPoint2D p7 = edgeCurve.at(0);
											}

											// 循环所有的等值线，搜索符合条件的等值线
											int g = 0;
											for (g = 0; g < Dcontour_TotalSet.at(k).size(); g++)
											{
												if (g == j)  // 如果当前搜索出的等值线是前面刚扫描到的第j条等值线则直接跳过
												{ 
													continue;
												}
												int w = 0;
												for (w = 0; w < many.size(); w++)
												{
													if (g == many.at(w))
													{
														break;
													}
												}
												if (w != many.size())
												{
													continue;
												}

												vector<Dcontour> Dcontour_Set;     // 创建等值线对象
												Dcontour_Set = Dcontour_TotalSet.at(k).at(g);
												// 非封闭等值线
												if (Dcontour_Set.at(0).is_begin)
												{
													CPoint2D p3 = Dcontour_Set.at(0).m_lines.at(0);
													CPoint2D p4 = Dcontour_Set.at(0).m_lines.at(1);
													CPoint2D p5 = Dcontour_Set.back().m_lines.at(0);
													CPoint2D p6 = Dcontour_Set.back().m_lines.at(1);

													// 在接口位置搜索防止漏搜
													if (n == edgeCurve.size() - 1)
													{
														// 遇到线头
														if ((p1 == p3 && p7 == p4) || (p1 == p4 && p7 == p3))
														{
															// 正向加入
															for (int h = 0; h < Dcontour_Set.size(); h++)
															{
																CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
																NonClosedRegion.push_back(temp);
															}
															// 搜索线尾
															for (int e = 1; e < edgeCurve.size(); e++)
															{
																CPoint2D tep3 = edgeCurve.at(e - 1);
																CPoint2D tep4 = edgeCurve.at(e);
																if ((p5 == tep3 && p6 == tep4) || (p5 == tep4 && p6 == tep3))
																{
																	if (e < i)
																	{
																		r_index = e; // 更新边缘线集合下标搜索点
																	}
																	else if (e > m && e < edgeCurve.size())
																	{
																		n = e - 2; // 更新边缘线集合下标搜索点，
																		many.push_back(g); //要判断当前边界边是否还存在其他等值线
																	}
																	break;
																}
															}
															break;
														}
														// 遇到线尾
														else if ((p1 == p5 && p7 == p6) || (p1 == p6 && p7 == p5))
														{
															// 反向加入
															for (int h = Dcontour_Set.size() - 1; h >= 0; h--)
															{
																CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
																NonClosedRegion.push_back(temp);
															}
															// 搜索线头
															for (int e = 1; e < edgeCurve.size(); e++)
															{
																CPoint2D tep3 = edgeCurve.at(e - 1);
																CPoint2D tep4 = edgeCurve.at(e);
																if ((p3 == tep3 && p4 == tep4) || (p3 == tep4 && p4 == tep3))
																{
																	if (e < i)
																	{
																		r_index = e; // 更新边缘线集合下标搜索点
																	}
																	else if (e > m && e < edgeCurve.size())
																	{
																		n = e - 2; // 更新边缘线集合下标搜索点
																		many.push_back(g); //要判断当前边界边是否还存在其他等值线
																	}
																	break;
																}
															}
															break;
														}

													}

													// 遇到线头
													if ((p1 == p3 && p2 == p4) || (p1 == p4 && p2 == p3))
													{
														// 正向加入
														for (int h = 0; h < Dcontour_Set.size(); h++)
														{
															CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
															NonClosedRegion.push_back(temp);
														}
														// 搜索线尾
														for (int e = 1; e < edgeCurve.size(); e++)
														{
															CPoint2D tep3 = edgeCurve.at(e - 1);
															CPoint2D tep4 = edgeCurve.at(e);
															if ((p5 == tep3 && p6 == tep4) || (p5 == tep4 && p6 == tep3))
															{
																if (e < i)
																{
																	r_index = e; // 更新边缘线集合下标搜索点
																}
																else if (e > m && e < edgeCurve.size())
																{
																	n = e - 2; // 更新边缘线集合下标搜索点，
																	many.push_back(g); //要判断当前边界边是否还存在其他等值线
																}
																break;
															}
														}
														break;
													}
													// 遇到线尾
													else if ((p1 == p5 && p2 == p6) || (p1 == p6 && p2 == p5))
													{
														// 反向加入
														for (int h = Dcontour_Set.size() - 1; h >= 0; h--)
														{
															CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
															NonClosedRegion.push_back(temp);
														}
														// 搜索线头
														for (int e = 1; e < edgeCurve.size(); e++)
														{
															CPoint2D tep3 = edgeCurve.at(e - 1);
															CPoint2D tep4 = edgeCurve.at(e);
															if ((p3 == tep3 && p4 == tep4) || (p3 == tep4 && p4 == tep3))
															{
																if (e < i)
																{
																	r_index = e; // 更新边缘线集合下标搜索点
																}
																else if (e > m && e < edgeCurve.size())
																{
																	n = e - 2; // 更新边缘线集合下标搜索点
																	many.push_back(g); //要判断当前边界边是否还存在其他等值线
																}
																break;
															}
														}
														break;
													}
													else
													{
														continue;
													}
												}
											}
											if (r_index <= i) // 如果成立就跳出当前循环
											{
												break;
											}
											// 未遇到等值线
											if (g == Dcontour_TotalSet.at(k).size())
											{
													CPoint3D temp = CPoint3D(edgeCurve.at(n).x, edgeCurve.at(n).y, Dcontour_Set.back().H);
													NonClosedRegion.push_back(temp);
											}
											
											else
											{
												continue;
											}
										}
										if (r_index < i)
										{
											for (int r = r_index; r < i; r++)
											{
												CPoint3D temp = CPoint3D(edgeCurve.at(r).x, edgeCurve.at(r).y, Dcontour_Set.back().H);
												NonClosedRegion.push_back(temp);
											}
										}
										else
										{
											for (int r = 0; r < i; r++)
											{
												CPoint3D temp = CPoint3D(edgeCurve.at(r).x, edgeCurve.at(r).y, Dcontour_Set.back().H);
												NonClosedRegion.push_back(temp);
											}
										}
										
									}
								}
								// 第二种情况
								else if (m <= i)
								{
									// 线头和线尾跨整个边缘线的距离小于1/2
									if ((i-m)*1.0 / edgeCurve.size() < 0.5)
									{
										// 直接获取与非封闭区域公共部分的边缘曲线
										vector<int> many; // 初始化
										for (int n = m; n < i; n++)
										{
											CPoint2D p1 = edgeCurve.at(n);
											CPoint2D p2 = edgeCurve.at(n - 1);
											// 循环所有的等值线，搜索符合条件的等值线
											int g = 0;
											for (g = 0; g < Dcontour_TotalSet.at(k).size(); g++)
											{
												if (g == j)  // 如果当前搜索出的等值线是前面刚扫描到的第j条等值线则直接跳过
												{
													continue;
												}
												int w = 0;
												for (w = 0; w < many.size(); w++)
												{
													if (g == many.at(w))
													{
														break;
													}
												}
												if (w != many.size())
												{
													continue;
												}

												vector<Dcontour> Dcontour_Set;     // 创建等值线对象
												Dcontour_Set = Dcontour_TotalSet.at(k).at(g);

												// 判断搜索方向，若错误则跳过当前
												if (m_capacity < Dcontour_Set.size())
												{
													continue;
												}
												// 非封闭等值线
												if (Dcontour_Set.at(0).is_begin)
												{

													CPoint2D p3 = Dcontour_Set.at(0).m_lines.at(0);
													CPoint2D p4 = Dcontour_Set.at(0).m_lines.at(1);
													CPoint2D p5 = Dcontour_Set.back().m_lines.at(0);
													CPoint2D p6 = Dcontour_Set.back().m_lines.at(1);
													// 遇到线头
													if ((p1 == p3 && p2 == p4) || (p1 == p4 && p2 == p3))
													{
	
														// 正向加入
														for (int h = 0; h < Dcontour_Set.size(); h++)
														{
															CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
															NonClosedRegion.push_back(temp);
														}
														// 搜索线尾
														for (int e = 1; e < edgeCurve.size(); e++)
														{
															CPoint2D tep3 = edgeCurve.at(e - 1);
															CPoint2D tep4 = edgeCurve.at(e);
															if ((p5 == tep3 && p6 == tep4) || (p5 == tep4 && p6 == tep3))
															{
																if (e < i)
																{
																	n = e-1; // 更新边缘线集合下标搜索点
																	many.push_back(g);
																}
																else if(e == i)
																{
																	n = i + 1;
																}
																break;
															}
														}
														break;
													}
													// 遇到线尾
													else if ((p1 == p5 && p2 == p6) || (p1 == p6 && p2 == p5))
													{
														// 反向加入
														for (int h = Dcontour_Set.size() - 1; h >= 0; h--)
														{
															CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
															NonClosedRegion.push_back(temp);
														}
														// 搜索线头
														for (int e = 1; e < edgeCurve.size(); e++)
														{
															CPoint2D tep3 = edgeCurve.at(e - 1);
															CPoint2D tep4 = edgeCurve.at(e);
															if ((p3 == tep3 && p4 == tep4) || (p3 == tep4 && p4 == tep3))
															{
																if (e < i)
																{
																	n = e-1; // 更新边缘线集合下标搜索点
																	many.push_back(g);
																}

																else if (e == i)
																{
																	n = i + 1;
																}
																break;
															}
														}
														break;
													}
													else
													{
														continue;
													}
												}
											}
											// 未搜索到等值线
											if (n > i)
											{
												break;
											}
											if (g == Dcontour_TotalSet.at(k).size())
											{
												if (n < i)
												{
													CPoint3D temp = CPoint3D(edgeCurve.at(n).x, edgeCurve.at(n).y, Dcontour_Set.back().H);
													NonClosedRegion.push_back(temp);
												}
												else if(n == i)
												{
													continue;
												}

											}
											else
											{
												continue;
											}
										}
									}
									// 线头和线尾跨整个边缘线的距离大于1/2
									else
									{
										// 反向获取与非封闭区域公共部分的边缘曲线
										int r_index = m;
										vector<int> many; // 初始化
										for (int n = m; n > 0; n--)
										{
											CPoint2D p1 = edgeCurve.at(n-1);
											CPoint2D p2 = edgeCurve.at(n);
											// 循环所有的等值线，搜索符合条件的等值线
											int g = 0;
											for (g = 0; g < Dcontour_TotalSet.at(k).size(); g++)
											{
												// 如果当前搜索出的等值线是前面刚扫描到的第j条等值线则直接跳过
												if (g == j)  
												{
													continue;
												}
												int w = 0;
												for (w = 0; w < many.size(); w++)
												{
													if (g == many.at(w))
													{
														break;
													}
												}
												if (w != many.size())
												{
													continue;
												}

												vector<Dcontour> Dcontour_Set;     // 创建等值线对象
												Dcontour_Set = Dcontour_TotalSet.at(k).at(g);
												// 非封闭等值线
												if (Dcontour_Set.at(0).is_begin)
												{
													CPoint2D p3 = Dcontour_Set.at(0).m_lines.at(0);
													CPoint2D p4 = Dcontour_Set.at(0).m_lines.at(1);
													CPoint2D p5 = Dcontour_Set.back().m_lines.at(0);
													CPoint2D p6 = Dcontour_Set.back().m_lines.at(1);
													// 遇到线头
													if ((p1 == p3 && p2 == p4) || (p1 == p4 && p2 == p3))
													{
														// 正向加入
														for (int h = 0; h < Dcontour_Set.size(); h++)
														{
															CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
															NonClosedRegion.push_back(temp);
														}
														// 搜索线尾
														for (int e = 1; e < edgeCurve.size(); e++)
														{
															CPoint2D tep3 = edgeCurve.at(e - 1);
															CPoint2D tep4 = edgeCurve.at(e);
															if ((p5 == tep3 && p6 == tep4) || (p5 == tep4 && p6 == tep3))
															{
																if (e > i && e < edgeCurve.size())
																{
																	r_index = e-1; // 更新边缘线集合下标搜索点
																}
																else if (e < m && e >= 0)
																{
																	n = e+1; // 更新边缘线集合下标搜索点
																	many.push_back(g);
																}
																break;
															}
														}
														break;
													}
													// 遇到线尾
													else if ((p1 == p5 && p2 == p6) || (p1 == p6 && p2 == p5))
													{
														// 反向加入
														for (int h = Dcontour_Set.size() - 1; h >= 0; h--)
														{
															CPoint3D temp = CPoint3D(Dcontour_Set.at(h).x, Dcontour_Set.at(h).y, Dcontour_Set.at(h).H);
															NonClosedRegion.push_back(temp);
														}
														// 搜索线头
														for (int e = 1; e < edgeCurve.size(); e++)
														{
															CPoint2D tep3 = edgeCurve.at(e - 1);
															CPoint2D tep4 = edgeCurve.at(e);
															if ((p3 == tep3 && p4 == tep4) || (p3 == tep4 && p4 == tep3))
															{
																if (e > i && e < edgeCurve.size())
																{
																	r_index = e-1; // 更新边缘线集合下标搜索点
																}
																else if (e < m && e >= 0)
																{
																	n = e + 1; // 更新边缘线集合下标搜索点
																	many.push_back(g);
																}
																break;
															}
														}
														break;
													}
													else
													{
														continue;
													}
												}
											}
											if (r_index > i)
											{
												break;
											}
											// 未遇到等值线
											if (g == Dcontour_TotalSet.at(k).size())
											{

												CPoint3D temp = CPoint3D(edgeCurve.at(n-1).x, edgeCurve.at(n-1).y, Dcontour_Set.back().H);
												NonClosedRegion.push_back(temp);
											}
											else
											{
												continue;
											}
										}
										if (r_index > i)
										{
											for (int r = r_index; r >= i; r--)
											{
												CPoint3D temp = CPoint3D(edgeCurve.at(r).x, edgeCurve.at(r).y, Dcontour_Set.back().H);
												NonClosedRegion.push_back(temp);
											}
										}
										else
										{
											for (int r = edgeCurve.size()-1; r >= i; r--)
											{
												CPoint3D temp = CPoint3D(edgeCurve.at(r).x, edgeCurve.at(r).y, Dcontour_Set.back().H);
												NonClosedRegion.push_back(temp);
											}
										}
									}
								}

								break;
							}
							else if ((tep1 == tep5 && tep2 == tep4) || (tep1 == tep4 && tep2 == tep5))
							{
								if (Dcontour_Set.size() == 2)
								{
									CPoint3D temp = CPoint3D(edgeCurve.at(0).x, edgeCurve.at(0).y, Dcontour_Set.at(0).H);
									NonClosedRegion.push_back(temp);
									break;
								}
								break;
							}


						}
						// 加入非封闭曲线区域
						NonClosedRegion_Set_Temp.push_back(NonClosedRegion);
						break;
					}
				}
			}
		}

		// 加入封闭区域
		for (int j = 0; j < Dcontour_TotalSet.at(k).size(); j++)
		{
			vector<Dcontour> Dcontour_Set;     // 创建等值线对象
			Dcontour_Set = Dcontour_TotalSet.at(k).at(j);
			if (Dcontour_Set.size() == 0)
			{
				continue;
			}

			// 加入封闭等值线组成的封闭区域
			if (!Dcontour_Set.at(0).is_begin)
			{
				vector<CPoint3D>  ClosedRegion; // 创建封闭区域对象
				// 循环加入当前等值线的坐标点和H值
				for (int m = 0; m < Dcontour_Set.size(); m++)
				{
					CPoint3D temp = CPoint3D(Dcontour_Set.at(m).x, Dcontour_Set.at(m).y, Dcontour_Set.at(m).H);
					ClosedRegion.push_back(temp);
				}
				// 加入封闭曲线区域
				ClosedRegion_Set_Temp.push_back(ClosedRegion);
			}
		}
		
		NonClosedRegion_Set.push_back(NonClosedRegion_Set_Temp);   // 保存非封闭区域
		ClosedRegion_Set.push_back(ClosedRegion_Set_Temp);					// 保存封闭区域
	}
	
	// 循环非封闭区域的所有z值即高程值

	//for (int i = 0; i < NonClosedRegion_Set.size(); i++)
	//{
	//	for (int j = 0; j < NonClosedRegion_Set.at(i).size(); j++)
	//	{
	//		// 获取rank值
	//		double H = NonClosedRegion_Set.at(i).at(j).at(0).z;
	//		for (int k = 0; k < NonClosedRegion_Set.at(i).at(j).size(); k++)
	//		{
	//			// 修改rank值，（按照高程值与颜色表之间的对应关系进行设计）
	//			NonClosedRegion_Set.at(i).at(j).at(k).z = (H - stress_Min.at(i)) / delta_Set.at(i);
	//		}
	//	}
	//}

	// 循环封闭区域的所有z值即高程值

	//for (int i = 0; i < ClosedRegion_Set.size(); i++)
	//{
	//	for (int j = 0; j < ClosedRegion_Set.at(i).size(); j++)
	//	{
	//		// 获取rank值
	//		double H = ClosedRegion_Set.at(i).at(j).at(0).z;
	//		for (int k = 0; k < ClosedRegion_Set.at(i).at(j).size(); k++)
	//		{
	//			// 修改rank值，（按照高程值与颜色表之间的对应关系进行设计）
	//			ClosedRegion_Set.at(i).at(j).at(k).z = (H - stress_Min.at(i)) / delta_Set.at(i);
	//		}
	//	}
	//}


	// 利用扫面线法计算区域

	 //循环每个阶段的等值线集合
	//for (int i = 0; i < Dcontour_TotalSet.size(); i++)
	//{
	//	// 循环第i阶段的所有等值线集合
	//	for (int j = 0; j < Dcontour_TotalSet.at(i).size(); j++)
	//	{
	//		// 获取第j条等值线
	//		vector<Dcontour> contour = Dcontour_TotalSet.at(i).at(j);
	//		// 当前等值线是非封闭曲线
	//		if (contour.at(0).is_begin)
	//		{
	//		}
	//		// 循环当前等值线坐标，寻找交点坐标
	//		for (int k = 1; k < contour.size(); k++)
	//		{
	//		}
	//	}
	//}

	// 循环每个阶段，对每个阶段的所有等值线进行扫描
	//for (int i = 0; i < Dcontour_TotalSet.size(); i++)
	//{
	//	vector<Scanner> Scanner_Set; // 创建扫描线集合
	//	// 自上而下用扫描线对模型进行扫描
	//	for (int h = int(Min_y.at(i)); h < int(Max_y.at(i)); h++)
	//	{
	//		Scanner scanLine; // 创建一条水平扫描线
	//		scanLine.y = h;       // 记录当前扫描线的纵坐标 
	//		// 循环第i阶段的所有等值线集合，记录扫描线与每条等值线相交的交点横坐标和当前的高程值
	//		for (int j = 0; j < Dcontour_TotalSet.at(i).size(); j++)
	//		{
	//			// 获取第j条等值线
	//			vector<Dcontour> contour = Dcontour_TotalSet.at(i).at(j);
	//			// 循环当前等值线坐标，寻找交点坐标
	//			for (int k = 1; k < contour.size(); k++)
	//			{
	//				double x1,y1, x2,y2;
	//				x1 = contour.at(k - 1).x;
	//				y1 = contour.at(k - 1).y;
	//				x2 = contour.at(k).x;
	//				y2 = contour.at(k).y;
	//				double judge = (h - y1)*(h - y2);
	//				if (judge < 0)            // 第k-1点与k点之间存在交点
	//				{
	//					double temp_x = x1 + (x2 - x1)*(h - y1) / (y2 - y1)*1.0;  // 交点横坐标
	//					scanLine.x_Set.push_back(temp_x);                                  // 记录交点横坐标
	//					scanLine.rank.push_back(contour.at(k).H);					    // 记录当前等值线的高程值
	//				}
	//			}
	//		}
	//		scanLine.x_Set.push_back(Min_x.at(i));
	//		scanLine.rank.push_back(0);
	//		scanLine.x_Set.push_back(Max_x.at(i));
	//		scanLine.rank.push_back(0);
	//		// 保存当前水平扫描线
	//		Scanner_Set.push_back(scanLine);
	//	}
	//	// 保存每个阶段的扫描线集合
	//	Scanner_TotalSet.push_back(Scanner_Set);
	//}
	//// 对每条扫描线上的交点的横坐标进行排序（循环每个阶段扫描线集合）
	//for (int i = 0; i < Scanner_TotalSet.size(); i++)
	//{
	//	// 循环第i阶段的扫描线集合
	//	for (int j = 0; j < Scanner_TotalSet.at(i).size(); j++)
	//	{
	//		// 对第j条扫描线冒泡排序
	//		int len = Scanner_TotalSet.at(i).at(j).x_Set.size(); // 获取交点个数
	//		for (int m = 0; m < len; m++) {
	//			//比较两个相邻的元素   
	//			for (int n = 0; n < len - m - 1; n++) {
	//				double x1 = Scanner_TotalSet.at(i).at(j).x_Set.at(n);
	//				double H1 = Scanner_TotalSet.at(i).at(j).rank.at(n);
	//				double x2 = Scanner_TotalSet.at(i).at(j).x_Set.at(n+1);
	//				double H2 = Scanner_TotalSet.at(i).at(j).rank.at(n+1);
	//				if (x1 > x2) {
	//					double temp_x = x1;
	//					double temp_H = H1;
	//					Scanner_TotalSet.at(i).at(j).x_Set.at(n) = x2;
	//					Scanner_TotalSet.at(i).at(j).rank.at(n) = H2;
	//					Scanner_TotalSet.at(i).at(j).x_Set.at(n + 1) = temp_x;
	//					Scanner_TotalSet.at(i).at(j).rank.at(n + 1) = temp_H;
	//				}
	//			}
	//		}
	//		// 循环第j条扫描线上的所有rank值
	//		for (int k = 0; k < Scanner_TotalSet.at(i).at(j).rank.size(); k++)
	//		{
	//			// 获取rank值
	//			double H = Scanner_TotalSet.at(i).at(j).rank.at(k);
	//			// 修改rank值，（按照高程值与颜色表之间的对应关系进行设计）
	//			Scanner_TotalSet.at(i).at(j).rank.at(k) = (H - stress_Min.at(i)) / delta_Set.at(i);
	//		}
	//	}
	//}

	return true;
}


void CFeatureReuseDoc::DrawModel(COpenGLDC *pDC)
{
	//int n=meshes.size();
	//if (n<1)
	//{
	//	return;
	//}

	// 此处代码用于每个模型对象的鼠标拖动旋转和缩放使用
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(m_xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(m_yAngle, 0.0f, 1.0f, 0.0f);
	glScalef(m_Scale, m_Scale, m_Scale);

	// 采用默认绘制模式
	if (m_Mode == 0) 
	{

		for (int i = 0; i < meshes.size(); i++)
		{
			TriMesh * Tmesh = meshes[i];
			int fn = Tmesh->faces.size();
			for (int j = 0; j < fn; j++)
			{
				int a, b, c;
				a = Tmesh->faces[j].v[0];
				b = Tmesh->faces[j].v[1];
				c = Tmesh->faces[j].v[2];
				pDC->DrawTriChip(Tmesh->FaceNormal[j][0], Tmesh->FaceNormal[j][1], Tmesh->FaceNormal[j][2],
					Tmesh->vertices[a][0], Tmesh->vertices[a][1], Tmesh->vertices[a][2],
					Tmesh->vertices[b][0], Tmesh->vertices[b][1], Tmesh->vertices[b][2],
					Tmesh->vertices[c][0], Tmesh->vertices[c][1], Tmesh->vertices[c][2], false);
			}
		}
	}

	// 采用绘制模式1，即绘制三角面
	else if (m_Mode == 1)
	{
		// 遍历容器，即遍历每一个三角片面对象
		for (auto iter = m_TriList.cbegin(); iter != m_TriList.cend(); iter++)
		{

				//TRACE(_T("调试信息：绘制模型1111默认绘制模型111！！！！！!"));
				pDC->DrawTriChip(
					iter->tri[0].x, iter->tri[0].y, iter->tri[0].z,
					iter->tri[1].x, iter->tri[1].y, iter->tri[1].z,
					iter->tri[2].x, iter->tri[2].y, iter->tri[2].z);
		}

		pDC->Shading(FALSE); // 关闭OpenGL的着色处理

	}

	// 采用绘制模式2，即绘制四面体
	else if (m_Mode == 2)
	{
		// 遍历容器，即遍历每一个三角片面对象
		for (auto iter = m_TriList.cbegin(); iter != m_TriList.cend(); iter++)
		{
			pDC->DrawTriChip(
				iter->fourFace[0].x, iter->fourFace[0].y, iter->fourFace[0].z,
				iter->fourFace[1].x, iter->fourFace[1].y, iter->fourFace[1].z,
				iter->fourFace[2].x, iter->fourFace[2].y, iter->fourFace[2].z,
				iter->fourFace[3].x, iter->fourFace[3].y, iter->fourFace[3].z, true, true);
		}
		pDC->Shading(FALSE); // 关闭OpenGL的着色处理

	}

	// 采用分步绘制模式
	else if (m_Mode == 3)
	{
		if (sections.size() == 0)
		{
			return;
		}

		// 采用彩色云图绘制模式
		if (is_Draw_cloudPicture)
		{
			pDC->TColor(Ncolor);
			int row = (int)((Max_y.at(stage) - Min_y.at(stage) + 1) / 20.0);
			int column = (int)((Max_x.at(stage) - Min_x.at(stage) + 1) / 20.0);
			double x = Min_x.at(stage);
			double y = Min_y.at(stage);

			// 废弃
			//for (int k = 1; k < ContourTriArr.at(stage).size(); k++)
			//{
			//	int row = (int)((Max_y - Min_y + 1) / 20.0);
			//	int column = (int)((Max_x - Min_x + 1) / 20.0);
			//	double x = Min_x;
			//	double y = Min_y;
			//	for (int i = 0; i < row - 1; i++)
			//	{
			//		for (int j = 0; j < column - 1; j++)
			//		{
			//			double dx = 20.0;
			//			double dy = 20.0;
			//			double z1 = ContourTriArr.at(stage).at(k - 1).point[0].stress;
			//			double z2 = ContourTriArr.at(stage).at(k - 1).point[1].stress;
			//			double z3 = ContourTriArr.at(stage).at(k - 1).point[2].stress;
			//			double z4 = ContourTriArr.at(stage).at(k).point[0].stress;
			//			pDC->TreeMath(x + j*dx, y + i*dy, dx, dy, z1, z2, z3, z4, stress_Min, Dc);
			//		}
			//	}
			////	//glFlush();
			////	// 绘制色码标识
			////	//pDC->DrawCodeIdentifier(x, y, Ncolor, column, Dc, stress_Min);
			//}



			//绘制等值线
			for (int j = 0; j < Dcontour_TotalSet.at(stage).size(); j++)  // stage = 16；size = 14-------第8条和第10条等值线不合格
			{
				for (int i = 1; i < Dcontour_TotalSet.at(stage).at(j).size(); i++)
				{
					Dcontour d1, d2;
					d1 = Dcontour_TotalSet.at(stage).at(j).at(i - 1);
					d2 = Dcontour_TotalSet.at(stage).at(j).at(i);
					pDC->DrawCloudPicture(d1.x, d1.y, d2.x, d2.y);
				}
			}

			// 绘制边缘曲线
			for (int j = 1; j < BorderLine_Set.at(stage).size(); j++)
			{
				CPoint2D p1 = BorderLine_Set.at(stage).at(j - 1);
				CPoint2D p2 = BorderLine_Set.at(stage).at(j);
				pDC->DrawEdge(p1, p2);
				if (j == BorderLine_Set.at(stage).size() - 1)
				{
					CPoint2D p3 = BorderLine_Set.at(stage).at(j);
					CPoint2D p4 = BorderLine_Set.at(stage).at(0);
					pDC->DrawEdge(p3, p4);
				}
			}

			//绘制等值线
		    	/*for (int j = 0; j < Dcontour_TotalSet.at(stage).size(); j++)
			{
				for (int i = 1; i < Dcontour_TotalSet.at(stage).at(j).size(); i++)
				{
					Dcontour d1, d2;
					d1 = Dcontour_TotalSet.at(stage).at(j).at(i - 1);
					d2 = Dcontour_TotalSet.at(stage).at(j).at(i);
					pDC->DrawCloudPicture(d1.x, d1.y, d2.x, d2.y);
				}
			}*/

			// 绘制边缘曲线
			/*for (int j = 1; j < BorderLine_Set.at(stage).size(); j++)
			{
				CPoint2D p1 = BorderLine_Set.at(stage).at(j - 1);
				CPoint2D p2 = BorderLine_Set.at(stage).at(j);
				pDC->DrawEdge(p1, p2);
				if (j == BorderLine_Set.at(stage).size() - 1)
				{
					CPoint2D p3 = BorderLine_Set.at(stage).at(j);
					CPoint2D p4 = BorderLine_Set.at(stage).at(0);
					pDC->DrawEdge(p3, p4);
				}
			}*/

			//绘制非封闭区域
			/*for (int j = 0; j < NonClosedRegion_Set.at(stage).size(); j++)
			{
				pDC->DrawNonClosedRegion(NonClosedRegion_Set.at(stage).at(j));
			}*/

			// 绘制非封闭区域
			/*for (int j = 0; j < NonClosedRegion_Set.at(stage).size(); j++)
			{
				for (int i = 1; i < NonClosedRegion_Set.at(stage).at(j).size(); i++)
				{
					CPoint3D  p1 = NonClosedRegion_Set.at(stage).at(j).at(i - 1);
					CPoint3D p2 = NonClosedRegion_Set.at(stage).at(j).at(i);
					pDC->DrawNonClosedRegion(p1, p2);
				}
			}*/

			// 绘制封闭区域
			/*for (int j = 0; j < ClosedRegion_Set.at(stage).size(); j++)
			{
				pDC->DrawNonClosedRegion(ClosedRegion_Set.at(stage).at(j));
			}*/

			// 绘制色码标识
			//pDC->DrawCodeIdentifier(x, y, Ncolor, column, Dc.at(stage), stress_Min.at(stage));

			// 利用扫描线绘制彩色云图
			/*	for (int j = 0; j < Scanner_TotalSet.at(stage).size(); j++)
			{
				pDC->DrawCloudPicture(Scanner_TotalSet.at(stage).at(j));
			}*/

		}
		else
		{
			// 绘制模型
			for (int i = 0; i < sections[stage].point_2D.size(); i = i + 3)
			{

				// 绘制模型
				pDC->DrawSingleModel(sections[stage].point_2D.at(i).x, sections[stage].point_2D.at(i).y,
					sections[stage].point_2D.at(i + 1).x, sections[stage].point_2D.at(i + 1).y,
					sections[stage].point_2D.at(i + 2).x, sections[stage].point_2D.at(i + 2).y);

				// 绘制矢量箭头
				if (is_PrintArrows)
				{
					pDC->DrawArrowsModel(sections[stage].point_2D.at(i).x, sections[stage].point_2D.at(i).y,
						sections[stage].point_2D.at(i + 1).x, sections[stage].point_2D.at(i + 1).y,
						sections[stage].point_2D.at(i + 2).x, sections[stage].point_2D.at(i + 2).y,
						sections[stage].speed_2D.at(i).x, sections[stage].speed_2D.at(i).y,
						sections[stage].speed_2D.at(i + 1).x, sections[stage].speed_2D.at(i + 1).y,
						sections[stage].speed_2D.at(i + 2).x, sections[stage].speed_2D.at(i + 2).y);
				}

			}

		}

	}

	// 采用动态显示模式
	else if (m_Mode == 4)
	{
		if (sections.size() == 0)
		{
			return;
		}

		for (int i = 0; i < sections[delta].point_2D.size(); i = i + 3)
		{
			if (is_PrintArrows) // 绘制矢量箭头
			{
				pDC->DrawArrowsModel(sections[delta].point_2D.at(i).x, sections[delta].point_2D.at(i).y,
					sections[delta].point_2D.at(i + 1).x, sections[delta].point_2D.at(i + 1).y,
					sections[delta].point_2D.at(i + 2).x, sections[delta].point_2D.at(i + 2).y,
					sections[delta].speed_2D.at(i).x, sections[delta].speed_2D.at(i).y,
					sections[delta].speed_2D.at(i + 1).x, sections[delta].speed_2D.at(i + 1).y,
					sections[delta].speed_2D.at(i + 2).x, sections[delta].speed_2D.at(i + 2).y);
			}
			
			// 绘制模型
			pDC->DrawSingleModel(sections[delta].point_2D.at(i).x, sections[delta].point_2D.at(i).y,
				sections[delta].point_2D.at(i + 1).x, sections[delta].point_2D.at(i + 1).y,
				sections[delta].point_2D.at(i + 2).x, sections[delta].point_2D.at(i + 2).y);
			
		}
	}

}

//获得网格模型的最小包围盒
TriMesh::BBox CFeatureReuseDoc::getbox()
{
	TriMesh::BBox box;
	int l=0;
	vector<float> minxx;
	vector<float> minyy;
	vector<float> minzz;
	vector<float> maxxx;
	vector<float> maxyy;
	vector<float> maxzz;
	for (int i=0;i<meshes.size();i++)
	{
		if(meshes.size()<1) break;
		//if(!meshes[i]->IsVisible()) 
		//	continue;
		point minc=meshes[i]->bbox.min;
		point maxc=meshes[i]->bbox.max;
		minxx.push_back(minc[0]);
		minyy.push_back(minc[1]);
		minzz.push_back(minc[2]);

		maxxx.push_back(maxc[0]);
		maxyy.push_back(maxc[1]);
		maxzz.push_back(maxc[2]);
		l=l+1;
	}

	int n=l;
	sort(minxx.begin(),minxx.end());
	sort(minyy.begin(),minyy.end());
	sort(minzz.begin(),minzz.end());
	point low(minxx[0],minyy[0],minzz[0]);
	sort(maxxx.begin(),maxxx.end());
	sort(maxyy.begin(),maxyy.end());
	sort(maxzz.begin(),maxzz.end());
	point hig(maxxx[n-1],maxyy[n-1],maxzz[n-1]);
	box.min=low;
	box.max=hig;
	return box;
}

void CFeatureReuseDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString str = "金属塑性成形自然单元法仿真可视化系统（NEM-VS）";
	CDocument::SetTitle(str);
}
