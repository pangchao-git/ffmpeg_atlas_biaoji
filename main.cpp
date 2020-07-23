

#include <QApplication>
#include <QTextCodec>

#include "mainwindow.h"
#include "login_dialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8"); //设置编码格式为UTF-8
    QTextCodec::setCodecForLocale(codec);
    Login_Dialog login ;
    login.show();
    int login_status = login.exec();
    qDebug()<<"status="<<login_status<<endl;
    if(login_status==QDialog::Rejected){
        MainWindow *w=new MainWindow();
        w->Start_video(login.Get_Rtsp().toStdString(),w);
        w->show();
        return a.exec();
    }else{
        return -1;

    }
    return a.exec();

}

