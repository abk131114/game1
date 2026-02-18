import { GameObjects, Scene, Tweens } from "phaser";
import { MenuData, ItemKind, ItemFlags, ItemData } from "./seuck.menu";

type BuilderContext = {
  x: number;
  y: number;
}

type ItemBuilder = (ctx: BuilderContext, scene: Scene, data: MenuData, item: ItemData) => MenuItem;

type MenuItem = {
  obj: GameObjects.Text;
  data: ItemData;
}

const BUILDERS = new Map<string, ItemBuilder>([
  [ItemKind.Toggle, (ctx, scene, data, item) => {
    const ref = data.toggles[item.ref];
    const obj = scene.add.text(ctx.x, ctx.y += 16, ref.on ? ref.onStr : ref.offStr);
    return { obj, data: item }
  }],

  [ItemKind.Action, (ctx, scene, data, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, data.actions[item.ref].textRef);
    return { obj, data: item }
  }],

  [ItemKind.Label, (ctx, scene, data, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, data.labels[item.ref].textRef);
    return { obj, data: item }
  }],

  [ItemKind.Spacer, (ctx, scene, _data, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, '');
    return { obj, data: item }
  }],

  [ItemKind.Submenu, (ctx, scene, data, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, data.submenus[item.ref].textRef);
    return { obj, data: item }
  }],
]);

export function createMenuItems( ctx: BuilderContext, scene: Scene, data: MenuData, menuIndex: number ) {
  const menuData = data.menus[menuIndex];
  const objects = [];

  for (let itemIdx = menuData.startIndex; itemIdx <= menuData.endIndex; itemIdx += 1) {
    const item = data.items[itemIdx];
    const builder = BUILDERS.get(item.kind);

    if (builder) {
      const obj = builder(ctx, scene, data, item);
      objects.push(obj);
    }
  }

  return objects;
}

type Callbacks =   {
  toggle: (menu: Menu, item: ItemData) => void;
  activate: (menu: Menu, item: ItemData) => number;
}

export function createMenu( ctx: BuilderContext, scene: Scene, data: MenuData, menuIndex: number, callbacks: Callbacks ) {
  const menu = {
    startContext: ctx,
    scene,
    data,
    callbacks: callbacks,

    selected: 0,
    items: createMenuItems({ ...ctx }, scene, data, menuIndex),

    keys: {
      up: scene.input.keyboard?.addKey(Phaser.Input.Keyboard.KeyCodes.W),
      left: scene.input.keyboard?.addKey(Phaser.Input.Keyboard.KeyCodes.A),
      down: scene.input.keyboard?.addKey(Phaser.Input.Keyboard.KeyCodes.S),
      right: scene.input.keyboard?.addKey(Phaser.Input.Keyboard.KeyCodes.D),
      activate: scene.input.keyboard?.addKey(Phaser.Input.Keyboard.KeyCodes.SPACE),
    },
  }

  selectFirstItem(menu);

  return menu;
}

export type Menu = {
  startContext: BuilderContext;
  scene: Scene,
  data: MenuData,
  callbacks: Callbacks,

  selected: number;
  selectedTween?: Tweens.Tween;
  items: MenuItem[];

  keys: {
    up?: Phaser.Input.Keyboard.Key;
    left?: Phaser.Input.Keyboard.Key;
    down?: Phaser.Input.Keyboard.Key;
    right?: Phaser.Input.Keyboard.Key;
    activate?: Phaser.Input.Keyboard.Key;
  }
}

const TWEEN_CONFIG = {
  alpha: 0,
  duration: 256,
  ease: 'Linear',
  yoyo: true,
  repeat: -1,
}

export function updateMenu( menu: Menu, scene: Scene ) {
  if (!menu.selectedTween) {
    menu.selectedTween = scene.tweens.add({ ...TWEEN_CONFIG, targets: [menu.items[menu.selected].obj] });
  }

  if (menu.keys.up && Phaser.Input.Keyboard.JustUp(menu.keys.up)) {
    up(menu);
  }

  if (menu.keys.down && Phaser.Input.Keyboard.JustUp(menu.keys.down)) {
    down(menu);
  }

  if (menu.keys.left && Phaser.Input.Keyboard.JustUp(menu.keys.left)) {
    left(menu);
  }

  if (menu.keys.right && Phaser.Input.Keyboard.JustUp(menu.keys.right)) {
    right(menu);
  }

  if (menu.keys.activate && Phaser.Input.Keyboard.JustUp(menu.keys.activate)) {
    activate(menu);
  }
}

function stopTween( menu: Menu ) {
  menu.items[menu.selected].obj.alpha = 1;
  menu.selectedTween?.stop()
  menu.selectedTween = undefined;
}

function down( menu: Menu ) {
  while (true) {
    stopTween(menu);
    menu.selected += 1;

    if (menu.selected >= menu.items.length) {
      menu.selected = 0;
    }

    if (menu.items[menu.selected].data.flags & ItemFlags.Selectable) {
      break;
    }
  }
}

function up( menu: Menu ) {
  while (true) {
    stopTween(menu);
    menu.selected -= 1;

    if (menu.selected < 0) {
      menu.selected = menu.items.length - 1;
    }

    if (menu.items[menu.selected].data.flags & ItemFlags.Selectable) {
      break;
    }
  }
}

function selectFirstItem( menu: Menu ) {
  stopTween(menu);
  menu.selected = 0;

  while (true) {
    if (menu.selected >= menu.items.length) {
      break;
    }

    if (menu.items[menu.selected].data.flags & ItemFlags.Selectable) {
      break;
    }

    menu.selected += 1;
  }
}

const LEFT_ACTIONS = new Map<string, (menu: Menu) => void>();

const RIGHT_ACTIONS = new Map<string, (menu: Menu) => void>();

function openMenu( menu: Menu, idx: number ) {
  menu.items.forEach(item => item.obj.destroy());
  menu.selected = 0;
  menu.items = createMenuItems({...menu.startContext}, menu.scene, menu.data, idx);
  selectFirstItem(menu);
}

const ACTIVATIONS = new Map<string, (menu: Menu, item: MenuItem) => void>([
  [ItemKind.Action, (menu, item) => {
    // const action = menu.data.actions[item.data.ref];
    const r = menu.callbacks.activate(menu, item.data);

    if (r < 0) {
      return;
    }

    openMenu(menu, r);
  }],

  [ItemKind.Submenu, (menu, item) => {
    const submenuItem = menu.data.submenus[item.data.ref];
    openMenu(menu, submenuItem.menuIdx);
  }],

  [ItemKind.Toggle, (menu, item) => {
    const toggle = menu.data.toggles[item.data.ref];
    toggle.on = !toggle.on;
    item.obj.text = toggle.on ? toggle.onStr : toggle.offStr;
    menu.callbacks.toggle(menu, item.data);
  }],
]);

function left( menu: Menu ) {
  const item = menu.items[menu.selected];
  const action = LEFT_ACTIONS.get(item.data.kind);

  if (action) {
    action(menu);
  }
}

function right( menu: Menu ) {
  const item = menu.items[menu.selected];
  const action = RIGHT_ACTIONS.get(item.data.kind);

  if (action) {
    action(menu);
  }
}

function activate( menu: Menu ) {
  const item = menu.items[menu.selected];
  const action = ACTIVATIONS.get(item.data.kind);

  if (action) {
    action(menu, item);
  }
}
