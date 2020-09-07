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
}

drawingWindow::~drawingWindow()
{
    delete ui;
}

bool normalNumber(QString str){
    for(QChar c : str){
        if(c < '0' || c > '9')
            return false;
    }
    return true;
}

void drawingWindow::insertNode(QLineEdit* line){
    if(normalNumber(line->text())){
        int newNumber = line->text().toInt();
        line->clear();
        std::cout << "New node: " << newNumber << std::endl;
        if(Ui()->drawWidget->typeRedBlack){
            Ui()->drawWidget->redBlackTree->insert(newNumber);
        } else {
            Ui()->drawWidget->bTree->insert(newNumber);
        }
        Ui()->drawWidget->redraw();
    }
}

void drawingWindow::on_insertButton_clicked()
{
    insertNode(Ui()->insertLine);
}

void drawingWindow::on_insertLine_returnPressed()
{
    insertNode(Ui()->insertLine);
}

void drawingWindow::deleteNode(QLineEdit* line){
    if(normalNumber(line->text())){
        int newNumber = line->text().toInt();
        line->clear();
        std::cout << "Delete node: " << newNumber << std::endl;
        if(Ui()->drawWidget->typeRedBlack){
            Ui()->drawWidget->redBlackTree->remove(newNumber);
        } else {
            Ui()->drawWidget->bTree->remove(newNumber);
        }
        Ui()->drawWidget->redraw();
    }
}

void drawingWindow::on_deleteButtton_clicked(){
    deleteNode(Ui()->deleteLine);
}

void drawingWindow::on_deleteLine_returnPressed(){
    deleteNode(Ui()->deleteLine);
}
