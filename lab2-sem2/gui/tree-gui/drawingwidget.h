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

struct Edge{
    int x1, y1, x2, y2;
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
    void setTree(drawingNode* root);
    void redraw();

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

    bool typeRedBlack = true;
    BTree<int>* bTree;
    RedBlackTree<int>* redBlackTree;
    drawingTree* tree;
    void drawTree();
    void drawNode(drawingNode* node, int radius, int x_left, int x_right, int y, int height);
    std::pair<std::vector<drawingNode*>,std::vector<Edge>> getImage(drawingTree* tree);
    std::vector<drawingNode*> getNodes(drawingNode* root);
    std::vector<Edge> getEdges(drawingNode* root);

};


#endif // DRAWINGWIDGET_H
