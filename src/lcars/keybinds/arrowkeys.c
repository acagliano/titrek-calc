
#include "keyfuncs.h"

void keybind_RightArrow(void){
    switch (screen){
        case SCRN_NAVIG:
        {
            uint24_t max_speed = engine_ref.engine[select.engine].max_speed;
            uint24_t speedinterval = max_speed/8;
            select.newspeed+=(speedinterval*(select.newspeed<max_speed));
        }
        break;
    }
}

void keybind_LeftArrow(void){
     switch (screen){
        case SCRN_NAVIG:
        {
            uint24_t max_speed = engine_ref.engine[select.engine].max_speed;
            uint24_t speedinterval = max_speed/8;
            select.newspeed-=(speedinterval*(select.newspeed>0));
        }
        break;
    }
}

void keybind_UpArrow(void){
    char i;
    switch(screen){
        case SCRN_TACT:
            for(i = select.tactical - 1; i >= 0; i--){
                int type = Ship.system[i].techclass;
                if( type == mTactical ){
                    select.tactical = i;
                    break;
                }
            }
            break;
        case SCRN_MAINS:
            for(i = select.mains - 1; i >= 0; i--){
                int type = Ship.system[i].techclass;
                if( type == mSystem ){
                    select.mains = i;
                    break;
                }
            }
            break;
        case SCRN_NAVIG:
            select.engine-=(select.engine>0);
            select.newspeed=engine_ref.engine[select.engine].current_speed;
            break;
    }
}

void keybind_DownArrow(void){
    char i;
    switch(screen){
        case SCRN_TACT:
            for(i = select.tactical + 1; i < (MAX_MODULES - 1); i++){
                int type = Ship.system[i].techclass;
                if( type == mTactical ){
                    select.tactical = i;
                    break;
                }
            }
            break;
        case SCRN_MAINS:
            for(i = select.mains + 1; i < (MAX_MODULES - 1); i++){
                int type = Ship.system[i].techclass;
                if( type == mSystem ){
                    select.mains = i;
                    break;
                }
            }
            break;
        case SCRN_NAVIG:
            select.engine+=(select.engine<2);
            select.newspeed=engine_ref.engine[select.engine].current_speed;
            break;
    }

}
