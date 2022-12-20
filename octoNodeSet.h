#ifndef __OCTONODESET_H__
#define __OCTONODESET_H__

#include "octoNode.h"

#define NODE_SET_SIZE 10000
#define BOOL int
#define TRUE 1
#define FALSE 0

typedef short setIndex_t;

typedef struct
{
    octoNode_t data; // data of the item
    setIndex_t next; // next item index
} octoNodeSetItem_t;

typedef struct
{
    octoNodeSetItem_t setData[NODE_SET_SIZE];     // data set
    setIndex_t freeQueueEntry;                    // first free item index
    setIndex_t fullQueueEntry;                    // first full item index
} octoNodeSet_t;

void octoNodeSetInit(octoNodeSet_t *codeSet);                     // initialize the set
setIndex_t octoNodeSetMalloc(octoNodeSet_t *nodeSet);             // malloc 8 items
BOOL octoNodeSetFree(octoNodeSet_t *nodeSet, setIndex_t delItem); // free 8 items
#endif