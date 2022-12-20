#ifndef __OCTOTREE_H__
#define __OCTOTREE_H__
#include <stdint.h>
#include "octoNode.h"
#include "coordinate.h"
// #include "octoNodeSet.h"

#define BOOL int
#define TRUE 1
#define FALSE 0

typedef struct
{
    coornidate_t center;     // the coordinate of the center --- (x,y,z): tuple
    coornidate_t origin;     // the origin coordinate of this tree --- (x,y,z): tuple
    uint8_t resolution;      // resolution of the tree
    uint8_t maxDepth;        // max depth of the tree
    uint16_t width;          // width of the tree
    octoNode_t *root; // root node of the tree
    // octoNodeSetItem_t *root; // root node of the tree
} octoTree_t;

void octoTreeInit(octoTree_t *octoTree);
uint16_t octoTreeRadis(octoTree_t *octoTree);
uint16_t octoTreeWidth(octoTree_t *octoTree);
coornidate_t octoTreeOrigin(octoTree_t *octoTree);
void octoTreeInsertPoint(octoTree_t *octoTree, octoMap_t *octoMap, coornidate_t *point);
void octoTreeRayCasting(octoTree_t *octoTree, octoMap_t *octoMap, coornidate_t *startPoint, coornidate_t *endPoint, uint8_t diffLogOdds);
BOOL octoTreeContains(octoTree_t *octoTree, coornidate_t point);
uint8_t octoTreeGetLogProbability(octoTree_t *octoTree, coornidate_t *point);
coornidate_t octoTreeGetCenter(octoTree_t *octoTree);
uint8_t octoTreeGetResolution(octoTree_t *octoTree);
uint8_t octoTreeGetMaxDepth(octoTree_t *octoTree);
uint16_t bresenham(coornidate_t *startPoint, coornidate_t *endPoint, coornidate_t *points);

#endif
