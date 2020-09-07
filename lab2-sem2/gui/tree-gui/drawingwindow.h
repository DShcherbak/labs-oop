#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QWidget>
#include <ui_drawingwindow.h>

namespace Ui {
class drawingWindow;
}


class drawingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit drawingWindow(QWidget *parent = nullptr);
    drawingWindow(BTree<int>* _bTree,QWidget *parent = nullptr);
    drawingWindow(RedBlackTree<int>* _bTree,QWidget *parent = nullptr);
    ~drawingWindow();

    Ui::drawingWindow* Ui(){
        return ui;
    }

private slots:
    void on_insertButton_clicked();

    void on_insertLine_returnPressed();

    void on_deleteButtton_clicked();

    void on_deleteLine_returnPressed();

    void on_searchButton_clicked();

    void on_searchLine_returnPressed();

private:
    Ui::drawingWindow *ui;
    void redrawBorders();
    void deleteNode(QLineEdit* line);
    void insertNode(QLineEdit* line);
    void searchNode(QLineEdit* line);

};

#endif // DRAWINGWINDOW_H
