#include <curses.h>

/* horizontal to vertical proportion */
/* #define HRZ_SCALE 1.5 */

/** dimensions **/

/* snake window dimensions and position */
#define HEIGHT 20
#define WIDTH 30
#define SWINX 0
#define SWINY 1

/* score window dimensions and position */
#define WHEIGHT SWINY
#define WWIDTH WIDTH
#define WWINX 0
#define WWINY 0

/* delay between keyreads in ms */
#define DELAY 300

/** colors **/

/* if true and terminal doesn't support color, program will terminate */
#define REQ_COLOR FALSE

/* choose colors for the background, snake, and food */
#define BACKG_C COLOR_BLACK
#define SNAKE_C COLOR_CYAN
#define FOOD_C COLOR_RED
