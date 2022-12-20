#include <stdint.h>
#include "octoMap.h"

static coornidate_t rrtStarRandomNode(coornidate_t *freeNodes);
static uint8_t rrtStarCollisionCheck(coornidate_t *freeNodes, coornidate_t *node);
static coornidate_t *rrtStarPlan(coornidate_t *freeNodes, coornidate_t *startNode, coornidate_t *endNode);
static uint8_t rrtStarGetNearestListIndex(coornidate_t *freeNodes, coornidate_t *randomNewNode);
static void rrtStarPlanPath(coornidate_t *freeNodes, coornidate_t *startNode, coornidate_t *endNode);
