#include "login_dialog.h"
#include "ui_login_dialog.h"
#include <QMessageBox>
Login_Dialog::Login_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login_Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Login");
}

Login_Dialog::~Login_Dialog()
{
    delete ui;
}
QString Login_Dialog::Get_Rtsp()
{
    return m_rtsp;
}

void Login_Dialog::on_Play_pushButton_clicked()
{
    QString url = ui->Rtsp_lineEdit->text();
    if(url.isEmpty() || url.length()<10){
        QMessageBox::information(NULL, "Warning", "Rtsp_addr is Vaild");
        return;
    }else{
        m_rtsp = url;
        this->close();
        this->done(0);
    }
}

void Login_Dialog::on_Cancle_pushButton_clicked()
{
   close();
   this->done(1);

}
void Login_Dialog::closeEvent(QCloseEvent *event)
{
# if 0
    QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("退出程序"),
            QString(tr("警告：程序有一个任务正在运行中，是否结束操作退出?")),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::No) {
            event->ignore();  //忽略退出信号，程序继续运行
        }
        else if (button == QMessageBox::Yes) {
            close();
            this->done(1);  //接受退出信号，程序退出
        }
#endif

}


