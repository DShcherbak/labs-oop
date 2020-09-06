#include "drawingwindow.h"
#include "ui_drawingwindow.h"

drawingWindow::drawingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::drawingWindow)
{
    ui->setupUi(this);
}

drawingWindow::drawingWindow(BTree<int>* _bTree, QWidget *parent):
    QWidget(parent),
    ui(new Ui::drawingWindow)
{
    ui->setupUi(this);
    ui->drawWidget->setBTree(_bTree);
}



drawingWindow::drawingWindow(RedBlackTree<int>* _redBlackTree, QWidget *parent):
    QWidget(parent),
    ui(new Ui::drawingWindow)
{
    ui->setupUi(this);
    ui->drawWidget->setRedBlackTree(_redBlackTree);
}

drawingWindow::~drawingWindow()
{
    delete ui;
}
