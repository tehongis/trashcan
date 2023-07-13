#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SOUND
#include "Extensions/olcPGEX_Sound.h"

#include <math.h>

#define MAP_SIZE_X 32
#define MAP_SIZE_Y 32
#define MAP_SIZE MAP_SIZE_X*MAP_SIZE_Y


class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

	//olc::SOUND::AudioSample music;
	int music;

	olc::Sprite* sprSheet = nullptr;
	olc::Decal* decSheet = nullptr;

	olc::Sprite* sprPlayer = nullptr;
	olc::Decal* decPlayer = nullptr;

	olc::Sprite* sprBlock = nullptr;
	olc::Decal* decBlock = nullptr;

	olc::Sprite* sprWall = nullptr;
	olc::Decal* decWall = nullptr;

	float fAngle = 0;

	float playerX = 20.0f;
	float playerY = 20.0f;

	int myMapArray [MAP_SIZE] = {\
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,\
		1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,\
		1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,\
		1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,\
		1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
		1,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,\
		1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,\
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,\
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

public:
	bool OnUserCreate() override
	{

		olc::SOUND::InitialiseAudio(44100, 2, 4, 256);

		music = olc::SOUND::LoadAudioSample("Chiptunes/Title Screen.wav");
		if (music == -1) {
			printf("Music loading failed.\n");
			exit(0);
		}

		olc::SOUND::PlaySample(music, true);

		sprSheet = new olc::Sprite("1bitpack_kenney_1.1/Tilesheet/colored_transparent_packed.png");
		decSheet = new olc::Decal(sprSheet);

		olc::vi2d vSize = {16 , 16};

		olc::vi2d vPos = {16 * 26 , 16 * 0};
		sprPlayer = sprSheet->Duplicate(vPos, vSize);
		decPlayer = new olc::Decal(sprPlayer);

		vPos = {16 * 16 , 16 * 0};
		sprBlock = sprSheet->Duplicate(vPos, vSize);
		decBlock = new olc::Decal(sprBlock);

		vPos = {16 * 26 , 16 * 1};
		sprWall = sprSheet->Duplicate(vPos, vSize);
		decWall = new olc::Decal(sprWall);

		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		if (GetKey(olc::Key::A).bHeld) {
			playerX = playerX - 32.0f * fElapsedTime;
		}
		if (GetKey(olc::Key::D).bHeld) {
			playerX = playerX + 32.0f * fElapsedTime;
		}

		if (GetKey(olc::Key::W).bHeld) {
			playerY = playerY - 32.0f * fElapsedTime;
		}
		if (GetKey(olc::Key::S).bHeld) {
			playerY = playerY + 32.0f * fElapsedTime;
		}

		Clear(olc::BLACK);

		for (int countY = 0; countY < MAP_SIZE_Y ; countY = countY + 1) {
			for (int countX = 0 ; countX < MAP_SIZE_X ; countX = countX + 1 ) {
				float mapX = countX * 16.0f;
				float mapY = countY * 16.0f;
				char value = myMapArray[MAP_SIZE_X*countY+countX];
				if (value == 1) {
					DrawDecal({mapX,mapY}, decBlock,{1.0f,1.0f});
				}
			}
		}

		//DrawDecal({0.0f,50.0f}, decSheet,{1.0f,1.0f});
		olc::vf2d mouse = { float(GetMouseX()),float(GetMouseY()) };
		olc::vf2d loc = {mouse.x,mouse.y};

		DrawDecal({playerX,playerY}, decPlayer,{1.0f,1.0f});

		//DrawDecal({20.0f,20.0f}, decBlock,{1.0f,1.0f});
		DrawRotatedDecal({80.0f,80.0f},decWall,fAngle,{8.0f,8.0f},{1.0f,1.0f});

		fAngle = fAngle + (1.0 * fElapsedTime);
		if (fAngle > M_PI*2) {
			fAngle = fAngle - M_PI*2;
		}

		return true;
	}

	// Note we must shut down the sound system too!!
	bool OnUserDestroy()
	{
		olc::SOUND::DestroyAudio();
		return true;
	}

};

int main()
{
	Example demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();

	return 0;
}
