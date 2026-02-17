export enum ItemKind {
  Label, Toggle, Submenu, Action, Spacer
}

export enum ItemFlags {
  Selectable
}

interface BasicItem {
  flags: ItemFlags;
}

interface ToggleItem extends BasicItem {
  kind: ItemKind.Toggle;
  active: string;
  inactive: string;
}

interface SubmenuItem extends BasicItem {
  kind: ItemKind.Submenu;
  textRef: string;
}

interface ActionItem extends BasicItem {
  kind: ItemKind.Action;
  textRef: string;
}

interface LabelItem extends BasicItem {
  kind: ItemKind.Label;
  textRef: string;
}

interface SpacerItem extends BasicItem {
  kind: ItemKind.Spacer;
}

// export enum ActionKind {
//   Navigate, Activate
// }

export interface ItemRegistry {
  [ItemKind.Toggle]: ToggleItem;
  [ItemKind.Submenu]: SubmenuItem;
  [ItemKind.Label]: LabelItem;
  [ItemKind.Action]: ActionItem;
  [ItemKind.Spacer]: SpacerItem;
}

export type ItemData<K extends ItemKind = ItemKind> = {
  item: ItemRegistry[K];
}

export type MenuData<K extends ItemKind = ItemKind> = {
  items: ItemData<K>[];
  // from: number;
  // to: number;
  // default_selection: number;
}
