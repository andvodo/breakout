#include "SoundManager.h"
/*#include "LevelManager.h"

using namespace sf;

SoundManager* SoundManager::_instance;

SoundManager::SoundManager() { }

void SoundManager::initialize() {
	Level* level = LevelManager::getCurrentLevel();
	SoundBuffer sb1;
	sb1.loadFromFile(level->getBrickDataForType(BrickType::Soft)->getHitSound());
	Sound sound1(sb1);
	soundsHit[BrickType::Soft] = sound1;

	SoundBuffer sb2;
	sb2.loadFromFile(level->getBrickDataForType(BrickType::Medium)->getHitSound());
	Sound sound2(sb2);
	soundsHit[BrickType::Medium] = sound2;

	SoundBuffer sb3;
	sb3.loadFromFile(level->getBrickDataForType(BrickType::Hard)->getHitSound());
	Sound sound3(sb3);
	soundsHit[BrickType::Hard] = sound3;

	SoundBuffer sb4;
	sb4.loadFromFile(level->getBrickDataForType(BrickType::Impenetrable)->getHitSound());
	Sound sound4(sb4);
	soundsHit[BrickType::Impenetrable] = sound4;


	Level* level = LevelManager::getCurrentLevel();
	SoundBuffer sb1;
	sb1.loadFromFile(level->getBrickDataForType(BrickType::Soft)->getBreakSound());
	Sound sound1(sb1);
	soundsHit[BrickType::Soft] = sound1;

	SoundBuffer sb2;
	sb2.loadFromFile(level->getBrickDataForType(BrickType::Medium)->getBreakSound());
	Sound sound2(sb2);
	soundsHit[BrickType::Medium] = sound2;

	SoundBuffer sb3;
	sb3.loadFromFile(level->getBrickDataForType(BrickType::Hard)->getBreakSound());
	Sound sound3(sb3);
	soundsHit[BrickType::Hard] = sound3;

	SoundBuffer sb4;
	sb4.loadFromFile(level->getBrickDataForType(BrickType::Impenetrable)->getBreakSound());
	Sound sound4(sb4);
	soundsHit[BrickType::Impenetrable] = sound4;
}

void SoundManager::playSoundHit(BrickType brickType)
{
	Sound sound = soundsHit.at(brickType);
	sound.setPitch(_volume);
	sound.play();
}

SoundManager* SoundManager::getInstance() {
	return _instance;
}
*/