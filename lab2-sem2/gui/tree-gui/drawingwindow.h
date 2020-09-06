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

private:
    Ui::drawingWindow *ui;
};

#endif // DRAWINGWINDOW_H
