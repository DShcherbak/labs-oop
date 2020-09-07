#ifndef LAB_DRAWINGNODE_H
#define LAB_DRAWINGNODE_H

#include "baseDrawingNode.h"
#include <memory>


struct drawingNode : baseDrawingNode {
    int value;
    std::shared_ptr<drawingNode> parent;
    std::shared_ptr<drawingNode> left;
    std::shared_ptr<drawingNode>right;
    drawingColor color;
    int x = 0, y = 0, radius = 0;

    drawingNode(int _value, drawingColor _color = White,
                std::shared_ptr<drawingNode> _parent = nullptr,
                std::shared_ptr<drawingNode> _left = nullptr,
                std::shared_ptr<drawingNode> _right = nullptr){
        value = _value;
        color = _color;
        parent = _parent;
        left = _left;
        right = _right;
    }

    void addDrawingParamethers(int _x, int _y, int _r){
        x = _x;
        y = _y;
        radius = _r;
    }

    bool gotCoordinates(){return x * y != 0;}

};

#endif //LAB_DRAWINGNODE_H
