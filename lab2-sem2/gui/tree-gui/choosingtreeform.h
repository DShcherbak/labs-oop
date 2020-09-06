#ifndef CHOOSINGTREEFORM_H
#define CHOOSINGTREEFORM_H

#include <QWidget>
#include <ui_choosingtreeform.h>

namespace Ui {
class choosingTreeForm;
}

class choosingTreeForm : public QWidget
{
    Q_OBJECT

public:
    static int tabs;
    explicit choosingTreeForm(QWidget *parent = nullptr);
    ~choosingTreeForm();

    Ui::choosingTreeForm* Ui(){
        return ui;
    }
    int tabNumber;

private:
    Ui::choosingTreeForm *ui;
};

#endif // CHOOSINGTREEFORM_H
