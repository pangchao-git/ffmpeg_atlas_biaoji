#include "config_widget.h"
#include "ui_config_widget.h"
#include <QImage>
#include "mainwindow.h"

int COnfig_Widget::rect_index=0;
int COnfig_Widget::circel_index=0;
COnfig_Widget::COnfig_Widget(QWidget *parent) :
    QWidget(parent),p_mainwindow(NULL),
    ui(new Ui::COnfig_Widget)
{
    ui->setupUi(this);
    const int widget_w = 1456 ;
    const int widget_h = 769 ;
    setFixedSize(widget_w,widget_h);
    ui->Result_show_label->setGeometry(589,89,471,451);
    ui->Result_show_label->setFixedSize(m_img_w,m_img_h);
    ui->Value_lineEdit->setReadOnly(true);

    ui->Kedu_num_lineEdit->setValidator(new QIntValidator(0, 20, this));
    ui->Point_num_lineEdit->setValidator(new QIntValidator(1, 2, this));

    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");

    m_IPValidator = new QRegExpValidator(rx, this);
    ui->RESP_ip_linedit->setValidator(m_IPValidator);
    ui->REST_portlineEdit->setValidator(new QIntValidator(0, 99999, this));
    ui->Recv_ip_lineEdit->setValidator(m_IPValidator);
    ui->Recv_port_lineEdit->setValidator(new QIntValidator(0, 99999, this));

    setWindowTitle("Config_Infer");
    rect={0,0,0,0};
    point={0,0};

    // set background
    QPalette label_palette_crop,label_palette_result;

    label_palette_result.setColor(QPalette::Background, QColor(200, 200, 200));


    ui->Result_show_label->setAutoFillBackground(true);
    ui->Result_show_label->setPalette(label_palette_result);
#if 1


    recv= new RecvThread();

    //Recv infor from Server

    connect(recv, SIGNAL(SendValue(double ,QString)),this,SLOT(RecvQimage(double ,QString )));

#endif

}


char *COnfig_Widget::makeJson()
{

    cJSON *root = NULL, *arr = NULL,*arr_circle=NULL;
       char *out = NULL;
       root = cJSON_CreateObject();
       cJSON_AddStringToObject(root,"Recv_ip",m_Recv_ip.toStdString().c_str());
       cJSON_AddNumberToObject(root,"Recv_port",m_Recv_port);
       cJSON_AddNumberToObject(root, "point_num", ui->Point_num_lineEdit->text().toInt());
       cJSON_AddNumberToObject(root, "kedu_num", ui->Kedu_num_lineEdit->text().toInt());
       cJSON_AddNumberToObject(root, "step", ui->Step_lineEdit->text().toDouble());
       cJSON_AddNumberToObject(root, "min_kedu", ui->Point_start_lineEdit_2->text().toDouble());
       cJSON_AddItemToObject(root, "kedu_index", arr = cJSON_CreateArray());
       cJSON_AddItemToObject(root, "Circle_index", arr_circle = cJSON_CreateArray());


       for(int i=0;i<m_Rect_map.size();i++){
           cJSON *json = cJSON_CreateObject();
           int x=0,y=0;
           if(m_Rect_map[i].x1<0||m_Rect_map[i].y1<0){
                x=m_Rect_map[i].x2;
                y=m_Rect_map[i].y2;
           }else{
               x=m_Rect_map[i].x1;
               y=m_Rect_map[i].y1;
           }

           cJSON_AddNumberToObject(json , "x", x);
           cJSON_AddNumberToObject(json , "y", y);
           cJSON_AddItemToArray(arr, json);
       }

       for(int i=0;i<m_Circle_map.size();i++){
           cJSON *json_b = cJSON_CreateObject();
           int x=0,y=0;
           if(m_Circle_map[i].x1<0||m_Circle_map[i].y1<0){
                x=m_Circle_map[i].x2;
                y=m_Circle_map[i].y2;
           }else{
               x=m_Circle_map[i].x1;
               y=m_Circle_map[i].y1;
           }
           cJSON_AddNumberToObject(json_b , "x", x);
           cJSON_AddNumberToObject(json_b , "y", y);
           cJSON_AddItemToArray(arr_circle, json_b);
       }

       out = cJSON_Print(root);
   //    printf("str=%s\n",out);
       return out;
}



COnfig_Widget::~COnfig_Widget()
{
    delete ui;
}

void COnfig_Widget::Init_Glob()
{
    m_Rect_map.clear();
    m_Circle_map.clear();
    rect_index=0;
    circel_index=0;
    m_Circle_biaoji =Qt::Unchecked;
    m_kedu_biaoji = Qt::Unchecked;
    ui->Kedu_num_lineEdit->clear();
    ui->Point_num_lineEdit->clear();
    ui->Point_start_lineEdit_2->clear();
    ui->Step_lineEdit->clear();
    ui->Value_lineEdit->clear();
    ui->Result_show_label->clear();
    recv->start();

}

void COnfig_Widget::Set_Image(QImage &image, QRect &rect)
{
    //resize
    mImage =image;
    m_rect_show=rect;

}

void COnfig_Widget::Set_Minwindow(void *maindow)
{
    p_mainwindow = maindow;

}

void COnfig_Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("退出程序"),
            QString(tr("警告：程序有一个任务正在运行中，是否结束操作退出?")),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::No) {
            event->ignore();  //忽略退出信号，程序继续运行
        }
        else if (button == QMessageBox::Yes) {
            close();
            if(p_mainwindow!=NULL){
                 ((MainWindow*)p_mainwindow)->show();
            }else{
                qDebug()<<"p_mainwindow IS NULL *************\n";
            }
        }

}

void COnfig_Widget::on_Commit_pushButton_clicked()
{
#if 1
    qDebug()<<"Recv new frame!!!\n"<<endl;

    if(ui->Point_num_lineEdit->text().isEmpty()||ui->Point_start_lineEdit_2->text().isEmpty()
            ||ui->Kedu_num_lineEdit->text().isEmpty()||ui->Step_lineEdit->text().isEmpty()){
        QMessageBox::information(NULL, "Warning", "Arguement must be add!");
        return;
    }
    m_Rest_ip = ui->RESP_ip_linedit->text();
    m_Rest_port = ui->REST_portlineEdit->text().toInt();

    m_Recv_ip = ui->Recv_ip_lineEdit->text();
    m_Recv_port = ui->Recv_port_lineEdit->text().toInt();
    m_Start_Infer=true;
#endif
}


void COnfig_Widget::GetNewFrame(QImage image)
{

    if(m_Start_Infer==true){
        //Image to jepg
        QImage m_image =image;
        m_image.save(m_save_image.c_str(), "JPG", 100);
        std::string image_path =m_save_image;
        std::string Reply;
        char * json_src=makeJson();
        qDebug("src=%s\n",json_src);
        json_analyseReq(const_cast<char *>(image_path.c_str()),m_params,Reply,json_src);

        free(json_src);
        json_src = NULL;

        cJSON* cjson = cJSON_Parse(Reply.c_str());
        char* status = cJSON_GetObjectItem(cjson,"status")->valuestring;
        qDebug("status=%s\n",status);
        cJSON_Delete(cjson);
        cjson=NULL;
    }



}

void COnfig_Widget::RecvQimage(double value,QString image_path)
{

    QFile fd(image_path);
    if(fd.exists()==false){
        qDebug()<<"file is not found!\n";
    }
    qDebug()<<"Recv_value="<<value<<endl;
    qDebug()<<"image_path="<<image_path<<endl;
    QImage image;
    image.load(image_path);
    ui->Result_show_label->setPixmap(QPixmap::fromImage(image));

    QString value_s = QString::fromUtf8(std::to_string(value).c_str());
    ui->Value_lineEdit->setText(value_s);

}

void COnfig_Widget::on_Cancle_pushButton_clicked()
{
    QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("退出程序"),
            QString(tr("警告：程序有一个任务正在运行中，是否结束操作退出?")),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::No) {

        }
        else if (button == QMessageBox::Yes) {
                close();
                qDebug()<<"close *************\n";
                if(p_mainwindow!=NULL){
                    ui->Cicle_checkBox->setCheckState(Qt::Unchecked);
                    ui->Big_kedu_checkBox->setCheckState(Qt::Unchecked);
                     ((MainWindow*)p_mainwindow)->show();
                }else{
                    qDebug()<<"p_mainwindow IS NULL *************\n";
                }


        }

}



void COnfig_Widget::paintEvent(QPaintEvent *event)
{


    QPainter painter(this);

    QRectF target(m_start_x, m_start_y, m_img_w, m_img_h); //建立目标矩形
    QRectF source(0, 0, m_img_w, m_img_h); //建立源矩形，用来框定源图像文件中要显示的部分

    if (mImage.size().width() <= 0) return;
    QImage reszie=mImage.scaled(m_img_w,m_img_h);
    painter.drawImage(target, reszie, source);
    int x=0,y=0;
    painter.setPen(QPen(Qt::red, 6));

      if(m_kedu_biaoji==Qt::Checked && m_Circle_biaoji == Qt::Unchecked){
           qDebug()<<"start draw point m_kedu_biaoji!\n";
          for(int index =0;index<m_Rect_map.size();index++){
              if(m_Rect_map[index].x1<0||m_Rect_map[index].y1<0){
                  x=m_Rect_map[index].x2;
                  y=m_Rect_map[index].y2;
              }else{
                  x=m_Rect_map[index].x1;
                  y=m_Rect_map[index].y1;
              }
            qDebug()<<"Draw_Kedu_x="<<x<<"Draw_Kedu_y="<<y<<endl;
           painter.drawPoint(x+m_start_x,y+m_start_y);
          }
      }
      if(m_Circle_biaoji ==Qt::Checked&& m_kedu_biaoji==Qt::Unchecked){
          qDebug()<<"start draw point m_Circle_biaoji!\n";
          for(int index =0;index<m_Circle_map.size();index++){
              if(m_Circle_map[index].x1<0||m_Circle_map[index].y1<0){
                  x=m_Circle_map[index].x2;
                  y=m_Circle_map[index].y2;
              }else{
                  x=m_Circle_map[index].x1;
                  y=m_Circle_map[index].y1;
              }
              qDebug()<<"Draw_Kedu_x="<<x<<"Draw_Kedu_y="<<y<<endl;
           painter.drawPoint(x+m_start_x,y+m_start_y);
          }
      }

      rect={0,0,0,0};

}


// mouse pressenter
void COnfig_Widget::mousePressEvent(QMouseEvent *event)
{
    rect.x1 = event->pos().x();
    rect.y1 = event->pos().y();
    QCursor cursor;
    cursor.setShape(Qt::ClosedHandCursor);
    QApplication::setOverrideCursor(cursor);
    if((m_kedu_biaoji==Qt::Checked)&&(m_Circle_biaoji==Qt::Unchecked)){
        if (event->button() & Qt::RightButton)  //右键按下
        {
            if(m_kedu_biaoji==Qt::Checked){
                qDebug()<<"size="<<m_Rect_map.size();
                if(m_Rect_map.size()!=0){
                    qDebug()<<" m_kedu_biaoji RightButton mousePressEvent **************\n"<<endl;
                    m_Rect_map.pop_back();
                    rect_index=m_Rect_map.size();
                    qDebug()<<"remove index is "<<rect_index<<endl;
                }

            }
        }
        if(event->button() & Qt::LeftButton){
           // rect_index++;
        }
    }

    if(m_Circle_biaoji==Qt::Checked&&(m_kedu_biaoji==Qt::Unchecked)){
        if (event->button() & Qt::RightButton)  //右键按下
        {
            if(m_Circle_biaoji==Qt::Checked){
                if(m_Circle_map.size()!=0){
                    rect_index=m_Circle_map.size();
                    qDebug()<<"m_Circle_biaoji RightButton mousePressEvent size is**************\n"<<rect_index<<endl;
                    m_Circle_map.pop_back();
                    rect_index=m_Circle_map.size();
                    qDebug()<<"remove index is "<<rect_index<<endl;
                   // circel_index--;
                }

            }
        }
        if(event->button() & Qt::LeftButton){
           // circel_index++;
        }

    }else{

    }

}

void COnfig_Widget::mouseReleaseEvent(QMouseEvent *event)
{
#if 1
    rect.x2 = event->pos().x(); //鼠标相对于所在控件的位置
    rect.y2 = event->pos().y();
    update();

    QApplication::restoreOverrideCursor();

    if(m_kedu_biaoji == Qt::Checked&& m_Circle_biaoji == Qt::Unchecked){
        if (event->button() & Qt::LeftButton){
            static int index=0;
            point.x1=rect.x1-m_start_x;
            point.y1=rect.y1-m_start_y;
            point.x2=rect.x2-m_start_x;
            point.y2=rect.y2-m_start_y;
            qDebug()<<"point.x1="<<point.x1<<endl;
            qDebug()<<"point.y1="<<point.y1<<endl;
            qDebug()<<"Big_kedu_checkBox_status is checked, current_index="<<index<<endl;
            m_Rect_map.push_back(point);
            index++;
        }

    }
    qDebug()<<"m_Circle_biaoji="<<m_Circle_biaoji<<endl;
    qDebug()<<"m_kedu_biaoji="<<m_kedu_biaoji<<endl;
    if(m_Circle_biaoji == Qt::Checked && m_kedu_biaoji == Qt::Unchecked){
        if (event->button() & Qt::LeftButton){
            static int index=0;
            point.x1=rect.x1-m_start_x;
            point.y1=rect.y1-m_start_y;
            point.x2=rect.x2-m_start_x;
            point.y2=rect.y2-m_start_y;
            qDebug()<<"point.x1="<<point.x1<<endl;
            qDebug()<<"point.y1="<<point.y1<<endl;
            m_Circle_map.push_back(point);
            qDebug()<<"m_Circle_biaoji_checkBox_status is checked,cureent_index="<<index<<endl;
            index++;
        }
    }
    else{
        //
    }

#endif

}

void COnfig_Widget::mouseMoveEvent(QMouseEvent *event)
{
#if 1
    if (event->buttons() & Qt::LeftButton) {
        qDebug()<<"letf mouseMoveEvent**************\n"<<endl;
        rect.x2 = event->pos().x(); //鼠标相对于所在控件的位置
        rect.y2 = event->pos().y();
        update();
    }
    if(event->buttons() & Qt::RightButton){
         qDebug()<<"RightButton mouseMoveEvent**************\n"<<endl;

    }
#endif
}


void COnfig_Widget::json_analyseReq(char *image_path,MRV4Para &para,std::string &Reply,char *json_str)
{
        using concurrency::streams::file_stream;
        using concurrency::streams::basic_istream;
        std::string ret="";
        std::string REST_SERVER="http://"+m_Rest_ip.toStdString()+":"+std::to_string(m_Rest_port);
        container_buffer<std::string> inStringBuffer;
        file_stream<unsigned char>::open_istream(U(image_path))
        .then([inStringBuffer](pplx::task<basic_istream<unsigned char>> streamTask) -> pplx::task<size_t> {
            try {
                auto fileStream = streamTask.get();
                return fileStream.read_to_end(inStringBuffer);
            } catch (const std::system_error& e) {
                printf("Error system_error:%s\n", e.what());
            }
            return pplx::task_from_result(size_t(0));
        })
        .then([=](size_t bytesRead) -> pplx::task<http_response> {
            std::string stInput = inStringBuffer.collection();
            std::vector<unsigned char> filedata(stInput.begin(), stInput.end());
            auto base64 = utility::conversions::to_base64(filedata);
            vector<json::value> jsonArr = {json::value::number(113)};
            json::value json_v;
            json_v["taskid"] = json::value::string("100055");
            json_v["algrtype"] = json::value::array(jsonArr);
            json_v["deviceid"] = json::value::string("1223590");
            json_v["picid"] = json::value::string("100055");
            json_v["filename"] = json::value::string("100055.jpg");
            json_v["filedata"] = json::value::string(base64);
            json_v["filesize"] = json::value::number(base64.size());
            json_v["param_infor"] = json::value::string(json_str);
            http_client client(REST_SERVER);
            qDebug("request: %s/%s!\n", client.base_uri().to_string().c_str(), "detections/image");
            return client.request(methods::PUT, U("detections/image"), json_v);
        })
        .then([](http_response response) -> pplx::task<string_t> {
            if(response.status_code() == status_codes::OK) {
                return response.extract_string();
            }
            return pplx::task_from_result(string_t());
        })
        .then([&ret](pplx::task<string_t> previousTask) {
            try {
                string_t const & v = previousTask.get();
                ret=std::string(const_cast<char*>(v.c_str()));
                qDebug("response: %s\n", v.c_str());
            } catch (http_exception const & e) {
                qDebug("Error http_exception:%s\n", e.what());
            }
        })
        .wait();
        qDebug("ret=%s\n",ret.c_str());
        Reply=ret;
}



void COnfig_Widget::Print_Rect(Rect &rect)
{
    qDebug()<<"Roi_x1="<<rect.x1<<endl;
    qDebug()<<"Roi_y1="<<rect.y1<<endl;
    qDebug()<<"Roi_x2="<<rect.x2<<endl;
    qDebug()<<"Roi_y2="<<rect.y2<<endl;

}

void COnfig_Widget::on_Big_kedu_checkBox_clicked()
{
    m_kedu_biaoji = ui->Big_kedu_checkBox->checkState();

}

void COnfig_Widget::on_Cicle_checkBox_clicked()
{
    m_Circle_biaoji = ui->Cicle_checkBox->checkState();

}
