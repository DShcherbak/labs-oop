#ifndef LAB_DRAWINGBNODE_H
#define LAB_DRAWINGBNODE_H

#include <memory>
#include <vector>

struct drawingBNode{
    std::vector<int> value;
    std::shared_ptr<drawingBNode> parent;
    std::vector<std::shared_ptr<drawingBNode>> children;
    int x = 0, y = 0, size = 0;

    drawingBNode(std::vector<int> _value,
                std::shared_ptr<drawingBNode> _parent = nullptr,
                 std::vector<std::shared_ptr<drawingBNode>> _children = {}){
        value = std::move(_value);
        parent = _parent;
        children = std::move(_children);
    }

    void addDrawingParamethers(int _x, int _y, int a){
        x = _x;
        y = _y;
        size = a;
    }

    bool gotCoordinates(){return x * y != 0;}

};

#endif //LAB_DRAWINGBNODE_H
