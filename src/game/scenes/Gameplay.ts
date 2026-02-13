import { Scene } from 'phaser';
import { GameplayData } from '../Data';

export class Gameplay extends Scene {
  gameplayData: GameplayData;

  camera: Phaser.Cameras.Scene2D.Camera;
  background: Phaser.GameObjects.Image;

  player: Phaser.GameObjects.Sprite;

  powerups: {
    speed: number;
    guns: number;
  }

  keys: {
    up: Phaser.Input.Keyboard.Key;
    left: Phaser.Input.Keyboard.Key;
    down: Phaser.Input.Keyboard.Key;
    right: Phaser.Input.Keyboard.Key;
    fire: Phaser.Input.Keyboard.Key;
    powerup: Phaser.Input.Keyboard.Key;
  }

  constructor() {
    super('Gameplay');
  }


  init(data: GameplayData) {
    console.log(`level: ${data.level}`);

    this.gameplayData = data;
    // this.add.image(512, 384, 'background');
  }

  preload() { // no data
    // this.load.binary(`level-${this.gameplayData.level}.bin`);
  }

  create(data: GameplayData) {
    this.camera = this.cameras.main;
    this.camera.setBackgroundColor(0x00ff00);

    this.background = this.add.image(512, 384, 'background');
    this.background.setAlpha(0.5);

    this.player = this.add.sprite(512, 384, "player-1");
    this.player.setOrigin(0.5, 0.5);

    this.powerups = {
      speed: 3.0,
      guns: 1,
    }

    if (this.input.keyboard) {
      this.keys = {
        up: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.W),
        left: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A),
        down: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.S),
        right: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D),
        fire: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.CTRL),
        powerup: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.SPACE),
      }
    }

    console.log(`level: ${data.level}`);

    /*
      map
      stars
      player bullets
      enemy bullets
      enemies
      players
      explosions

      ui
    */
  }

  handleKeyboardInput() {
    if (this.keys.up.isDown) { this.player.y -= 1; }
    if (this.keys.left.isDown) { this.player.x -= 1; }
    if (this.keys.down.isDown) { this.player.y += 1; }
    if (this.keys.right.isDown) { this.player.x += 1; }
  }

  update() {
    this.handleKeyboardInput();
  }
}
