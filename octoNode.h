#ifndef __OCTONODE_H__
#define __OCTONODE_H__
#include <stdint.h>
#include <stdbool.h>
#include "coordinate.h"
#include "octoMap.h"

#define BOOL int

typedef struct octoNode_t
{
    struct octoNode_t *children[8];
    coornidate_t origin;
    uint16_t children_temp : 12; // first child node index (the following 7 children are in order, rft, rbt, lbt, lft, rfn, rbn, lbn, lfn)
    uint16_t logOdds : 3;   // occupation probability level
    uint16_t isLeaf : 1;    // whether is leaf node
} octoNode_t;

void octoNodeInit(octoNode_t *node);
uint8_t octoNodeGetProbability(octoNode_t *octoNode);
BOOL octoNodeIsLeaf(octoNode_t *octoNode);
BOOL octoNodeHasChildren(octoNode_t *octoNode);
uint8_t octoNodeGetChildren(octoNode_t *octoNode);
void octoNodeSplit(octoNode_t *octoNode, octoMap_t *octoMap);
void octoNodePrune(octoNode_t *octoNode, octoMap_t *octoMap);

uint8_t octoNodeIndex(octoNode_t *octoNode, coornidate_t *point, coornidate_t origin, uint16_t width);
BOOL octoNodeContains(octoNode_t *octoNode, coornidate_t point, uint16_t width);
coornidate_t calOrigin(uint8_t index, coornidate_t origin, uint16_t width);
coornidate_t octoNodeGetOrigin(octoNode_t *octoNode);
void octoNodeUpdate(octoNode_t *octoNode, octoMap_t *octoMap, coornidate_t *point, uint8_t diffLogOdds, coornidate_t origin, uint16_t width, uint8_t maxDepth);
BOOL octoNodeCheckChildrenLogOdds(octoNode_t *octoNode);
void octoNodeUpdateLogOdds(octoNode_t *octoNode, uint8_t diffLogOdds);
uint8_t octoNodeGetLogOdds(octoNode_t *octoNode);
uint8_t octoNodeLogOddsAt(octoNode_t *octoNode, coornidate_t *point, coornidate_t origin, uint16_t width);

#endif
