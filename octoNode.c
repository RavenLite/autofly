#include <stdio.h>
#include <math.h>
#include "octoNode.h"
#define BOOL int
#define TRUE 1
#define FALSE 0

void octoNodeInit(octoNode_t *octoNode)
{
    // octoNode->children = {0};
    octoNode->logOdds = 3;
    octoNode->isLeaf = FALSE;
}

/**
 * @brief Return occupancy probability of node
 *
 * @param octoNode self
 * @return uint8_t occupancy probability of node
 */
uint8_t octoNodeGetProbability(octoNode_t *octoNode)
{
    return 1 / (1 + exp(-octoNode->logOdds));
}

/**
 * @brief whether this node is leaf
 *
 * @param octoNode self
 */
BOOL octoNodeIsLeaf(octoNode_t *octoNode)
{
    return octoNode->isLeaf;
}

/**
 * @brief whether this is a leaf node
 *
 * @param octoNode self
 */
BOOL octoNodeHasChildren(octoNode_t *octoNode)
{
    // return true if any child is not null
//    for (int i = 0; i < OctoNode_Children_Number; i++)
//    {
//        if (octoNode->children[i] != NULL)
//        {
//            return TRUE;
//        }
//    }
//    return octoNode->children != 0;
    return octoNode->isLeaf == FALSE;
}

/**
 * @brief Return node's childern
 *
 * @param octoNode
 * @return uint8_t node's childern address index
 */
uint8_t octoNodeGetChildren(octoNode_t *octoNode)
{
    return octoNode->children;
}

/**
 * @brief Splits the node into 8 child nodes.
 * Child nodes are given the occupancy probability of this parent node as the initial probability
 *
 * @param octoNode self
 */
void octoNodeSplit(octoNode_t *octoNode, octoMap_t *octoMap)
{
    // split 8 children nodes
    // TODO: dynamic memory allocation
    octoNode->children_temp = octoNodeSetMalloc(&octoMap->octoNodeSet);
    for (int i = 0; i < 8; i++)
    {
        octoNode->children[i] = octoNode;
    }
}

void octoNodePrune(octoNode_t *octoNode, octoMap_t *octoMap)
{
    octoNode->logOdds = octoNode->children[0]->logOdds;
    octoNode->isLeaf = 1;
    octoNodeSetFree(&octoMap->octoNodeSet, octoNode->children_temp);
    // free(octoNode->children);
    // octoNode->children = NULL;
}

/**
 * @brief Calculates the index of the child containing point.
 *
 * @param octoNode self
 * @param point the coornidate of the child node --- (x,y,z): tuple
 * @param origin the origin coornidate of the parent node -- (x,y,z): tuple
 * @param width the width of the parent node --- int
 * @return uint8_t the index of the child
 */
uint8_t octoNodeIndex(octoNode_t *octoNode, coornidate_t *point, coornidate_t origin, uint16_t width)
{
    // print parameters
//    printf("[OctoNode.Index] point: (%d, %d, %d)\n", point->x, point->y, point->z);
//    printf("[OctoNode.Index] origin: (%d, %d, %d)\n", origin.x, origin.y, origin.z);
//    printf("[OctoNode.Index] width: %d\n", width);
    uint8_t index = 0;
    uint8_t halfWidth = width / 2;
    if (point->x >= origin.x + halfWidth)
    {
        index += 1;
        origin.x += halfWidth;
    }
    if (point->y >= origin.y + halfWidth)
    {
        index += 2;
        origin.y += halfWidth;
    }
    if (point->z >= origin.z + halfWidth)
    {
        index += 4;
        origin.z += halfWidth;
    }
    return index;
}

/**
 * @brief Return whether the point is contained by this node --- bool
 *
 * @param point
 * @param origin
 * @param width
 * @return bool
 */
BOOL octoNodeContains(octoNode_t *octoNode, coornidate_t point, uint16_t width)
{
    return point.x >= octoNode->origin.x && point.x < octoNode->origin.x + width &&
           point.y >= octoNode->origin.y && point.y < octoNode->origin.y + width &&
           point.z >= octoNode->origin.z && point.z < octoNode->origin.z + width;
}

/**
 * @brief Calculates the origin of the node with given index.
 *
 * @param index
 * @param origin
 * @param width
 * @return coornidate_t
 */
coornidate_t calOrigin(uint8_t index, coornidate_t origin, uint16_t width)
{
//    printf("[OctoNode.calOrigin] origin: (%d, %d, %d)\n", origin.x, origin.y, origin.z);
    coornidate_t newOrigin = origin;
    uint8_t halfWidth = width / 2;
    if (index & 1)
    {
        newOrigin.x += halfWidth;
    }
    if (index & 2)
    {
        newOrigin.y += halfWidth;
    }
    if (index & 4)
    {
        newOrigin.z += halfWidth;
    }
//    printf("[OctoNode.calOrigin] newOrigin: (%d, %d, %d)\n", newOrigin.x, newOrigin.y, newOrigin.z);
    return newOrigin;
}

coornidate_t octoNodeGetOrigin(octoNode_t *octoNode)
{
    return octoNode->origin;
}

/**
 * @brief Updates the node with a new observation.
 * @param octoNode self
 * @param point the point coordinate of the observation --- (x,y,z): tuple
 * @param diffLogOdds the difference value of logodds --- int
 * @param origin origin of this node --- (x,y,z): tuple
 * @param width width of this node --- int
 * @param maxDepth maximum depth this node can be branched --- int
 */
void octoNodeUpdate(octoNode_t *octoNode, octoMap_t *octoMap, coornidate_t *point, uint8_t diffLogOdds, coornidate_t origin, uint16_t width, uint8_t maxDepth)
{
    printf("[OctoNode.update] point: (%d, %d, %d)\n", point->x, point->y, point->z);
    printf("[OctoNode.update] diffLogOdds: %d\n", diffLogOdds);
    printf("[OctoNode.update] origin: (%d, %d, %d)\n", origin.x, origin.y, origin.z);
    printf("[OctoNode.update] width: %d\n", width);
    printf("[OctoNode.update] maxDepth: %d\n", maxDepth);
    printf("[OctoNode.update] self.is_leaf: %d\n", octoNode->isLeaf);
    octoNode->origin = origin;
    if (maxDepth == 0)
    {
        printf("[OctoNode.update] maxDepth == 0\n");
        octoNodeUpdateLogOdds(octoNode, diffLogOdds);
        return;
    }
    else
    {
        if (!octoNodeHasChildren(octoNode)) {
            printf("[OctoNode.update] !octoNodeHasChildren(octoNode)\n");
            octoNodeSplit(octoNode, octoMap);
        }
        printf("[OctoNode.update] maxDepth != 0\n");
        uint8_t index = octoNodeIndex(octoNode, point, origin, width);
        printf("[OctoNode.update] childIndex: %d\n", index);
        coornidate_t newOrigin = calOrigin(index, origin, width);
        printf("[OctoNode.update] newOrigin: (%d, %d, %d)\n", newOrigin.x, newOrigin.y, newOrigin.z);
        octoNodeUpdate(octoNode->children[index], octoMap, point, diffLogOdds, newOrigin, width / 2, maxDepth - 1);
        return;
        if (maxDepth > 1)
        {
            BOOL allSame = TRUE;
            for (int i = 0; i < 8; i++)
            {
                if (octoNode->children[i]->logOdds != octoNode->children[index]->logOdds)
                {
                    allSame = FALSE;
                    break;
                }
            }
            if (allSame)
            {
                octoNodePrune(octoNode, octoMap);
            }
        }
    }
}

/**
 * @brief Return whether the logodds of all children are the same and arrive thresholds
 *
 * @param octoNode self
 * @return true
 * @return false
 */
BOOL octoNodeCheckChildrenLogOdds(octoNode_t *octoNode)
{
    uint8_t logOdds = octoNode->children[0]->logOdds;
    for (int i = 1; i < 8; i++)
    {
        if (octoNode->children[i]->logOdds != logOdds)
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * @brief Return the occupancy probability of the voxel at a given point coordinate.
 *
 * @param octoNode self
 * @param point coordinate of some voxel to get probability --- (x,y,z): tuple
 * @param origin the origin coornidate of the parent node -- (x,y,z): tuple
 * @param width the width of the parent node --- int
 * @return uint8_t occupancy probability of the corresponding voxel
 */
void octoNodeUpdateLogOdds(octoNode_t *octoNode, uint8_t diffLogOdds)
{
    if (octoNode->logOdds > 0 && diffLogOdds == 0) {
        octoNode->logOdds -= 1;
    } else if (octoNode->logOdds < 7 && diffLogOdds == 1) {
        octoNode->logOdds += 1;
    }
}

/**
 * @brief Return the occupancy probability of the voxel at a given point coordinate.
 *
 * @param octoNode self
 * @param point coordinate of some voxel to get probability --- (x,y,z): tuple
 * @param origin the origin coornidate of the parent node -- (x,y,z): tuple
 * @param width the width of the parent node --- int
 * @return uint8_t occupancy probability of the corresponding voxel
 */
uint8_t octoNodeGetLogOdds(octoNode_t *octoNode)
{
    return octoNode->logOdds;
}

/**
 * @brief Return the occupancy probability of the voxel at a given point coordinate.
 *
 * @param octoNode self
 * @param point coordinate of some voxel to get probability --- (x,y,z): tuple
 * @param origin the origin coornidate of the parent node -- (x,y,z): tuple
 * @param width the width of the parent node --- int
 * @return uint8_t occupancy probability of the corresponding voxel
 */
uint8_t octoNodeLogOddsAt(octoNode_t *octoNode, coornidate_t *point, coornidate_t origin, uint16_t width)
{
    if (octoNode->isLeaf)
    {
        return octoNode->logOdds;
    }
    else
    {
        uint8_t index = octoNodeIndex(octoNode, point, origin, width);
        coornidate_t newOrigin = calOrigin(index, origin, width);
        return octoNodeLogOddsAt(octoNode->children[index], point, newOrigin, width / 2);
    }
}
