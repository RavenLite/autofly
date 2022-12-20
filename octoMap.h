#ifndef __OCTOMAP_H__
#define __OCTOMAP_H__
#include <stdint.h>
#include <stdbool.h>
#include "octoTree.h"
#include "octoNodeSet.h"

#define BOOL int
#define TRUE 1
#define FALSE 0

#define TREE_CENTER_X 128
#define TREE_CENTER_Y 128
#define TREE_CENTER_Z 128
#define TREE_RESOLUTION 4
#define TREE_MAX_DEPTH 6

typedef struct
{
    octoTree_t *octoTree;
    octoNodeSet_t *octoNodeSet;
//    uint8_t octoNodeSetSize;
//    uint8_t octoNodeSetItemSize;
//    uint8_t octoNodeSetItemNumber;
} octoMap_t;

void octoMapInit(octoMap_t *octoMap, octoTree_t *octoTree);

#endif
