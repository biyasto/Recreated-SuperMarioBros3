#include <iostream>
#include <fstream>

#include "Game.h"
#include "Utils.h"

#include "IntroScene.h"
#include "SelectionScene.h"
#include "PlayScene.h"

#include "SpriteManager.h"
#include "AnimationSet.h"


CGame* CGame::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void CGame::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 
	
	
	

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	screen_height = r.bottom + 1;
	screen_width = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);
	font = NULL;
	D3DXCreateFont(d3ddv, 10, 7, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial", &font);
	OutputDebugString(L"[INFO] InitGame done;\n");

	// Backup initial matrix of SpriteHandler
	//spriteHandler->GetTransform(&initialTransform);
}

//void CGame::ResetTransform() {
//	spriteHandler->SetTransform(&initialTransform);
//}
//void CGame::SetNewTransform(D3DXMATRIX newTransform) {
//	spriteHandler->SetTransform(&newTransform);
//}

void CGame::KDrawBoardDetails(float x, float y, LPCSTR text) {
	
	RECT* a = new RECT();
	a->left = x; a->right = 100; a->top = y; a->bottom = 100;

	CGame::GetInstance()->font->DrawTextA(NULL, text, -1, a, DT_LEFT, D3DCOLOR_XRGB(0, 0, 0));

}

/*
	Utility function to wrap LPD3DXSPRITE::Draw
*/
//void CGame::Draw(Vector2 finalPos, D3DXVECTOR2 pivot, LPDIRECT3DTEXTURE9 texture, RECT r, int alpha)
//{
//	Vector3 pInt((int)(finalPos.x - pivot.x), (int)(finalPos.y - pivot.y), 0);
//	spriteHandler->Draw(texture, &r, NULL, &pInt, D3DCOLOR_ARGB(alpha, 255, 255, 255));
//
//}
//
//void CGame::DrawFlipY(Vector2 finalPos, Vector2 pointCenter, Vector2 pivot, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha)
//{
//
//	Vector3 p(finalPos.x, finalPos.y, 0);
//	Vector3 pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
//	Vector2 pScale(-1, 1);
//	D3DXMATRIX oldMatrix, newMatrix;
//	spriteHandler->GetTransform(&oldMatrix);
//
//	D3DXMatrixTransformation2D(&newMatrix, &finalPos, 0.0f, &pScale, NULL, 0.0f, NULL);
//	spriteHandler->SetTransform(&newMatrix);
//
//	Vector3 pInt((int)(finalPos.x), (int)(finalPos.y), 0);
//
//	spriteHandler->Draw(texture, &rect, &pCenter, &pInt, D3DCOLOR_ARGB(alpha, 255, 255, 255));
//	spriteHandler->SetTransform(&oldMatrix);
//}

void CGame::DrawWithScaling(Vector2 finalPos, Vector2 pivot, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha, Vector2 scale)
{
	finalPos.x = (int)finalPos.x;
	finalPos.y = (int)finalPos.y;
	Vector2 deltaToCenter = Vector2((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);
	Vector3 pCenter = Vector3(deltaToCenter.x + pivot.x, deltaToCenter.y + pivot.y, 0);
	Vector3 pPosition = Vector3(finalPos.x, finalPos.y, 0);
	if (scale.x == 1 && scale.y == 1) {
		
		spriteHandler->Draw(texture, &rect, &pCenter, &pPosition, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}
	else {
		D3DXMATRIX oldMatrix, newMatrix;
		spriteHandler->GetTransform(&oldMatrix);

		D3DXMatrixTransformation2D(&newMatrix, &(finalPos), 0.0f, &scale, &finalPos, 0.0f, NULL);
		spriteHandler->SetTransform(&newMatrix);

		spriteHandler->Draw(texture, &rect, &pCenter, &pPosition, D3DCOLOR_ARGB(alpha, 255, 255, 255));
		spriteHandler->SetTransform(&oldMatrix);
	}
	
	
}

//void CGame::DrawWithScaling(float x, float y, LPDIRECT3DTEXTURE9 texture,
//	int left, int top, int right, int bottom, 
//	D3DXVECTOR2 scalingCenter, D3DXVECTOR2 scale, int alpha)
//{
//	
//	/*D3DXVECTOR3 p(x - cam_x, y - cam_y, 0);*/
//	D3DXVECTOR3 p(x, y, 0);
//	RECT r;
//	r.left = left;
//	r.top = top;
//	r.right = right;
//	r.bottom = bottom;
//
//	D3DXMATRIX oldMatrix, newMatrix;
//	spriteHandler->GetTransform(&oldMatrix);
//	
//	/*D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2(x, y), 0, &D3DXVECTOR2(0.8, 1),
//		&D3DXVECTOR2(x, y), 0.0f, &D3DXVECTOR2(0.0f, 0.0f));*/
//	/*D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2(x - cam_x + ((right - left) / 2), y - cam_y + ((bottom - top) / 2)), 0, &scale,
//		&D3DXVECTOR2(x - cam_x + ((right - left) / 2), y - cam_y + ((bottom - top) / 2)), 0.0f, &D3DXVECTOR2(0.0f, 0.0f));*/
//	D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2(x  + ((right - left) / 2), y  + ((bottom - top) / 2)), 0, &scale,
//		&D3DXVECTOR2(x + ((right - left) / 2), y  + ((bottom - top) / 2)), 0.0f, &D3DXVECTOR2(0.0f, 0.0f));
//	spriteHandler->SetTransform(&newMatrix);
//	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
//	spriteHandler->SetTransform(&oldMatrix);
//}

int CGame::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}

void CGame::InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
			);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8Create failed!\n");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}

void CGame::ProcessKeyboard()
{
	HRESULT hr;

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	keyHandler->KeyState((BYTE*)&keyStates);



	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}

/*
	Standard sweptAABB implementation
	Source: GameDev.net
*/
void CGame::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (st == sb || sl == sr) return;

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}

CGame* CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2


/*
	Load game campaign file and load/initiate first scene
*/
bool CGame::Load(std::string gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	TiXmlDocument document(gameFile.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();
	TiXmlElement* KScenes = root->FirstChildElement("Scenes");
	current_scene = (KScenes->Attribute("start"));

	for (TiXmlElement* KScene = KScenes->FirstChildElement("Scene"); KScene != nullptr; KScene = KScene->NextSiblingElement("Scene")) {
		std::string sceneId = KScene->Attribute("id");
		std::string scenePath = KScene->Attribute("path");
		std::string type = KScene->Attribute("type");
		
		if (type == "play-scene") {
			LPSCENE scene = new CPlayScene(sceneId, scenePath, type);
			scenes[sceneId] = scene;
		}
		else if (type == "selection-scene") {
			LPSCENE scene = new CSelectionScene(sceneId, scenePath, type);
			scenes[sceneId] = scene;
		}
		else if (type == "intro-scene") {
			LPSCENE scene = new CIntroScene(sceneId, scenePath, type);
			scenes[sceneId] = scene;
		}
		
	}
	

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);
	SwitchScene(current_scene);
	
}

void CGame::SwitchScene(std::string scene_id)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);

	scenes[current_scene]->Unload();

	CTextures::GetInstance()->Clear();
	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();

	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	current_scene_type = s->GetSceneType();
	CGame::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());
	s->Load();
}

SKeyboardEvent CGame::GenerateKeyboardEvent(int key, bool isHold, bool isKeyUp) {
	SKeyboardEvent result;
	result.key = key;
	result.isHold = isHold;
	result.isKeyUp = isKeyUp;
	return result;
}