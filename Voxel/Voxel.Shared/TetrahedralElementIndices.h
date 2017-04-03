#pragma once
struct TetrahedralElementIndices
{
public:
	TetrahedralElementIndices(int inA, int inB, int inC, int inD)
	{
		a = inA;
		b = inB;
		c = inC;
		d = inD;
	}

	int a;
	int b;
	int c;
	int d;
};

