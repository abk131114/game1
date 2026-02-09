import { Scene } from 'phaser';
import { VirtualJoystick } from 'phaser-virtual-joystick';

export class Gameplay extends Scene {
  camera: Phaser.Cameras.Scene2D.Camera;
  background: Phaser.GameObjects.Image;

  player: Phaser.GameObjects.Sprite;
  playerX: number;
  playerY: number;

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

  // -1 links/oben, 1 rechts/unten
  movePlayer() {
    this.player.x += this.powerups.speed * this.playerX;
    this.player.y += this.powerups.speed * this.playerY;
  }

  create() {
    const joystick = new VirtualJoystick({ scene: this });

    joystick.on('move', (data) => {
      this.playerX = data.x;
      this.playerY = data.y;
    });

    this.add.existing(joystick);

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

  handleKeyboardInput() {
    this.playerX = 0;
    this.playerY = 0;
    if (this.keys.w.isDown) { this.playerY -= 1; }
    if (this.keys.a.isDown) { this.playerX -= 1; }
    if (this.keys.s.isDown) { this.playerY += 1; }
    if (this.keys.d.isDown) { this.playerX += 1; }
  }

  update() {
    this.handleKeyboardInput();
    this.movePlayer();
  }
}
