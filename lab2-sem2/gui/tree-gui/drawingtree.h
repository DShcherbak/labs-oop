#ifndef DRAWINGTREE_H
#define DRAWINGTREE_H

#include <algorithm>
#include "drawingNode.h"




class drawingTree
{
public:
    drawingTree();
    drawingTree(drawingNode* node);
    ~drawingTree();
    drawingNode* root = nullptr;
    int getHeight();

};

drawingNode* testTree1();

#endif // DRAWINGTREE_H
