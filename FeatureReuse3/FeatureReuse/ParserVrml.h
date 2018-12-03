#pragma once


#include "StdAfx.h"
#include "TriMesh\include\TriMesh.h"

#include <stdlib.h>
#include <crtdbg.h>

#define MAX_LINE_VRML 10000
#define MAX_WORD_VRML 1000
class CParserVrml
{
private :

	int				m_SizeFile;
	char				*m_pBuffer;
	int				m_IndexBuffer;
	CString		m_FileName;
	char				m_pBufferLine[MAX_LINE_VRML];
	char				m_pBufferWord[MAX_WORD_VRML];

public:
	CParserVrml(void);
	~CParserVrml(void);

	// Running
	void Free(void);
	//int Run(char *filename,CSceneGraph3d *pSceneGraph);
	int Run(char *filename, TriMesh *Tmesh);

	// Word processing
	int ReadFile(char *filename);
	int ReadLine();
	int ReadWord();
	int OffsetToString(char *string);
	int OffsetToStringBefore(char *string,char *before);
	int OffsetToStringBeginLine(char *string);

	int CheckVersion();
	void CountDef(void);
	int CountMesh(void);
	int CheckMesh();
	int ReadMesh(TriMesh *Tmesh);
	int SizeMesh(int *pNbVertex,int *pNbFace,int HasTexture,int *pNbTextureCoordinate = NULL);
};
