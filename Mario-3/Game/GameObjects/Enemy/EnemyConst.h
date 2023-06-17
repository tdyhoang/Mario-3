#pragma once

enum class EnemyType
{
	basic, red
};

enum class EnemyTag
{
	Goomba, ParaGoomba, Koopa, KoopaShell, ParaKoopa, Piranha, Venus, BoomerangBrother
};

const int GOOMBA_DIE_TIME = 300;
const float GOOMBA_GRAVITY = 0.00093f * 2;
const float GOOMBA_SPEED = 0.039f * 2;
const float GOOMBA_HIT_FORCE = 0.5f * 1.5f;
const float GOOMBA_HIT_MAX_HEIGHT = 170.0f * 1.5f;
const float GOOMBA_HIT_FORCE_X = 0.08f * 2;
const D3DXVECTOR2 GOOMBA_BBOX(16 * 3, 16 * 3);
const D3DXVECTOR2 GOOMBA_DIE_BBOX(16 * 3, 9 * 3);

enum class GoombaState
{
	Walk, Die, HeadShot
};