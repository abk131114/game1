import { MenuItemsData, ItemFlags, ItemKind, ToggleItemData, SubmenuItemData, SpacerItemData, ActionItemData, ItemData, LabelItemData, MenuData } from "./seuck.menu";

const actions: ActionItemData[] = [
  { textRef: 'START GAME' },
  { textRef: 'LEVEL 1' },
  { textRef: 'LEVEL 2' },
  { textRef: 'LEVEL 3' },
];

const labels: LabelItemData[] = [
  { textRef: "- CHOOSE LEVEL -" },
  // { textRef: "           - COMMENTS -" },
  // { textRef: "- ADVANTAGES -     - DISADVANTAGES -" },
  // { textRef: "      CRUX II - BATTLE CLASS" },
  // { textRef: "* HEAVY ARSENAL      * SLOW SPEED" },
  // { textRef: "* BUILDUP POWER      * HEAVY INERTIA" },
  // { textRef: "     THE ULTIMATE BATTLE CRAFT" },
  // { textRef: "     HYPERION - CRUISER CLASS" },
  // { textRef: "* GOOD ARSENAL       * NONE" },
  // { textRef: "* GOOD SPEED" },
  // { textRef: "   GOOD ALL-PURPOSE FIGHTER CRAFT" },
  // { textRef: "     HUNTER MK 7 - SCOUT CLASS" },
  // { textRef: "* EXCELLENT SPEED    * LIMITED ARSENAL" },
  // { textRef: "* STEALTH FACILITY" },
  // { textRef: "  SUPERFAST AND AGILE FIGHTER CRAFT" },
];

const spacer: SpacerItemData[] = [
  {},
];

const submenus: SubmenuItemData[] = [
  { textRef: 'START LEVEL', menuIdx: 1 },
  // { textRef: 'OPTIONS', menuIdx: 2 },
];

const toggles: ToggleItemData[] = [
  { on: false, offStr: 'ONE PLAYER', onStr: 'TWO PLAYER' },
];

const items: ItemData[] = [
  // main
  { kind: ItemKind.Toggle, ref: 0, flags: ItemFlags.Selectable },
  { kind: ItemKind.Submenu, ref: 0, flags: ItemFlags.Selectable },
  // { kind: ItemKind.Submenu, ref: 1, flags: ItemFlags.Selectable },
  { kind: ItemKind.Spacer, ref: 0, flags: ItemFlags.None },
  { kind: ItemKind.Spacer, ref: 0, flags: ItemFlags.None },
  { kind: ItemKind.Action, ref: 0, flags: ItemFlags.Selectable },

  // start level
  { kind: ItemKind.Label, ref: 0, flags: ItemFlags.None },
  { kind: ItemKind.Spacer, ref: 0, flags: ItemFlags.None },
  { kind: ItemKind.Action, ref: 1, flags: ItemFlags.Selectable },
  { kind: ItemKind.Action, ref: 2, flags: ItemFlags.Selectable },
  { kind: ItemKind.Action, ref: 3, flags: ItemFlags.Selectable },

  // heavy
  // { kind: ItemKind.Label, ref: 4, flags: ItemFlags.None },
  // { kind: ItemKind.Spacer, ref: 0, flags: ItemFlags.None },
  // { kind: ItemKind.Label, ref: 2, flags: ItemFlags.None },
  // { kind: ItemKind.Label, ref: 4, flags: ItemFlags.None },
  // { kind: ItemKind.Label, ref: 5, flags: ItemFlags.None },
  // { kind: ItemKind.Spacer, ref: 0, flags: ItemFlags.None },
  // { kind: ItemKind.Label, ref: 1, flags: ItemFlags.None },
  // { kind: ItemKind.Label, ref: 6, flags: ItemFlags.None },
];

const menus: MenuItemsData[] = [
  { startIndex: 0, endIndex: 4 }, // main
  { startIndex: 5, endIndex: 9 }, // start level
];

export default {
  actions,
  toggles,
  submenus,
  labels,
  spacer,
  items,
  menus,
} satisfies MenuData;
