#ifndef LAB_VISITOR_H
#define LAB_VISITOR_H
#include <iostream>
#include "drawingNode.h"
#include "drawingBNode.h"

template <class Widget>
class Visitor{
public:

    template<class T>
    void visitRedBlackTree(T root, T null_node = nullptr);

    template<class T>
    void visitBTree(T root);

    Visitor(Widget* widget){
        drawingWidget = widget;
    }


private:
    Widget* drawingWidget;

    template <class T>
    drawingNode* getSubTree(T root, T null_node = nullptr, drawingNode* parent = nullptr);
    template <class T>
    drawingBNode* getSubBTree(T root);
};

template <class Widget>
template <class T>
void Visitor<Widget>::visitRedBlackTree(T root, T null_node) {
    drawingWidget->setTree(getSubTree(root, null_node));
    //drawingWidget->redraw();
}

template <class Widget>
template <class T>
void Visitor<Widget>::visitBTree(T root) {
    drawingWidget->setTree(getSubBTree(root));
    //drawingWidget->redraw();
}


template <class Widget>
template <class T>
drawingNode* Visitor<Widget>::getSubTree(T root, T null_node, drawingNode* parent){
    if(!root || root == null_node){
        return nullptr;
    }
    auto node = new drawingNode(root->key,
                                (root->isBlack() ? drawingColor::Black : drawingColor::Red),
                                parent,
                                nullptr,
                                nullptr);
    node->left = getSubTree(root->left, null_node, node);
    node->right = getSubTree(root->right, null_node, node);
    return node;
}

template <class Widget>
template <class T>
drawingBNode* Visitor<Widget>::getSubBTree(T root){
    if(!root){
        return nullptr;
    }
    auto node = new drawingBNode(root->value,
                                root->color,
                                root->parent,
                                getSubTree(root->left),
                                getSubTree(root->right));
    return node;
}



#endif //LAB_VISITOR_H
