import { Scene, GameObjects } from 'phaser';
import { GameplayData } from '../Data';
import { createMenuObjects } from '../../seuck.menu.phaser';
import { menus } from '../../px.data';

export class MainMenu extends Scene {
  background: GameObjects.Image;
  logo: GameObjects.Image;
  title: GameObjects.Text;
  items: GameObjects.GameObject[];

  constructor() {
    super('MainMenu');
  }

  create() {
    this.background = this.add.image(512, 384, 'background');

    this.items = createMenuObjects(this.add, menus[0]);

    // this.logo = this.add.image(512, 300, 'logo');
    // this.title = this.add.text(512, 460, 'Main Menu', {
    //   fontFamily: 'Arial Black', fontSize: 38, color: '#ffffff',
    //   stroke: '#000000', strokeThickness: 8,
    //   align: 'center'
    // }).setOrigin(0.5);

    this.input.once('pointerdown', () => {
      const data: GameplayData = {
        level: 1
      }

      this.scene.start('Gameplay', data);
    });
  }

  update(_time: number, _delta: number) {
  }
}
