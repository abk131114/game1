import { GameObjects } from "phaser";
import { ItemData, MenuData, ItemKind } from "./seuck.menu";

type ItemBuilder = (factory: GameObjects.GameObjectFactory, item: any) => GameObjects.GameObject;

const ItemBuilders: Record<ItemKind, ItemBuilder> = {
  [ItemKind.Toggle]: (factory, item) => factory.text(0, 0, item.active),
  [ItemKind.Action]: (factory, item) => factory.text(0, 0, item.textRef),
  [ItemKind.Label]: (factory, item) => factory.text(0, 0, item.textRef),
  [ItemKind.Spacer]: (factory, _item) => factory.text(0, 0, ''),
  [ItemKind.Submenu]: (factory, item) => factory.text(0, 0, item.textRef),
}

export function registerItemBuilder(kind: ItemKind, handler: ItemBuilder): void {
  ItemBuilders[kind] = handler;
}

function createItemGameObject( factory: GameObjects.GameObjectFactory, kind: ItemKind, item: any ): GameObjects.GameObject {
  const handler = ItemBuilders[kind];
  if (!handler) {
    throw new Error(`No handler registered for ItemKind: ${kind}`);
  }
  return handler(factory, item);
}

export function createMenuItem( factory: GameObjects.GameObjectFactory, data: ItemData ) : GameObjects.GameObject {
  return createItemGameObject(factory, data.item.kind, data.item);
}

export function createMenuObjects( factory: GameObjects.GameObjectFactory, menu: MenuData ) {
  return menu.items.map(item => createMenuItem(factory, item));
}
