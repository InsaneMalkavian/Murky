#pragma once
#include <string>

using namespace std;

static const size_t F_VA_POSITION		= 1<<0;
static const size_t F_VA_NORMAL0		= 1<<1;
static const size_t F_VA_NORMAL1		= 1<<2;
static const size_t F_VA_UV0			= 1<<3;
static const size_t F_VA_UV1			= 1<<4;
static const size_t F_VA_COLOR0			= 1<<5;
static const size_t F_VA_COLOR1			= 1<<6;
static const size_t F_VA_TANGENT		= 1<<7;
static const size_t F_VA_BINORMAL		= 1<<8;

// we can use these enum values as flag also
enum VertexAttr {
	VA_POSITION		= F_VA_POSITION,
	VA_NORMAL0		= F_VA_NORMAL0,
	VA_NORMAL1		= F_VA_NORMAL1,
	VA_UV0			= F_VA_UV0,
	VA_UV1			= F_VA_UV1,
	VA_COLOR0		= F_VA_COLOR0,
	VA_COLOR1		= F_VA_COLOR1,
	VA_TANGENT		= F_VA_TANGENT,
	VA_BINORMAL		= F_VA_BINORMAL
	};

class MeshLoader
	{
	public:
		explicit MeshLoader(void);
		~MeshLoader(void);
		bool	LoadOBJ(wstring fileName);
		bool	LoadDAE(wstring fileName);
		void	Optimize(void);
		size_t	GetTotalVerts()	const		{ return mTotalVertices; }
		size_t	GetPolyCount()	const		{ return mPolyCount; }
		size_t	GetTotalSize()	const		{ return mTotalVertices * mVertexSize; }
		size_t	GetVertexSize()	const		{ return mVertexSize; }
		float*	VertexBuffer()	const		{ return mVertexBuffer; }
	private:
		void	Release();
		void	CreateData(VertexAttr attr, string str, size_t number);
		void	CreateVertexBuffer(void);
		// it analyzes string from collada file and returns one of possible attr type
		static VertexAttr GetVertexAttr(string str);
		// returns size of one vertex in floats!!!, based on mAttrFlag value
		size_t	GetVertexSizeInFloat(void) const;

		// buffers for each attr
		float*	mPositions;
		float*	mNormal0;
		float*	mNormal1;
		float*	mUV0;
		float*	mUV1;
		float*	mColor0;
		float*	mColor1;
		float*	mTangent;
		float*	mBinormal;

		// model specific info
		size_t	mTotalVertices;
		size_t	mPolyCount;
		string	mName;
		size_t	mAttrFlag;
		size_t	mVertexSize;

		size_t*	mRawIndexes;
		float*	mVertexBuffer;
		void*	mIndexBuffer;
	};

