/* octoMap.c: Do the mapping task */
#include <stdio.h>
#include <math.h>
#include "octoMap.h"
#include "octoNodeSet.h"

// OctoMap
void octoMapInit(octoMap_t *octoMap, octoTree_t *octoTree)
{
    octoMap->octoTree = octoTree;
}

/**
 * @brief Initialize the octoNodeSet
 * 
 * @param OctoNodeSet 
 */
void octoNodeSetInit(octoNodeSet_t *OctoNodeSet)
{
    OctoNodeSet->freeQueueEntry = 0;
    OctoNodeSet->fullQueueEntry = 0;
    for (int i = 0; i < NODE_SET_SIZE; i++)
    {
        OctoNodeSet->setData[i].next = i + 1;
    }
    OctoNodeSet->setData[NODE_SET_SIZE - 1].next = -1;
}

/**
 * @brief Malloc 8 items
 * 
 * @param OctoNodeSet all the items
 * @return setIndex_t the first item index
 */
setIndex_t octoNodeSetMalloc(octoNodeSet_t *OctoNodeSet)
{
    setIndex_t itemIndex = OctoNodeSet->freeQueueEntry;
    OctoNodeSet->freeQueueEntry = OctoNodeSet->setData[itemIndex].next;
    OctoNodeSet->setData[itemIndex].next = OctoNodeSet->fullQueueEntry;
    OctoNodeSet->fullQueueEntry = itemIndex;
    return itemIndex;
}

/**
 * @brief Free 8 items
 * 
 * @param OctoNodeSet all the items
 * @param delItem delete item index
 * @return true 
 * @return false 
 */
BOOL octoNodeSetFree(octoNodeSet_t *OctoNodeSet, setIndex_t delItem)
{
    if (OctoNodeSet->fullQueueEntry == delItem) {
        OctoNodeSet->fullQueueEntry = OctoNodeSet->setData[delItem].next;
        OctoNodeSet->setData[delItem].next = OctoNodeSet->freeQueueEntry;
        OctoNodeSet->freeQueueEntry = delItem;
        return TRUE;
    } else {
        setIndex_t itemIndex = OctoNodeSet->fullQueueEntry;
        while (OctoNodeSet->setData[itemIndex].next != delItem) {
            itemIndex = OctoNodeSet->setData[itemIndex].next;
            if (itemIndex == -1) {
                return FALSE;
            }
        }
        OctoNodeSet->setData[itemIndex].next = OctoNodeSet->setData[delItem].next;
        OctoNodeSet->setData[delItem].next = OctoNodeSet->freeQueueEntry;
        OctoNodeSet->freeQueueEntry = delItem;
        return TRUE;
    }
}

int main() 
{
    // init
    octoNode_t root = {
        .children = 0,
        .logOdds = 3,
        .isLeaf = FALSE
    };
    octoTree_t octoTree = {
        .center = {
            .x = TREE_CENTER_X,
            .y = TREE_CENTER_Y,
            .z = TREE_CENTER_Z,
        },
        .origin = {
            .x = TREE_CENTER_X - TREE_RESOLUTION * (1 << TREE_MAX_DEPTH) / 2,
            .y = TREE_CENTER_Y - TREE_RESOLUTION * (1 << TREE_MAX_DEPTH) / 2,
            .z = TREE_CENTER_Z - TREE_RESOLUTION * (1 << TREE_MAX_DEPTH) / 2,
        },
        .resolution = TREE_RESOLUTION,
        .maxDepth = TREE_MAX_DEPTH,
        .width = TREE_RESOLUTION * pow(2, TREE_MAX_DEPTH),
        .root = &root
    };
    octoNodeSet_t nodeSet;
    octoNodeSetInit(&nodeSet);
    // test node set address
    printf("nodeSet address: %p\n", &nodeSet);
    setIndex_t itemIndex = octoNodeSetMalloc(&nodeSet);
    printf("itemIndex: %d\n", itemIndex);
    setIndex_t itemIndex2 = octoNodeSetMalloc(&nodeSet);
    printf("itemIndex2: %d\n", itemIndex2);
    octoNodeSetFree(&nodeSet, itemIndex);
    setIndex_t itemIndex3 = octoNodeSetMalloc(&nodeSet);
    printf("itemIndex3: %d\n", itemIndex3);
    octoNodeSetFree(&nodeSet, itemIndex2);
    setIndex_t itemIndex4 = octoNodeSetMalloc(&nodeSet);
    printf("itemIndex4: %d\n", itemIndex4);
    octoMap_t octoMap = {
        .octoTree = &octoTree,
        .octoNodeSet = &nodeSet,
    };
    // test raycasting
    coornidate_t start = {
        .x = 0,
        .y = 0,
        .z = 0,
    };
    coornidate_t *startPtr = &start;
    coornidate_t end = {
        .x = 64,
        .y = 32,
        .z = 48,
    };
    coornidate_t *endPtr = &end;
    // print octoMap
    printf("octoMap.octoTree->center = (%d, %d, %d)\n", octoMap.octoTree->center.x, octoMap.octoTree->center.y, octoMap.octoTree->center.z);
    printf("octoMap.octoTree->origin = (%d, %d, %d)\n", octoMap.octoTree->origin.x, octoMap.octoTree->origin.y, octoMap.octoTree->origin.z);
    printf("octoMap.octoTree->resolution = %d\n", octoMap.octoTree->resolution);
    printf("octoMap.octoTree->maxDepth = %d\n", octoMap.octoTree->maxDepth);
    printf("octoMap.octoTree->width = %d\n\n", octoMap.octoTree->width);

    octoTreeRayCasting(octoMap.octoTree, startPtr, endPtr, 1);
    return 0;
}
