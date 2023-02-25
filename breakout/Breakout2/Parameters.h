#pragma once
class Parameters
{
	private:
		const static int WINDOW_WIDTH = 700;
		const static int WINDOW_HEIGHT = 500;
		const static int FRAMES_PER_SECOND = 60;
		const static int UPDATES_PER_FRAME = 30;
		const static int WALL_THICKNESS = 3;
		const static int BOTTOM_OFFSET = 70;
		const static int PLAYER_WIDTH_PERCENTAGE = 20;
		const static int PLAYER_HEIGHT= 5;
		const static int PLAYER_LIVES = 5;

	public:
		static float getWindowWidth() { return WINDOW_WIDTH; }
		static float getWindowHeight() { return WINDOW_HEIGHT; }
		static float getBottomOffset() { return BOTTOM_OFFSET; }
		static float getWallThickness() { return WALL_THICKNESS; }
		static int getFramesPerSecond() { return FRAMES_PER_SECOND; }
		static int getUpdatesPerFrame() { return UPDATES_PER_FRAME; }
		static int getPlayerHeight() { return PLAYER_HEIGHT; }
		static float getPlayerWidthPercentage() { return (float)PLAYER_WIDTH_PERCENTAGE/100; }
		static int getPlaygroundBottom() { return WINDOW_HEIGHT - BOTTOM_OFFSET - WALL_THICKNESS; }
		static int getPlayerLives() { return PLAYER_LIVES; }
};

