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
    std::shared_ptr<drawingNode> getSubTree(T root, T null_node = nullptr, std::shared_ptr<drawingNode> parent = nullptr);
    template <class T>
    std::shared_ptr<drawingBNode> getSubBTree(T root, std::shared_ptr<drawingBNode> parent = nullptr);
};

template <class Widget>
template <class T>
void Visitor<Widget>::visitRedBlackTree(T root, T null_node) {
    drawingWidget->setDrawingTree(getSubTree(root, null_node));
    //drawingWidget->redraw();
}

template <class Widget>
template <class T>
void Visitor<Widget>::visitBTree(T root) {
    drawingWidget->setDrawingBTree(getSubBTree(root));
    //drawingWidget->redraw();
}


template <class Widget>
template <class T>
std::shared_ptr<drawingNode> Visitor<Widget>::getSubTree(T root, T null_node, std::shared_ptr<drawingNode> parent){
    if(!root || root == null_node){
        return nullptr;
    }
    auto node = std::make_shared<drawingNode>(root->key,
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
std::shared_ptr<drawingBNode> Visitor<Widget>::getSubBTree(T root, std::shared_ptr<drawingBNode> parent){
    if(!root){
        return nullptr;
    }
    auto node = std::make_shared<drawingBNode>(root->keys,
                                parent);
    for(auto child : root->children) {
        node->children.push_back(getSubBTree(child, node));
    }
    return node;
}



#endif //LAB_VISITOR_H
