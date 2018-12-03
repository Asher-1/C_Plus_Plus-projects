/*
TriMesh_connectivity.cpp
Manipulate data structures that describe connectivity between faces and verts.
*/

#include <stdio.h>
#include "..\include\TriMesh.h"
#include "..\include\KDtree.h"
#include <algorithm>
using std::find;


// Find the direct neighbors of each vertex
void TriMesh::need_neighbors()
{
	if (!neighbors.empty())
		return;

	need_faces();
	if (faces.empty())
		return;

	printf("Finding vertex neighbors... ");
	int nv = vertices.size(), nf = faces.size();

	vector<int> numneighbors(nv);
	for (int i = 0; i < nf; i++) {
		numneighbors[faces[i][0]]++;
		numneighbors[faces[i][1]]++;
		numneighbors[faces[i][2]]++;
	}

	neighbors.resize(nv);
	for (int i = 0; i < nv; i++)
		neighbors[i].reserve(numneighbors[i]+2); // Slop for boundaries

	for (int i = 0; i < nf; i++) {
		for (int j = 0; j < 3; j++) {
			vector<int> &me = neighbors[faces[i][j]];
			int n1 = faces[i][(j+1)%3];
			int n2 = faces[i][(j+2)%3];
			if (find(me.begin(), me.end(), n1) == me.end())
				me.push_back(n1);
			if (find(me.begin(), me.end(), n2) == me.end())
				me.push_back(n2);
		}
	}

	printf("Done.\n");
}


// Find the faces touching each vertex
void TriMesh::need_adjacentfaces()
{
	if (!adjacentfaces.empty())
		return;

	need_faces();
	if (faces.empty())
		return;

	printf("Finding vertex to triangle maps... ");
	int nv = vertices.size(), nf = faces.size();

	vector<int> numadjacentfaces(nv);
	for (int i = 0; i < nf; i++) {
		numadjacentfaces[faces[i][0]]++;
		numadjacentfaces[faces[i][1]]++;
		numadjacentfaces[faces[i][2]]++;
	}

	adjacentfaces.resize(vertices.size());
	for (int i = 0; i < nv; i++)
		adjacentfaces[i].reserve(numadjacentfaces[i]);

	for (int i = 0; i < nf; i++) {
		for (int j = 0; j < 3; j++)
			adjacentfaces[faces[i][j]].push_back(i);
	}
	printf("Done.\n");
}

// Find the face across each edge from each other face (-1 on boundary)
// If topology is bad, not necessarily what one would expect...
void TriMesh::need_across_edge()
{
	if (!across_edge.empty())
		return;

	need_adjacentfaces();
	if (adjacentfaces.empty())
		return;

	printf("Finding across-edge maps... ");

	int nf = faces.size();
	across_edge.resize(nf, Face(-1,-1,-1));

	for (int i = 0; i < nf; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			if (across_edge[i][j] != -1)
				continue;
			int v1 = faces[i][(j+1)%3];
			int v2 = faces[i][(j+2)%3];
			const vector<int> &a1 = adjacentfaces[v1];
			const vector<int> &a2 = adjacentfaces[v2];
			for (int k1 = 0; k1 < a1.size(); k1++) 
			{
				int other = a1[k1];
				if (other == i)
					continue;
				vector<int>::const_iterator it =	find(a2.begin(), a2.end(), other);
				if (it == a2.end())
					continue;
				int ind = (faces[other].indexof(v1)+1)%3;
				if (faces[other][(ind+1)%3] != v2)
					continue;
				across_edge[i][j] = other;
				across_edge[other][ind] = i;
				break;
			}
		}
	}

	printf("Done.\n");
}

void TriMesh::build_adjacencies()            //为测地线用
{
	for (long l=0;l<faces.size();l++)
	{
		faces[l]._id=l;
	}
	//find all edges
	//i.e. find all half-edges, sort and combine them into edges
	std::vector<HalfEdge> half_edges(faces.size()*3);

	long k = 0;
	for(long i=0; i<faces.size(); ++i)
	{
		Face& f = faces[i];
		for(int j=0; j<3; ++j)
		{
			half_edges[k].face_id = i;
			long vertex_id_1 = f[j];
			long vertex_id_2 = f[(j+1) % 3];
			half_edges[k].vertex_0 = __min(vertex_id_1, vertex_id_2);
			half_edges[k].vertex_1 = __max(vertex_id_1, vertex_id_2);
			k++;
		}
	}
	std::sort(half_edges.begin(), half_edges.end());

	long number_of_edges = 1;
	for(int i=1; i<half_edges.size(); ++i)
	{
		if(half_edges[i] != half_edges[i-1])
		{
			++number_of_edges;
		}
		else
		{
			if(i<half_edges.size()-1)		//sanity check: there should be at most two equal half-edges
			{								//if it fails, most likely the input data are messed up
				assert(half_edges[i] != half_edges[i+1]);
			}
		}
	}

	//		Edges->adjacent Vertices and Faces
	m_edges.resize(number_of_edges);
	long edge_id = 0;
	for(int i=0; i<half_edges.size();)
	{
		Tedge& e = m_edges[edge_id];
		e.m_id = edge_id++;

		e.m_adjacent_vertices[0] = half_edges[i].vertex_0;
		e.m_adjacent_vertices[1] = half_edges[i].vertex_1;

		e.length() = len(vertices[e.m_adjacent_vertices[0]]-vertices[e.m_adjacent_vertices[1]]);
		assert(e.length() > 1e-100);		//algorithm works well with non-degenerate meshes only 

		if(i != half_edges.size()-1 && half_edges[i] == half_edges[i+1])	//double edge
		{
			e.m_adjacent_faces.push_back(faces[half_edges[i].face_id]) ;
			e.m_adjacent_faces.push_back(faces[half_edges[i+1].face_id]) ;
			i += 2;
		}
		else			//single edge
		{			//one adjucent faces
			e.m_adjacent_faces.push_back(faces[half_edges[i].face_id]) ;
			i += 1;
		}
	}
	
}

//2009.3.9为模型上的局部区域选择服务，方法见Metamorphosis of Arbitrary Triangular Meshes 一文
void TriMesh::build_halfedge()
{
	halfedges_for_face.resize(faces.size());
	for(int i=0; i<faces.size(); ++i)
	{
		Face& f = faces[i];
		for(int j=0; j<3; ++j)
		{
			long vertex_id_1 = f[j];
			long vertex_id_2 = f[(j+1) % 3];

			HalfEdge temp;
			temp.face_id=i;
			temp.vertex_0=vertex_id_1;
			temp.vertex_1=vertex_id_2;
			halfedges_for_face[i].push_back(temp);
		}
	}
}
//bj存储一系列按照逆时针排序的顶点，构成所选区域的边界
void TriMesh::zone_select(vector<int>& bj, vector<int>& zone, vector<int>& SelFaces)
{
	build_halfedge();
	build_adjacencies();
	need_neighbors();
	need_adjacentfaces();

	vector<int>  v_in_sel;
	vector<bool> beSel(vertices.size(),false);

	int n=bj.size();
	for (int i=0; i<n;i++)
	{
		int z=bj[i];
		beSel[z]=true;
		v_in_sel.push_back(z);
	}

	bool seed_face_id=false;
	int  seed_v_bsel=-1;   //选择区域内的一个点，作为广度优先搜索的种子点
	for (int i=0; i<n-1;i++)
	{
		point v0=vertices[bj[i]];
		point v1=vertices[bj[i+1]];

		for (int j=0;j<faces.size();j++)
		{
			for (int k=0;k<3;k++)
			{
				int cp0=halfedges_for_face[j][k].vertex_0;
				int cp1=halfedges_for_face[j][k].vertex_1;
				if (vertices[cp0]==v0 && vertices[cp1]==v1)
				{
					//faces[j]被标记为与边v0v1相连的左侧三角面，三角面的法矢必须向外
					for (int m=0;m<3;m++)
					{
						int v=faces[j][m];
						if (vertices[v]!=v0 && vertices[v]!=v1)
						{
							if(beSel[v]) continue;   //种子点就是边界点的情况
							seed_v_bsel=v;        //找到一个种子点，跳出循环
							seed_face_id=true;
							break;
						}
					}
					break;
				}
			}
			if(seed_face_id) 
				break;
		}
		if(seed_face_id) break;
	}

	//广度优先搜索
	if(seed_v_bsel!=-1)
	{
		beSel[seed_v_bsel] = true;
		v_in_sel.push_back(seed_v_bsel);
		vector<int> nearby;
		// add all the neighbors of the starting vertex into nearby
		for(int j=0;j< neighbors[seed_v_bsel].size();j++)
		{
			int nid=neighbors[seed_v_bsel][j];
			nearby.push_back(nid);
		}

		// repeat until nearby is empty:
		while(nearby.size()>0)
		{
			// for each nearby point:
			int iNearby,iNeighbor;
			for(int i=0; i<nearby.size(); i++)
			{
				iNearby = nearby[i];
				if(beSel[iNearby]) 
				{
					vector<int>::iterator iter;
					iter=find(nearby.begin(),nearby.end(),iNearby);
					nearby.erase(iter);
					continue;
				}
				beSel[iNearby] = true;
				v_in_sel.push_back(iNearby);

				vector<int>::iterator iter;
				iter=find(nearby.begin(),nearby.end(),iNearby);
				nearby.erase(iter);

				for(int j=0;j<neighbors[iNearby].size();j++)
				{
					iNeighbor = neighbors[iNearby][j];
					if(beSel[iNeighbor]) continue;
					nearby.push_back(iNeighbor);
				}
			}
		}
	}
	int nn=v_in_sel.size();
	zone.clear();
	for (int l=0;l<nn; l++)
	{
		int x=v_in_sel[l];
		zone.push_back(x);
	}
	int nf=faces.size();
	vector<bool> fangwen(nf, false);
	SelFaces.clear();
	for (int l=0;l<nn; l++)
	{
		int x=v_in_sel[l];
		const vector<int> &a = adjacentfaces[x];
		if (a.empty()) continue;
		for (int j=0; j<a.size(); j++)
		{
			int t=a[j];
			if(fangwen[t]) continue;
			bool flag=false;
			for (int k=0;k<3; k++)
			{
				if (!beSel[faces[t].v[k]])
				{
					flag=true;       //边界外三角形
					break;;
				}
			}
			if(flag) continue;
			SelFaces.push_back(t);
			fangwen[t]=true;
		}
	}
}
