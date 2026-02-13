import { Menu } from "./seuck.menu";

export const menus: Menu.MenuData[] = [
  { items: [
    { item: { kind: Menu.ItemKind.Toggle, active: 'ONE PLAYER', inactive: 'TWO PLAYER' }, flags: Menu.ItemFlags.Selectable, userData: 0 },
    { item: { kind: Menu.ItemKind.Submenu, textRef: 'START LEVEL' }, flags: Menu.ItemFlags.Selectable, userData: 0 },
    { item: { kind: Menu.ItemKind.Submenu, textRef: 'OPTIONS' }, flags: Menu.ItemFlags.Selectable, userData: 0 },
    { item: { kind: Menu.ItemKind.Spacer }, flags: Menu.ItemFlags.Selectable, userData: 0 },
    { item: { kind: Menu.ItemKind.Action, textRef: 'START GAME' }, flags: Menu.ItemFlags.Selectable, userData: 0 },
  ] },

  { items: [

  ] },
];
