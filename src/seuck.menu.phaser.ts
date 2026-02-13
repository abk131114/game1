import { GameObjects } from "phaser";
import { ItemData, ItemRegistry, MenuData } from "./seuck.menu";

export type ItemFactory = {
  [K in keyof ItemRegistry]: (factory: GameObjects.GameObjectFactory, data: ItemData<K>) => GameObjects.GameObject;
}

export const itemFactories: ItemFactory = {
  toggle: (factory, data) => {
    return factory.text(0, 0, data.item.active);
  },

  action: (factory, data) => {
    return factory.text(0, 0, data.item.textRef);
  },

  label: (factory, data) => {
    return factory.text(0, 0, data.item.textRef);
  },

  spacer: (factory, _data) => {
    return factory.text(0, 0, '');
  },

  submenu: (factory, data) => {
    return factory.text(0, 0, data.item.textRef);
  },
}

export function createMenuItem<K extends keyof ItemRegistry>( factory: GameObjects.GameObjectFactory, data: ItemData<K> ) : GameObjects.GameObject {
  const f = itemFactories[data.type];
  return f(factory, data);
}

export function createMenuObjects( factory: GameObjects.GameObjectFactory, menu: MenuData ) {
  return menu.items.map(item => createMenuItem(factory, item));
}
