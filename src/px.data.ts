import { MenuItemsData, ItemFlags, ItemKind, ToggleItemData, SubmenuItemData, SpacerItemData, ActionItemData, ItemData, LabelItemData, MenuData } from "./seuck.menu";

const actions: ActionItemData[] = [
  { textRef: 'START GAME' },
];

const labels: LabelItemData[] = [
];

const spacer: SpacerItemData[] = [
  {},
];

const submenus: SubmenuItemData[] = [
  { textRef: 'START LEVEL' },
  { textRef: 'OPTIONS' },
];

const toggles: ToggleItemData[] = [
  { active: 'ONE PLAYER', inactive: 'TWO PLAYER' },
];

const items: ItemData[] = [
  { kind: ItemKind.Toggle, ref: 0, flags: ItemFlags.Selectable },
  { kind: ItemKind.Submenu, ref: 0, flags: ItemFlags.Selectable },
  { kind: ItemKind.Submenu, ref: 1, flags: ItemFlags.Selectable },
  { kind: ItemKind.Spacer, ref: 0, flags: ItemFlags.None },
  { kind: ItemKind.Action, ref: 0, flags: ItemFlags.Selectable },
];

const menus: MenuItemsData[] = [
  { startIndex: 0, endIndex: 4 },
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
