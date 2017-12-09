#ifndef __Construct__
#define __Construct__

#include "GluTess.h"

void construct_out(ElarOperator &op, Solid* &any_obj, double point[][3], int num)
	{

		Vertex *vertex;
		HalfEdge* he;
		any_obj = op.mvfs(&point[0][0], vertex);
		he = op.mev(vertex, &point[1][0], any_obj->faces->out_lp);
		for (int i = 2; i < num; i++)
		{
			he = op.mev(he->ev, &point[i][0], any_obj->faces->out_lp);
		}
		op.mef(he->ev, vertex, any_obj->faces->out_lp, false);
	}
void construct_inner(ElarOperator &op, Solid* &any_obj, double point[][3], int num)
	{
		Loop *lp = any_obj->faces->out_lp;
		Vertex *vertex = any_obj->faces->out_lp->halfedges->ev;
		HalfEdge* he;
		he = op.mev(vertex, &point[0][0], any_obj->faces->out_lp);
		for (int i = 1; i < num; i++)
		{
			he = op.mev(he->ev, &point[i][0], any_obj->faces->out_lp);
		}
		Vertex *vertex2 = any_obj->faces->out_lp->halfedges->next->ev;
		op.mef(he->ev, vertex2, any_obj->faces->out_lp, false);
		op.kemr(vertex, vertex2, lp);
	}

void face_show(Face* f)
	{
		HalfEdge* he , *org;
		double c = 0;
		c += 0.05;
		glColor3f(1, c, 1 - c);
		if (f->inner_lp == NULL)
		{
			GLUtesselator* tobj = gluNewTess();
			if (!tobj) return;

			gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
			gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
			gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
			gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
			gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);

			gluTessBeginPolygon(tobj, NULL);

			he = f->out_lp->halfedges;
			org = he;
			gluTessBeginContour(tobj);
			do{
				gluTessVertex(tobj, he->sv->coordinate, he->sv->coordinate);
				he = he->next;
			} while (org != he);
			gluTessEndContour(tobj);

			gluTessEndPolygon(tobj);
			gluDeleteTess(tobj);

		}
		else
		{
			GLUtesselator* tobj = gluNewTess();
			if (!tobj) return;

			gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
			gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
			gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
			gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
			gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);

			gluTessBeginPolygon(tobj, NULL);

			he = f->out_lp->halfedges;
			org = he;
			gluTessBeginContour(tobj);
			do{
				gluTessVertex(tobj, he->sv->coordinate, he->sv->coordinate);
				he = he->next;
			} while (org != he);
			gluTessEndContour(tobj);

			Loop* inner = f->inner_lp;
			while (inner != NULL)
			{
				he = inner->halfedges;
				org = he;
				gluTessBeginContour(tobj);
				do{
					gluTessVertex(tobj, he->sv->coordinate, he->sv->coordinate);
					he = he->next;
				} while (org != he);
				gluTessEndContour(tobj);
				inner = inner->next;
			}

			gluTessEndPolygon(tobj);
			gluDeleteTess(tobj);

		}
		he = f->out_lp->halfedges;
		org = he;
		do{
			glLineWidth(2);
			glColor3f(0.5, 0.6, 1);
			glBegin(GL_LINES);
			glVertex3f(he->sv->coordinate[0], he->sv->coordinate[1], he->sv->coordinate[2]);
			glVertex3f(he->ev->coordinate[0], he->ev->coordinate[1], he->ev->coordinate[2]);
			glEnd();
			he = he->next;
		} while (org != he);
	}


#endif