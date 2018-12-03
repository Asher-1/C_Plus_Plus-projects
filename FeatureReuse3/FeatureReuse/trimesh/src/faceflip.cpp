/*
faceflip.cpp
Flip the order of vertices in each face.  Turns the mesh inside out.
*/

#include <stdio.h>
#include "..\include\TriMesh.h"
#include "..\include\TriMesh_algo.h"

void faceflip(TriMesh *mesh)
{
	bool had_tstrips = !mesh->tstrips.empty();
	mesh->need_faces();
	mesh->tstrips.clear();

	printf("Flipping faces... ");
	int nf = mesh->faces.size();
#pragma omp parallel for
	for (int i = 0; i < nf; i++)
		swap(mesh->faces[i][0], mesh->faces[i][2]);
	printf("Done.\n");

	if (had_tstrips)
		mesh->need_tstrips();
}

