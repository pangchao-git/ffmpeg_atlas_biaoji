
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QInputDialog>
#include <QtMath>
#include <QDebug>

#include<iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    this->setGeometry(0,0,m_SrcImg_w,m_SrcImg_h);
    this->setFixedSize(m_SrcImg_w,m_SrcImg_h);
    ui->setupUi(this);
    SetLabel_Ui();
    //ui->Value_edit->setReadOnly(true);
   // ui->Value_edit->setText("17.45");
    rect={0,0,0,0};
//    ui->Config_pushButton->sets
    mPlayer = new VideoPlayer;
//    cfg_widget.Set_Minwindow((void*)this);
    connect(mPlayer,SIGNAL(sig_GetOneFrame(QImage)),this,SLOT(slotGetOneFrame(QImage)));
    cfg_widget=new COnfig_Widget();
    connect(this,SIGNAL(Send_Img_rect(QImage)),cfg_widget,SLOT(GetNewFrame(QImage)));


}

MainWindow * MainWindow::GetInstance()
{
    return m_Mainwindow;

}

void MainWindow::Start_video(const string &url ,MainWindow* p_MainWindow)
{
    std::string  url_test="rtsp://admin:abcd1234@10.1.1.121:554/h264/ch1/main/av_stream";
   // std::string url = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
    mPlayer->SetRtsp_url(url_test);
    mPlayer->startPlay();
    cfg_widget->Set_Minwindow((void*)p_MainWindow);;

}


MainWindow::~MainWindow()
{
    delete ui;
    delete Config_label;
}

void MainWindow::SetLabel_Ui()
{
    Config_label=new QLabel();
    //set name
    Config_label->setWindowTitle("Config");
    Config_label->setGeometry(0,0,m_SrcImg_w,m_SrcImg_h);
    Config_label->setFixedSize(m_SrcImg_w,m_SrcImg_h);

}

void MainWindow::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    QRectF target(0, 0, m_SrcImg_w, m_SrcImg_h); //建立目标矩形
    QRectF source(0, 0, m_SrcImg_w, m_SrcImg_h); //建立源矩形，用来框定源图像文件中要显示的部分

    if (mImage.size().width() <= 0) return;
    QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);
      painter.drawImage(target, img, source);
      painter.setPen(QPen(Qt::red, 2));
      if(rect.x1!=0||rect.y1!=0||rect.x2!=0||rect.y1!=0){
           painter.drawRect(QRect(rect.x1, rect.y1, rect.x2 - rect.x1, rect.y2 - rect.y1));
      }

}





void MainWindow::slotGetOneFrame(QImage img)
{

    mImage = img;
    QRect q_rect(rect.x1, rect.y1, rect.x2-rect.x1, rect.y2-rect.y1);

    QImage m_image = mImage.copy(q_rect);

    emit Send_Img_rect(m_image);
    qDebug()<<"slotGetOneFrame"<<endl;


    update(); //调用update将执行 paintEvent函数
}

void MainWindow::on_Config_pushButton_clicked()
{
    qDebug()<<"start to config data"<<endl;
    Print_Rect(rect);
    if(rect.x1==0||rect.y1==0||rect.x2==0||rect.y2==0){
        return ;
    }
    QRect q_rect(rect.x1, rect.y1, rect.x2-rect.x1, rect.y2-rect.y1);
    QImage m_image = mImage.copy(q_rect);

    cfg_widget->Set_Image(m_image,q_rect);
    cfg_widget->Init_Glob();
    cfg_widget->show();
    rect={0,0,0,0};
    close();


}

// mouse pressenter
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    rect.x1 = event->pos().x();
    rect.y1 = event->pos().y();
    QCursor cursor;
    cursor.setShape(Qt::ClosedHandCursor);
    QApplication::setOverrideCursor(cursor);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    rect.x2 = event->pos().x(); //鼠标相对于所在控件的位置
    rect.y2 = event->pos().y();
    update();
    QApplication::restoreOverrideCursor();
    Print_Rect(rect);

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        rect.x2 = event->pos().x(); //鼠标相对于所在控件的位置
        rect.y2 = event->pos().y();
        update();
    }
}

void MainWindow::Print_Rect(Rect &rect)
{
    qDebug()<<"Roi_x1="<<rect.x1<<endl;
    qDebug()<<"Roi_y1="<<rect.y1<<endl;
    qDebug()<<"Roi_x2="<<rect.x2<<endl;
    qDebug()<<"Roi_y2="<<rect.y2<<endl;

}





void MainWindow::on_Exit_pushButton_clicked()
{
    close();
}
