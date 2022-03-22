#include <stdlib.h>
#include <time.h>

#include "defender.h"
#include "spy.h"
#include "position.h"
#include "direction.h"

/*----------------------------------------------------------------------------*/
/*                                  LOGIC MACROS                              */
/*----------------------------------------------------------------------------*/

#define SPY_USE_THRESHOLD 3
#define DIFFERENCE_RATIO 2
#define MOVE_DOWN_PROB 0.3
#define MOVE_UP_PROB 0.3

/*----------------------------------------------------------------------------*/
/*                                 PRIVATE FUNCTIONS                          */
/*----------------------------------------------------------------------------*/

bool d_rand_seed_set = false;

double d_random_between_0_1() {
    if (!d_rand_seed_set) {
        srand((unsigned int)time(NULL));
    }
    return ((double)rand()) / ((double)RAND_MAX);
}

/*----------------------------------------------------------------------------*/
/*                                PUBLIC FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

direction_t execute_defender_strategy(
    position_t defender_position,
    Spy attacker_spy
) {
    size_t spy_uses = get_spy_number_uses(attacker_spy);

    direction_t next_move;

    if (spy_uses > 0 && d_random_between_0_1() > 0.3) {
        // Using spy
        position_t enemy_position = get_spy_position(attacker_spy);

        // Horizontal movement
        if (enemy_position.i < defender_position.i) {
            next_move.i = -1; // up
        } else if (enemy_position.i > defender_position.i) {
            next_move.i =  1; // down
        } else {
            next_move.i =  0; // keep line
        }

        // Vertical movement
        if (enemy_position.j < defender_position.j) {
            next_move.j = -1; // left
        } else if (enemy_position.j > defender_position.j) {
            next_move.j =  1; // right
        } else {
            next_move.j =  0; // keep column
        }
    } else {
        // Not using spy

        next_move.j = -1; // always move left
        
        double up_down_prob = d_random_between_0_1();
        if (up_down_prob < MOVE_UP_PROB) {
            next_move.i = -1; // move up
        } else if (up_down_prob + MOVE_UP_PROB < MOVE_DOWN_PROB)  {
            next_move.i =  1; // move down
        } else {
            next_move.i =  0; // keep same line 
        }
    }

    return next_move;
}
