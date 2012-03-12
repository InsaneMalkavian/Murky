#pragma once
#include <xnamath.h>

struct VerPC {
	XMFLOAT3 Pos;
	XMFLOAT3 Color;
	};

struct VerPT {
	XMFLOAT3 Pos;
	XMFLOAT2 Tex1;
	};

// @TODO: these types should be refactored
struct SimpleVertex {
    XMFLOAT3 Pos;
};