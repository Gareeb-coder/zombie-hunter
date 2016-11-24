#include <terrainobject.h>
#ifndef TREE_H
#define TREE_H

const char CHAR_TREE = 'T';  //"\u263A\x00";
const int TERRAIN_TREE = 1;

class Tree : public TerrainObject {
    static char const * names[];
    static char const * conversations[];
  public:
    Tree(unsigned int initialPosX, unsigned int initialPosY);
    virtual ~Tree();
    virtual char const * getName();
    virtual char const * getConversation();
};

#endif