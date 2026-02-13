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
  toggle: ToggleItem;
  submenu: SubmenuItem;
  label: LabelItem;
  action: ActionItem;
  spacer: SpacerItem;
}

export type ItemData<K extends keyof ItemRegistry = keyof ItemRegistry> = {
  type: K;
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
