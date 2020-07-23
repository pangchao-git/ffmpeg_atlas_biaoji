
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPaintEvent>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "videoplayer.h"
#include <QDebug>
#include "config_widget.h"
#include "common.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MainWindow *GetInstance();
    void Start_video(const std::string &url,MainWindow* p_MainWindow);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void SetLabel_Ui();
    Ui::MainWindow *ui;
    MainWindow* m_Mainwindow=NULL;
    VideoPlayer *mPlayer; //播放线程

    QImage mImage; //记录当前的图像

    QString m_rtsp_url;

    bool open_red=false;
    int m_SrcImg_h=1080;
    int m_SrcImg_w=1920;
    Rect rect;
    //contorl

    QLabel *Config_label=NULL;
    void Print_Rect(Rect &rect);

    COnfig_Widget *cfg_widget=NULL;
signals:
    void Send_Img_rect(QImage image);
private slots:
    void slotGetOneFrame(QImage img);

    void on_Config_pushButton_clicked();

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
    void on_Exit_pushButton_clicked();
};

#endif // MAINWINDOW_H
