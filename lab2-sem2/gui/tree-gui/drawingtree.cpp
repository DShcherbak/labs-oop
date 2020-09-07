#include "drawingtree.h"

drawingTree::drawingTree()
{

}

drawingTree::drawingTree(std::shared_ptr<drawingNode> node)
{
    root = node;
}

drawingTree::~drawingTree()
{
}

int getNodeHeight(std::shared_ptr<drawingNode> node){
    if(!node)
        return 0;
    return std::max(getNodeHeight(node->left) + 1, getNodeHeight(node->right) + 1);
}

int drawingTree::getHeight(){
    return getNodeHeight(root);
}

drawingBTree::drawingBTree(std::shared_ptr<drawingBNode> node)
{
    root = node;
}

drawingBTree::~drawingBTree()
{
}

int dfs(std::shared_ptr<drawingBNode> node, std::vector<std::shared_ptr<drawingBNode>> &leafes, int& leafes_count, int leafes_capacity){
    int result = 1;
    if(node->children.empty()){
        leafes_count++;
        leafes_capacity += node->value.size();
        leafes.push_back(node);
    } else {
        for(auto ch : node->children){
            result = 1 + dfs(ch, leafes, leafes_count, leafes_capacity);
        }
    }
    return result;
}

std::pair<int,int> drawingBTree::getSize(){
    auto node = root;
    int leafes_count = 0;
    int leafes_capacity = 0;

    std::vector<std::shared_ptr<drawingBNode>> leafes;
    int max_height = dfs(root, leafes, leafes_count, leafes_capacity);
    int capacity = leafes_capacity + leafes_count + 1;
    const int W = 1000;
    const int H = 600;
    int size = std::min(50,W / capacity);
    int y_size = H / (max_height + 1);
    int x = size;
    for(int i = 0; i < leafes_count; i++){
        leafes[i]->addDrawingParamethers(x,H-y_size,size);
        x += (leafes[i]->value.size() + 1) * size;
    }
    return {size, y_size};
}


