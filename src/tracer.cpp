﻿#include "structures.h"
#include "util.h"
#include "tracer.h"

#include <cmath>

bool getIntersection(prim_t &p, ray_t &r, double &t)
{
	point_t PoPc;
	double B, D, x1, x2;

	//algoritm depends on primitive type
	switch(p.type)
	{

	case SPHERE: //sfera

		PoPc = r.src - p.sphere.center;
		B = r.dst * PoPc; 
		D = B*B - PoPc*PoPc + p.sphere.r*p.sphere.r; 

		if(D < 0.0f) return 0;

		x1 = -B - sqrt(D);
		x2 = -B + sqrt(D);

		if(x1 < x2)
		{
			if(t > x1) 
			{
				t = x1;     
				return 1;
			}
		}
		else
		{
			if(t > x1)
			{
				t = x2;
				return 1;
			}
		}
		return 0;
		
		break;

	case POLIGON: //poligono

		break;

	case CONSTRUCTOR: //costrutto = cosa brutta e cattiva

		break;

	}

	return 0; // se arriva qui è un male!
}

color_t getColor(int x, int y, scene_t &scene)
{
	color_t c = { 0.0f, 0.0f, 0.0f};

	double pixelSizeX = scene.screenSizeX/scene.screenResX;
	double pixelSizeY = scene.screenSizeY/scene.screenResY;

	ray_t ray = { {x * pixelSizeX, y * pixelSizeY, -1000.0f}, {0.0f, 0.0f, 1.0f}};
	norm(ray.dst);

	c = trace(ray, scene);

	return c;
}

color_t trace(ray_t &ray, scene_t &scene)
{
	color_t background = { 0.15f, 0.15f, 0.15f};
	color_t c = { 0.0f, 0.0f, 0.0f };
	prim_t *p = NULL;
	material_t *m = NULL;
	double t = 2000.0f;
	double temp;

	//get nearest intersection
	for(int i = 0; i < scene.primCount; i++)
		if(getIntersection(scene.prim[i], ray, t))
			p = &scene.prim[i];
			
	if(!p) return background; // no intersections

	//find material for primitive
	for(int i = 0; i < scene.matCount; i++)
		if(scene.material[i].mId == p->mat)
			m = &scene.material[i];

	if(!m) return background; // non existant material

	
	//calculation shadows
	vector_t intrPoint = ray.src + ray.dst * t;
	ray_t lightRay;
	bool inShadow;

	lightRay.src = intrPoint;
	vector_t normal = intrPoint - p->sphere.center; // normal in intersection point
	norm(normal);


	for(int i = 0; i < scene.lightCount; i++)
	{
		inShadow = false;
		lightRay.dst = scene.light[i] - intrPoint;
		if(normal * lightRay.dst <= 0.0f) continue;

		norm(lightRay.dst);

		for(int j = 0; j < scene.primCount; j++)
			if(getIntersection(scene.prim[i], lightRay, temp))
			{
				inShadow = true;
				break;
			}

		if(!inShadow)
		{
			double lambert = (lightRay.dst * normal);
			c = m->col * m->coefDiffuse * lambert;
		}
	}

	return c;
}