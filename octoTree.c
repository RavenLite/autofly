#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "octoTree.h"
#include "coordinate.h"

/**
 * @brief initialize an octoTree
 *
 * @param octoTree
 */
// static void octoTreeInit(octoTree_t *octoTree)
// {
//     octoTree->center = ;
//     octoTree->resolution = 4;
//     octoTree->maxDepth = 6;
//     octoTree->root = root;
// }

/**
 * @brief Get the radius of the octoTree
 *
 * @param octoTree self
 * @return uint16_t the radius of this tree (also width/2)
 */
uint16_t octoTreeRadis(octoTree_t *octoTree)
{
    return octoTree->resolution * pow(2, octoTree->maxDepth - 1);
}

/**
 * @brief
 *
 * @param octoTree self
 * @return uint16_t the width of this tree
 */
uint16_t octoTreeWidth(octoTree_t *octoTree)
{
    return octoTree->resolution * pow(2, octoTree->maxDepth);
}

/**
 * @brief Get the octoTree node at the given position
 *
 * @param octoTree self
 * @return coornidate_t the origin coordinate of this tree
 */
coornidate_t octoTreeOrigin(octoTree_t *octoTree)
{
    uint16_t radis = octoTreeRadis(octoTree);
    coornidate_t origin = {octoTree->center.x - radis, octoTree->center.y - radis, octoTree->center.z - radis};
    return origin;
}

/**
 * @brief Add an observation to the octomap.
 *
 * @param octoTree self
 * @param point the coordinate of the observation lidar point --- (x,y,z): tuple
 * @param diffLogOdds the difference value of logodds, 0: free, 1: occupied
 */
void octoTreeInsertPoint(octoTree_t *octoTree, octoMap_t *octoMap, coornidate_t *point)
{
    printf("[OctoTree.insert_point] point: (%d, %d, %d) diffLogOdds: %d\n", point->x, point->y, point->z, 1);
    octoNodeUpdate(octoTree->root, octoMap, point, 1, octoTree->origin, octoTree->width, octoTree->maxDepth);
}

/**
 * @brief Add the probability of the grid occupied by the ray path to the tree
 *
 * @param octoTree self
 * @param startPoint the coordinate of the sensor --- (x,y,z): tuple
 * @param endPoint the coordinate of the observation point  --- (x,y,z): tuple
 * @param diffLogOdds the difference value of logodds, 0: free, 1: occupied
 */
void octoTreeRayCasting(octoTree_t *octoTree, octoMap_t *octoMap, coornidate_t *startPoint, coornidate_t *endPoint, uint8_t diffLogOdds)
{
    // print parameters
    printf("[OctoTree.ray_casting] start_point: (%d, %d, %d) end_point: (%d, %d, %d) diffLogOdds: %d\n",
           startPoint->x, startPoint->y, startPoint->z,
           endPoint->x, endPoint->y, endPoint->z,
           diffLogOdds);
    // Insert occupancy voxel
    octoTreeInsertPoint(octoTree, octoMap, endPoint);
    return;
    // call bresenham algorithm
    coornidate_t *points;
    uint16_t pointsNum = bresenham(startPoint, endPoint, points);

    for (int i = 0; i < sizeof(points); i++)
    {
        octoNodeUpdate(octoTree->root, octoMap, &points[i], diffLogOdds, octoTree->origin, octoTree->width, octoTree->maxDepth);
    }
}

/**
 * @brief Return whether the point is contained in this tree.
 *
 * @param octoTree self
 * @param point coordinate of the point to check --- (x,y,z): tuple
 * @return true in
 * @return false not in
 */
BOOL octoTreeContains(octoTree_t *octoTree, coornidate_t point)
{
    uint16_t radius = octoTreeRadis(octoTree);
    return point.x >= octoTree->origin.x && point.x < octoTree->origin.x + radius &&
           point.y >= octoTree->origin.y && point.y < octoTree->origin.y + radius &&
           point.z >= octoTree->origin.z && point.z < octoTree->origin.z + radius;
}

/**
 * @brief Return the occupancy probability of the voxel at a given point coordinate.
 *
 * @param octoTree self
 * @param point coordinate of some voxel to get probability --- (x,y,z): tuple
 * @return uint8_t occupancy probability of the corresponding voxel
 */
uint8_t octoTreeGetLogProbability(octoTree_t *octoTree, coornidate_t *point)
{
    return octoNodeLogOddsAt(octoTree->root, point, octoTree->origin, octoTree->width);
}

/**
 * @brief Return the tree center.
 *
 * @param octoTree self
 * @return coornidate_t center of the tree --- (x,y,z): tuple
 */
coornidate_t octoTreeGetCenter(octoTree_t *octoTree)
{
    return octoTree->center;
}

/**
 * @brief Return the tree resolution.
 *
 * @param octoTree self
 * @return uint8_t resolution of the tree
 */
uint8_t octoTreeGetResolution(octoTree_t *octoTree)
{
    return octoTree->resolution;
}

/**
 * @brief Return the tree max depth.
 *
 * @param octoTree self
 * @return uint8_t max depth of the tree
 */
uint8_t octoTreeGetMaxDepth(octoTree_t *octoTree)
{
    return octoTree->maxDepth;
}

/**
 * @brief bresenham algorithm
 *
 * @param start the start point of the ray --- (x,y,z): tuple
 * @param end the end point of the ray --- (x,y,z): tuple
 * @param points the points on the ray path
 * @param length the length of the points
 */
uint16_t bresenham(coornidate_t *start, coornidate_t *end, coornidate_t *points)
{
    int16_t dx = abs(end->x - start->x);
    int16_t dy = abs(end->y - start->y);
    int16_t dz = abs(end->z - start->z);
    int16_t sx = start->x < end->x ? 1 : -1;
    int16_t sy = start->y < end->y ? 1 : -1;
    int16_t sz = start->z < end->z ? 1 : -1;
    int16_t err = dx - dy - dz;
    int16_t e2;
    uint16_t i = 0;

    while (1)
    {
        points[i] = *start;
        i++;
        if (start->x == end->x && start->y == end->y && start->z == end->z)
        {
            break;
        }
        e2 = 2 * err;
        if (e2 > -dx)
        {
            err = err - dy - dz;
            start->x = start->x + sx;
        }
        if (e2 < dx)
        {
            err = err + dy + dz;
            start->y = start->y + sy;
        }
        if (e2 < dy)
        {
            err = err + dy + dz;
            start->z = start->z + sz;
        }
    }
    return i;
}
