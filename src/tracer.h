#ifndef P0_TRACER_H
#define P0_TRACER_H

#include "structures.h"

bool getIntersection(prim_t &p, ray_t &r, double &t);

color_t getColor(int x, int y, scene_t &scene); // return pixel color, also calculates initial ray parameters

color_t trace(ray_t &ray, scene_t &scene); // traces a ray

#endif