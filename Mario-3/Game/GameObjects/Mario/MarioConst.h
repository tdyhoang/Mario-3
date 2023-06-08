#pragma once

const float MARIO_GRAVITY = 0.0026f;

const D3DXVECTOR2 SUPER_MARIO_BBOX(42, 81);
const D3DXVECTOR2 SUPER_MARIO_CROUCH_BBOX(42, 42);
const D3DXVECTOR2 SMALL_MARIO_BBOX(36, 45);

const float  MARIO_JUMP_FORCE = 0.55f;
const float  MARIO_HIGH_JUMP_HEIGHT = 180.0f;
const float  MARIO_SUPER_JUMP_HEIGHT = 210.0f;
const float  MARIO_WALKING_SPEED = 0.24f;
const float  MARIO_RUNNING_SPEED = 0.42f;
const float  MARIO_RUNNING_ACCELERATION = 0.001175f;
const float  MARIO_WALKING_ACCELERATION = 0.000875f;

const float  MARIO_PUSH_FORCE = 0.55f;

const float DIE_VELOCITY_Y = 0.3f;
const float DIE_JUMP = 100;
const float DIE_FALL = 200;