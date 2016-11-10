#include <engine.h>
#include <display.h>
#include <terrainmap.h>
#include <input.h>
#include <avatar.h>
#include <baddie.h>

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

unsigned int lastrows = 0;
unsigned int lastcols = 0;

#define MAP_WIDTH 100
#define MAP_HEIGHT 100
#define BUFFER_SIZE 65536

#define TREE_COUNT MAP_WIDTH
#define GRAVE_COUNT MAP_HEIGHT

#define AVATAR_COUNT 2
#define AVATAR_START_X MAP_HEIGHT/2
#define AVATAR_START_Y MAP_WIDTH/2

void game_loop(void) {
  int viewX = 0;
  int viewY = 0;

  // setup random numbers
  srand(time(NULL));

  TerrainMap map = TerrainMap(MAP_WIDTH, MAP_HEIGHT, TREE_COUNT, GRAVE_COUNT);

  // this is the first calculation :)
  recalculateViewPosition(&viewX, &viewY);

  Avatar ** avatars = new Avatar*[AVATAR_COUNT];

  avatars[0] = new Avatar(AVATAR_START_X, AVATAR_START_Y);
  avatars[1] = new Baddie(AVATAR_START_X-2, AVATAR_START_Y);

  //Baddie baddie = Baddie(MAP_WIDTH/2, MAP_HEIGHT/2);
  Avatar *player = avatars[0];

  map.setAt(player->getX(), player->getY(), TERRAIN_PLAYER);
  map.setAt(avatars[1]->getX(), avatars[1]->getY(), TERRAIN_BADDIE);

  draw(avatars, AVATAR_COUNT, &map, &viewX, &viewY);

  int key = getkey();
  bool change = false;
  bool cancel = false;
  int inputStep = 0;
  while (!cancel) {
    if (key > 0) {
       switch(key) {
         case LEFT_KEY:
           if (map.getAt(player->getX(), player->getY() - 1) == TERRAIN_EMPTY) {
             viewY--;
             map.setAt(player->getX(), player->getY(), TERRAIN_EMPTY);
             player->setY(player->getY() - 1);
             map.setAt(player->getX(), player->getY(), TERRAIN_PLAYER);
             change = true;
           }
           player->setDirection(AVATAR_DIRECTION_LEFT);
         break;
         case RIGHT_KEY:
           if (map.getAt(player->getX(), player->getY() + 1) == TERRAIN_EMPTY) {
             viewY++;
             map.setAt(player->getX(), player->getY(), TERRAIN_EMPTY);
             player->setY(player->getY() + 1);
             map.setAt(player->getX(), player->getY(), TERRAIN_PLAYER);
             change = true;
           }
           player->setDirection(AVATAR_DIRECTION_RIGHT);
         break;
         case UP_KEY:
           if (map.getAt(player->getX() - 1, player->getY()) == TERRAIN_EMPTY) {
             viewX--;
             map.setAt(player->getX(), player->getY(), TERRAIN_EMPTY);
             player->setX(player->getX() - 1);
             map.setAt(player->getX(), player->getY(), TERRAIN_PLAYER);
             change = true;
           }
           player->setDirection(AVATAR_DIRECTION_UP);
         break;
         case DOWN_KEY:
           if (map.getAt(player->getX() + 1, player->getY()) == TERRAIN_EMPTY) {
             viewX++;
             map.setAt(player->getX(), player->getY(), TERRAIN_EMPTY);
             player->setX(player->getX() + 1);
             map.setAt(player->getX(), player->getY(), TERRAIN_PLAYER);
             change = true;
           }
           player->setDirection(AVATAR_DIRECTION_DOWN);
         break;
         case FIRE_KEY:
           switch(player->getDirection())
           {
             case AVATAR_DIRECTION_LEFT:
             break;
             case AVATAR_DIRECTION_RIGHT:
             break;
             case AVATAR_DIRECTION_UP:
             break;
             case AVATAR_DIRECTION_DOWN:
             break;
           }
         break;
       }
      cancel = key == ESCAPE_KEY;
    }
    inputStep++;
    if (inputStep >= 9) {
      for (unsigned int avatarIndex = 0; avatarIndex < AVATAR_COUNT; avatarIndex++) {
        change |= avatars[avatarIndex]->update(&map);
      }
      inputStep = 0;
    }
    if (change) {
     change = false;
     draw(avatars, AVATAR_COUNT, &map, &viewX, &viewY);
    }
    key = getkey();
  }
  for (unsigned int avatarIndex = 0; avatarIndex < AVATAR_COUNT; avatarIndex++) {
    delete avatars[avatarIndex];
  }
  delete avatars;
  clear();
}

void draw(Avatar ** avatar, unsigned int avatarCount, TerrainMap * const map, int * viewX, int * viewY) {
  unsigned int rows = getRows();
  unsigned int cols = getCols();
  unsigned int viewableRows = rows-2;

  if (rows != lastrows || cols != lastcols) {
    recalculateViewPosition(viewX, viewY);
  }

  // Clear Screen
  clear();
  // Draw HEADER
  cout << "Rows " << rows << " Cols " << cols << " ViewX " << *viewX << " ViewY " << *viewY << " Px " << avatar[0]->getX() << " Py " << avatar[0]->getY() << endl;
  // Draw BOARD
  char buffer[BUFFER_SIZE];
  memset(&buffer[0], 0, BUFFER_SIZE);
  char * ptr = &buffer[0];
  for(unsigned int x = 0; x < viewableRows; x++) {
    *ptr = CHAR_EMPTY;
    ptr++;
      for (unsigned int y = 1; y < cols-1; y++) {
        char terrainChar = map->getCharacterAt(*viewX + x, *viewY + y);
        for (unsigned int avatarIndex = 0; avatarIndex < AVATAR_COUNT; avatarIndex++) {
          if(avatar[avatarIndex]->getX() == x + *viewX && avatar[avatarIndex]->getY() == y + *viewY) {
            terrainChar = avatar[avatarIndex]->getCharacter();
          }
        }
        *ptr = terrainChar;
        ptr++;
      }
      *ptr = '\n';
      ptr++;
  }
  cout << buffer;
  lastrows = rows;
  lastcols = cols;
}

void recalculateViewPosition(int * viewX, int * viewY)
{
  if ((int)getRows() - MAP_HEIGHT > 0) {
    *viewX = (((int)getRows() - MAP_HEIGHT)/2) * -1;
  } else {
    *viewX =  AVATAR_START_X - (int)getRows()/2;
  }
  if ((int)getCols() - MAP_WIDTH > 0) {
    *viewY = (((int)getCols() - MAP_WIDTH)/2) * -1;
  } else {
    *viewY = AVATAR_START_Y - (int)getCols()/2;
  }
}