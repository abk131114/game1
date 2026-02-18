export enum ItemKind {
  Action = 'action',
  Label = 'label',
  Spacer = 'spacer',
  Submenu = 'submenu',
  Toggle = 'toggle',
}

export enum ItemFlags {
  None = 0,
  Selectable = 1,
}

export type ActionItemData = {
  textRef: string;
}

export type ToggleItemData = {
  active: string;
  inactive: string;
}

export type SubmenuItemData = {
  textRef: string;
}

export type LabelItemData = {
  textRef: string;
}

export type SpacerItemData = {
}

export type ItemData = {
  kind: string;
  ref: number;
  flags: ItemFlags;
}

export type MenuItemsData = {
  // items: ItemData[];
  startIndex: number;
  endIndex: number;
  // default_selection: number;
}

export type MenuData = {
  actions: ActionItemData[];
  toggles: ToggleItemData[];
  submenus: SubmenuItemData[];
  labels: LabelItemData[];
  spacer: SpacerItemData[];
  items: ItemData[];
  menus: MenuItemsData[];
}
