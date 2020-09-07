#include <iostream>
#include <QToolButton>
#include <QTableWidget>
#include <QLabel>
#include <QWidget>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDesktopWidget dw;
    setFixedSize(dw.width()*0.8,dw.height()*0.8);

    QToolButton *tb = new QToolButton();
    tb->setText("+");
    tb->setAutoRaise(true);
    connect(tb, SIGNAL(clicked()), this, SLOT(addTab()));

    ui->tabWidget->addTab(new QLabel("You can add tabs by pressing <b>\"+\"</b>"), QString());
    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, tb);
    ui->tabWidget->tabBar()->setTabsClosable(true);
    addTab();
    ui->tabWidget->setTabEnabled(0, true);
    connect(ui->tabWidget->tabBar(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    //check if user clicked at a tab
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
}

void MainWindow::tabSelected(){
    int index = ui->tabWidget->currentIndex() ;
    std::cout << index << " is up.\n";
    ui->tabWidget->widget(index)->layout();
}

void MainWindow::closeTab(const int& index)
{
    if (index == -1) {
        return;
    }

    //QWidget* tabItem = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);

    //delete(tabItem);
    //tabItem = nullptr;
}

MainWindow::~MainWindow()
{
    for(auto tab : widgets)
        delete tab;
    for(auto tab : layouts)
        delete tab;
    for(auto tab : tabs)
        delete tab;
    delete ui;
}

void MainWindow::addTab()
{
    static int number = 0;
    QLabel *tab = new QLabel(this);
    QString tabName = QString("Tab #%1").arg(++number);
    QGridLayout *centralLayout = new QGridLayout;
    auto widget = new choosingTreeForm();
    widget->show();

    layouts.push_back(centralLayout);
    centralLayout->addWidget(widget);
    tab->setLayout(centralLayout);

    ui->tabWidget->insertTab(ui->tabWidget->count() - 1, tab, tabName);
    ui->tabWidget->setCurrentWidget(ui->tabWidget->widget(ui->tabWidget->count() - 1));
    tabs.push_back(tab);
    forms.push_back(widget);
    connect(widget->Ui()->BTreeButton, SIGNAL(clicked()), this, SLOT(newBTab()));
    connect(widget->Ui()->RedBlackTreeButton, SIGNAL(clicked()), this, SLOT(newRedBlackTab()));

}

void deleteLayout(QLayout* layout){
    QLayoutItem * item;
    QLayout * sublayout;
    QWidget * oldWidget;
    while ((item = layout->takeAt(0))) {
        if ((sublayout = item->layout()) != 0) {/* do the same for sublayout*/}
        else if ((oldWidget = item->widget()) != 0) {oldWidget->hide(); delete oldWidget;}
        else {delete item;}
    }
    delete layout;
}


void MainWindow::newBTab(){
    static int number = 0;
    BTree<int>* bTree = new BTree<int>();
    int id = ui->tabWidget->currentIndex();
    std::cout << "Current id is " << id << std::endl;
    auto tab = ui->tabWidget->currentWidget();

    auto layout = tab->layout();
    deleteLayout(layout);

    auto newLayout = new QGridLayout();
    auto widget = new drawingWindow();

    widget->Ui()->drawWidget->setBTree(bTree);
    widget->Ui()->drawWidget->redraw();
    widgets.push_back(widget->Ui()->drawWidget);
    widget->show();
    layouts.push_back(newLayout);

    widget->Ui()->drawWidget->redraw();

    newLayout->addWidget(widget);
    tab->setLayout(newLayout);

    QString tabName = QString("B-Tree #%1").arg(++number);
    //ui->tabWidget->insertTab(ui->tabWidget->count() - 1, tab, tabName);
    //tabs.push_back(tab);

    //tab->layout()->replaceWidget(widgets[id], new drawingWidget(bTree, this));
}

void MainWindow::newRedBlackTab(){
    static int number = 0;
    RedBlackTree<int>* redBlackTree = new RedBlackTree<int>();

    for(int x = 0; x < 20; x++){
     //   redBlackTree->insert(x * 2);
    }
    int id = ui->tabWidget->currentIndex();
    std::cout << "Current id is " << id << std::endl;
    auto tab = ui->tabWidget->currentWidget();

    auto layout = tab->layout();
    deleteLayout(layout);

    auto newLayout = new QGridLayout();
    auto widget = new drawingWindow();
    widget->Ui()->drawWidget->setRedBlackTree(redBlackTree);
    widget->Ui()->drawWidget->redraw();

    widgets.push_back(widget->Ui()->drawWidget);
    widget->show();
    layouts.push_back(newLayout);

    widget->Ui()->drawWidget->redraw();

    newLayout->addWidget(widget);
    tab->setLayout(newLayout);


    QString tabName = QString("Red-Black Tree #%1").arg(++number);
    ui->tabWidget->insertTab(ui->tabWidget->currentIndex(), tab, tabName);
    //ui->tabWidget->setCurrentWidget(ui->tabWidget->findChild())
    //tabs.push_back(tab);

    //tab->layout()->replaceWidget(widgets[id], new drawingWidget(bTree, this));
}
