
#include "keyfuncs.h"

void keybind_Prgm(void){
    char input[LOG_LINE_SIZE] = {0};
    prompt_for("[Chat Msg]", &input, LOG_LINE_SIZE-1, 5, 15, 0);
    if(input[0]) ntwk_send(MESSAGE, PS_STR(input));
}
