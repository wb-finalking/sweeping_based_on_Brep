#ifndef __HALF_EDGE_STRUCTURE__
#define __HALF_EDGE_STRUCTURE__

#include <stdlib.h>

struct Solid;
struct Face;
struct Loop;
struct HalfEdge;
struct Vertex;
struct Edge;

struct Solid
{
	int id;
	Face *faces; 
	Edge *edges; 
	Solid *next;
	Solid *pre;

	int vnum;
	int fnum;
	int lnum;


	Solid() : id(0), faces(NULL), edges(NULL), next(NULL), pre(NULL), fnum(0), vnum(0), lnum(0){}
};

struct Face
{
	int id;
	Solid *solid;
	Loop *out_lp; 
	Loop *inner_lp;
	Face *next;
	Face *pre;
	int innum;

	Face() : id(0), solid(NULL), out_lp(NULL), next(NULL), pre(NULL), inner_lp(NULL), innum(0){}
};

struct Loop
{
	int id;
	HalfEdge *halfedges; 
	Face *face; 
	Loop *next;
	Loop *pre;

	Loop() : id(0), halfedges(NULL), face(NULL), next(NULL), pre(NULL){}
};

struct Edge
{
	HalfEdge *half_l; 
	HalfEdge *half_r;
	Edge *next;
	Edge *pre;

	Edge() : half_l(NULL), half_r(NULL), next(NULL), pre(NULL){}
};

struct HalfEdge
{
	Edge *edge;
	Vertex *sv; 
	Vertex *ev; 
	Loop *lp; 
	HalfEdge *next;
	HalfEdge *pre;
	HalfEdge *brother;

	HalfEdge() : edge(NULL), sv(NULL), lp(NULL), next(NULL), pre(NULL), brother(NULL){}
};

struct Vertex
{
	int id;
	double coordinate[3];
	Vertex *next;
	Vertex *pre;

	Vertex(double x, double y, double z) : id(0), next(NULL), pre(NULL)
	{
		coordinate[0] = x;
		coordinate[1] = y;
		coordinate[2] = z;
	}
};

#endif
