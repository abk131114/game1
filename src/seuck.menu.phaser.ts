import { GameObjects, Scene, Tweens } from "phaser";
import { MenuData, ItemKind, ItemData, ItemFlags } from "./seuck.menu";

type BuilderContext = {
  x: number;
  y: number;
}

type ItemBuilder = (ctx: BuilderContext, scene: Scene, item: any) => GameObjects.Text;

function addSelectableFlag(obj: GameObjects.Text, item: any) {
  obj.setData('selectable', item.flags & ItemFlags.Selectable)
}

const ItemBuilders: Record<ItemKind, ItemBuilder> = {
  [ItemKind.Toggle]: (ctx, scene, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, item.active);
    addSelectableFlag(obj, item);
    return obj;
  },

  [ItemKind.Action]: (ctx, scene, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, item.textRef);
    addSelectableFlag(obj, item);
    return obj;
  },

  [ItemKind.Label]: (ctx, scene, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, item.textRef);
    addSelectableFlag(obj, item);
    return obj;
  },

  [ItemKind.Spacer]: (ctx, scene, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, '');
    addSelectableFlag(obj, item);
    return obj;
  },

  [ItemKind.Submenu]: (ctx, scene, item) => {
    const obj = scene.add.text(ctx.x, ctx.y += 16, item.textRef);
    addSelectableFlag(obj, item);
    return obj;
  },

}

export function registerItemBuilder(kind: ItemKind, handler: ItemBuilder): void {
  ItemBuilders[kind] = handler;
}

function createItemGameObject( ctx: BuilderContext, scene: Scene, item: ItemData ): GameObjects.Text {
  const handler = ItemBuilders[item.item.kind];

  if (!handler) {
    throw new Error(`No handler registered for ItemKind: ${item.item.kind}`);
  }

  return handler(ctx, scene, item.item);
}

export function createMenu( ctx: BuilderContext, scene: Scene, menu: MenuData ) {
  return {
    selected: 0,
    items: menu.items.map(item => createItemGameObject(ctx, scene, item))
  }
}

export type Menu = {
  selected: number;
  selectedTween?: Tweens.Tween;
  items: GameObjects.Text[];
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
    menu.selectedTween = scene.tweens.add({ ...TWEEN_CONFIG, targets: [menu.items[menu.selected]] });
  }
}

export function down( menu: Menu ) {
  while (true) {
    // menu.selectedTween.stop()
    // menu.selectedTween.setToEnd()
    menu.selectedTween?.complete();
    menu.selectedTween?.stop()
    menu.selectedTween = undefined;
    menu.selected += 1;

    if (menu.selected >= menu.items.length) {
      menu.selected = 0;
    }

    if (menu.items[menu.selected].getData('selectable')) {
      break;
    }
  }
}

export function up( menu: Menu ) {
  while (true) {
    menu.selectedTween?.complete();
    menu.selectedTween?.stop()
    menu.selectedTween = undefined;
    menu.selected -= 1;

    if (menu.selected < 0) {
      menu.selected = menu.items.length - 1;
    }

    if (menu.items[menu.selected].getData('selectable')) {
      break;
    }
  }
}
