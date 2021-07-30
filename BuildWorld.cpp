#include "BuildWorld.h"
#include "Game.h"
#include "RectCollision.h"
#include "RectPlatform.h"
#include "QuestionBlock.h"
#include "QuestionBlockItem.h"
#include "Venus.h"
#include "Coin.h"
#include "RedGoomba.h"
#include "Koopas.h"
#include "KoopasFly.h"
#include "BrownGoomba.h"
#include "PipePortal.h"
#include "SelectionPortal.h"
#include "SelectionTree.h"
#include "IntroSceneObj.h"
#include "GoldenBrick.h"
#include "EndSceneItem.h"
#include "Death.h"
#include "TeleportGate.h"
#include "NoteBlock.h"
#include "BoomerangBrother.h"
#include "MiniGoomba.h"
#include "PlayScene.h"
#include "FireBullet.h"
#include "Boomerang.h"
#include "GoldenBrickCoin.h"

#define marginXWindow	96
#define	marginYWindow	272

#pragma region Init
CGameBuildWorld::CGameBuildWorld() {
}

CGameBuildWorld::~CGameBuildWorld()
{
	layers.clear();
	tilesets.clear();
}
#pragma endregion

#pragma region Update
void CGameBuildWorld::Update(int dt)
{
}
#pragma endregion

#pragma region CameraPosition
void CGameBuildWorld::UpdateCamPosition(Vector2 newPos) {
	camPosition = newPos;
}

Vector2 CGameBuildWorld::ConvertToPositionInCam(Vector2 oldPos) {
	return Vector2(oldPos.x - camPosition.x, oldPos.y - camPosition.y);
}
#pragma endregion

#pragma region Tile
Vector2 CGameBuildWorld::GetBound()
{
	return Vector2(this->width * tileWidth, this->height * tileHeight);
}

CTileSet* CGameBuildWorld::GetTileSetByTileID(int id)
{


	if (!(id >= kRender.rangeId.x && id <= kRender.rangeId.y)) {
		for (int i = 0; i < tilesets.size(); i++) {
			Vector2 tilesetRangeId = tilesets[i]->GetRangeId();
			if (id >= tilesetRangeId.x && id <= tilesetRangeId.y) {
				kRender.rangeId.x = tilesetRangeId.x;
				kRender.rangeId.y = tilesetRangeId.y;
				kRender.tileset = tilesets[i];
			}
		}
	}


	return kRender.tileset;
}

void CGameBuildWorld::AddTileSet(int firstgid, CTileSet* tileSet)
{
	this->tilesets[firstgid] = tileSet;
}
#pragma endregion

#pragma region Render
void CGameBuildWorld::AddLayer(CMapLayer* layer) {
	this->layers.push_back(layer);
}
void CGameBuildWorld::Render(float bottomMargin)
{
	int col = this->camPosition.x / tileWidth;
	int row = this->camPosition.y / tileHeight;

	float screenWidth = CGame::GetInstance()->GetScreenWidth() + marginXWindow;
	float screenHeight = CGame::GetInstance()->GetScreenHeight() + marginYWindow - bottomMargin;

	Vector2 drawingSize = Vector2(screenWidth / tileWidth, screenHeight / tileHeight);

	for (int i = col; i < drawingSize.x + col; i++) {
		for (int j = row; j < drawingSize.y + row; j++) {

			int x = i * tileWidth + tileWidth / 2;
			int y = j * tileHeight + tileHeight / 2;




			for (CMapLayer* layer : layers) {
				if (layer->Hidden) continue;
				int id = layer->GetTileID(i % width, j % height);
				if (id > 0) {
					this->GetTileSetByTileID(id)->Draw(id, ConvertToPositionInCam(Vector2(x, y)));
				}


			}
		}
	}
}
#pragma endregion

#pragma region Build Play Screen
CGameBuildWorld* CGameBuildWorld::FromTMX(string filePath, LPGAMEOBJECT* cameraLimitController, vector<LPGAMEOBJECT>* staticObjects)
{
	string fullPath = filePath;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		CGameBuildWorld* gameMap = new CGameBuildWorld();

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		Vector2 mapSize = gameMap->GetMapSize();
		gameMap->grid = CGrid(mapSize.x, mapSize.y);

		//tileset
		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			CTileSet* tileSet = new CTileSet(node, filePath);

			gameMap->tilesets.push_back(tileSet);
		}

		//layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			CMapLayer* layer = new CMapLayer(node);
			gameMap->AddLayer(layer);
		}

		//group objects
		
		for (TiXmlElement* objGroupNode = root->FirstChildElement("objectgroup"); objGroupNode != nullptr; objGroupNode = objGroupNode->NextSiblingElement("objectgroup")) {
			
			#pragma region Platform and Portal
			//Deathzone
			if (std::string(objGroupNode->Attribute("name")) == "Death") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CDeath(
						atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2,
						atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2,
						atoi(objNode->Attribute("width")),
						atoi(objNode->Attribute("height"))
					);
					
					staticObjects->push_back(obj);
				}
			}
			//Teleport_Gate
			else if (std::string(objGroupNode->Attribute("name")) == "Teleport_Gate") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {

					Vector2 Position, CameraLeftTopLimit, CameraRightBottomLimit;
					TiXmlElement* propertiesNode = objNode->FirstChildElement("properties");

					for (TiXmlElement* propertyNode = propertiesNode->FirstChildElement("property"); propertyNode != nullptr; propertyNode = propertyNode->NextSiblingElement("property")) {

						if (std::string(propertyNode->Attribute("name")) == "limit1_x")
							CameraLeftTopLimit.x = atoi(propertyNode->Attribute("value"));
						else if (std::string(propertyNode->Attribute("name")) == "limit1_y")
							CameraLeftTopLimit.y = atoi(propertyNode->Attribute("value"));

						else if (std::string(propertyNode->Attribute("name")) == "limit2_x")
							CameraRightBottomLimit.x = atoi(propertyNode->Attribute("value"));
						else if (std::string(propertyNode->Attribute("name")) == "limit2_y")
							CameraRightBottomLimit.y = atoi(propertyNode->Attribute("value"));

						else if (std::string(propertyNode->Attribute("name")) == "x")
							Position.x = atoi(propertyNode->Attribute("value"));
						else if (std::string(propertyNode->Attribute("name")) == "y")
							Position.y = atoi(propertyNode->Attribute("value"));
					}
					LPGAMEOBJECT obj = new CTeleportGate(
						atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2,
						atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2,
						atoi(objNode->Attribute("width")),
						atoi(objNode->Attribute("height")),
						Position, CameraLeftTopLimit, CameraRightBottomLimit
					);
					staticObjects->push_back(obj);
				}
			}
			//Pipe portal
			else if (std::string(objGroupNode->Attribute("name")) == "MiniPortal") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					//in
					if (std::string(objNode->Attribute("type")) == "In") {
						std::string portalName = std::string(objNode->Attribute("name"));
						int width = atoi(objNode->Attribute("width"));
						int height = atoi(objNode->Attribute("height"));
						int x = atoi(objNode->Attribute("x")) + width / 2;
						int y = atoi(objNode->Attribute("y")) + height / 2;
						int targetX = 0, targetY = 0, targetWidth = 0, targetHeight = 0;
						Vector2 CameraLeftTopLimit, CameraRightBottomLimit;
						std::string accessKeycode;
						//out
						for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
							if (std::string(objNode->Attribute("name")) == portalName && std::string(objNode->Attribute("type")) == "Out") {
								targetWidth = atoi(objNode->Attribute("width"));
								targetHeight = atoi(objNode->Attribute("height"));
								targetX = atoi(objNode->Attribute("x")) + targetWidth / 2;
								targetY = atoi(objNode->Attribute("y")) + targetHeight / 2;

								TiXmlElement* propertiesNode = objNode->FirstChildElement("properties");
								for (TiXmlElement* propertyNode = propertiesNode->FirstChildElement("property"); propertyNode != nullptr; propertyNode = propertyNode->NextSiblingElement("property")) {
									if (std::string(propertyNode->Attribute("name")) == "AccessKeyCode")
										accessKeycode = std::string(propertyNode->Attribute("value"));
									if (std::string(propertyNode->Attribute("name")) == "CameraLeftTopLimitX")
										CameraLeftTopLimit.x = atoi(propertyNode->Attribute("value"));
									else if (std::string(propertyNode->Attribute("name")) == "CameraLeftTopLimitY")
										CameraLeftTopLimit.y = atoi(propertyNode->Attribute("value"));
									else if (std::string(propertyNode->Attribute("name")) == "CameraRightBottomLimitX")
										CameraRightBottomLimit.x = atoi(propertyNode->Attribute("value"));
									else if (std::string(propertyNode->Attribute("name")) == "CameraRightBottomLimitY")
										CameraRightBottomLimit.y = atoi(propertyNode->Attribute("value"));
								}

								break;
							}
						}
						LPGAMEOBJECT obj = new CPipePortal(
							accessKeycode,
							Vector2(x, y),
							Vector2(width, height),
							Vector2(targetX, targetY),
							CameraLeftTopLimit,
							CameraRightBottomLimit
						);
						obj->AddPriority(EPriorityFlag::TEMP_OBJECT);
						gameMap->grid.AddObjectToGrid(obj);
					}
				}
			}
			//Rectangle Collision
			else if (std::string(objGroupNode->Attribute("name")) == "RectCollision") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CRectCollision(
						atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2,
						atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2,
						atoi(objNode->Attribute("width")),
						atoi(objNode->Attribute("height"))
					);
					staticObjects->push_back(obj);
				}
			}
			//Reactangle Platform
			else if (std::string(objGroupNode->Attribute("name")) == "RectPlatform") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CRectPlatform(
						atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2,
						atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2,
						atoi(objNode->Attribute("width")),
						atoi(objNode->Attribute("height"))
					);
					staticObjects->push_back(obj);
				}
			}
			#pragma endregion

			#pragma region Block
			//Golden Brick
			else if (std::string(objGroupNode->Attribute("name")) == "GoldenBrick") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
				std::string hiddenItemStr = "";
				auto hiddenItem = objNode->Attribute("type");
				if (hiddenItem) hiddenItemStr = std::string(hiddenItem);
				LPGAMEOBJECT obj = new CGoldenBrick(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					),
					hiddenItemStr
				);
				obj->AddPriority(EPriorityFlag::MAP_OBJECT);
				gameMap->grid.AddObjectToGrid(obj);
			}
			}
			//QuestionBox Coin
			else if (std::string(objGroupNode->Attribute("name")) == "QuestionBox_Coin") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
				LPGAMEOBJECT obj = new CQuestionBlock(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2))
				);
				obj->AddPriority(EPriorityFlag::MAP_OBJECT);
				gameMap->grid.AddObjectToGrid(obj);
			}
			}
			//Golden Brick Coin
			else if (std::string(objGroupNode->Attribute("name")) == "GoldenBrick_Coin") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {

				LPGAMEOBJECT obj = new GoldenBrickCoin(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2))
				);
				obj->AddPriority(EPriorityFlag::MAP_OBJECT);
				gameMap->grid.AddObjectToGrid(obj);
			}
			}
			//Note Block
			else if (std::string(objGroupNode->Attribute("name")) == "NoteBlock") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
				std::string hiddenItemStr = "";
				auto hiddenItem = objNode->Attribute("type");
				if (hiddenItem) hiddenItemStr = std::string(hiddenItem);
				LPGAMEOBJECT obj = new CNoteBlock(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					),
					hiddenItemStr);

				obj->AddPriority(EPriorityFlag::MAP_OBJECT);
				gameMap->grid.AddObjectToGrid(obj);

			}
			}
			//QuestionBox Item
			else if (std::string(objGroupNode->Attribute("name")) == "QuestionBox_Item") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
				LPGAMEOBJECT obj = new CQuestionBlockItem(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2))
				);
				obj->AddPriority(EPriorityFlag::MAP_OBJECT);
				gameMap->grid.AddObjectToGrid(obj);
			}
			}

			#pragma endregion

			#pragma region Item
			//Floating Coin
			else if (std::string(objGroupNode->Attribute("name")) == "Coin") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
				LPGAMEOBJECT obj = new CCoin(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2))
				);
				obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
				gameMap->grid.AddObjectToGrid(obj);

			}
			}
			//Winning card
			else if (std::string(objGroupNode->Attribute("name")) == "EndSceneItem") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
				LPGAMEOBJECT obj = new CEndSceneItem(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2))
				);
				obj->AddPriority(EPriorityFlag::TEMP_OBJECT);
				gameMap->grid.AddObjectToGrid(obj);
			}
			}
			#pragma endregion

			#pragma region Enemy
			//Goomba
			else if (std::string(objGroupNode->Attribute("name")) == "Goomba") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CGoomba(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					);
					obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
					gameMap->grid.AddObjectToGrid(obj);
					//dynamicObjects->push_back(obj);
				}
			}
			//Red-Paragoomba: Goomba co canh, nhay
			else if (std::string(objGroupNode->Attribute("name")) == "RedGoomba") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CRedGoomba(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					);
					obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
					gameMap->grid.AddObjectToGrid(obj);
				}
			}
			//Brown-Paragoomba; Goomba co canh bay
			else if (std::string(objGroupNode->Attribute("name")) == "BrownGoomba") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CBrownGoomba(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					);
					obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
					gameMap->grid.AddObjectToGrid(obj);
					for (int i = 0; i < 5; i++) {
						((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEnemyBullet(new CMiniGoomba(0, 0, 1));
					}
				}
			}
			//Koppa
			else if (std::string(objGroupNode->Attribute("name")) == "Koopas") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CKoopas(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					);
					obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
					gameMap->grid.AddObjectToGrid(obj);
				}
			}
			//Flying Koppa
			else if (std::string(objGroupNode->Attribute("name")) == "KoopasFly") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CKoopasFly(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					);
					obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
					gameMap->grid.AddObjectToGrid(obj);
				}
			}
			//Venus plant
			else if (std::string(objGroupNode->Attribute("name")) == "Venus") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
				LPGAMEOBJECT obj = new CVenus(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					),
					Vector2(
						(int)atoi(objNode->Attribute("width")),
						(int)atoi(objNode->Attribute("height"))
					),
					atoi(objNode->Attribute("type"))
				);
				obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT_BEHIND_MAP);
				gameMap->grid.AddObjectToGrid(obj);
				for (int i = 0; i < 2; i++) {
					((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEnemyBullet(new CFireBullet(0, 0, 1, 1));
				}
			}
			}
			//Boomerang Brother
			else if (std::string(objGroupNode->Attribute("name")) == "BoomerangBrother") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CBoomerangBrother(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					);
					obj->AddPriority(EPriorityFlag::DYNAMIC_OBJECT);
					gameMap->grid.AddObjectToGrid(obj);
					for (int i = 0; i < 2; i++) {
						((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->PushEnemyBullet(new CBoomerang(0, 0, 1));
					}
				}
			}
#pragma endregion			
			
		}
		return gameMap;
	}
	throw "Load Play scene bi Loi";
}
#pragma endregion

#pragma region Build Selection Scene

CGameBuildWorld* CGameBuildWorld::FromTMX(string filePath, vector<LPGAMEOBJECT>* staticObjects, std::unordered_map<std::string, CSelectionNode*>* selectionNodes)
{
	string fullPath = filePath;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		CGameBuildWorld* gameMap = new CGameBuildWorld();

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		//Load tileset

		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			CTileSet* tileSet = new CTileSet(node, filePath);

			gameMap->tilesets.push_back(tileSet);
		}

		//Load layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			CMapLayer* layer = new CMapLayer(node);
			gameMap->AddLayer(layer);
		}

		// Load collision group objects
		for (TiXmlElement* objGroupNode = root->FirstChildElement("objectgroup"); objGroupNode != nullptr; objGroupNode = objGroupNode->NextSiblingElement("objectgroup")) {


			// Selection Scene
			if (std::string(objGroupNode->Attribute("name")) == "SelectionPortal") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {

					LPGAMEOBJECT obj = new CSelectionPortal(
						Vector2(
							(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
							(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
						),
						Vector2(
							(int)(atoi(objNode->Attribute("width")) + atoi(objNode->Attribute("width")) / 2),
							(int)(atoi(objNode->Attribute("height")) + atoi(objNode->Attribute("height")) / 2)
						),
						std::string(objNode->Attribute("name"))
					);
					staticObjects->push_back(obj);
				}
			}

			else if (std::string(objGroupNode->Attribute("name")) == "SelectionTree") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {

					LPGAMEOBJECT obj = new CSelectionTree(
						Vector2(
							(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
							(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
						)
					);
					staticObjects->push_back(obj);
				}
			}

			else if (std::string(objGroupNode->Attribute("name")) == "SelectionNode") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {

					std::string nodeName = std::string(objNode->Attribute("name"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));
					int x = atoi(objNode->Attribute("x")) + width / 2;
					int y = atoi(objNode->Attribute("y")) + height / 2;

					CSelectionNode* seNode = new CSelectionNode(nodeName, Vector2(x, y));


					TiXmlElement* propertiesNode = objNode->FirstChildElement("properties");
					for (TiXmlElement* propertyNode = propertiesNode->FirstChildElement("property"); propertyNode != nullptr; propertyNode = propertyNode->NextSiblingElement("property")) {
						std::string keyDirection = std::string(propertyNode->Attribute("name"));
						std::string targetNodeName = std::string(propertyNode->Attribute("value"));
						seNode->AddMovableNode(keyDirection, targetNodeName);
					}
					(*selectionNodes)[nodeName] = seNode;

				}
			}



		}

		return gameMap;
	}

	throw "Load map that bai!!";
}

#pragma endregion

#pragma region Build Intro Scence

CGameBuildWorld* CGameBuildWorld::FromTMX(string filePath, vector<LPGAMEOBJECT>* staticObjects)
{
	string fullPath = filePath;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		CGameBuildWorld* gameMap = new CGameBuildWorld();

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		//Load tileset

		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			CTileSet* tileSet = new CTileSet(node, filePath);

			gameMap->tilesets.push_back(tileSet);
		}

		//Load layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			CMapLayer* layer = new CMapLayer(node);
			gameMap->AddLayer(layer);
		}

		// Load collision group objects
		for (TiXmlElement* objGroupNode = root->FirstChildElement("objectgroup"); objGroupNode != nullptr; objGroupNode = objGroupNode->NextSiblingElement("objectgroup")) {
			std::string objName = std::string(objGroupNode->Attribute("name"));

			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {

				LPGAMEOBJECT obj = new CIntroSceneObj(
					Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					),
					objName
				);
				staticObjects->push_back(obj);
			}


		}

		return gameMap;
	}

	throw "Load map Loi!!";
}


#pragma endregion

#pragma region Map Size

void CGameBuildWorld::GetMapSize(Vector2& out) {
	out.x = this->width * tileWidth;
	out.y = this->height * tileHeight;
}

Vector2 CGameBuildWorld::GetMapSize() {
	return Vector2(this->width * tileWidth, this->height * tileHeight);
}
#pragma endregion




