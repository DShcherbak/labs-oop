#include "drawingwidget.h"
#include <QtWidgets>

drawingWidget::drawingWidget(QWidget *parent) : QWidget(parent)
{
    //bTree = _binaryTree;

    tree = new drawingTree(testTree1());

    update();
}

drawingWidget::drawingWidget(BTree<int>* _bTree, QWidget *parent) : QWidget(parent)
{
    bTree = _bTree;

    tree = new drawingTree(testTree1());

    update();
}

drawingWidget::drawingWidget(RedBlackTree<int>* _redBlackTree, QWidget *parent) : QWidget(parent)
{
    redBlackTree = _redBlackTree;

    tree = new drawingTree(testTree1());

    update();
}


drawingWidget::~drawingWidget()
{
    if(bTree)
        delete bTree;
    if(redBlackTree)
        delete redBlackTree;
    delete tree;
}


void drawingWidget::setPen(const QPen &pen)
{
    this->pen = pen;
    //update();
}


void drawingWidget::setBrush(const QBrush &brush)
{
    this->brush = brush;
    //update();
}


void drawingWidget::updateEvents(){
    colorCode = (colorCode + 1) % 4;
    drawTree();
    update();
}


void drawingWidget::drawTree(){
    auto root = tree->root;
    const int width = 1000; //TODO: fix width to correspond with windowsize
    int height = tree->getHeight();
    if(height > 32){
        //some kind of error, just in case
        return;
    }
    int maxKids = (1 << height);
    int radius = std::min(100, width / (2 * maxKids));
    drawNode(root, radius, 0, width, radius * 2);
}


void drawingWidget::drawNode(drawingNode* node, int radius, int x_left, int x_right, int y){
    if(!node)
        return;
    int x = (x_left + x_right) >> 1;
    node->addDrawingParamethers(x, y, radius);
    y += 2*radius;
    drawNode(node->left, radius, x_left, x, y);
    drawNode(node->right, radius, x, x_right, y);

}


std::vector<drawingNode*> drawingWidget::getNodes(drawingNode* root){
    if(root == nullptr)
        return {};
    auto v1 = getNodes(root->left);
    auto v2 = getNodes(root->right);
    std::vector<drawingNode*> result;
    result.insert(result.end(), v1.begin(), v1.end());
    result.insert(result.end(), v2.begin(), v2.end());
    result.push_back(root);
    return result;
}


std::vector<Edge> drawingWidget::getEdges(drawingNode* root){
    if(root == nullptr || !root->gotCoordinates())
        return {};
    auto v1 = getEdges(root->left);
    auto v2 = getEdges(root->right);
    std::vector<Edge> result(v1.size() + v2.size());
    result.insert(result.end(), v1.begin(), v1.end());
    result.insert(result.end(), v2.begin(), v2.end());

    if(root->left && root->left->gotCoordinates()){
        result.push_back({root->x, root->y, root->left->x, root->left->y});
    }
    if(root->right && root->right->gotCoordinates()){
        result.push_back({root->x, root->y, root->right->x, root->right->y});
    }
    return result;
}


std::pair<std::vector<drawingNode*>,std::vector<Edge>> drawingWidget::getImage(drawingTree* tree){
    return {getNodes(tree->root), getEdges(tree->root)};
}



void drawingWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);


    auto image = getImage(tree);
    std::vector<drawingNode*> nodes = std::move(image.first);
    std::vector<Edge> edges = std::move(image.second);
    for(auto edge : edges){
        QPen pen;
        pen.setWidth(3);
        pen.setBrush(Qt::black);
        painter.setPen(pen);
        painter.drawLine(edge.x1,
                         edge.y1,
                         edge.x2,
                         edge.y2);
    }
    for(auto node : nodes){
        painter.setPen(Qt::black);
        if(node->color == Red)
            painter.setBrush(QColor::fromRgb(200,0,0));
        else if(node->color == Gray)
            painter.setBrush(Qt::gray);
        else if(node->color == White)
            painter.setBrush(Qt::white);
        else{
            painter.setBrush(Qt::black);
            painter.setPen(Qt::white);
        }
        if(node->x * node->y * node->radius == 0){
            continue;
        }
        painter.drawEllipse(node->x - node->radius,
                            node->y - node->radius,
                            2 * node->radius,
                            2 * node->radius);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(node->x - 10, node->y + 12, QString::fromStdString(std::to_string(node->value)));


    }
}




























