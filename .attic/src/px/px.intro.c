#include "px.core.h"

void pxi_intro_init( struct px_context *px ) {
    // TODO (DK) remove me
    px->state = PX_STATE_MENU;
    pxi_menu_init(px);
}

void pxi_intro_update( struct px_context *px ) {
}

void pxi_intro_draw( struct px_context *px ) {
}
