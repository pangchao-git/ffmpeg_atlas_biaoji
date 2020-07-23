#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class Login_Dialog;
}

class Login_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Login_Dialog(QWidget *parent = 0);
    ~Login_Dialog();
    QString Get_Rtsp();
private slots:
    void on_Play_pushButton_clicked();
    void closeEvent(QCloseEvent *event);
    void on_Cancle_pushButton_clicked();

private:
    QString m_rtsp;
    Ui::Login_Dialog *ui;
};

#endif // LOGIN_DIALOG_H
