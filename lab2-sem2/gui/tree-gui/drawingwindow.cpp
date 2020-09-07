#include "drawingwindow.h"
#include "ui_drawingwindow.h"

drawingWindow::drawingWindow(QWidget *parent) :
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

void drawingWindow::redrawBorders(){
    Ui()->insertLine->setStyleSheet("border: 1px solid black");
    Ui()->deleteLine->setStyleSheet("border: 1px solid black");
    Ui()->searchLine->setStyleSheet("border: 1px solid black");
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
    } else {
        line->setStyleSheet("border: 1px solid red");
    }
}

void drawingWindow::on_insertButton_clicked()
{
    redrawBorders();
    insertNode(Ui()->insertLine);
}

void drawingWindow::on_insertLine_returnPressed()
{
    redrawBorders();
    insertNode(Ui()->insertLine);
}

void drawingWindow::deleteNode(QLineEdit* line){
    if(normalNumber(line->text())){
        int newNumber = line->text().toInt();
        line->clear();
        std::cout << "Delete node: " << newNumber << std::endl;
        if(Ui()->drawWidget->typeRedBlack){
            if(!Ui()->drawWidget->redBlackTree->includes(newNumber)){
                line->setStyleSheet("border: 1px solid yellow");
            } else {
                Ui()->drawWidget->redBlackTree->remove(newNumber);
            }
        } else {
            if(!Ui()->drawWidget->bTree->includes(newNumber)){
                line->setStyleSheet("border: 1px solid yellow");
            } else {
                Ui()->drawWidget->bTree->remove(newNumber);
            }
        }
        Ui()->drawWidget->redraw();
    } else {
        line->setStyleSheet("border: 1px solid red");
    }
}

void drawingWindow::on_deleteButtton_clicked(){
    redrawBorders();
    deleteNode(Ui()->deleteLine);
}

void drawingWindow::on_deleteLine_returnPressed(){
    redrawBorders();
    deleteNode(Ui()->deleteLine);
}

void drawingWindow::searchNode(QLineEdit* line){
    int newNumber;
    bool found = false;
    if(normalNumber(line->text())){
        newNumber = line->text().toInt();
        line->clear();
        std::cout << "Delete node: " << newNumber << std::endl;
        if(Ui()->drawWidget->typeRedBlack){
            found = Ui()->drawWidget->redBlackTree->includes(newNumber);
        } else {
            found = Ui()->drawWidget->bTree->includes(newNumber);
        }
        if(found)
            line->setStyleSheet("border: 1px solid green");
        else
            line->setStyleSheet("border: 1px solid yellow");
        Ui()->drawWidget->redraw();
    } else {
        line->setStyleSheet("border: 1px solid red");
    }
    if(found){
        Ui()->drawWidget->findAndMark(newNumber);
    }
}


void drawingWindow::on_searchButton_clicked()
{
    redrawBorders();
    searchNode(Ui()->searchLine);
}

void drawingWindow::on_searchLine_returnPressed()
{
    redrawBorders();
    searchNode(Ui()->searchLine);
}
