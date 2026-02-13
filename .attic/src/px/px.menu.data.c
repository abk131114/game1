#include "px.core.h"

struct seuck_menu_str PX_STRINGS[] = {
    "ONE PLAYER",
    "TWO PLAYER",
    "START LEVEL",
    "OPTIONS",
    "START GAME",
    "- CHOOSE LEVEL -",
    "LEVEL1",
    "LEVEL2",
    "LEVEL3",
    "SELECT CRAFT",
    // 10
    "ARCADE MODE",
    "ROOKIE MODE",
    "LOAD HIGHSCORE",
    "SAVE HIGHSCORE",
    "AUTOFIRE ON",
    "AUTOFIRE OFF",
    "EXIT",
    "PLAYER ONE",
    "PLAYER TWO",
    "           - COMMENTS -",
    // 20
    "- ADVANTAGES -     - DISADVANTAGES -",
    "      CRUX II - BATTLE CLASS",
    "* HEAVY ARSENAL      * SLOW SPEED",
    "* BUILDUP POWER      * HEAVY INERTIA",
    "     THE ULTIMATE BATTLE CRAFT",
    "     HYPERION - CRUISER CLASS",
    "* GOOD ARSENAL       * NONE",
    "* GOOD SPEED",
    "   GOOD ALL-PURPOSE FIGHTER CRAFT",
    "     HUNTER MK 7 - SCOUT CLASS",
    // 30
    "* EXCELLENT SPEED    * LIMITED ARSENAL",
    "* STEALTH FACILITY",
    "  SUPERFAST AND AGILE FIGHTER CRAFT",
};

struct seuck_menu_toggle_data PX_TOGGLES[] = {
    { 0, 1 }, // MM: ONE PLAYER / TWO PLAYER
    { 10, 11 }, // OP: ARCADE / ROOKIE
    { 15, 16 }, // OP: AUTOFIRE ON/OFF
};

struct seuck_menu_submenu_data PX_SUBMENUS[] = {
    { 2 }, // MM: START LEVEL
    { 3 }, // MM: OPTIONS
    { 9 }, // OP: SELECT CRAFT
    { 16 }, // EXIT
};

struct seuck_menu_label_data PX_LABELS[] = {
    // 0
    { 5 }, // - CHOOSE LEVEL -
    { 19 }, // - COMMENTS -
    { 20 }, // - ADVANTAGES -     - DISADVANTAGES -
    { 21 }, // CRUX II - BATTLE CLASS
    { 22 }, // * HEAVY ARSENAL      * SLOW SPEED

    // 5
    { 23 }, // * BUILDUP POWER      * HEAVY INERTIA
    { 24 }, // THE ULTIMATE BATTLE CRAFT
    { 25 }, // HYPERION - CRUISER CLASS",
    { 26 }, // * GOOD ARSENAL       * NONE",
    { 27 }, // * GOOD SPEED",

    // 10
    { 28 }, // GOOD ALL-PURPOSE FIGHTER CRAFT",
    { 29 }, // HUNTER MK 7 - SCOUT CLASS",
    { 30 }, // * EXCELLENT SPEED    * LIMITED ARSENAL",
    { 31 }, // * STEALTH FACILITY",
    { 32 }, // SUPERFAST AND AGILE FIGHTER CRAFT",
};

struct seuck_menu_action_data PX_ACTIONS[] = {
    { 4 }, // START GAME

    { 6 }, // LEVEL1
    { 7 }, // LEVEL2
    { 8 }, // LEVEL3

    { 12 }, // LOAD HIGHSCORE
    { 13 }, // SAVE HIGHSCORE

    { 17 }, // player1
    { 18 }, // player2
};

// TODO (DK) userdata could be per kind
struct seuck_menu_item_data PX_MENU_ITEMS[] = {
    // main menu
    { seuck_menu_item_kind_toggle, 0, 0 },
    { seuck_menu_item_kind_submenu, 0, 1 },
    { seuck_menu_item_kind_submenu, 1, 2 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_action, 0, 3 },

    // start level menu
    { seuck_menu_item_kind_label, 0 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_action, 1, 4 },
    { seuck_menu_item_kind_action, 2, 5 },
    { seuck_menu_item_kind_action, 3, 6 },

    // options menu
    { seuck_menu_item_kind_submenu, 2, 7 },
    { seuck_menu_item_kind_toggle, 1, 8 },
    { seuck_menu_item_kind_action, 4, 9 },
    { seuck_menu_item_kind_action, 5, 10 },
    { seuck_menu_item_kind_toggle, 2, 11 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_submenu, 3, 12 },

    // heavy
    { seuck_menu_item_kind_label, 4 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_label, 2 },
    { seuck_menu_item_kind_label, 4 },
    { seuck_menu_item_kind_label, 5 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_label, 1 },
    { seuck_menu_item_kind_label, 6 },

    // middle
    { seuck_menu_item_kind_label, 7 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_label, 2 },
    { seuck_menu_item_kind_label, 8 },
    { seuck_menu_item_kind_label, 9 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_label, 1 },
    { seuck_menu_item_kind_label, 10 },

    // light
    { seuck_menu_item_kind_label, 12 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_label, 2 },
    { seuck_menu_item_kind_label, 12 },
    { seuck_menu_item_kind_label, 13 },
    { seuck_menu_item_kind_empty_separator },
    { seuck_menu_item_kind_label, 1 },
    { seuck_menu_item_kind_label, 14 },

    // player sel
    { seuck_menu_item_kind_action, 6, 13 },
    { seuck_menu_item_kind_action, 7, 14 },
};

struct seuck_menu_sound_data PX_MENU_SOUNDS[] = {
    { 0 },
    { 1 },
};

struct seuck_menu_data PX_MENUS[] = {
    { 0, 4, 0 }, // main menu
    { 5, 9, 2 }, // start level
    { 10, 16, 6 }, // options
    { 17, 24, -1 }, // heavy
    { 25, 32, -1 }, // middle
    { 33, 40, -1 }, // light
    { 41, 42, 0 }, // player selection for craft
};
