#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"
#include <QListWidgetItem>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>

gamewidget* gwidget = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    key_map = 0;
    stop = true;
    init_UI();
    init_slot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_UI()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(Qt::gray));
    //this->setPalette(palette);
    //this->setAutoFillBackground(true);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    //this->setAttribute(Qt::WA_NoSystemBackground, false);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏水平方向和垂直方向的滚动条
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gwidget = new gamewidget(ui->frame_game);
    QVBoxLayout* layout = new QVBoxLayout;

    layout->addSpacerItem(new QSpacerItem(10,10));
    layout->addWidget(gwidget);
    layout->addSpacerItem(new QSpacerItem(10,10));

    ui->frame_game->setLayout(layout);

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::init_slot()
{
    connect(ui->pushButton_up, SIGNAL(pressed()),this, SLOT(pushbutton_up_pressed()));
    connect(ui->pushButton_down, SIGNAL(pressed()),this, SLOT(pushbutton_down_pressed()));
    connect(ui->pushButton_left, SIGNAL(pressed()),this, SLOT(pushbutton_left_pressed()));
    connect(ui->pushButton_right, SIGNAL(pressed()),this, SLOT(pushbutton_right_pressed()));
    connect(ui->pushButton_A, SIGNAL(pressed()),this, SLOT(pushbutton_A_pressed()));
    connect(ui->pushButton_B, SIGNAL(pressed()),this, SLOT(pushbutton_B_pressed()));
    connect(ui->pushButton_C, SIGNAL(pressed()),this, SLOT(pushbutton_C_pressed()));
    connect(ui->pushButton_D, SIGNAL(pressed()),this, SLOT(pushbutton_D_pressed()));

    connect(ui->pushButton_up, SIGNAL(released()),this, SLOT(pushbutton_up_released()));
    connect(ui->pushButton_down, SIGNAL(released()),this, SLOT(pushbutton_down_released()));
    connect(ui->pushButton_left, SIGNAL(released()),this, SLOT(pushbutton_left_released()));
    connect(ui->pushButton_right, SIGNAL(released()),this, SLOT(pushbutton_right_released()));
    connect(ui->pushButton_A, SIGNAL(released()),this, SLOT(pushbutton_A_released()));
    connect(ui->pushButton_B, SIGNAL(released()),this, SLOT(pushbutton_B_released()));
    connect(ui->pushButton_C, SIGNAL(released()),this, SLOT(pushbutton_C_released()));
    connect(ui->pushButton_D, SIGNAL(released()),this, SLOT(pushbutton_D_released()));

    connect(&timer, SIGNAL(timeout()),this, SLOT(play()));
}

void MainWindow::setApplicationPath(QString path)
{
    changeDir(path);
    show_list();
}

void MainWindow::changeDir(QString path)
{
    list.clear();
    QDir dir(path);

    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        list.push_back((mfi));
    }
}

void MainWindow::show_list()
{
    ui->listWidget->clear();
    for(int i=0; i<list.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(list[i].fileName());
        ui->listWidget->addItem(item);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
        pushbutton_up_pressed();
        break;
    case Qt::Key_S:
        pushbutton_down_pressed();
        break;
    case Qt::Key_A:
        pushbutton_left_pressed();
        break;
    case Qt::Key_D:
        pushbutton_right_pressed();
        break;
    case Qt::Key_U:
        pushbutton_A_pressed();
        break;
    case Qt::Key_I:
        pushbutton_B_pressed();
        break;
    case Qt::Key_J:
        pushbutton_C_pressed();
        break;
    case Qt::Key_K:
        pushbutton_D_pressed();
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
        pushbutton_up_released();
        break;
    case Qt::Key_S:
        pushbutton_down_released();
        break;
    case Qt::Key_A:
        pushbutton_left_released();
        break;
    case Qt::Key_D:
        pushbutton_right_released();
        break;
    case Qt::Key_U:
        pushbutton_A_released();
        break;
    case Qt::Key_I:
        pushbutton_B_released();
        break;
    case Qt::Key_J:
        pushbutton_C_released();
        break;
    case Qt::Key_K:
        pushbutton_D_released();
        break;
    }
}

void MainWindow::pushbutton_up_pressed()
{
    int row = ui->listWidget->currentRow();
    row--;
    row<0?row=0:0;

    ui->listWidget->setCurrentRow(row);

    gwidget->setFlag(UP);
}

void MainWindow::pushbutton_down_pressed()
{
    int row = ui->listWidget->currentRow();
    row++;
    ui->listWidget->item(row)?0:row--;

    ui->listWidget->setCurrentRow(row);

    gwidget->setFlag(DOWN);
}

void MainWindow::pushbutton_left_pressed()
{
    gwidget->setFlag(LEFT);
}

void MainWindow::pushbutton_right_pressed()
{
    gwidget->setFlag(RIGHT);
}

void MainWindow::pushbutton_A_pressed()
{
    if(ui->stackedWidget->currentIndex()==1)
    {
        stop = true;
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }

    int row = ui->listWidget->currentRow();

    if(row>=0 && row<list.size())
    {
        if(list[row].isDir())
        {
            changeDir(list[row].filePath());
            show_list();
        }
        else
        {
            QString file = list[row].filePath();
            if(file.endsWith(".nes"))
            {
                load_file(file.toStdString().c_str());
                ui->stackedWidget->setCurrentIndex(1);
                fce_init();
                stop = false;
                timer.start(5);
            }
        }
    }
    gwidget->setFlag(A);

}

void MainWindow::pushbutton_B_pressed()
{
    gwidget->setFlag(B);
}

void MainWindow::pushbutton_C_pressed()
{
    gwidget->setFlag(C);
}

void MainWindow::pushbutton_D_pressed()
{
    gwidget->setFlag(D);
}

void MainWindow::pushbutton_up_released()
{
    gwidget->removeFlag(UP);
}

void MainWindow::pushbutton_down_released()
{
    gwidget->removeFlag(DOWN);
}

void MainWindow::pushbutton_left_released()
{
    gwidget->removeFlag(LEFT);
}

void MainWindow::pushbutton_right_released()
{
    gwidget->removeFlag(RIGHT);
}

void MainWindow::pushbutton_A_released()
{

    gwidget->removeFlag(A);
}

void MainWindow::pushbutton_B_released()
{
    gwidget->removeFlag(B);
}

void MainWindow::pushbutton_C_released()
{
    gwidget->removeFlag(C);
}

void MainWindow::pushbutton_D_released()
{
    gwidget->removeFlag(D);
}

void MainWindow::play()
{
    timer.stop();
    fce_run();
    if(!stop)
        timer.start(1);
}
