/*#pragma once
#include <SFML/Audio.hpp>
#include "BrickData.h"
#include <map>

class SoundManager
{
	private:
		SoundManager();
		static SoundManager* _instance;
		std::map<BrickType, sf::Sound> soundsBreak;
		std::map<BrickType, sf::Sound> soundsHit;
		std::vector<sf::SoundBuffer> soundBuffers;
		float _volume = 1;

	public:
		static SoundManager* getInstance();
		void initialize();
		void playSoundHit(BrickType brickType);
		void playBrickHit(BrickType brickType);
		void playBrickBroken(BrickType brickType);
};
*/