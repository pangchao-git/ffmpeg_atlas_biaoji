

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include <QImage>
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
}
#include <chrono>
#include <thread>
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class VideoPlayer : public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayer();
    ~VideoPlayer();

    void startPlay();
    void SetRtsp_url(const std::string &url);
signals:
    void sig_GetOneFrame(QImage); //每获取到一帧图像 就发送此信号

protected:
    void run();

private:
    std::string m_rtsp_url;
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame, *pFrameRGB;
    AVPacket *packet;
    uint8_t *out_buffer;
    QString mFileName;


    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
};

#endif // VIDEOPLAYER_H
