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

const D3DXVECTOR2 BLOCK_BBOX(48, 48);
const D3DXVECTOR2 BRICK_BBOX(48, 48);
const D3DXVECTOR2 COIN_BBOX(48, 48);

#define DEBRIS_GRAVITY 0.0026f
#define DEBRIS_MIN_FALL 0.8f