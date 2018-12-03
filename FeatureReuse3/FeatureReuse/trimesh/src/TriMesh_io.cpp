
#include <map>
#include <set>
#include <algorithm>
using namespace std;
#include "..\include\TriMesh.h"
#include "..\include\fyStruct.h"

#ifdef WIN32
# ifndef strncasecmp
#  define strncasecmp strnicmp
# endif
#endif
//
#define GET_LINE() do { if (!fgets(buf, 1024, f)) return false; } while (0)
#define FPRINTF(...) do { if (fprintf(__VA_ARGS__) < 0) return false; } while (0)


//#define GET_LINE() if (!fgets(buf, 1024, f)) return false
//#define COND_READ(cond, where, len) if ((cond) && !fread((void *)&(where), (len), 1, f)) return false
#define COND_READ(cond, where, len) do { if ((cond) && !fread((void *)&(where), (len), 1, f)) return false; } while (0)
#define LINE_IS(text) !strncasecmp(buf, text, strlen(text))
#define FWRITE(ptr, size, nmemb, stream) do { if (fwrite((ptr), (size), (nmemb), (stream)) != (nmemb)) return false; } while (0)
#define BIGNUM 1.0e10

// Forward declarations.
static bool read_off(FILE *f, TriMesh *mesh);

static bool read_ply(FILE *f, TriMesh *mesh);
static bool read_verts_bin(FILE *f, TriMesh *mesh, bool &need_swap,
						   int nverts, int vert_len, int vert_pos, int vert_norm,
						   int vert_color, bool float_color, int vert_conf);
static bool slurp_verts_bin(FILE *f, TriMesh *mesh, bool need_swap,
							int nverts);
static bool read_verts_asc(FILE *f, TriMesh *mesh,
						   int nverts, int vert_len, int vert_pos, int vert_norm,
						   int vert_color, bool float_color, int vert_conf);
static bool read_faces_bin(FILE *f, TriMesh *mesh, bool need_swap,
						   int nfaces, int face_len, int face_count, int face_idx);
static bool read_faces_asc(FILE *f, TriMesh *mesh, int nfaces,
						   int face_len, int face_count, int face_idx, bool read_to_eol = false);
static bool read_strips_bin(FILE *f, TriMesh *mesh, bool need_swap);
static bool read_strips_asc(FILE *f, TriMesh *mesh);
static bool read_grid_bin(FILE *f, TriMesh *mesh, bool need_swap);
static bool read_grid_asc(FILE *f, TriMesh *mesh);
static bool ply_property(const char *buf, int &len, bool binary);
static bool we_are_little_endian();
static void check_need_swap(const point &p, bool &need_swap);
static void check_ind_range(TriMesh *mesh);
static void skip_comments(FILE *f);
static void tess(const vector<point> &verts, const vector<int> &thisface,
				 vector<TriMesh::Face> &tris);

static bool write_verts_asc(TriMesh *mesh, FILE *f,
							const char *before_vert,
							const char *before_norm,
							const char *before_color,
							bool float_color,
							const char *before_conf,
							const char *after_line);

static bool write_faces_asc(TriMesh *mesh, FILE *f,
							const char *before_face, const char *after_line);
//
// Byte swap uints, ints, and floats
static inline void swap_unsigned(volatile unsigned &x)
{
	x = (x << 24u) |
		((x << 8u) & 0x00ff0000u) |
		((x >> 8u) & 0x0000ff00u) |
		(x >> 24u);
}

static inline void swap_int(int &x)
{
	swap_unsigned(* (unsigned *)(&x));
}

static inline void swap_float(float &x)
{
	swap_unsigned(* (unsigned *)(&x));
}

static inline void swap_double(double &x)
{
	unsigned char buf[8];
	memcpy(buf, &x, 8);
	swap(buf[0], buf[7]);
	swap(buf[1], buf[6]);
	swap(buf[2], buf[5]);
	swap(buf[3], buf[4]);
	memcpy(&x, buf, 8);
}

static inline void swap_ushort(volatile unsigned short &x)
{
	x = (x << 8u) | (x >> 8u);
}

static inline void swap_short(signed short &x)
{
	swap_ushort(* (unsigned short *)(&x));
}
//
// Read an off file
static bool read_off(FILE *f, TriMesh *mesh)
{
	skip_comments(f);
	char buf[1024];
	GET_LINE();
	int nverts, nfaces, unused;
	if (sscanf(buf, "%d %d %d", &nverts, &nfaces, &unused) < 2)
		return false;
	if (!read_verts_asc(f, mesh, nverts, 3, 0, -1, -1, false, -1))
		return false;
	if (!read_faces_asc(f, mesh, nfaces, 1, 0, 1, true))
		return false;

	return true;
}
TriMesh *TriMesh::load_off(const char *filename)
{
	TriMesh *mesh = new TriMesh();
	FILE *f ;
	if((f=fopen(filename,"r"))==NULL)
		return NULL;

	skip_comments(f);
	char buf[1024];
	GET_LINE();
	int nverts, nfaces, unused;
	fscanf(f,"%d %d %d",&(nverts),&(nfaces),&(unused));
	if (nverts <3)
		return NULL;
	if (!read_verts_asc(f, mesh, nverts, 3, 0, -1, -1, false, -1))
		return NULL;
	if (!read_faces_asc(f, mesh, nfaces, 1, 0, 1, true))
		return NULL;

	return mesh;

}
//------------------------------------------------------------
// Read a TriMesh from a file.  Defined to use a helper function to make
// subclassing easier.
TriMesh *TriMesh::read(const char *filename)
{
	TriMesh *mesh = new TriMesh();

	if (read_helper(filename, mesh))
		return mesh;

	delete mesh;
	return NULL;
}
//
// Actually read a mesh.  Tries to figure out type of file from first
// few bytes.  Filename can be "-" for stdin
bool TriMesh::read_helper(const char *filename, TriMesh *mesh)
{
	if (!filename || *filename == '\0')
		return false;

	FILE *f = NULL;
	bool ok = false;
	int c;

	if (strcmp(filename, "-") == 0) 
	{
		f = stdin;
		filename = "standard input";
	} 
	else 
	{
		f = fopen(filename, "rb");
		if (!f) 
		{
			perror("fopen");
			goto out;
		}
	}
	printf("Reading %s... ", filename);

	c = fgetc(f);
	if (c == EOF) 
	{
		fprintf(stderr, "Can't read header\n");
		goto out;
	}

	if (c == 'p') 
	{
		// See if it's a ply file
		char buf[4];
		if (!fgets(buf, 4, f)) 
		{
			fprintf(stderr, "Can't read header\n");
			goto out;
		}
		if (strncmp(buf, "ly", 2) == 0)
			ok = read_ply(f, mesh);
	} 
	else 
	{
		fprintf(stderr, "Unknown file type\n");
	}

out:
	if (f)
		fclose(f);
	if (!ok || (mesh->vertices.empty() && mesh->faces.empty())) {
		fprintf(stderr, "\nError reading file [%s]\n", filename);
		return false;
	}

	printf("Done.\n");
	check_ind_range(mesh);
	return true;
}
//--------------------------------------------------------------
struct facepoint 
{
	long p[3];
};

//---------------------------------读取二进制STL文件-----------------------------
TriMesh *TriMesh::load_stl(const char *filename)
{
	TriMesh *mesh = new TriMesh();
	FILE *f = fopen(filename, "rb");
	//int c = fgetc(f);
	bool need_swap = !we_are_little_endian();

	char header[80];
	COND_READ(true, header, 80);

	int nfacets;
	COND_READ(true, nfacets, 4);
	if (need_swap)
		swap_int(nfacets);

	mesh->faces.reserve(nfacets);
	mesh->vertices.reserve(3*nfacets);
	for (int i = 0; i < nfacets; i++) 
	{
		float fbuf[12];
		COND_READ(true, fbuf, 48);
		if (need_swap) {
			for (int j = 3; j < 12; j++)
				swap_float(fbuf[j]);
		}
		int v = mesh->vertices.size();
		mesh->vertices.push_back(point(fbuf[3], fbuf[4], fbuf[5]));
		mesh->vertices.push_back(point(fbuf[6], fbuf[7], fbuf[8]));
		mesh->vertices.push_back(point(fbuf[9], fbuf[10], fbuf[11]));
		mesh->faces.push_back(TriMesh::Face(v, v+1, v+2));
		unsigned char att[2];
		COND_READ(true, att, 2);
	}
	fclose(f);

	typedef std::set<fyVec3f>		fyVec3fSet;
	typedef std::vector<fyVec3f>		fyVec3fVector;
	typedef std::map<fyVec3f,long>		fyVec3fLongMap;
	typedef std::vector<long>			fyLongVector;

	fyVec3fVector  vexp;                    //临时储存顶点信息
	vector<facepoint> fp;
	int s=0;
	int nf=mesh->faces.size();
	for (int i=0; i<nf; i++)
	{
		point v0=mesh->vertices[mesh->faces[i][0]];
		facepoint threep;
		vexp.push_back(fyVec3f(v0[0],v0[1],v0[2]));
		threep.p[0]=s;

		v0=mesh->vertices[mesh->faces[i][1]];
		vexp.push_back(fyVec3f(v0[0],v0[1],v0[2]));
		threep.p[1]=s+1;

		v0=mesh->vertices[mesh->faces[i][2]];
		vexp.push_back(fyVec3f(v0[0],v0[1],v0[2]));
		threep.p[2]=s+2;
		fp.push_back(threep);
		s=s+3;
	}
	fyLongVector   iv;
	fyVec3fLongMap vm;

	long nPP = vexp.size();
	iv.resize(nPP);

	for(long ip=0; ip<nPP; ip++)
	{
		fyVec3f* pV = &vexp[ip];
		fyVec3fLongMap::iterator it = vm.find(*pV);
		if(it != vm.end())									
		{
			iv[ip] = it->second;
		}
		else												
		{
			long last_index = vm.size(); //
			vm.insert(fyVec3fLongMap::value_type(*pV, last_index));//□□□□maps
			iv[ip] = last_index;
		}
	}
	mesh->faces.clear();
	mesh->vertices.clear();
	long new_v_no = vm.size();
	printf("\n vm.size() = %d", new_v_no);

	fyVec3fVector vf(new_v_no);
	fyVec3fLongMap::iterator it;
	for(it = vm.begin(); it!=vm.end(); it++)
	{
		vf[it->second] = it->first;
	}
	long fpsize=fp.size();       //三角面片个数
	for (int i=0;i<fpsize;i++)
	{
		Face f1;
		f1.v[0]=iv[fp[i].p[0]];
		f1.v[1]=iv[fp[i].p[1]];
		f1.v[2]=iv[fp[i].p[2]];
		mesh->faces.push_back(f1);
	}
	for (int i=0;i<new_v_no;i++)
	{
		point vp=point(vf[i].x,vf[i].y,vf[i].z);
		mesh->vertices.push_back(vp);
	}

	mesh->Is_clouds=false;

	return mesh;
}

//---------------------------------读取文本STL文件----------------------------
TriMesh *TriMesh::read_stl( string f)
{
	typedef std::set<fyVec3f>		fyVec3fSet;
	typedef std::vector<fyVec3f>		fyVec3fVector;
	typedef std::map<fyVec3f,long>		fyVec3fLongMap;
	typedef std::vector<long>			fyLongVector;

	TriMesh *mesh = new TriMesh();
	fyVec3fVector  vexp;                    //临时储存顶点信息
	vector<facepoint> fp;

	const char *ff=f.c_str();
	int type=0;
	FILE *file;
	if((file=fopen(ff,"r"))==NULL)
		return NULL;
	char str[80];
	long s = 0;
	while (fscanf(file,"%s",str)==1)
	{
		if (strncmp(str,"normal",6)==0)
		{
			float x,y,z;
			fscanf(file,"%f %f %f",&(x),&(y),&(z));
			vec nor(x,y,z);
			mesh->FaceNormal.push_back(nor);                      //存取法矢
			fscanf(file,"%*s %*s");
			facepoint threep;

			fscanf(file,"%*s %f %f %f", &(x), &(y), &(z));
			threep.p[0] = s;
			vexp.push_back(fyVec3f(x,y,z));

			fscanf(file,"%*s %f %f %f", &(x), &(y), &(z));				// 存取三角面的三个顶点坐标
			threep.p[1] = s + 1;
			vexp.push_back(fyVec3f(x,y,z));

			fscanf(file,"%*s %f %f %f", &(x), &(y), &(z));
			threep.p[2]= s + 2;
			vexp.push_back(fyVec3f(x,y,z));

			fp.push_back(threep);
			s = s + 3;
		}
	}

	fyLongVector   iv;
	fyVec3fLongMap vm;

	long nPP = vexp.size();
	iv.resize(nPP);
	for(long ip=0; ip<nPP; ip++)
	{
		fyVec3f* pV = &vexp[ip];
		fyVec3fLongMap::iterator it = vm.find(*pV);
		if(it != vm.end())									
		{
			iv[ip] = it->second;
		}
		else												
		{
			long last_index = vm.size(); //
			vm.insert(fyVec3fLongMap::value_type(*pV, last_index));//□□□□maps
			iv[ip] = last_index;
		}
	}

	long new_v_no = vm.size();
	printf("\n vm.size() = %d", new_v_no);

	fyVec3fVector vf(new_v_no);
	fyVec3fLongMap::iterator it;
	for(it = vm.begin(); it!=vm.end(); it++)
	{
		vf[it->second] = it->first;
	}

	long fpsize=fp.size();       //三角面片个数
	for (int i=0;i<fpsize;i++)
	{
		Face f1;
		f1.v[0]=iv[fp[i].p[0]];
		f1.v[1]=iv[fp[i].p[1]];
		f1.v[2]=iv[fp[i].p[2]];
		mesh->faces.push_back(f1);
	}
	for (int i=0;i<new_v_no;i++)
	{
		point vp=point(vf[i].x,vf[i].y,vf[i].z);
		mesh->vertices.push_back(vp);
	}

	mesh->Is_clouds=false;
	fclose(file);
	return mesh;
}
//
TriMesh *TriMesh::read_pts(string fname)
{
	const char *ff=fname.c_str();
	int type=0;
	FILE *file;
	if((file=fopen(ff,"r"))==NULL)
		return NULL;


	TriMesh* pg= new TriMesh();                        
	while (!feof(file))
	{
		float x,y,z;
		fscanf(file,"%f %f %f",&(x),&(y),&(z));
		pg->vertices.push_back(point(x,y,z));
	}
	pg->Is_clouds=true;
	
	fclose(file);
	return pg;
}
//
// Read a ply file
static bool read_ply(FILE *f, TriMesh *mesh)
{
	char buf[1024];	
	bool binary = false, need_swap = false, float_color = false;
	int result, nverts = 0, nfaces = 0, nstrips = 0, ngrid = 0;
	int vert_len = 0, vert_pos = -1, vert_norm = -1;
	int vert_color = -1, vert_conf = -1;
	int face_len = 0, face_count = -1, face_idx = -1;

	// Read file format
	GET_LINE();
	while (buf[0] && isspace(buf[0]))
		GET_LINE();
	if (LINE_IS("format binary_big_endian 1.0")) {
		binary = true;
		need_swap = we_are_little_endian();
	} else if (LINE_IS("format binary_little_endian 1.0")) {
		binary = true;
		need_swap = !we_are_little_endian();
	} else if (LINE_IS("format ascii 1.0")) {
		binary = false;
	} else {
		printf("Unknown ply format or version.\n");
		return false;
	}

	// Skip comments and unknown obj_info lines
	GET_LINE();
	while (LINE_IS("obj_info") || LINE_IS("comment")) {
		if (LINE_IS("obj_info num_cols"))
			sscanf(buf, "obj_info num_cols %d", &mesh->grid_width);
		if (LINE_IS("obj_info num_rows"))
			sscanf(buf, "obj_info num_rows %d", &mesh->grid_height);
		GET_LINE();
	}

	// Skip until we find vertices
	int skip1 = 0;
	while (!LINE_IS("end_header") && !LINE_IS("element vertex")) {
		char elem_name[1024];
		int nelem = 0, elem_len = 0;
		sscanf(buf, "element %s %d", elem_name, &nelem);
		GET_LINE();
		while (LINE_IS("property")) {
			if (!ply_property(buf, elem_len, binary))
				return false;
			GET_LINE();
		}
		skip1 += nelem * elem_len;
	}

	// Find number of vertices
	result = sscanf(buf, "element vertex %d\n", &nverts);
	if (result != 1) {
		printf("Expected \"element vertex\".\n");
		return false;
	}

	// Parse vertex properties
	GET_LINE();
	while (LINE_IS("property")) {
		if (LINE_IS("property float x") ||
			LINE_IS("property float32 x"))
			vert_pos = vert_len;
		if (LINE_IS("property float nx") ||
			LINE_IS("property float32 nx"))
			vert_norm = vert_len;
		if (LINE_IS("property uchar diffuse_red") ||
			LINE_IS("property uint8 diffuse_red") ||
			LINE_IS("property uchar red") ||
			LINE_IS("property uint8 red"))
			vert_color = vert_len;
		if (LINE_IS("property float diffuse_red") ||
			LINE_IS("property float32 diffuse_red") ||
			LINE_IS("property float red") ||
			LINE_IS("property float32 red"))
			vert_color = vert_len, float_color = true;
		if (LINE_IS("property float confidence") ||
			LINE_IS("property float32 confidence"))
			vert_conf = vert_len;

		if (!ply_property(buf, vert_len, binary))
			return false;

		GET_LINE();
	}

	// Skip until we find faces
	int skip2 = 0;
	while (!LINE_IS("end_header") && !LINE_IS("element face") &&
		!LINE_IS("element tristrips") && !LINE_IS("element range_grid")) {
			char elem_name[1024];
			int nelem = 0, elem_len = 0;
			sscanf(buf, "element %s %d", elem_name, &nelem);
			GET_LINE();
			while (LINE_IS("property")) {
				if (!ply_property(buf, elem_len, binary))
					return false;
				GET_LINE();
			}
			skip2 += nelem * elem_len;
	}


	// Look for faces, tristrips, or range grid
	if (LINE_IS("element face")) {
		if (sscanf(buf, "element face %d\n", &nfaces) != 1)
			return false;
		GET_LINE();
		while (LINE_IS("property")) {
			if (LINE_IS("property list uchar int vertex_indices") ||
				LINE_IS("property list uint8 int32 vertex_indices") ||
				LINE_IS("property list char int vertex_indices") ||
				LINE_IS("property list int8 int32 vertex_indices") ||
				LINE_IS("property list uchar int vertex_index") ||
				LINE_IS("property list uint8 int32 vertex_index") ||
				LINE_IS("property list char int vertex_index") ||
				LINE_IS("property list int8 int32 vertex_index")) {
					face_count = face_len;
					face_idx = face_len + 1;
					face_len += 1;
			} else if
				(LINE_IS("property list uint int vertex_indices") ||
				LINE_IS("property list uint32 int32 vertex_indices") ||
				LINE_IS("property list int int vertex_indices") ||
				LINE_IS("property list int32 int32 vertex_indices") ||
				LINE_IS("property list uint int vertex_index") ||
				LINE_IS("property list uint32 int32 vertex_index") ||
				LINE_IS("property list int int vertex_index") ||
				LINE_IS("property list int32 int32 vertex_index")) {
					face_count = face_len;
					face_idx = face_len + (binary ? 4 : 1);
					face_len += (binary ? 4 : 1);
			} else if (!ply_property(buf, face_len, binary))
				return false;
			GET_LINE();
		}
	} else if (LINE_IS("element tristrips")) {
		nstrips = 1;
		GET_LINE();
		if (!LINE_IS("property list int int vertex_indices") &&
			!LINE_IS("property list int32 int32 vertex_indices"))
			return false;
		GET_LINE();
	} else if (LINE_IS("element range_grid")) {
		if (sscanf(buf, "element range_grid %d\n", &ngrid) != 1)
			return false;
		if (ngrid != mesh->grid_width*mesh->grid_height) {
			printf("Range grid size does not equal num_rows*num_cols.\n");
			return false;
		}
		GET_LINE();
		if (!LINE_IS("property list uchar int vertex_indices") &&
			!LINE_IS("property list uint8 int32 vertex_indices") &&
			!LINE_IS("property list char int vertex_indices") &&
			!LINE_IS("property list int8 int32 vertex_indices"))
			return false;
		GET_LINE();
	}

	while (LINE_IS("property")) {
		if (!ply_property(buf, face_len, binary))
			return false;
		GET_LINE();
	}

	// Skip to the end of the header
	while (!LINE_IS("end_header"))
		GET_LINE();
	if (binary && buf[10] == '\r') {
		printf("Warning: possibly corrupt file. (Transferred as ASCII instead of BINARY?)\n");
	}


	// Actually read everything in
	if (skip1) {
		if (binary)
			fseek(f, skip1, SEEK_CUR);
		else
			for (int i = 0; i < skip1; i++)
				fscanf(f, "%s", buf);
	}
	if (binary) {
		if (!read_verts_bin(f, mesh, need_swap, nverts, vert_len,
			vert_pos, vert_norm, vert_color,
			float_color, vert_conf))
			return false;
	} else {
		if (!read_verts_asc(f, mesh, nverts, vert_len,
			vert_pos, vert_norm, vert_color,
			float_color, vert_conf))
			return false;
	}

	if (skip2) {
		if (binary)
			fseek(f, skip2, SEEK_CUR);
		else
			for (int i = 0; i < skip2; i++)
				fscanf(f, "%s", buf);
	}

	if (ngrid) {
		if (binary) {
			if (!read_grid_bin(f, mesh, need_swap))
				return false;
		} else {
			if (!read_grid_asc(f, mesh))
				return false;
		}
	} else if (nstrips) {
		if (binary) {
			if (!read_strips_bin(f, mesh, need_swap))
				return false;
		} else {
			if (!read_strips_asc(f, mesh))
				return false;
		}
		mesh->convert_strips(TriMesh::TSTRIP_LENGTH);
	} else if (nfaces) {
		if (binary) {
			if (!read_faces_bin(f, mesh, need_swap, nfaces,
				face_len, face_count, face_idx))
				return false;
		} else {
			if (!read_faces_asc(f, mesh, nfaces,
				face_len, face_count, face_idx))
				return false;
		}
	}

	return true;
}
//
// Optimized reader for the simple case of just vertices w/o other properties
static bool slurp_verts_bin(FILE *f, TriMesh *mesh, bool need_swap, int nverts)
{
	int first = mesh->vertices.size() - nverts + 1;
	COND_READ(true, mesh->vertices[first][0], (nverts-1)*12);
	if (need_swap) {
		for (int i = first; i < mesh->vertices.size(); i++) {
			swap_float(mesh->vertices[i][0]);
			swap_float(mesh->vertices[i][1]);
			swap_float(mesh->vertices[i][2]);
		}
	}
	return true;
}
// Read a bunch of vertices from an ASCII file.
// Parameters are as in read_verts_bin, but offsets are in
// (white-space-separated) words, rather than in bytes
static bool read_verts_asc(FILE *f, TriMesh *mesh,
						   int nverts, int vert_len, int vert_pos, int vert_norm,
						   int vert_color, bool float_color, int vert_conf)
{
	if (nverts <= 0 || vert_len < 3 || vert_pos < 0)
		return false;

	int old_nverts = mesh->vertices.size();
	int new_nverts = old_nverts + nverts;
	mesh->vertices.resize(new_nverts);
	if (vert_norm > 0)
		mesh->normals.resize(new_nverts);
	if (vert_color > 0)
		mesh->colors.resize(new_nverts);
	if (vert_conf > 0)
		mesh->confidences.resize(new_nverts);

	char buf[1024];
	skip_comments(f);
	printf("\n  Reading %d vertices... ", nverts);
	for (int i = old_nverts; i < new_nverts; i++) {
		for (int j = 0; j < vert_len; j++) {
			if (j == vert_pos) {
				if (fscanf(f, "%f %f %f",
					&mesh->vertices[i][0],
					&mesh->vertices[i][1],
					&mesh->vertices[i][2]) != 3)
					return false;
				j += 2;
			} else if (j == vert_norm) {
				if (fscanf(f, "%f %f %f",
					&mesh->normals[i][0],
					&mesh->normals[i][1],
					&mesh->normals[i][2]) != 3)
					return false;
				j += 2;
			} else if (j == vert_color && float_color) {
				float r, g, b;
				if (fscanf(f, "%f %f %f", &r, &g, &b) != 3)
					return false;
				mesh->colors[i] = Color(r,g,b);
				j += 2;
			} else if (j == vert_color && !float_color) {
				int r, g, b;
				if (fscanf(f, "%d %d %d", &r, &g, &b) != 3)
					return false;
				mesh->colors[i] = Color(r,g,b);
				j += 2;
			} else if (j == vert_conf) {
				if (fscanf(f, "%f", &mesh->confidences[i]) != 1)
					return false;
			} else {
				fscanf(f, " %1024s", buf);
			}
		}
	}

	return true;
}

// Read nfaces faces from a binary file.
// face_len = total length of face record, *not counting the indices*
//  (Yes, this is bizarre, but there is potentially a variable # of indices...)
// face_count = offset within record of the count of indices in this face
//  (If this is -1, does not read a count and assumes triangles)
// face_idx = offset within record of the indices themselves
static bool read_faces_bin(FILE *f, TriMesh *mesh, bool need_swap,
						   int nfaces, int face_len, int face_count, int face_idx)
{
	if (nfaces < 0 || face_idx < 0)
		return false;

	if (nfaces == 0)
		return true;

	printf("\n  Reading %d faces... ", nfaces);

	int old_nfaces = mesh->faces.size();
	int new_nfaces = old_nfaces + nfaces;
	mesh->faces.reserve(new_nfaces);

	// face_len doesn't include the indices themeselves, since that's
	// potentially variable-length
	int face_skip = face_len - face_idx;

	vector<unsigned char> buf(__max(face_idx, face_skip));
	vector<int> thisface;
	for (int i = 0; i < nfaces; i++) {
		COND_READ(face_idx > 0, buf[0], face_idx);

		unsigned this_ninds = 3;
		if (face_count >= 0) {
			// Read count - either 1 or 4 bytes
			if (face_idx - face_count == 4) {
				this_ninds = * (unsigned *) &(buf[face_count]);
				if (need_swap)
					swap_unsigned(this_ninds);
			} else {
				this_ninds = buf[face_count];
			}
		}
		thisface.resize(this_ninds);
		COND_READ(true, thisface[0], 4*this_ninds);
		if (need_swap) {
			for (size_t j = 0; j < thisface.size(); j++)
				swap_int(thisface[j]);
		}
		tess(mesh->vertices, thisface, mesh->faces);
		COND_READ(face_skip > 0, buf[0], face_skip);
	}

	return true;
}


// Read a bunch of faces from an ASCII file
static bool read_faces_asc(FILE *f, TriMesh *mesh, int nfaces,
						   int face_len, int face_count, int face_idx, bool read_to_eol /* = false */)
{
	if (nfaces < 0 || face_idx < 0)
		return false;

	if (nfaces == 0)
		return true;

	int old_nfaces = mesh->faces.size();
	int new_nfaces = old_nfaces + nfaces;
	mesh->faces.reserve(new_nfaces);

	char buf[1024];
	skip_comments(f);
	printf("\n  Reading %d faces... ", nfaces);
	vector<int> thisface;
	for (int i = 0; i < nfaces; i++) {
		thisface.clear();
		int this_face_count = 3;
		for (int j = 0; j < face_len + this_face_count; j++) {
			if (j >= face_idx && j < face_idx + this_face_count) {
				thisface.push_back(0);
				if (!fscanf(f, " %d", &(thisface.back()))) {
					printf("Couldn't read vertex index %d for face %d\n",
						j - face_idx, i);
					return false;
				}
			} else if (j == face_count) {
				if (!fscanf(f, " %d", &this_face_count)) {
					printf("Couldn't read vertex count for face %d\n", i);
					return false;
				}
			} else {
				fscanf(f, " %s", buf);
			}
		}
		tess(mesh->vertices, thisface, mesh->faces);
		if (read_to_eol) {
			while (1) {
				int c = fgetc(f);
				if (c == EOF || c == '\n')
					break;
			}
		}
	}

	return true;
}


// Read triangle strips from a binary file
static bool read_strips_bin(FILE *f, TriMesh *mesh, bool need_swap)
{
	int striplen;
	COND_READ(true, striplen, 4);
	if (need_swap)
		swap_int(striplen);

	int old_striplen = mesh->tstrips.size();
	int new_striplen = old_striplen + striplen;
	mesh->tstrips.resize(new_striplen);

	printf("\n  Reading triangle strips... ");
	COND_READ(true, mesh->tstrips[old_striplen], 4*striplen);
	if (need_swap) {
		for (int i = old_striplen; i < new_striplen; i++)
			swap_int(mesh->tstrips[i]);
	}

	return true;
}


// Read triangle strips from an ASCII file
static bool read_strips_asc(FILE *f, TriMesh *mesh)
{
	skip_comments(f);
	int striplen;
	if (fscanf(f, "%d", &striplen) != 1)
		return false;
	int old_striplen = mesh->tstrips.size();
	int new_striplen = old_striplen + striplen;
	mesh->tstrips.resize(new_striplen);

	printf("\n  Reading triangle strips... ");
	skip_comments(f);
	for (int i = old_striplen; i < new_striplen; i++)
		if (fscanf(f, "%d", &mesh->tstrips[i]) != 1)
			return false;

	return true;
}


// Read range grid data from a binary file
static bool read_grid_bin(FILE *f, TriMesh *mesh, bool need_swap)
{
	printf("\n  Reading range grid... ");
	int ngrid = mesh->grid_width * mesh->grid_height;
	mesh->grid.resize(ngrid, TriMesh::GRID_INVALID);
	for (int i = 0; i < ngrid; i++) {
		int n = fgetc(f);
		if (n == EOF)
			return false;
		while (n--) {
			if (!fread((void *)&(mesh->grid[i]), 4, 1, f))
				return false;
			if (need_swap)
				swap_int(mesh->grid[i]);
		}
	}

	mesh->triangulate_grid();
	return true;
}


// Read range grid data from an ASCII file
static bool read_grid_asc(FILE *f, TriMesh *mesh)
{
	printf("\n  Reading range grid... ");
	int ngrid = mesh->grid_width * mesh->grid_height;
	mesh->grid.resize(ngrid, TriMesh::GRID_INVALID);
	for (int i = 0; i < ngrid; i++) {
		int n;
		if (fscanf(f, "%d", &n) != 1)
			return false;
		while (n--) {
			if (fscanf(f, "%d", &(mesh->grid[i])) != 1)
				return false;
		}
	}

	mesh->triangulate_grid();
	return true;
}

// Parse a PLY property line, and figure how many bytes it represents
// Increments "len" by the number of bytes, or by 1 if !binary
static bool ply_property(const char *buf, int &len, bool binary)
{
	if (LINE_IS("property char") ||
		LINE_IS("property uchar") ||
		LINE_IS("property int8") ||
		LINE_IS("property uint8")) {
			len += 1;
	} else if (LINE_IS("property short") ||
		LINE_IS("property ushort") ||
		LINE_IS("property int16") ||
		LINE_IS("property uint16")) {
			len += (binary ? 2 : 1);
	} else if (LINE_IS("property int") ||
		LINE_IS("property uint") ||
		LINE_IS("property float") ||
		LINE_IS("property int32") ||
		LINE_IS("property uint32") ||
		LINE_IS("property float32")) {
			len += (binary ? 4 : 1);
	} else if (LINE_IS("property double") ||
		LINE_IS("property float64")) {
			len += (binary ? 8 : 1);
	} else {
		fprintf(stderr, "Unsupported vertex property: %s\n", buf);
		return false;
	}
	return true;
}

// Figure out whether this machine is little- or big-endian
static bool we_are_little_endian()
{
	char buf[4];
	*(int *)(&buf[0]) = 1;
	return (buf[0] == 1);
}
// Figure out whether the need_swap setting makes sense, or whether this
// file incorrectly declares its endianness
static void check_need_swap(const point &p, bool &need_swap)
{
	float p0 = p[0], p1 = p[1], p2 = p[2];
	if (need_swap) {
		swap_float(p0);
		swap_float(p1);
		swap_float(p2);
	}
	bool makes_sense = (p0 > -BIGNUM && p0 < BIGNUM &&
		p1 > -BIGNUM && p1 < BIGNUM &&
		p2 > -BIGNUM && p2 < BIGNUM);
	if (makes_sense)
		return;

	swap_float(p0);
	swap_float(p1);
	swap_float(p2);

	bool makes_sense_swapped = (p0 > -BIGNUM && p0 < BIGNUM &&
		p1 > -BIGNUM && p1 < BIGNUM &&
		p2 > -BIGNUM && p2 < BIGNUM);
	if (makes_sense_swapped) {
		fprintf(stderr, "Compensating for bogus endianness.\n");
		need_swap = !need_swap;
	}
}
// Check whether the indices in the file mistakenly go
// from 1..N instead of 0..N-1
static void check_ind_range(TriMesh *mesh)
{
	if (mesh->faces.empty())
		return;
	int min_ind = mesh->faces[0][0];
	int max_ind = mesh->faces[0][0];
	for (int i = 0; i < mesh->faces.size(); i++) {
		for (int j = 0; j < 3; j++) {
			min_ind = __min(min_ind, mesh->faces[i][j]);
			max_ind = __max(max_ind, mesh->faces[i][j]);
		}
	}

	int nv = mesh->vertices.size();

	// All good
	if (min_ind == 0 && max_ind == nv-1)
		return;

	// Simple fix: offset everything
	if (max_ind - min_ind == nv-1) {
		printf("Found indices ranging from %d through %d\n",
			min_ind, max_ind);
		printf("Remapping to %d through %d\n", 0, nv-1);
		for (int i = 0; i < mesh->faces.size(); i++)
			for (int j = 0; j < 3; j++)
				mesh->faces[i][j] -= min_ind;
		return;
	}

	// Else can't do anything...
}
//
// Skip comments in an ASCII file (lines beginning with #)
static void skip_comments(FILE *f)
{
	int c;
	bool in_comment = false;
	while (1) {
		c = fgetc(f);
		if (c == EOF)
			return;
		if (in_comment) {
			if (c == '\n')
				in_comment = false;
		} else if (c == '#') {
			in_comment = true;
		} else if (!isspace(c)) {
			break;
		}
	}
	ungetc(c, f);
}


// Tesselate an arbitrary n-gon.  Appends triangles to "tris".
static void tess(const vector<point> &verts, const vector<int> &thisface,
				 vector<TriMesh::Face> &tris)
{
	if (thisface.size() < 3)
		return;
	if (thisface.size() == 3) {
		tris.push_back(TriMesh::Face(thisface[0],
			thisface[1],
			thisface[2]));
		return;
	}
	if (thisface.size() == 4) {
		// Triangulate in the direction that
		// gives the shorter diagonal
		const point &p0 = verts[thisface[0]], &p1 = verts[thisface[1]];
		const point &p2 = verts[thisface[2]], &p3 = verts[thisface[3]];
		float d02 = dist2(p0, p2);
		float d13 = dist2(p1, p3);
		int i = (d02 < d13) ? 0 : 1;
		tris.push_back(TriMesh::Face(thisface[i],
			thisface[(i+1)%4],
			thisface[(i+2)%4]));
		tris.push_back(TriMesh::Face(thisface[i],
			thisface[(i+2)%4],
			thisface[(i+3)%4]));
		return;
	}

	// 5-gon or higher - just tesselate arbitrarily...
	for (int i = 2; i < thisface.size(); i++)
		tris.push_back(TriMesh::Face(thisface[0],
		thisface[i-1],
		thisface[i]));
}
// Read nverts vertices from a binary file.
// vert_len = total length of a vertex record in bytes
// vert_pos, vert_norm, vert_color, vert_conf =
//   position of vertex coordinates / normals / color / confidence in record
// need_swap = swap for opposite endianness
// float_color = colors are 4-byte float * 3, vs 1-byte uchar * 3
static bool read_verts_bin(FILE *f, TriMesh *mesh, bool &need_swap,
						   int nverts, int vert_len, int vert_pos, int vert_norm,
						   int vert_color, bool float_color, int vert_conf)
{
	const int vert_size = 12;
	const int norm_size = 12;
	const int color_size = float_color ? 12 : 3;
	const int conf_size = 4;

	if (nverts <= 0 || vert_len < 12 || vert_pos < 0)
		return false;

	int old_nverts = mesh->vertices.size();
	int new_nverts = old_nverts + nverts;
	mesh->vertices.resize(new_nverts);

	bool have_norm = (vert_norm >= 0);
	bool have_color = (vert_color >= 0);
	bool have_conf = (vert_conf >= 0);
	if (have_norm)
		mesh->normals.resize(new_nverts);
	if (have_color)
		mesh->colors.resize(new_nverts);
	if (have_conf)
		mesh->confidences.resize(new_nverts);

	unsigned char *buf = new unsigned char[vert_len];
	COND_READ(true, buf[0], vert_len);

	int i = old_nverts;
	memcpy(&mesh->vertices[i][0], &buf[vert_pos], vert_size);
	if (have_norm)
		memcpy(&mesh->normals[i][0], &buf[vert_norm], norm_size);
	if (have_color && float_color)
		memcpy(&mesh->colors[i][0], &buf[vert_color], color_size);
	if (have_color && !float_color)
		mesh->colors[i] = Color(&buf[vert_color]);
	if (have_conf)
		memcpy(&mesh->confidences[i], &buf[vert_conf], conf_size);

	check_need_swap(mesh->vertices[i], need_swap);
	if (need_swap) {
		swap_float(mesh->vertices[i][0]);
		swap_float(mesh->vertices[i][1]);
		swap_float(mesh->vertices[i][2]);
		if (have_norm) {
			swap_float(mesh->normals[i][0]);
			swap_float(mesh->normals[i][1]);
			swap_float(mesh->normals[i][2]);
		}
		if (have_color && float_color) {
			swap_float(mesh->colors[i][0]);
			swap_float(mesh->colors[i][1]);
			swap_float(mesh->colors[i][2]);
		}
		if (have_conf)
			swap_float(mesh->confidences[i]);
	}

	printf("\n  Reading %d vertices... ", nverts);
	if (vert_len == 12 && sizeof(point) == 12 && nverts > 1)
		return slurp_verts_bin(f, mesh, need_swap, nverts);
	while (++i < new_nverts) {
		COND_READ(true, buf[0], vert_len);
		memcpy(&mesh->vertices[i][0], &buf[vert_pos], vert_size);
		if (have_norm)
			memcpy(&mesh->normals[i][0], &buf[vert_norm], norm_size);
		if (have_color && float_color)
			memcpy(&mesh->colors[i][0], &buf[vert_color], color_size);
		if (have_color && !float_color)
			mesh->colors[i] = Color(&buf[vert_color]);
		if (have_conf)
			memcpy(&mesh->confidences[i], &buf[vert_conf], conf_size);

		if (need_swap) {
			swap_float(mesh->vertices[i][0]);
			swap_float(mesh->vertices[i][1]);
			swap_float(mesh->vertices[i][2]);
			if (have_norm) {
				swap_float(mesh->normals[i][0]);
				swap_float(mesh->normals[i][1]);
				swap_float(mesh->normals[i][2]);
			}
			if (have_color && float_color) {
				swap_float(mesh->colors[i][0]);
				swap_float(mesh->colors[i][1]);
				swap_float(mesh->colors[i][2]);
			}
			if (have_conf)
				swap_float(mesh->confidences[i]);
		}
	}

	return true;
}
// Write an STL file
bool write_stl(TriMesh *mesh, FILE *f)
{
	bool need_swap = !we_are_little_endian();

	char header[80];
	memset(header, ' ', 80);
	FWRITE(header, 80, 1, f);

	int nfaces = mesh->faces.size();
	if (need_swap)
		swap_int(nfaces);
	FWRITE(&nfaces, 4, 1, f);

	for (size_t i = 0; i < mesh->faces.size(); i++) {
		float fbuf[12];
		vec tn = mesh->trinorm(i);
		normalize(tn);
		fbuf[0] = tn[0]; fbuf[1] = tn[1]; fbuf[2] = tn[2];
		fbuf[3]  = mesh->vertices[mesh->faces[i][0]][0];
		fbuf[4]  = mesh->vertices[mesh->faces[i][0]][1];
		fbuf[5]  = mesh->vertices[mesh->faces[i][0]][2];
		fbuf[6]  = mesh->vertices[mesh->faces[i][1]][0];
		fbuf[7]  = mesh->vertices[mesh->faces[i][1]][1];
		fbuf[8]  = mesh->vertices[mesh->faces[i][1]][2];
		fbuf[9]  = mesh->vertices[mesh->faces[i][2]][0];
		fbuf[10] = mesh->vertices[mesh->faces[i][2]][1];
		fbuf[11] = mesh->vertices[mesh->faces[i][2]][2];
		if (need_swap) {
			for (int j = 0; j < 12; j++)
				swap_float(fbuf[j]);
		}
		FWRITE(fbuf, 48, 1, f);
		unsigned char att[2] = { 0, 0 };
		FWRITE(att, 2, 1, f);
	}
	fclose(f);
	return true;
}

// Write a off file
 bool write_off(TriMesh *mesh, FILE *f)
{
	FPRINTF(f, "OFF\n");
	mesh->need_faces();
	FPRINTF(f, "%lu %lu 0\n", (unsigned long) mesh->vertices.size(),
		(unsigned long) mesh->faces.size());
	return write_verts_asc(mesh, f, "", 0, 0, false, 0, "") &&
		write_faces_asc(mesh, f, "3 ", "");
}

// Write a bunch of vertices to an ASCII file
static bool write_verts_asc(TriMesh *mesh, FILE *f,
							const char *before_vert,
							const char *before_norm,
							const char *before_color,
							bool float_color,
							const char *before_conf,
							const char *after_line)
{
	for (size_t i = 0; i < mesh->vertices.size(); i++) {
		FPRINTF(f, "%s%.7g %.7g %.7g", before_vert,
			mesh->vertices[i][0],
			mesh->vertices[i][1],
			mesh->vertices[i][2]);
		if (!mesh->normals.empty() && before_norm)
			FPRINTF(f, "%s%.7g %.7g %.7g", before_norm,
			mesh->normals[i][0],
			mesh->normals[i][1],
			mesh->normals[i][2]);

		FPRINTF(f, "%s\n", after_line);
	}
	return true;
}

// Write a bunch of faces to an ASCII file
static bool write_faces_asc(TriMesh *mesh, FILE *f,
							const char *before_face, const char *after_line)
{
	mesh->need_faces();
	for (size_t i = 0; i < mesh->faces.size(); i++) {
		FPRINTF(f, "%s%d %d %d%s\n", before_face, mesh->faces[i][0],
			mesh->faces[i][1], mesh->faces[i][2], after_line);
	}
	return true;
}
