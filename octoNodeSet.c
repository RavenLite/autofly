#include "octoNodeSet.h"

void octoNodeSetInit(octoNodeSet_t *nodeSet)
{
    setIndex_t i;
    for (i = 0; i < NODE_SET_SIZE - 1; i++)
    {
        nodeSet->setData[i].next = i + 1;
    }
    nodeSet->setData[i].next = -1;
    nodeSet->freeQueueEntry = 0;
    nodeSet->fullQueueEntry = -1;
}

setIndex_t octoNodeSetMalloc(octoNodeSet_t *nodeSet)
{
    if (nodeSet->freeQueueEntry == -1)
    {
//        DEBUG_PRINT_OLSR_SET("Full of sets!!!! can not malloc!!!\n");
        return -1;
    }
    else
    {
        setIndex_t candidate = nodeSet->freeQueueEntry;
        nodeSet->freeQueueEntry = nodeSet->setData[candidate].next;
        // insert to full queue
        setIndex_t tmp = nodeSet->fullQueueEntry;
        nodeSet->fullQueueEntry = candidate;
        nodeSet->setData[candidate].next = tmp;
        return candidate;
    }
}

BOOL octoNodeSetFree(octoNodeSet_t *nodeSet,
                                setIndex_t delItem)
{
    if (-1 == delItem)
        return TRUE;
    // del from full queue
    setIndex_t pre = nodeSet->fullQueueEntry;
    if (delItem == pre)
    {
        nodeSet->fullQueueEntry = nodeSet->setData[pre].next;
        // insert to empty queue
        nodeSet->setData[delItem].next = nodeSet->freeQueueEntry;
        nodeSet->freeQueueEntry = delItem;
        return TRUE;
    }
    else
    {
        while (pre != -1)
        {
            if (nodeSet->setData[pre].next == delItem)
            {
                nodeSet->setData[pre].next = nodeSet->setData[delItem].next;
                // insert to empty queue
                nodeSet->setData[delItem].next = nodeSet->freeQueueEntry;
                nodeSet->freeQueueEntry = delItem;
                return TRUE;
            }
            pre = nodeSet->setData[pre].next;
        }
    }
    return FALSE;
}