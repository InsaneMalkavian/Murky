#include "MeshLoader.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <irrXML.h>

using namespace irr;
using namespace io;
using namespace std;

static const string GEOMETRY			= "geometry";
static const string SOURCE				= "source";
static const string VERTICES			= "vertices";
static const string POLYGONS			= "polygons";
static const string FLOAT_ARRAY			= "float_array";
static const string INPUTPARAM			= "input";
static const string POLY				= "p";

MeshLoader::MeshLoader(void) : mPositions(0), mNormal0(0), mUV0(0), mNormal1(0), mUV1(0), mColor0(0), mColor1(0), mBinormal(0), mTangent(0), 
	mTotalVertices(0), mName(), mRawIndexes(NULL), mVertexBuffer(NULL), mIndexBuffer(NULL), mAttrFlag(0) {
	}

MeshLoader::~MeshLoader(void) {
	Release();
	}

void MeshLoader::Release()
{
    mTotalVertices = 0;

    if (mPositions != 0) delete[] mPositions;
    if (mNormal0 != 0) delete[] mNormal0;
	if (mNormal1 != 0) delete[] mNormal1;
    if (mUV0 != 0) delete[] mUV0;
	if (mUV1 != 0) delete[] mUV1;
	if (mColor0 != 0) delete[] mColor0;
	if (mColor1 != 0) delete[] mColor1;
	if (mTangent != 0) delete[] mTangent;
	if (mBinormal != 0) delete[] mBinormal;
	if (mRawIndexes != 0) delete[] mRawIndexes;
	//if (mVertexBuffer != 0) delete[] mVertexBuffer;
	//if (mIndexBuffer != 0) delete[] mIndexBuffer;

    mPositions = mNormal0 = mNormal1 = mColor0 = mColor1 = mUV0 = mUV1 = mTangent = mBinormal = 0;
	mRawIndexes = 0;
}

bool MeshLoader::LoadOBJ(wstring fileName) {
    Release();
	// @TODO: it returns plain only for now
	mTotalVertices = 4;
	mPositions = new float[mTotalVertices*3]; // xyz
	mNormal0 = new float[mTotalVertices*3]; // xyz
	mUV0 = new float[mTotalVertices*2]; // uv
	mPositions[0] = -0.5f; // 1st vertex
	mPositions[1] = -0.5f;
	mPositions[2] = -0.5f;
	mPositions[3] = 0.5f; // 2nd vertex
	mPositions[4] = -0.5f;
	mPositions[5] = -0.5f;
	mPositions[6] = 0.5f; // 3rd vertex
	mPositions[7] = 0.5f;
	mPositions[8] = -0.5f;
	mPositions[9] = -0.5f; // 4th vertex
	mPositions[10] = 0.5f;
	mPositions[11] = -0.5f;
	
	ZeroMemory(mNormal0, mTotalVertices*3);
	ZeroMemory(mUV0, mTotalVertices*2);	
    return true;
}

bool MeshLoader::LoadDAE(wstring fileName) {
    Release();
	IrrXMLReader* xml = createIrrXMLReader("cube12.dae");

	// parse the file until end reached
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT:
			{
			string name = xml->getNodeName();
			if (name==GEOMETRY) {
				int count = xml->getAttributeCount();
				if (count>0) {
					mName = xml->getAttributeValue(0);
					}
				}
			else if (name==FLOAT_ARRAY) {
				// here we load positions/normals/etc separatly
				int count = xml->getAttributeCount();
				if (count>1) {
					string type = xml->getAttributeValue(0);
					int number = xml->getAttributeValueAsInt(1);
					xml->read();
					if (xml->getNodeType()==EXN_TEXT) {
						string puredata = xml->getNodeData();
						// see what kind of data we process: pos/normals/texcoord/etc
						VertexAttr attr = GetVertexAttr(type);
						CreateData(attr, puredata, number);
						}
					}
				}
			else if (name==VERTICES) {
				int count = xml->getAttributeCount();
				}
			else if (name==POLYGONS) {
				int count = xml->getAttributeCount();
				if (count>0) {
					mPolyCount = xml->getAttributeValueAsInt(0);
					mTotalVertices = mPolyCount*3;
					// number of vertex attributes (pos,nor,uv,etc)
					int attrCount = 0;
					name = xml->getNodeName();
					// count attrs untill we reach POLY block
					while (name!=POLY)
					{
						xml->read();
						name=xml->getNodeName();
						if (name==INPUTPARAM)
							attrCount++;
					}

					// now we know how much memory it needs
					mRawIndexes = new size_t[attrCount*mPolyCount*3]; // 3 vertices per 1 poly, attrCount per 1 vertex
					ZeroMemory(mRawIndexes, sizeof(size_t)*attrCount*mPolyCount*3);

					int ptr=0; // fill buffer with data
					while (name==POLY)
						{
						name=xml->getNodeName();
						xml->read();
						string rawIndexes=xml->getNodeData();
						stringstream ss(rawIndexes);
						while (!(ss >> mRawIndexes[ptr]).fail()) {ptr++;}
						xml->read();
						}
					}
				}
			}
			break;
		}
	}

	// delete the xml parser after usage
	delete xml;
	CreateVertexBuffer();
    return true;
}

VertexAttr MeshLoader::GetVertexAttr(string str) {
	int found = str.find("Position");
	if (found!=string::npos)
		return VA_POSITION;
	found = str.find("Normal0");
	if (found!=string::npos)
		return VA_NORMAL0;
	found = str.find("Normal1");
	if (found!=string::npos)
		return VA_NORMAL1;
	found = str.find("UV0");
	if (found!=string::npos)
		return VA_UV0;
	found = str.find("UV1");
	if (found!=string::npos)
		return VA_UV1;
	return VA_POSITION; // we shouldn't be here, this is error
	}

void MeshLoader::CreateData(VertexAttr attr, string str, size_t number) {

	float* buffer = new float[number];
	size_t ptr=0;
	stringstream ss(str);
	while (!(ss >> buffer[ptr]).fail()) {ptr++;}
	mAttrFlag += attr;
	switch (attr)
	{
	case VA_POSITION:
		mPositions = buffer;
		break;
	case VA_NORMAL0:
		mNormal0 = buffer;
		break;
	case VA_NORMAL1:
		mNormal1 = buffer;
		break;
	case VA_UV0:
		mUV0 = buffer;
		break;
	case VA_UV1:
		mUV1 = buffer;
		break;
	default: // delete buffer if it doesnt match any attr
		delete[] buffer;
	}

	return;
}

void MeshLoader::CreateVertexBuffer() {
	size_t vertexSize = GetVertexSizeInFloat();
	mVertexSize = vertexSize * sizeof(float);
	size_t bufSize = vertexSize*mTotalVertices;
	size_t ptrRaw=0;
	mVertexBuffer = new float[bufSize];
	for (size_t i=0; i<mTotalVertices; i++)
	{
		size_t offset=0;
		if (mAttrFlag&VA_POSITION)
		{
			// @TODO: do memcpy instead of this shit
			mVertexBuffer[i*vertexSize+0]=mPositions[mRawIndexes[ptrRaw]*3+0];
			mVertexBuffer[i*vertexSize+1]=mPositions[mRawIndexes[ptrRaw]*3+1];
			mVertexBuffer[i*vertexSize+2]=mPositions[mRawIndexes[ptrRaw]*3+2];
			offset+=3;
			ptrRaw++;
		}
		if (mAttrFlag&VA_NORMAL0)
		{
			mVertexBuffer[i*vertexSize+offset+0]=mNormal0[mRawIndexes[ptrRaw]*3+0];
			mVertexBuffer[i*vertexSize+offset+1]=mNormal0[mRawIndexes[ptrRaw]*3+1];
			mVertexBuffer[i*vertexSize+offset+2]=mNormal0[mRawIndexes[ptrRaw]*3+2];
			offset+=3;
			ptrRaw++;
		}
		if (mAttrFlag&VA_UV0)
		{
			mVertexBuffer[i*vertexSize+offset+0]=mUV0[mRawIndexes[ptrRaw]*2+0];
			mVertexBuffer[i*vertexSize+offset+1]=mUV0[mRawIndexes[ptrRaw]*2+1];
			offset+=2;
			ptrRaw++;
		}
		if (mAttrFlag&VA_COLOR0)
		{
			mVertexBuffer[i*vertexSize+offset+0]=mColor0[mRawIndexes[ptrRaw]*3+0];
			mVertexBuffer[i*vertexSize+offset+1]=mColor0[mRawIndexes[ptrRaw]*3+1];
			mVertexBuffer[i*vertexSize+offset+2]=mColor0[mRawIndexes[ptrRaw]*3+1];
			offset+=3;
			ptrRaw++;
		}
		if (mAttrFlag&VA_NORMAL1)
		{
			mVertexBuffer[i*vertexSize+offset+0]=mNormal1[mRawIndexes[ptrRaw]*3+0];
			mVertexBuffer[i*vertexSize+offset+1]=mNormal1[mRawIndexes[ptrRaw]*3+1];
			mVertexBuffer[i*vertexSize+offset+2]=mNormal1[mRawIndexes[ptrRaw]*3+2];
			offset+=3;
			ptrRaw++;
		}
		if (mAttrFlag&VA_UV1)
		{
			mVertexBuffer[i*vertexSize+offset+0]=mUV1[mRawIndexes[ptrRaw]*2+0];
			mVertexBuffer[i*vertexSize+offset+1]=mUV1[mRawIndexes[ptrRaw]*2+1];
			offset+=2;
			ptrRaw++;
		}
		if (mAttrFlag&VA_COLOR1)
		{
			mVertexBuffer[i*vertexSize+offset+0]=mColor1[mRawIndexes[ptrRaw]*3+0];
			mVertexBuffer[i*vertexSize+offset+1]=mColor1[mRawIndexes[ptrRaw]*3+1];
			mVertexBuffer[i*vertexSize+offset+2]=mColor1[mRawIndexes[ptrRaw]*3+2];
			offset+=3;
			ptrRaw++;
		}
		if (mAttrFlag&VA_TANGENT)
		{
			mVertexBuffer[i*vertexSize+offset+0]=mTangent[mRawIndexes[ptrRaw]*3+0];
			mVertexBuffer[i*vertexSize+offset+1]=mTangent[mRawIndexes[ptrRaw]*3+1];
			mVertexBuffer[i*vertexSize+offset+2]=mTangent[mRawIndexes[ptrRaw]*3+2];
			offset+=3;
			ptrRaw++;
		}
		if (mAttrFlag&VA_BINORMAL)
		{
			mVertexBuffer[i*vertexSize+offset+0]=mBinormal[mRawIndexes[ptrRaw]*3+0];
			mVertexBuffer[i*vertexSize+offset+1]=mBinormal[mRawIndexes[ptrRaw]*3+1];
			mVertexBuffer[i*vertexSize+offset+2]=mBinormal[mRawIndexes[ptrRaw]*3+2];
			offset+=3;
			ptrRaw++;
		}
	}
	return;
}

size_t MeshLoader::GetVertexSizeInFloat() const {
	size_t size=0;
	if (mAttrFlag&VA_POSITION)
		size+=3;
	if (mAttrFlag&VA_NORMAL0)
		size+=3;
	if (mAttrFlag&VA_NORMAL1)
		size+=3;
	if (mAttrFlag&VA_UV0)
		size+=2;
	if (mAttrFlag&VA_UV1)
		size+=2;
	if (mAttrFlag&VA_COLOR0)
		size+=3;
	if (mAttrFlag&VA_COLOR1)
		size+=3;
	if (mAttrFlag&VA_TANGENT)
		size+=3;
	if (mAttrFlag&VA_BINORMAL)
		size+=3;
	return size;
}