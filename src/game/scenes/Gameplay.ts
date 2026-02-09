import { Scene } from 'phaser';

export class Gameplay extends Scene {
  camera: Phaser.Cameras.Scene2D.Camera;
  background: Phaser.GameObjects.Image;
  player: Phaser.GameObjects.Sprite;

  powerups: {
    speed: number;
  }

  keys: {
    w: Phaser.Input.Keyboard.Key;
    a: Phaser.Input.Keyboard.Key;
    s: Phaser.Input.Keyboard.Key;
    d: Phaser.Input.Keyboard.Key;
  }

  constructor() {
    super('Gameplay');
  }

  create() {
    this.camera = this.cameras.main;
    this.camera.setBackgroundColor(0x00ff00);

    this.background = this.add.image(512, 384, 'background');
    this.background.setAlpha(0.5);

    this.player = this.add.sprite(512, 384, "player-1");
    this.player.setOrigin(0.5, 0.5);

    this.powerups = {
      speed: 3.0,
    }

    if (this.input.keyboard) {
      this.keys = {
        w: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.W),
        a: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A),
        s: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.S),
        d: this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D),
      }
    }
  }

  update() {
    const speed = this.powerups.speed;

    if (this.keys.w.isDown) {
      this.player.y -= speed;
    }

    if (this.keys.a.isDown) {
      this.player.x -= speed;
    }

    if (this.keys.s.isDown) {
      this.player.y += speed;
    }

    if (this.keys.d.isDown) {
      this.player.x += speed;
    }
  }
}
