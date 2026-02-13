export type LayerData = {
  tilesetColumns: number;
  tiles: number[];
}

export type LevelData = {
  width: number;
  height: number;
  layers: LayerData[];
}

export type BulletData = {
  damage: number;
  kind: number;
  animationRef: number;
}

export type BulletSpawnData = {
  bulletRef: number;
  flags: number;
  dirX: number;
  dirY: number;
}

export type EnemyData = {
  // faction: number;
  animationRef: number;
  score: number;
  hp: number;
}

export type RegionData = {
  atlasRef: number;
  x: number;
  y: number;
  w: number;
  h: number;
}

export type SpritesheetFrame = {
  regionRef: number;
  duration: number;
}

export type SpritesheetAnimation = {
  framesStartRef: number;
  framesEndRef: number;
  repeat: number;
}

export type SpritesheetAnimationState = {
  frame: number;
  frameIndex: number;
  elapsed: number;
  repeat: number;
}

enum BulletInstanceFlag {
  Alive = 0,
}

export type BulletInstance = {
  animation: SpritesheetAnimation;
  state: SpritesheetAnimationState;
  bullet: BulletData;
  flags: BulletInstanceFlag;
  age: number;
  x: number;
  y: number;
  w: number;
  h: number;
  dx: number;
  dy: number;

  bulletRef: number;
  animationRef: number;
}

export type EnemyInstance = {
  animation: SpritesheetAnimation;
  state: SpritesheetAnimationState;
  enemyRef: number;
  x: number;
  y: number;
  w: number;
  h: number;
  hp: number;
  age: number;
  dx: number;
  dy: number;
}

export type PlayerInstance = {
  animation: SpritesheetAnimation;
  state: SpritesheetAnimationState;
  shipRef: number;
  x: number;
  y: number;
  w: number;
  h: number;
  dx: number;
  dy: number;
}
