#ifndef LAB_VISITOR_H
#define LAB_VISITOR_H
#include <iostream>
#include "../../gui/tree-gui/drawingNode.h"
#include "../../gui/tree-gui/drawingBNode.h"

template <class Widget>
class Visitor {
public:
    template<class T>
    void visitRedBlackTree(T root) ;

    template<class T>
    void visitBTree(T root);



private:
    Widget* drawingWidget;

    template <class T>
    drawingNode* getSubTree(T root);
    template <class T>
    drawingBNode* getSubBTree(T root);
};


#endif //LAB_VISITOR_H
