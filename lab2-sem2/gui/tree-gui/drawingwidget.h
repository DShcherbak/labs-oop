#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QtWidgets>
#include <QPainter>
#include "../../src/BTree.hpp"
#include "../../src/RedBlackTree.hpp"
#include "../../src/TreeCommands.hpp"
#include "../../src/TreeFactory.hpp"
#include "Visitor.h"
#include <vector>
#include "drawingtree.h"
#include <memory>

struct Edge{
    int x1, y1, x2, y2;
};

struct Border{
    bool typeRedBlack = true;
    int x, y, size;

    Border(int _x, int _y, int _r){
        x = _x;
        y = _y;
        size = _r;
        typeRedBlack = true;
    }

    Border(int _x, int _y, int _r, bool redBlack){
        x = _x;
        y = _y;
        size = _r;
        typeRedBlack = redBlack;
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class drawingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit drawingWidget(QWidget *parent = 0);
    ~drawingWidget();

    void setBTree(BTree<int>* _bTree);
    void setRedBlackTree(RedBlackTree<int>* _redBlackTree);
    void setDrawingTree(std::shared_ptr<drawingNode> root);
    void setDrawingBTree(std::shared_ptr<drawingBNode> root);
    void redraw();
    void quickRedraw();
    void findAndMarkNumber(int number);


    void paintTree(QPainter &painter);

    BTree<int>* bTree;
    RedBlackTree<int>* redBlackTree;
    bool typeRedBlack = true;

public slots:
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void updateEvents();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int colorCode = 0;
    void setupActions();
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;
    QPixmap pixmap;


    std::shared_ptr<drawingTree> drawing_tree;
    std::shared_ptr<drawingBTree> drawing_b_tree;
    std::shared_ptr<Border> border;
    void drawTree();
    void drawBTree();

    void findAndMark(int number, std::shared_ptr<drawingNode> node = nullptr);
    void findAndMark(int number, std::shared_ptr<drawingBNode> node = nullptr);

    void addBorder(int x, int y, int r, bool isRedBlack = true);
    void paintBorder(QPainter &painter);

    void drawNode(std::shared_ptr<drawingNode> node, int radius, int x_left, int x_right, int y, int height);
    void drawBNode(std::shared_ptr<drawingBNode> node, int a, int y_size);

    std::pair<std::vector<std::shared_ptr<drawingNode>>,std::vector<Edge>> getImage(std::shared_ptr<drawingTree> tree);
    std::vector<std::shared_ptr<drawingNode>> getNodes(std::shared_ptr<drawingNode> root);
    std::vector<Edge> getEdges(std::shared_ptr<drawingNode> root);

    void getBNodes(std::shared_ptr<drawingBNode> root, std::vector<std::shared_ptr<drawingBNode>>& base);
    void getBEdges(std::shared_ptr<drawingBNode> root, std::vector<Edge>& base);
    std::pair<std::vector<std::shared_ptr<drawingBNode>>,std::vector<Edge>> getBImage(std::shared_ptr<drawingBTree> tree);

};


#endif // DRAWINGWIDGET_H
