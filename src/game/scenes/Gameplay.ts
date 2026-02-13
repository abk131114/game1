import { Scene } from 'phaser';
import { staticAssets } from '../../static-assets';

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

  init() {
    this.powerups = {
      speed: 3.0,
    }
  }

  preload() {
    this.load.atlas("player", staticAssets("assets/player.png"), staticAssets("assets/player.json"));
    this.load.font
  }

  create() {
    this.camera = this.cameras.main;
    this.camera.setBackgroundColor(0x008080);

    // this.background = this.add.image(512, 384, 'background');
    // this.background.setAlpha(0.5);

    // var atlasTexture = this.textures.get('megaset');

    this.anims.create({ key: 'light',  frames: this.anims.generateFrameNames('player', { prefix: 'light-',  end: 0, zeroPad: 3 }), repeat: -1 });
    this.anims.create({ key: 'medium', frames: this.anims.generateFrameNames('player', { prefix: 'med-',    end: 0, zeroPad: 3 }), repeat: -1 });
    this.anims.create({ key: 'heavy',  frames: this.anims.generateFrameNames('player', { prefix: 'heavy-',  end: 0, zeroPad: 3 }), repeat: -1 });

    this.player = this.add.sprite(512, 384, "player").play("heavy");
    this.player.setOrigin(0.5, 0.5);

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
    if (this.keys.w.isDown) { this.player.y -= this.powerups.speed; }
    if (this.keys.a.isDown) { this.player.x -= this.powerups.speed; }
    if (this.keys.s.isDown) { this.player.y += this.powerups.speed; }
    if (this.keys.d.isDown) { this.player.x += this.powerups.speed; }
  }

  update() {
    this.handleKeyboardInput();
  }
}
