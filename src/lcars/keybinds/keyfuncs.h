#ifndef keyfuncs_h
#define keyfuncs_h

#include "../screens.h"
#include "../errors.h"
#include "../../network/controlcodes.h"
#include "../../network/network.h"
#include "../../flags.h"

#define setbits(bits, mask) (bits|mask)
#define resbits(bits, mask) (bits^mask)

void keybind_Clear(void);
void keybind_Prgm(void);
void keybind_Log(void);
void keybind_Store(void);
void keybind_Stat(void);
void keybind_Mode(void);
void keybind_RightArrow(void);
void keybind_LeftArrow(void);
void keybind_UpArrow(void);
void keybind_DownArrow(void);

#endif
