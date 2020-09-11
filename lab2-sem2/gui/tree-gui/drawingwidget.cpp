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
    if(typeRedBlack)
        drawTree();
    else
        drawBTree();
    update();
}

void drawingWidget::quickRedraw(){
    update();
}


void drawingWidget::drawTree(){
    Visitor<drawingWidget> visitor = Visitor(this);
    redBlackTree->acceptVisitor(visitor);

    auto tree = drawing_tree;
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

void drawingWidget::drawBTree(){
    Visitor<drawingWidget> visitor = Visitor(this);
    bTree->acceptVisitor(visitor);

    auto tree = drawing_b_tree;
    if(!tree || !tree->root)
        return;
    auto root = tree->root;

    std::pair<int, int> size = tree->getSize();
    int square_size = size.first;
    int y_size = size.second;

    drawBNode(root, square_size, y_size);
}


void drawingWidget::drawNode(std::shared_ptr<drawingNode> node, int radius, int x_left, int x_right, int y, int height){
    if(!node)
        return;
    int x = (x_left + x_right) >> 1;
    node->addDrawingParamethers(x, y, radius);
    y += height;
    drawNode(node->left, radius, x_left, x, y, height);
    drawNode(node->right, radius, x, x_right, y, height);

}

void drawingWidget::drawBNode(std::shared_ptr<drawingBNode> node, int a, int y_size){
    if(!node || node->children.empty()){
        return;
    }
    for(auto child : node->children){
        drawBNode(child, a, y_size);
    }
    auto last = node->children[node->children.size() - 1];
    int x_left = node->children[0]->x;
    int x_right = last->x;
    x_right += a * (last->value.size());
    int x = (x_left + x_right - (a * node->value.size())) >> 1;
    node->addDrawingParamethers(x, node->children[0]->y - y_size - a, a);
}


std::vector<std::shared_ptr<drawingNode>> drawingWidget::getNodes(std::shared_ptr<drawingNode> root){
    if(root == nullptr)
        return {};
    auto v1 = getNodes(root->left);
    auto v2 = getNodes(root->right);
    std::vector<std::shared_ptr<drawingNode>> result;
    result.insert(result.end(), v1.begin(), v1.end());
    result.insert(result.end(), v2.begin(), v2.end());
    result.push_back(root);
    return result;
}


std::vector<Edge> drawingWidget::getEdges(std::shared_ptr<drawingNode> root){
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

void drawingWidget::getBNodes(std::shared_ptr<drawingBNode> root, std::vector<std::shared_ptr<drawingBNode>>& base){
    if(root == nullptr)
        return;
    for(auto ch : root->children){
        getBNodes(ch, base);
    }
    std::vector<std::shared_ptr<drawingNode>> result;
    base.push_back(root);
}


void drawingWidget::getBEdges(std::shared_ptr<drawingBNode> root, std::vector<Edge>& base){
    if(root == nullptr || !root->gotCoordinates() || root->children.empty())
        return;
    for(auto ch : root->children){
        getBEdges(ch, base);
    }

    int x = root->x, y = root->y + root->size;
    for(int i = 0, n = root->children.size(); i < n; i++){
        auto child = root->children[i];
        int child_x = (child->x + (child->x + child->value.size() * child->size)) >> 1;
        int child_y = child->y;
        base.push_back({x + i*root->size,y,child_x, child_y});
    }
    /*
    if(root->left && root->left->gotCoordinates()){
        result.push_back({root->x, root->y, root->left->x, root->left->y});
    }
    if(root->right && root->right->gotCoordinates()){
        result.push_back({root->x, root->y, root->right->x, root->right->y});
    }
    return result;*/
}


std::pair<std::vector<std::shared_ptr<drawingNode>>,std::vector<Edge>> drawingWidget::getImage(std::shared_ptr<drawingTree> tree){
    return {getNodes(tree->root), getEdges(tree->root)};
}

std::pair<std::vector<std::shared_ptr<drawingBNode>>,std::vector<Edge>> drawingWidget::getBImage(std::shared_ptr<drawingBTree> tree){
    std::vector<std::shared_ptr<drawingBNode>> base;
    std::vector<Edge> edge;
    getBNodes(tree->root, base);
    getBEdges(tree->root, edge);
    return {base, edge};
}

int getFont(int radius){
    if(radius > 50){
        return 30;
    } else if(radius > 30){
        return 20;
    } else if(radius > 10){
        return 10;
    } else {
        return 6;
    }
}

void paintNode(QPainter& painter, std::shared_ptr<drawingNode> node){
    painter.setPen(Qt::black);
    if(node->color == Red)
        painter.setBrush(QColor::fromRgb(200,0,0));
    else if(node->color == Gray)
        painter.setBrush(Qt::gray);
    else if(node->color == White)
        painter.setBrush(Qt::white);
    else if(node->color == Green)
        painter.setBrush(QColor::fromRgb(0,200,0));
    else{
        painter.setBrush(Qt::black);
        painter.setPen(Qt::white);
    }
    if(node->x * node->y * node->radius == 0){
        return;
    }

    painter.drawEllipse(node->x - node->radius,
                        node->y - node->radius,
                        2 * node->radius,
                        2 * node->radius);
    painter.setFont(QFont("Arial", getFont(node->radius)));
    painter.drawText(node->x - node->radius,
                     node->y - node->radius,
                     2 * node->radius,
                     2 * node->radius, Qt::AlignHCenter | Qt::AlignVCenter, QString::fromStdString(std::to_string(node->value)));
}

void paintNode(QPainter& painter, std::shared_ptr<drawingBNode> node){
    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);

    int x = node->x;
    int y = node->y;
    int a = node->size;
    painter.setFont(QFont("Arial", getFont(node->size)));
    for(auto val : node->value){
        painter.drawRect(x,y,a,a);
        painter.drawText(x,y,a,a, Qt::AlignHCenter | Qt::AlignVCenter, QString::fromStdString(std::to_string(val)));
        x += a;


    }
}

void paintEdges(QPainter& painter, const std::vector<Edge> &edges){
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
}

void drawingWidget::paintTree(QPainter &painter){
    std::vector<Edge> edges;
    if(typeRedBlack){
        auto image = getImage(drawing_tree);
        auto nodes = std::move(image.first);
        edges = std::move(image.second);
        paintEdges(painter, edges);
        if(nodes.empty()){
            painter.setFont(QFont("Arial", 30));
            painter.drawText(100,100, QString::fromStdString("Tree is empty.\n"));
            return;
        }
        for(auto node : nodes){
            paintNode(painter, node);
        }
    } else {
        auto image = getBImage(drawing_b_tree);
        auto nodes = std::move(image.first);
        edges = std::move(image.second);
        paintEdges(painter, edges);
        if(nodes.empty()){
            painter.setFont(QFont("Arial", 30));
            painter.drawText(100,100, QString::fromStdString("Tree is empty.\n"));
            return;
        }
        for(auto node : nodes){
            paintNode(painter, node);
        }
    }

}

void drawingWidget::paintBorder(QPainter &painter){
    if(border == nullptr)
        return;
    int x = border->x;
    int y = border->y;
    int size = border->size;
    pen.setWidth(5);
    pen.setBrush(Qt::green);
    painter.setPen(pen);
    if(border->typeRedBlack){
        painter.drawArc(x-size,y-size,size*2,size*2, 0, 360*16);
    } else {
        QPolygonF polygon;
        polygon << QPointF(x, y) << QPointF(x+size, y) << QPointF(x + size, y + size) << QPointF(x, y+size);
        painter.setBrush(Qt::NoBrush);
        painter.drawPolygon(polygon);
    }
    pen.setWidth(3);
    pen.setBrush(Qt::black);
    painter.setPen(pen);
    border = nullptr;

}

void drawingWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    paintTree(painter);
    paintBorder(painter);

}

void drawingWidget::setBTree(BTree<int>* _bTree){
    bTree = _bTree;
    typeRedBlack = false;
}
void drawingWidget::setRedBlackTree(RedBlackTree<int>* _redBlackTree){
    redBlackTree = _redBlackTree;
    typeRedBlack = true;
}

void drawingWidget::setDrawingTree(std::shared_ptr<drawingNode> root){
    drawing_tree = std::make_shared<drawingTree>(root);
}
void drawingWidget::setDrawingBTree(std::shared_ptr<drawingBNode> root){
    drawing_b_tree = std::make_shared<drawingBTree>(root);
}

void drawingWidget::findAndMarkNumber(int number){
    if(typeRedBlack){
        auto node = drawing_tree->root;
        findAndMark(number, node);
    } else {
        auto node = drawing_b_tree->root;
        findAndMark(number, node);
    }
}

void drawingWidget::findAndMark(int number, std::shared_ptr<drawingNode> node){
    if(!node)
        node = drawing_tree->root;
    while(node != nullptr){
        if(node->value == number){
            //auto prev_color = node->color;
            int x = node->x;
            int y = node->y;
            int r = node->radius;
            addBorder(x,y,r);
            quickRedraw();
            //node->color = prev_color;
            break;
        }
        else if(node->value > number){
            node = node->left;
        }
        else {
            node = node->right;
        }
    }
}

void drawingWidget::findAndMark(int number, std::shared_ptr<drawingBNode> node){
    if(!node)
        node = drawing_b_tree->root;
    while(node != nullptr){
        int i = 0, n = node->value.size();
        while(i < n && node->value[i] < number) i++;
        if(i < n){
            if(node->value[i] == number){
                int x = node->x;
                int y = node->y;
                int r = node->size;
                addBorder(x+r*i,y,r, false);
                quickRedraw();
                //node->color = prev_color;
                return;
            } else {
                if(node->children.empty())
                    return;
                node = node->children[i];
            }
        }
        else {
            if(node->children.empty())
                return;
            node = node->children[i];
        }
    }

}

void drawingWidget::addBorder(int x, int y, int r, bool isRedBlack){
    border = std::make_shared<Border>(x, y, r, isRedBlack);
    updateEvents();
}








