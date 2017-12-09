#ifndef __ELAR_OPERATOR__
#define __ELAR_OPERATOR__

#include "structure.h"
#include <vector>
#include <iostream>

using namespace std;

class ElarOperator
{
public:
	void addEdgeIntoSolid(Edge *edge, Solid *&solid);
	void addFaceIntoSolid(Face *face, Solid *&solid);
	void addLoopIntoFace(Loop *loop, Face *face);
	Solid *mvfs(double point[3], Vertex *&vertex);
	HalfEdge *mev(Vertex *sv, double point[3], Loop *lp);
	Loop *mef(Vertex *sv, Vertex *ev, Loop *lp, bool mark);
	Loop *kemr(Vertex *sv, Vertex *ev, Loop *lp);
	void kfmrh(Face *fa, Face *fb);
	void sweep(double dir[3], double dist);
	void sweep(Solid* solid,double dir[3], double dist);

};

Solid *ElarOperator::mvfs(double point[3], Vertex *&vertex)
{
	Solid *solid = new Solid();
	Face *face = new Face();
	Loop *out_lp = new Loop();
	vertex = new Vertex(point[0], point[1], point[2]);

	vertex->id = solid->vnum;
	out_lp->id = solid->lnum;
	face->id = solid->fnum;

	solid->vnum += 1;
	solid->fnum += 1;
	solid->lnum += 1;


	solid->faces = face;
	face->solid = solid;

	face->out_lp = out_lp;
	out_lp->face = face;

	return solid;
}


HalfEdge *ElarOperator::mev(Vertex *sv, double point[3], Loop *loop)
{
	Solid *solid = loop->face->solid;
	Edge *edge = new Edge();
	HalfEdge *half_l = new HalfEdge();
	HalfEdge *half_r = new HalfEdge();
	Vertex *ev = new Vertex(point[0], point[1], point[2]);

	ev->id = solid->vnum;
	solid->vnum += 1;

	half_l->sv = sv;
	half_l->ev = ev;
	half_r->sv = ev;
	half_r->ev = sv;

	edge->half_l = half_l;
	edge->half_r = half_r;
	half_l->edge = edge;
	half_r->edge = edge;

	half_r->brother = half_l;
	half_l->brother = half_r;

	half_l->lp = loop;
	half_r->lp = loop;

	//add the new two halfedges into the loop
	if (loop->halfedges == NULL)
	{
		half_l->next = half_r;
		half_r->next = half_l;

		half_l->pre = half_r;
		half_r->pre = half_l;
		loop->halfedges = half_l;
	}
	else
	{
		HalfEdge *thalf = loop->halfedges;
		while (thalf->ev != sv)thalf = thalf->next;
		half_r->next = thalf->next;
		thalf->next->pre = half_r;
		thalf->next = half_l;
		half_l->pre = thalf;
		half_l->next = half_r;
		half_r->pre = half_l;
	}

	//add the edge into the edge list of solid
	addEdgeIntoSolid(edge, solid);
	return half_l;
}

Loop *ElarOperator::mef(Vertex *sv, Vertex *ev, Loop *loop, bool mark)
{
	Solid *solid = loop->face->solid;
	Edge *edge = new Edge();
	HalfEdge *half_l = new HalfEdge();
	HalfEdge *half_r = new HalfEdge();
	Loop *newLoop = new Loop();

	half_l->sv = sv;
	half_l->ev = ev;
	half_r->sv = ev;
	half_r->ev = sv;

	half_r->brother = half_l;
	half_l->brother = half_r;

	half_l->edge = edge;
	half_r->edge = edge;
	edge->half_l = half_l;
	edge->half_r = half_r;


	//add the new two halfedge into the loop
	HalfEdge *thalf = loop->halfedges;
	HalfEdge *tmpa, *tmpb, *tmpc;
	while (thalf->ev != sv)thalf = thalf->next;
	tmpa = thalf;

	while (thalf->ev != ev)thalf = thalf->next;
	tmpb = thalf;

	thalf = thalf->next;
	while (thalf->ev != ev)thalf = thalf->next;
	tmpc = thalf;

	//divide the big loop into two small loop
	half_r->next = tmpa->next;
	tmpa->next->pre = half_r;
	tmpa->next = half_l;
	half_l->pre = tmpa;

	half_l->next = tmpb->next;
	tmpb->next->pre = half_l;
	tmpb->next = half_r;
	half_r->pre = tmpb;
	loop->halfedges = half_l;
	newLoop->halfedges = half_r;
	half_l->lp = loop;
	half_r->lp = newLoop;

	Face *face = new Face();

	newLoop->id = solid->lnum;
	solid->lnum += 1;

	addFaceIntoSolid(face, solid);

	addLoopIntoFace(newLoop, face);

	addEdgeIntoSolid(edge, solid);

	return loop;
}

Loop *ElarOperator::kemr(Vertex *sv, Vertex *ev, Loop *loop)
{
	HalfEdge *tmpa, *tmpb, *hal;
	Face *face = loop->face;
	Loop *inlp = new Loop();
	Solid *solid = loop->face->solid;

	hal = loop->halfedges;

	while (hal->sv != sv || hal->ev != ev)hal = hal->next;
	tmpa = hal;

	while (hal->sv != ev || hal->ev != sv)hal = hal->next;
	tmpb = hal;

	tmpb->pre->next = tmpa->next;
	tmpa->pre->next = tmpb->next;

	loop->face->solid->faces->out_lp->halfedges = tmpa->pre;

	inlp->halfedges = tmpb->pre;
	tmpb->pre->lp = inlp; 

	inlp->id = solid->lnum;
	solid->lnum += 1;

	addLoopIntoFace(inlp, face);
	delete tmpa;
	delete tmpb;

	return NULL;
}

void ElarOperator::kfmrh(Face *fa, Face *fb)
{
	Loop *loop = fb->out_lp;
	addLoopIntoFace(loop, fa);
	fa->solid->lnum -= 1;
	fa->solid->fnum -= 1;

	Solid *solid = fa->solid;
	Face *face = solid->faces;
	if (face == fb)
	{
		solid->faces = face->next;
	}
	else
	{
		Face *tf = face;
		while (face != fb && face != NULL)
		{
			tf = face;
			face = face->next;
		}
		tf->next = face->next;
	}
	//delete fb;
}

void ElarOperator::sweep(Solid* solid,double dir[3], double d)
{
	Vertex *startv, *nextv, *upv, *upprev;
	HalfEdge *he, *suphe, *uphe;
	double point[3];

	int num = 0;
	Face* f = solid->faces->next;
	while (f != NULL)
	{
		num++;
		f = f->next;
	}
	f = solid->faces->next;
	for (int i = 0; i < num;i++)
	{
		Loop *loop = f->out_lp;
		f = f->next;
		he = loop->halfedges;
		startv = he->sv;
		point[0] = startv->coordinate[0] + d*dir[0];
		point[1] = startv->coordinate[1] + d*dir[1];
		point[2] = startv->coordinate[2] + d*dir[2];


		suphe = mev(startv, point, loop);
		upprev = suphe->ev;
		he = he->next;
		nextv = he->sv;

		Loop *lp = loop;

		while (nextv != startv)
		{
			point[0] = nextv->coordinate[0] + d*dir[0];
			point[1] = nextv->coordinate[1] + d*dir[1];
			point[2] = nextv->coordinate[2] + d*dir[2];
			uphe = mev(nextv, point, lp);
			upv = uphe->ev;

			lp = mef(upprev, upv, loop, false);

			upprev = upv;
			he = he->next;
			nextv = he->sv;
		}
		mef(upprev, suphe->ev, loop, false);
	}
	f = solid->faces->next;
	Face* tmpf=f;
	for (int i = 1; i < num; i++)
	{
		f = f->next;
		kfmrh(tmpf,f);
		//addLoopIntoFace(f->out_lp, tmpf);
		//tmpf->next = f->next;
	}
}
inline void ElarOperator::addEdgeIntoSolid(Edge *edge, Solid *&solid)
{
	Edge *te = solid->edges;

	if (te == NULL)solid->edges = edge;
	else{
		while (te->next != NULL)te = te->next;
		te->next = edge;
		edge->pre = te;
	}
}

inline void ElarOperator::addFaceIntoSolid(Face *face, Solid *&solid)
{
	Face *tface = solid->faces;
	if (tface == NULL)
	{
		solid->faces = face;
	}
	else
	{
		while (tface->next != NULL)tface = tface->next;
		tface->next = face;
		face->pre = tface;
	}
	face->solid = solid;

	face->id = solid->fnum;

	solid->fnum += 1;// increase the num of faces
}

inline void ElarOperator::addLoopIntoFace(Loop *loop, Face *face)
{
	loop->face = face;

	//there is only one out loop but there may have lots of inner loop
	if (face->out_lp == NULL)
	{
		face->out_lp = loop;
	}
	else
	{
		Loop *tlp = face->inner_lp;
		if (tlp == NULL)face->inner_lp = loop;
		else
		{
			while (tlp->next != NULL)tlp = tlp->next;
			tlp->next = loop;
			loop->pre = tlp;
		}
		face->innum += 1;
	}
}

#endif
