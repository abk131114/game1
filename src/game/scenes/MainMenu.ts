import { Scene, GameObjects } from 'phaser';
import { GameplayData } from '../Data';
import { menus } from '../../px.data';

export class MainMenu extends Scene {
  background: GameObjects.Image;
  logo: GameObjects.Image;
  title: GameObjects.Text;

  constructor() {
    super('MainMenu');
  }

  create() {
    const menuData = menus[0];

    this.background = this.add.image(512, 384, 'background');
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
}
