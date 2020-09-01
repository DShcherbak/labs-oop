//
// Created by sadoffnick on 01.09.20.
//

#ifndef LAB_DRAWINGNODE_H
#define LAB_DRAWINGNODE_H


enum drawingColor{White, Red, Black, Gray, Green};

struct drawingNode{
    int value;
    drawingNode* parent;
    drawingNode* left;
    drawingNode* right;
    drawingColor color;
    int x = 0, y = 0, radius = 0;

    drawingNode(int _value, drawingColor _color = White,
                drawingNode* _parent = nullptr,
                drawingNode* _left = nullptr,
                drawingNode* _right = nullptr){
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
