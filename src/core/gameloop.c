#include <stdbool.h>

#include "../graphics/menus.h"
#include "gameloop.h"

game_error_t game_error = NO_ERROR;

void PlayGame(void){
    char keyfile[9] = {0};
    if(!menu_SelectKeyFile(keyfile)) return;
}
