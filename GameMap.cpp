#include "GameMap.h"
#include "SpriteManager.h"
#include "RectCollision.h"

//GameMap::GameMap(const char* filePath, std::vector<LPGAMEOBJECT>* listObjects)
GameMap::GameMap(const char* filePath)
{
    LoadMap(filePath);
    //this->listObjects = listObjects;

}

GameMap::~GameMap()
{
    delete mMap;
}

void GameMap::UpdateCamPosition(Vector2 newPos) {
    camPosition = newPos;
}

Vector2 GameMap::ConvertToPositionInCam(Vector2 oldPos) {
    return Vector2(oldPos.x - camPosition.x, oldPos.y - camPosition.y);
}

void GameMap::LoadMap(const char* filePath)
{
    mMap = new Tmx::Map();
    mMap->ParseFile(filePath);

    RECT r;
    r.left = 0;
    r.top = 0;
    r.right = this->GetWidth();
    r.bottom = this->GetHeight();

    for (size_t i = 0; i < mMap->GetNumTilesets(); i++)
    {
        string idTileSet = "132034" + i;
        const Tmx::Tileset* tileset = mMap->GetTileset(i);

        CTextures::GetInstance()->Add(idTileSet, ToLPCWSTR(tileset->GetImage()->GetSource().c_str()), D3DCOLOR_XRGB(255, 128, 192));
        //CSprite* sprite = new CSprite(tileset->GetImage()->GetSource().c_str(), RECT(), 0, 0, D3DCOLOR_XRGB(255, 128, 192));
        //CSprite* sprite = new CSprite(idTileSet, 0,0,160,160, CTextures::GetInstance()->Get(idTileSet));
        mListTilesetId.push_back(idTileSet);

        //mListTileset.insert(std::pair<int, CSprite*>(i, sprite));
    }

}

Tmx::Map* GameMap::GetMap()
{
    return mMap;
}

int GameMap::GetWidth()
{
    return mMap->GetWidth() * mMap->GetTileWidth();
}

int GameMap::GetHeight()
{
    return mMap->GetHeight() * mMap->GetTileHeight();
}

int GameMap::GetTileWidth()
{
    return mMap->GetTileWidth();
}

int GameMap::GetTileHeight()
{
    return mMap->GetTileHeight();
}

bool GameMap::isExistInList(int a) {
    for (int i = 0; i < listNoCollision.size(); i++) {

    }
    return false;
}

void GameMap::Draw()
{
    for (size_t i = 0; i < mMap->GetNumTileLayers(); i++)
    {
        auto abc = mMap->GetNumTileLayers();
        const Tmx::TileLayer* layer = mMap->GetTileLayer(i);

        if (!layer->IsVisible())
        {
            continue;
        }

        RECT sourceRECT;



        for (size_t m = 0; m < layer->GetHeight(); m++)
        {
            for (size_t n = 0; n < layer->GetWidth(); n++)
            {
                int tilesetIndex = layer->GetTileTilesetIndex(n, m);

                if (tilesetIndex != -1)
                {
                    const Tmx::Tileset* tileSet = mMap->GetTileset(tilesetIndex);

                    int tileWidth = mMap->GetTileWidth();
                    int tileHeight = mMap->GetTileHeight();

                    auto margin = tileSet->GetMargin();

                    int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
                    int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;


                    string kID = mListTilesetId[layer->GetTileTilesetIndex(n, m)];

                    //tile index
                    int tileID = layer->GetTileId(n, m);

                    int y = tileID / tileSetWidth;
                    int x = tileID - y * tileSetWidth;

                    //TODO
                    /*sourceRECT.top = y * tileHeight + margin;
                    sourceRECT.bottom = sourceRECT.top + tileHeight - margin;
                    sourceRECT.left = x * tileWidth + margin;
                    sourceRECT.right = sourceRECT.left + tileWidth - margin;*/

                    sourceRECT.top = y * tileHeight;
                    sourceRECT.bottom = sourceRECT.top + tileHeight;
                    sourceRECT.left = x * tileWidth;
                    sourceRECT.right = sourceRECT.left + tileWidth;

                    //TODO
                    /*tileWidth -= 1;
                    tileHeight -= 1;*/



                    //tru tilewidth/2 va tileheight/2 vi Sprite ve o vi tri giua hinh anh cho nen doi hinh de cho
                    //dung toa do (0,0) cua the gioi thuc la (0,0) neu khong thi se la (-tilewidth/2, -tileheigth/2);
                    D3DXVECTOR2 position(
                        n * tileWidth,
                        m * tileHeight);
                    auto nameLayer = layer->GetName();
                    /*sprite->SetWidth(tileWidth);
                    sprite->SetHeight(tileHeight);*/
                    //sprite->Draw(position, sourceRECT, D3DXVECTOR2(), D3DXVECTOR2(), 0.0f, D3DXVECTOR2(), D3DCOLOR_XRGB(255, 128, 192));
                    //CSprite(0, sourceRECT.left, sourceRECT.top, sourceRECT.right, sourceRECT.bottom, CTextures::GetInstance()->Get(kID)).Draw(17, 100, 0);
                    auto aaa = ConvertToPositionInCam(Vector2((int)position.x, (int)position.y));
                    CSprite az = CSprite("0", sourceRECT.left, sourceRECT.top, sourceRECT.right, sourceRECT.bottom, CTextures::GetInstance()->Get(kID));
                    
                    //az.Draw(ConvertToPositionInCam(Vector2((int)position.x, (int)position.y)), 255);
                    az.DrawWithScaling(ConvertToPositionInCam(Vector2((int)position.x, (int)position.y)));



                    /*CSprites::GetInstance()->Add(1111, 0, 0, 16, 16, CTextures::GetInstance()->Get(kID));
                    CSprites::GetInstance()->Get(1111)->Draw(17, 100, 200);*/
                }
            }
        }

    }

    if (!hasLoaded) {
        hasLoaded = true;
        for (size_t i = 0; i < mMap->GetNumObjectGroups(); i++)
        {

            if (mMap->GetObjectGroup(i)->GetName() == "Collision")
            {
                auto objs = mMap->GetObjectGroup(i)->GetObjects();
                for (int j = 0; j < objs.size(); ++j)
                {
                    const Tmx::Object* kObject = objs.at(j);
                    float x = kObject->GetX();
                    float y = kObject->GetY();
                    float width = kObject->GetWidth();
                    float height = kObject->GetHeight();
                    LPGAMEOBJECT rectCollision = new CRectCollision(x, y, width, height);
                    //listObjects->push_back(rectCollision);


                }



            }

        }
    }

}
