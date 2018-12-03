/*
TriMesh_connectivity.cpp
Manipulate data structures that describe connectivity between faces and verts.
*/

#include <stdio.h>
#include "..\include\KDtree.h"
#include "..\include\TriMesh_algo.h"
#include <algorithm>
#include <set>
using namespace std;
using std::find;

// Helper class for finding k-nearest-neighbors: returns true iff
// a point is not in the given set of points
class NotInSet : public KDtree::CompatFunc 
{
private:
	const float *plist;
	const set<int> &s;
public:
	NotInSet(const float *plist_, const set<int> &s_) :
	  plist(plist_), s(s_)
	  {}
	  virtual bool operator () (const float *p) const
	  {
		  int ind = p - plist; 
		  return (s.find(ind) == s.end());
	  }
};

struct NumberedItem
{
	int index;
	double Item;
};

bool operator<(NumberedItem  NI1, NumberedItem  NI2)
{
	return NI1.Item < NI2.Item;
}

void TriMesh::remove_outliers(float threshold_percent)
{
	
	int vn=vertices.size();
	vector<double>  avgdis(vn);
	const float *v0 = &vertices[0][0];
	KDtree *kd = new KDtree(v0, vn);

	int k=6;

	for (int i=0; i<vn; i++)
	{
		const float *vi = &vertices[i][0];
		set<int> s;
		s.insert(vi - v0);
		for (int j = 0; j < k; j++) 
		{
			NotInSet ns(v0, s);
			const float *match =	kd->closest_to_pt(vi, 0.0f, &ns);
			if (!match)
				break;
			s.insert(match - v0);
		}

		double di=0;
		for (set<int>::iterator it = s.begin(); it != s.end(); it++) 
		{
			int ind = *it / 3;
			if (ind == i)
				continue;
			di = di+dist2(vertices[ind],vertices[i]);
		}
		avgdis[i]=di;
	}

	vector<NumberedItem> Pairs(vn);
#pragma omp parallel for
	for( int i = 0; i < vn; i++)
	{
		int n=avgdis.size();
		if (n==0)
		{
			avgdis[i]=1e10;
		}
		else
		{
			avgdis[i]=avgdis[i]/n;
		}
		Pairs[i].index = i;
		Pairs[i].Item = avgdis[i];
	}
	sort(Pairs.begin(), Pairs.end());
	vector<int>SortedIndices(vn);
#pragma omp parallel for
	for( int i = 0; i < vn; i++)
	{
		 SortedIndices[i] = Pairs[i].index;
	}

	vector<bool> beremove(vn,false);
	int n=(int)(vn*threshold_percent);
#pragma omp parallel for
	for (int i=vn-n; i<vn; i++)
	{
		int vi=SortedIndices[i];
		beremove[vi]=true;
	}
	remove_vertices(this, beremove);
}