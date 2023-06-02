#pragma once
#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <map>

#include "Animation.h"

class CAnimationManager;
typedef CAnimationManager* LPAnimationManager;
class CAnimationManager
{
private:
	static LPAnimationManager instance;
	std::map<std::string, LPAnimation> animations;
public:
	void Init();
	void Clear();
	bool InitAnAnimationSet(std::string textureName, std::string filePath);
	bool LoadAnimation(std::string texName, std::string filePath);
	void AddAnimation(std::string id, LPAnimation ani);
	LPAnimation Get(std::string id);
	LPAnimation Clone(std::string id); // Cloning is neccessary, without it every object will have the same animation
	static LPAnimationManager GetInstance();
};
#endif