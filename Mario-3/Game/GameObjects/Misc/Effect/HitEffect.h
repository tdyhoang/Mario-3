#pragma once

#include "EffectObject.h"

class CHitEffect : public CEffectObject
{
	DWORD startHitTime;
public:
	CHitEffect();
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;
	void SetStartHitTime(DWORD t);

};