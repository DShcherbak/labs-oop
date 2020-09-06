#include "Visitor.h"

template <class T>
template <class Widget>
void Visitor<Widget>::visitRedBlackTree(T root) {
    drawingWidget->setTree(getSubTree(root));
    drawingWidget->redraw();
}

template <class Widget>
template <class T>
void Visitor<Widget>::visitBTree(T root) {
    drawingWidget->setTree(getSubBTree(root));
    drawingWidget->redraw();
}


template <class Widget>
template <class T>
drawingNode* Visitor<Widget>::getSubTree(T root){
    if(!root){
        return nullptr;
    }
    auto node = new drawingNode(root->value,
                                root->color,
                                root->parent,
                                getSubTree(root->left),
                                getSubTree(root->right));
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
