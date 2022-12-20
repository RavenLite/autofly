#include "rrtStar.h"

/**
 * @brief Randomly pick from free_nodes and return
 * 
 * @param freeNodes 
 * @return * Randomly 
 */
static coornidate_t rrtStarRandomNode(coornidate_t *freeNodes) {
    uint16_t randomIndex = rand() % freeNodes->length;
    return freeNodes[randomIndex];
}

/**
 * @brief Check if the node is in collision with the map
 * 
 * @param freeNodes 
 * @param node 
 * @return uint8_t
 */
static uint8_t rrtStarCollisionCheck(coornidate_t *occuNodes, coornidate_t *node)
{
    // The space occupied by the obstacle
    uint8_t size = 1  
    for (uint16_t i = 0; i < occuNodes->length; i++)
    {
        uint8_t distance_x = abs(occuNodes[i].x - node->x);
        uint8_t distance_y = abs(occuNodes[i].y - node->y);
        uint8_t distance_z = abs(occuNodes[i].z - node->z);
        uint8_t distance = sqrt(pow(distance_x, 2) + pow(distance_y, 2) + pow(distance_z, 2));
        // collision
        if (distance < size)
        {
            return 0;
        }
    }
    return 1;
}

static coornidate_t *rrtStarPlan(coornidate_t *freeNodes, coornidate_t *startNode, coornidate_t *endNode)
{

}
