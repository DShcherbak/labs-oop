#include "drawingwidget.h"
#include <QtWidgets>

drawingWidget::drawingWidget(QWidget *parent) : QWidget(parent)
{
    //bTree = _binaryTree;

    //tree = new drawingTree(testTree1());

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
    redraw();
}

void drawingWidget::redraw(){
    colorCode = (colorCode + 1) % 4;
    drawTree();
    update();
}


void drawingWidget::drawTree(){
    Visitor<drawingWidget> visitor = Visitor(this);
    if(typeRedBlack){
        redBlackTree->acceptVisitor(visitor);
    } else {
        //bTree->acceptVisitor(visitor);
    }
    if(!tree || !tree->root)
        return;
    auto root = tree->root;
    const int width = 1000; //TODO: fix width to correspond with windowsize
    int height = tree->getHeight();
    if(height > 32){
        //some kind of error, just in case
        return;
    }
    int maxKids = (1 << height);
    height = 600 / height;
    int radius = std::min(100,  width / (maxKids));
    drawNode(root, radius, 0, width, radius, height);
}


void drawingWidget::drawNode(drawingNode* node, int radius, int x_left, int x_right, int y, int height){
    if(!node)
        return;
    int x = (x_left + x_right) >> 1;
    node->addDrawingParamethers(x, y, radius);
    y += height;
    drawNode(node->left, radius, x_left, x, y, height);
    drawNode(node->right, radius, x, x_right, y, height);

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


int getFont(int radius){
    if(radius > 50){
        return 30;
    } else {
        return 10;
    }
}

void drawingWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);


    auto image = getImage(tree);
    std::vector<drawingNode*> nodes = std::move(image.first);
    if(nodes.empty()){
        painter.setFont(QFont("Arial", 30));
        painter.drawText(100,100, QString::fromStdString("Tree is empty.\n"));
        return;
    }
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
        painter.setFont(QFont("Arial", getFont(nodes[0]->radius)));
        painter.drawText(node->x - node->radius,
                         node->y - node->radius,
                         2 * node->radius,
                         2 * node->radius, Qt::AlignHCenter | Qt::AlignVCenter, QString::fromStdString(std::to_string(node->value)));


    }
}

void drawingWidget::setBTree(BTree<int>* _bTree){
    bTree = _bTree;
    typeRedBlack = false;
}
void drawingWidget::setRedBlackTree(RedBlackTree<int>* _redBlackTree){
    redBlackTree = _redBlackTree;
    typeRedBlack = true;
}

void drawingWidget::setTree(drawingNode* root){
    tree = new drawingTree(root);
}




























