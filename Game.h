#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Scene.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024


struct SKeyboardEvent {
	int key;
	bool isHold;
	bool isKeyUp;
};

class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;
	

	/*float cam_x = 0.0f;
	float cam_y = 0.0f;*/

	int screen_width;
	int screen_height;

	unordered_map<std::string, LPSCENE> scenes;
	std::string current_scene;
	std::string current_scene_type;
	

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	/*void Draw(Vector2 finalPos, Vector2 pivot = Vector2(0,0), LPDIRECT3DTEXTURE9 texture = NULL, RECT r = RECT(), int alpha = 255);
	void DrawFlipY(Vector2 finalPos, Vector2 pointCenter, Vector2 pivot = Vector2(0, 0), LPDIRECT3DTEXTURE9 texture = NULL, RECT rect = RECT(), int alpha = 255);*/
	void DrawWithScaling(Vector2 finalPos, Vector2 pivot = Vector2(0, 0), LPDIRECT3DTEXTURE9 texture = NULL, RECT rect = RECT(), int alpha = 255, Vector2 scale = Vector2(1,1));

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	bool Load(std::string gameFile);

	std::string GetCurrentSceneType() { return current_scene_type; }
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(std::string scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }
	ID3DXFont* font;
	void KDrawBoardDetails(float x, float y, LPCSTR text);
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	/*void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }*/

	static CGame* GetInstance();

	// Sprite Handler
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 
	
	SKeyboardEvent GenerateKeyboardEvent(int key, bool isHold = false, bool isKeyUp = false);

	~CGame();
};


