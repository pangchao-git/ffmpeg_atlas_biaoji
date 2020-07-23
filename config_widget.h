#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QPainter>
#include <QIntValidator>
#include <QRegExpValidator>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <boost/algorithm/string/replace.hpp>
#include <cpprest/filestream.h>
#include <cpprest/containerstream.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include "recvthread.h"
//#define SERVER_API_URL  (U("http://10.1.1.14:6502"))
#include <vector>
#include "common.h"
#include "cJSON.h"
//#include "json.h"
#define HYAMR_MAX_PT_LIST 20
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;



typedef struct Point_t{
    int x1;
    int y1;
    int x2;
    int y2;
}Point;

typedef struct
{
    int x;
    int y;
} MRV4POINT, *PMRV4POINT;

typedef struct MRV4Para_t
{
    MRV4POINT StartPt;
    MRV4POINT EndPt;
    long lPtNum;
    MRV4POINT ptPosList[HYAMR_MAX_PT_LIST];
    double dPtValList[HYAMR_MAX_PT_LIST];
    MRV4POINT circleCoord;
    int ptnum;
}MRV4Para;

namespace Ui {
class COnfig_Widget;
}

class COnfig_Widget : public QWidget
{
    Q_OBJECT


public:
    explicit COnfig_Widget(QWidget *parent = 0);
    ~COnfig_Widget();
    void Init_Glob();
    void Set_Image(QImage &image,QRect &rect);
    void Set_Minwindow(void *maindow);
    void paintEvent(QPaintEvent *event);
public slots:
    void RecvQimage(double value,QString image_path );
    void GetNewFrame(QImage image);
private slots:
    void on_Commit_pushButton_clicked();

    void on_Cancle_pushButton_clicked();

    void closeEvent(QCloseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);


    void on_Big_kedu_checkBox_clicked();

    void on_Cicle_checkBox_clicked();

private:
    bool m_Start_Infer=false;
    RecvThread *recv=NULL;
    int m_kedu_biaoji=-1;
    int m_Circle_biaoji=-1;
    MRV4Para m_params;
    static int rect_index;
    static int circel_index;
    int m_img_w=416;
    int m_img_h=416;
    int m_start_x=70;
    int m_start_y=90;
    QRect m_rect_show;
    Ui::COnfig_Widget *ui;
    void *p_mainwindow;
    Rect rect;
    std::vector<Point> m_Rect_map;
    std::vector<Point> m_Circle_map;
    Point point;
    void Print_Rect(Rect &rect);
    void json_analyseReq(char *image_path,MRV4Para &para,std::string &Reply,char *json_str);
    char* makeJson();
    cJSON *pJsonRoot  = NULL;
    cJSON *pIntArray  = NULL;
    cJSON *pCommArray = NULL;
    cJSON *pSubJson_kedu   = NULL;
    cJSON *pSubJson_circle   = NULL;
    char   *p          = NULL;

    int label_x=0;
    int label_y=0;
    std::string m_save_image="qt.save.jpg";
    QImage mImage;
    QRegExpValidator *m_IPValidator=NULL;

    // ip and port
    //rest ip
    QString m_Rest_ip;
    int m_Rest_port;
    QString m_Recv_ip;
    int m_Recv_port;

};

#endif // CONFIG_WIDGET_H
