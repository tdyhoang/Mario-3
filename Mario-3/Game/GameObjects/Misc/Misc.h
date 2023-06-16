#pragma once

const int PIPE_DEFAULT_TILE_SIZE = 48;

const int PIPE_START_X = 0;
const int PIPE_START_Y = 0;
const int PIPE_HEIGHT = 192;
const int PIPE_WIDTH = 672;


const D3DXVECTOR2 DEFAULT_OFFSET = D3DXVECTOR2(289, 44);

enum class PipeDirection
{
	Left, Right, Up, Down
};

const int BOUNCE_TIME = 50;
const int BOUNCE_DELTA = 24;
const float BOUNCE_VEL = (float)BOUNCE_DELTA / (float)BOUNCE_TIME;
const D3DXVECTOR2 BLOCK_BBOX(48, 48);
const D3DXVECTOR2 BRICK_BBOX(48, 48);
const D3DXVECTOR2 COIN_BBOX(48, 48);

constexpr auto COIN_GRAVITY = 0.00093f;
constexpr auto COIN_BOUNCE = 0.5f;
constexpr auto DEBRIS_GRAVITY = 0.0026f;
constexpr auto MUSHROOM_VEL = 0.8f;
constexpr auto MUSHROOM_GRAVITY = 0.00093f *2;
constexpr auto MUSHROOM_SPEED = 0.05f*2;
constexpr auto DELTA = 60;
constexpr auto TIME = 100;
constexpr auto GROW_MAX_HEIGHT = 50;
const D3DXVECTOR2 MUSHROOM_BBOX(48, 48);
constexpr auto MUSHROOM_FORCE_Y = 0.004f;
constexpr auto MUSHROOM_PUSH_FORCE = 0.02f*2;