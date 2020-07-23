#ifndef RECVTHREAD_H
#define RECVTHREAD_H
#include <QThread>

#include <sys/stat.h>
#include <sys/types.h>
#include <evhttp.h>
#include <event.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <map>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_compat.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <event2/listener.h>
#include<memory>
#include <thread>
#include <atomic>
#define MYHTTPD_SIGNATURE   "MoCarHttpd v0.1"

class RecvThread : public QThread
{
    Q_OBJECT
public:
    explicit RecvThread(QObject *parent=nullptr);
    ~RecvThread();
protected:
    void run();
signals:
    void SendValue(double value ,QString file_path);
private:
    static void thread_start();
    bool is_stop=true;
    bool is_send =false;
    double m_value;
    QString m_image_path;
    static RecvThread* m_this;
    struct evhttp *m_http_server = NULL;
    std::string m_http_addr;
    int m_port;
    std::string m_url;
    int Init_Server();
    void get_post_message(struct evhttp_request *req);

    static void wrap_http_post(struct evhttp_request *req, void *arg);

    void http_handler_testpost_msg(struct evhttp_request *req,void *arg);
};

#endif // RECVTHREAD_H
