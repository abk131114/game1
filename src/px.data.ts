import { MenuData, ItemFlags, ItemKind } from "./seuck.menu";

export const menus: MenuData[] = [
  { items: [
    { type: "toggle", item: { kind: ItemKind.Toggle, active: 'ONE PLAYER', inactive: 'TWO PLAYER' }, flags: ItemFlags.Selectable, userData: 0 },

    // { item: { type: , kind: ItemKind.Submenu, textRef: 'START LEVEL' }, flags: ItemFlags.Selectable, userData: 0 },
    // { item: { type: , kind: ItemKind.Submenu, textRef: 'OPTIONS' }, flags: ItemFlags.Selectable, userData: 0 },
    // { item: { type: , kind: ItemKind.Spacer }, flags: ItemFlags.Selectable, userData: 0 },
    // { item: { type: , kind: ItemKind.Action, textRef: 'START GAME' }, flags: ItemFlags.Selectable, userData: 0 },
  ] },

  { items: [

  ] },
];
