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

  gameplaySettings: {
    coop: boolean;
    startLevel: number;
    ship1: string;
    ship2: string;
  }
  constructor() {
    super('MainMenu');
  }

  create() {
    this.background = this.add.image(512, 384, 'background');

    // TODO (DK) map this to menu items so it's not defined twice
    this.gameplaySettings = {
      coop: false,
      startLevel: 1,
      ship1: 'light',
      ship2: 'light',
    }

    const ctx = { x: this.game.renderer.width / 2, y: this.game.renderer.height / 2 }

    this.menu = createMenu(ctx, this, Menus, 0, {
      toggle: (_menu, item) => {
        switch (item.ref) {
          case 0: this.gameplaySettings.coop = !this.gameplaySettings.coop;
        }
      },

      activate: (_menu, item) => {
        switch (item.ref) {
          case 0:
            this.scene.start('Gameplay', this.gameplaySettings);
            break;

          case 1:
            this.gameplaySettings.startLevel = 1;
            return 0;

          case 2:
            this.gameplaySettings.startLevel = 2;
            return 0;

          case 3:
            this.gameplaySettings.startLevel = 3;
            return 0;
        }

        return -1;
      },
    });
  }

  update(_time: number, _delta: number) {
    updateMenu(this.menu, this);
  }
}
