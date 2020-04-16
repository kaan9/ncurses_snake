#include "config.h"

/* 
 * numeric macros for representing colors 
 * last 2 bits of grid positions should be
 * set to one of these for curse_update
 */
#define SNAKE_N 1
#define FOOD_N 2
#define BACKG_N 3

void curse_init(void);

int curse_timed_key(void);

void curse_update(unsigned int score, unsigned char (*pgrid)[WIDTH][HEIGHT]);

void curse_term(void);
