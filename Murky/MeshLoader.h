#pragma once
#include <string>

using namespace std;

class MeshLoader
	{
	public:
		MeshLoader(void);
		~MeshLoader(void);
		bool LoadOBJ(wstring fileName);
		float* GetPositions() const		{ return m_Positions; }
		float* GetNormals() const		{ return m_Normals; }
		float* GetTexCoords() const		{ return m_TexCoords; }
		int    GetTotalVerts() const	{ return m_TotalVertices; }
	private:
		void Release();
		float*	m_Positions;
		float*	m_Normals;
		float*	m_TexCoords;
		int		m_TotalVertices;
	};

