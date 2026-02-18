import { Scene, GameObjects } from 'phaser';
// import { GameplayData } from '../Data';
import { createMenu, Menu, updateMenu } from '../../seuck.menu.phaser';
import Menus from '../../px.data';

// use rex stuff? https://rexrainbow.github.io/phaser3-rex-notes/docs/site/plugin-list/

export class MainMenu extends Scene {
  background: GameObjects.Image;
  logo: GameObjects.Image;
  title: GameObjects.Text;
  menu: Menu;

  constructor() {
    super('MainMenu');
  }

  create() {
    this.background = this.add.image(512, 384, 'background');

    const ctx = { x: this.game.renderer.width / 2, y: this.game.renderer.height / 2 }
    this.menu = createMenu(ctx, this, Menus, 0);

    // this.input.once('pointerdown', () => {
    //   const data: GameplayData = {
    //     level: 1
    //   }

    //   this.scene.start('Gameplay', data);
    // });
  }

  update(_time: number, _delta: number) {
    updateMenu(this.menu, this);
  }
}
