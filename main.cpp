#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <stdint.h>

#define FULLSCREEN true
#define SCREEN_RES_W 1920
#define SCREEN_RES_H 1080
#define PIXEL_SIZE 2

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Testing";
	}

private:

	olc::vi2d vWorldSize = { 20, 20 };	// Number of tiles in world
	olc::vi2d vTileSize = { 40, 20 };	// Size of single tile graphic
	olc::vi2d vOrigin = { 11, 3 };		// Where to place tile (0,0) on screen (in tile size steps)	
	olc::Sprite *sprIsom = nullptr;		// Sprite that holds all imagery
	int *pWorld = nullptr;				// Pointer to create 2D world array

	char worldMap[20*20] = {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'T', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'T', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'T', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'T', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'T', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 's',
		' ', ' ', ' ', ' ', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 's', 's', 's', 's', 'w',
		' ', ' ', 't', ' ', ' ', 't', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 's', 's', 's', 's', 's', 'w',
		' ', ' ', ' ', 't', ' ', 't', ' ', 't', ' ', ' ', ' ', ' ', ' ', ' ', 's', 'w', 'w', 'w', 'w', 'w',
		' ', 't', ' ', ' ', 't', 't', ' ', 't', ' ', 't', ' ', ' ', ' ', ' ', 's', 'w', 'w', 'w', 'w', 'w',
		' ', ' ', 't', 't', ' ', ' ', 't', ' ', 't', ' ', ' ', ' ', ' ', 's', 's', 'w', 'w', 'w', 'w', 'w',
		' ', ' ', 't', ' ', ' ', 't', ' ', 't', ' ', ' ', ' ', ' ', ' ', 's', 'w', 'w', 'w', 'w', 'w', 'w'
	};

public:
	bool OnUserCreate() override
	{
		
		sprIsom = new olc::Sprite("./gfx/iso_tiles.png");			// Load sprites used in demonstration
		pWorld = new int[vWorldSize.x * vWorldSize.y]{ 0 };			// Create empty world
		for(int i = 0; i < vWorldSize.x * vWorldSize.y; i++){
			if(worldMap[i] == ' ')
				pWorld[i] = 1;
			if(worldMap[i] == 't')
				pWorld[i] = 2;
			if(worldMap[i] == 'T')
				pWorld[i] = 3;
			if(worldMap[i] == 's')
				pWorld[i] = 4;
			if(worldMap[i] == 'w')
				pWorld[i] = 5;
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::Pixel(150, 150, 250));

		if(GetKey(olc::Key::ESCAPE).bPressed) return false;							// Close the app on 'Esc' key

		olc::vi2d vMouse = { GetMouseX(), GetMouseY() };								// Get Mouse in world
		
		olc::vi2d vCell = { vMouse.x / vTileSize.x, vMouse.y / vTileSize.y };			// Work out active cell
		olc::vi2d vOffset = { vMouse.x % vTileSize.x, vMouse.y % vTileSize.y };			// Work out mouse offset into cell

		olc::Pixel col = sprIsom->GetPixel(3 * vTileSize.x + vOffset.x, vOffset.y);		// Sample into cell offset colour

		olc::vi2d vSelected = 															// Work out selected cell by transforming screen cell
		{
			(vCell.y - vOrigin.y) + (vCell.x - vOrigin.x),
			(vCell.y - vOrigin.y) - (vCell.x - vOrigin.x) 
		};

		if (col == olc::RED) vSelected += {-1, +0};										// "Bodge" selected cell by sampling corners
		if (col == olc::BLUE) vSelected += {+0, -1};
		if (col == olc::GREEN) vSelected += {+0, +1};
		if (col == olc::YELLOW) vSelected += {+1, +0};

		if (GetMouse(0).bPressed) {														// Handle mouse click
			// Guard array boundary
			if (vSelected.x >= 0 && vSelected.x < vWorldSize.x && vSelected.y >= 0 && vSelected.y < vWorldSize.y)
				++pWorld[vSelected.y * vWorldSize.x + vSelected.x] %= 6;				// Toggle if a tile is visible or not
		}
		
		auto ToScreen = [&](int x, int y) {												// Labmda function to convert "world" coordinate into screen space
			return olc::vi2d {
				(vOrigin.x * vTileSize.x) + (x - y) * (vTileSize.x / 2),
				(vOrigin.y * vTileSize.y) + (x + y) * (vTileSize.y / 2)
			};
		};
		
		SetPixelMode(olc::Pixel::MASK);													// Draw World - has binary transparancy so enable masking

		// (0,0) is at top, defined by vOrigin, so draw from top to bottom
		// to ensure tiles closest to camera are drawn last
		for (int y = 0; y < vWorldSize.y; y++) {
			for (int x = 0; x < vWorldSize.x; x++) {
				// Convert cell coordinate to world space
				olc::vi2d vWorld = ToScreen(x, y);
				
				switch(pWorld[y*vWorldSize.x + x]) {
				case 0: // Invisble Tile
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 1: // Visible Tile
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 2 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 2: // Tree
					DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 0 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
					break; 
				case 3: // Spooky Tree
					DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 1 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
					break;
				case 4: // Beach
					DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 2 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
					break;
				case 5: // Water
					DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 3 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
					break;
				default:
					DrawString(10, 10, std::to_string(pWorld[y*vWorldSize.x + x]));
				}
			}
		}

		SetPixelMode(olc::Pixel::ALPHA);										// Draw Selected Cell - Has varying alpha components

		olc::vi2d vSelectedWorld = ToScreen(vSelected.x, vSelected.y);			// Convert selected cell coordinate to world space

		DrawPartialSprite(vSelectedWorld.x, vSelectedWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);	// Draw "highlight" tile

		SetPixelMode(olc::Pixel::NORMAL);										// Go back to normal drawing with no expected transparency
		
		return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(SCREEN_RES_W/PIXEL_SIZE, SCREEN_RES_H/PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, FULLSCREEN))
		demo.Start();
		
	return 0;
}
