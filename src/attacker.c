#include <stdlib.h>
#include <time.h>

#include "attacker.h"
#include "spy.h"
#include "position.h"
#include "direction.h"

/*----------------------------------------------------------------------------*/
/*                                  LOGIC MACROS                              */
/*----------------------------------------------------------------------------*/

#define SPY_USE_THRESHOLD 4
#define DIFFERENCE_RATIO 2
#define MOVE_DOWN_PROB 0.2
#define MOVE_UP_PROB 0.2

/*----------------------------------------------------------------------------*/
/*                                 PRIVATE FUNCTIONS                          */
/*----------------------------------------------------------------------------*/

typedef enum difference {
    MUCH_LESS,
    LESS,
    SAME,
    MORE,
    MUCH_MORE
} difference;

difference how_different(size_t x, size_t y) {
    if (x < (y / DIFFERENCE_RATIO)) {
        return MUCH_LESS;
    } if (x < y) {
        return LESS;
    } if (y < (x / DIFFERENCE_RATIO)) {
        return MUCH_MORE;
    } if (y < x) {
        return MORE;
    }
    return SAME;
}

bool a_rand_seed_set = false;

double a_random_between_0_1() {
    if (!a_rand_seed_set) {
        srand((unsigned int)time(NULL));
    }
    return ((double)rand()) / ((double)RAND_MAX);
}


/*----------------------------------------------------------------------------*/
/*                                PUBLIC FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

direction_t execute_attacker_strategy(
    position_t attacker_position,
    Spy defender_spy
) {
    int dist_right = (int)attacker_position.j;
    int spy_uses = get_spy_number_uses(defender_spy);

    direction_t next_move;

    if (spy_uses > 0 && dist_right % spy_uses >= SPY_USE_THRESHOLD) {
        // Using spy

        position_t enemy_position = get_spy_position(defender_spy);

        difference diff_horizontal = how_different(
            attacker_position.i,
            enemy_position.i
        );

        difference diff_vertical = how_different(
            attacker_position.j,
            enemy_position.j
        );

        // Note about direction_t:
        // It would be best not to define the next_move by
        // raw integers, such as done below

        switch (diff_horizontal) {
            case MUCH_LESS: next_move.i = 1;  break; // down
            case LESS:      next_move.i = 1;  break; // down
            case SAME:      next_move.i = 1;  break; // down
            case MORE:      next_move.i = -1; break; // up
            case MUCH_MORE: next_move.i = -1; break; // up
        }

        switch (diff_vertical) {
            case MUCH_LESS: next_move.j = 1;  break; // right
            case LESS:      next_move.j = 1;  break; // right
            case SAME:      next_move.j = 1;  break; // right
            case MORE:      next_move.j = 0;  break; // none
            case MUCH_MORE: next_move.j = 1;  break; // right
        }
    } else {
        // Not using spy

        next_move.j = 1; // always move right

        double up_down_prob = a_random_between_0_1();
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