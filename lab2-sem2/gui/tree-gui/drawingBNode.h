#ifndef LAB_DRAWINGBNODE_H
#define LAB_DRAWINGBNODE_H


#include <vector>

struct drawingBNode{
    std::vector<int> value;
    drawingNode* parent;
    drawingNode* left;
    drawingNode* right;
    drawingColor color;
    int x = 0, y = 0, radius = 0;

    drawingBNode(std::vector<int> _value, drawingColor _color = White,
                drawingNode* _parent = nullptr,
                drawingNode* _left = nullptr,
                drawingNode* _right = nullptr){
        value = std::move(_value);
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

#endif //LAB_DRAWINGBNODE_H
