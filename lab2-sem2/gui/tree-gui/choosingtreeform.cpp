#include "choosingtreeform.h"
#include "ui_choosingtreeform.h"

choosingTreeForm::choosingTreeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::choosingTreeForm)
{
    ui->setupUi(this);
    tabNumber = tabs++;
}

int choosingTreeForm::tabs = 0;

choosingTreeForm::~choosingTreeForm()
{
    delete ui;
}
