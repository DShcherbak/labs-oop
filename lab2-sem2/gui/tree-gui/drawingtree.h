#ifndef DRAWINGTREE_H
#define DRAWINGTREE_H

#include <algorithm>
#include <memory>
#include "drawingNode.h"
#include "drawingBNode.h"




class drawingTree
{
public:
    drawingTree();
    drawingTree(std::shared_ptr<drawingNode> node);
    ~drawingTree();
    std::shared_ptr<drawingNode> root;
    int getHeight();

};

class drawingBTree
{

public:
    drawingBTree();
    drawingBTree(std::shared_ptr<drawingBNode> node);
    ~drawingBTree();
    std::shared_ptr<drawingBNode> root = nullptr;
    std::pair<int,int> getSize();

};



drawingNode* testTree1();

#endif // DRAWINGTREE_H
