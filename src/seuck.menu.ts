export enum ItemKind {
  Label, Toggle, Submenu, Action, Spacer
}

interface ToggleItem {
  kind: ItemKind.Toggle;
  active: string;
  inactive: string;
}

interface SubmenuItem {
  kind: ItemKind.Submenu;
  textRef: string;
}

interface ActionItem {
  kind: ItemKind.Action;
  textRef: string;
}

interface LabelItem {
  kind: ItemKind.Label;
  textRef: string;
}

interface SpacerItem {
  kind: ItemKind.Spacer;
}

// export enum ActionKind {
//   Navigate, Activate
// }

export enum ItemFlags {
  Selectable
}

export interface ItemRegistry {
  [ItemKind.Toggle]: ToggleItem;
  [ItemKind.Submenu]: SubmenuItem;
  [ItemKind.Label]: LabelItem;
  [ItemKind.Action]: ActionItem;
  [ItemKind.Spacer]: SpacerItem;
}

export type ItemData<K extends ItemKind = ItemKind> = {
  item: ItemRegistry[K];
  flags: ItemFlags;
  userData: number;
}

export type MenuData = {
  items: ItemData[];
  // from: number;
  // to: number;
  // default_selection: number;
}
