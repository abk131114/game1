import { MenuData, ItemFlags, ItemKind } from "./seuck.menu";

export const menus: MenuData[] = [
  { items: [
    { item: { kind: ItemKind.Toggle, active: 'ONE PLAYER', inactive: 'TWO PLAYER' }, flags: ItemFlags.Selectable },
    { item: { kind: ItemKind.Submenu, textRef: 'START LEVEL' }, flags: ItemFlags.Selectable },
    { item: { kind: ItemKind.Submenu, textRef: 'OPTIONS' }, flags: ItemFlags.Selectable },
    { item: { kind: ItemKind.Spacer }, flags: ItemFlags.Selectable },
    { item: { kind: ItemKind.Action, textRef: 'START GAME' }, flags: ItemFlags.Selectable },
  ] },

  { items: [

  ] },
];
