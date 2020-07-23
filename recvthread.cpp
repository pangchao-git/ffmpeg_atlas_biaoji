#include "recvthread.h"
#include <QDebug>
#include <thread>
RecvThread* RecvThread::m_this=NULL;


void RecvThread::thread_start()
{
    m_this->Init_Server();
}

RecvThread::RecvThread(QObject *parent):QThread(parent)
{
    m_this=this;
    qDebug()<<"Init Server_Inint!!!\n";
    m_value=-1;
    m_this = this;
    m_http_addr ="0.0.0.0";
    m_port=56789;
    std::thread thread_server(thread_start);
    thread_server.detach();




}

void RecvThread::run(){


    while(is_stop){
        qDebug()<<"SEND INFOR .......\n";
        sleep(1);
        if(is_send){
            qDebug()<<"emit infor .......\n";
            qDebug()<<"SEND VALUE:"<<m_value;
            emit SendValue(m_value,m_image_path );
            is_send=false;
        }

    }

}


void RecvThread::get_post_message(struct evhttp_request *req)
{
    qDebug()<<"Client is alread!!\n";
    struct evkeyvalq *headers;
    struct evkeyval *header;
    struct evbuffer *evb = NULL;
    FILE *fp=NULL;
    const char *uri = evhttp_request_get_uri (req);
    qDebug("url=%s\n",uri);
    struct evhttp_uri *decoded = NULL;
    double Value_data=0.0;
    char file_name[100]={0};
    char net_mac[100]={0};

    char *request_data_buf=NULL;

    headers = evhttp_request_get_input_headers(req);
    int i=0;
    for (header = headers->tqh_first; header; header = header->next.tqe_next)
    {
        qDebug("header->key=%s,header->value=%s\n",header->key,header->value);
        if(i==3){
            memcpy(file_name ,header->value,strlen(header->value));
        }
        if(i==4){
           m_value = strtod(header->value, NULL);;
           qDebug()<<"valueuuuuuuu="<<m_value<<endl;
        }
        i++;
    }

    char response_data[100]={0};
    decoded = evhttp_uri_parse (uri);
    if (! decoded)
    {
        qDebug ("It's not a good URI. Sending BADREQUEST\n");
        evhttp_send_error (req, HTTP_BADREQUEST, 0);
        return;
    }
#if 1
    struct evbuffer *buf_tmp = evhttp_request_get_input_buffer (req);
//    evbuffer_add (buf_tmp, "", 1);    /* NUL-terminate the buffer */
    char *payload = (char *) evbuffer_pullup (buf_tmp, -1);

    int post_data_len = evbuffer_get_length(buf_tmp);

    if(post_data_len<0){
        printf("upload_failed ,send masg to client\n");
        memset(response_data,0,strlen(response_data));
        memcpy(response_data,"postfile_failed",strlen("postfile_failed"));
    }else{
        request_data_buf = (char*)malloc(post_data_len);
        if(NULL==request_data_buf){
            perror("malloc error!\n");
            return ;
        }else{
//            printf("upload_ok ,send msg to client\n");
            memcpy(response_data,"msg from server",sizeof("msg from server"));
            memset(response_data,0, sizeof(response_data));
            char *reply ="postfile_ok";
            memcpy(response_data,reply,strlen(reply));
        }
    }
    memset(request_data_buf,0,post_data_len);
    qDebug()<<"post_data_len="<<post_data_len<<endl;
    memcpy(request_data_buf, payload, post_data_len);

    //保存文件
    qDebug("file_name=%s\n", file_name);
    //判断文件是否存在

    fp=fopen(file_name,"w");
    if(NULL==fp){
        qDebug("fopen faild!\n!");
        return ;
    }

    int ret =fwrite(request_data_buf,post_data_len,1,fp);
    if(0>ret){
        qDebug("fwrite faild!\n!");
    }
    qDebug()<<"save image ok!\n";
    m_image_path = QString::fromUtf8(std::string(file_name).c_str());
    is_send=true;

#endif


    qDebug()<<"Send msg to client!\n";
    evb = evbuffer_new ();
    memcpy(response_data,"Send msg from server",strlen("Send msg from server"));
    printf("response_data=%s\n",response_data);
    evbuffer_add_printf(evb, response_data);
    //将封装好的evbuffer 发送给客户端
    evhttp_send_reply(req, HTTP_OK, "OK", evb);

//    if (decoded)
//        evhttp_uri_free (decoded);
//    if (evb)
//        evbuffer_free (evb);
//    fclose(fp);
//    free(request_data_buf);
}

void RecvThread::wrap_http_post(struct evhttp_request *req, void *arg) {
    m_this->http_handler_testpost_msg(req,arg);
}

void RecvThread::http_handler_testpost_msg(struct evhttp_request *req,void *arg)
{
    if(req == NULL){
        printf("====line:%d,%s\n",__LINE__,"input param req is null.");
        return;
    }
    get_post_message(req);//获取请求数据，一般是json格式的数据
}

int RecvThread::Init_Server()
{

         qDebug()<<"Server is start listenning!\n";
        event_init();
        m_http_server = evhttp_start(m_http_addr.c_str(),m_port);
        if(m_http_server == NULL){
            qDebug("====line:%d,%s\n",__LINE__,"http server start failed.");
            return -1;
        }
        evhttp_set_timeout(m_http_server,5);
        evhttp_set_cb(m_http_server,"/Result_upload",RecvThread::wrap_http_post,NULL);
        event_dispatch();
        printf("*********ENDNNDNNDNDN!\n");

        return 0;


}

RecvThread::~RecvThread()
{

     evhttp_free(m_http_server);

}
