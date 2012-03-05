#include "MeshLoader.h"
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
//#include "TokenStream.h"


MeshLoader::MeshLoader(void) : m_Positions(0), m_Normals(0), m_TexCoords(0), m_TotalVertices(0) {
	}

MeshLoader::~MeshLoader(void) {
	Release();
	}

void MeshLoader::Release()
{
    m_TotalVertices = 0;

    if (m_Positions != 0) delete[] m_Positions;
    if (m_Normals != 0) delete[] m_Normals;
    if (m_TexCoords != 0) delete[] m_TexCoords;

    m_Positions = 0;
    m_Normals = 0;
    m_TexCoords = 0;
}


bool MeshLoader::LoadOBJ(wstring fileName) {
    Release();
	// @TODO: it returns plain only for now
	m_TotalVertices = 4;
	m_Positions = new float[m_TotalVertices*3]; // xyz
	m_Normals = new float[m_TotalVertices*3]; // xyz
	m_TexCoords = new float[m_TotalVertices*2]; // uv
	m_Positions[0] = -0.5f; // 1st vertex
	m_Positions[1] = -0.5f;
	m_Positions[2] = -0.5f;
	m_Positions[3] = 0.5f; // 2nd vertex
	m_Positions[4] = -0.5f;
	m_Positions[5] = -0.5f;
	m_Positions[6] = 0.5f; // 3rd vertex
	m_Positions[7] = 0.5f;
	m_Positions[8] = -0.5f;
	m_Positions[9] = -0.5f; // 4th vertex
	m_Positions[10] = 0.5f;
	m_Positions[11] = -0.5f;
	
	ZeroMemory(m_Normals, m_TotalVertices*3);
	ZeroMemory(m_TexCoords, m_TotalVertices*2);	
    return true;
}