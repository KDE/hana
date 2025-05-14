/*
    SPDX-FileCopyrightText: 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MOVIEDECODER_H
#define MOVIEDECODER_H

#include <QString>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavformat/avformat.h>
}

class QImage;

class FrameDecoder
{
public:
    explicit FrameDecoder(const QString &filename);
    ~FrameDecoder();

    QString getCodec();
    void seek(int timeInSeconds);
    bool decodeVideoFrame();
    void getScaledVideoFrame(int scaledSize, bool maintainAspectRatio, QImage &videoFrame);

    int getWidth();
    int getHeight();
    int getDuration();

    void initialize(const QString &filename);
    void destroy();
    bool isInitialized();

private:
    bool initializeVideo();

    bool decodeVideoPacket();
    bool getVideoPacket();
    void convertAndScaleFrame(AVPixelFormat format, int scaledSize, bool maintainAspectRatio, int &scaledWidth, int &scaledHeight);
    void createAVFrame(AVFrame **avFrame, quint8 **frameBuffer, int width, int height, AVPixelFormat format);
    void calculateDimensions(int squareSize, bool maintainAspectRatio, int &destWidth, int &destHeight);

    void deleteFilterGraph();
    bool initFilterGraph(enum AVPixelFormat pixfmt, int width, int height);
    bool processFilterGraph(AVFrame *dst, const AVFrame *src, enum AVPixelFormat pixfmt, int width, int height);

private:
    int m_videoStream {-1};
    AVFormatContext *m_formatContext{nullptr};
    AVCodecContext *m_videoCodecContext{nullptr};
    const AVCodec *m_videoCodec{nullptr};
    AVFrame *m_frame{nullptr};
    quint8 *m_frameBuffer{nullptr};
    AVPacket *m_packet{nullptr};
    bool m_formatContextWasGiven {false};
    bool m_allowSeek {true};
    bool m_initialized {false};
    AVFilterContext *m_bufferSinkContext{nullptr};
    AVFilterContext *m_bufferSourceContext{nullptr};
    AVFilterGraph *m_filterGraph{nullptr};
    AVFrame *m_filterFrame{nullptr};
    uint m_lastWidth {0};
    uint m_lastHeight {0};
    enum AVPixelFormat m_lastPixfmt {AV_PIX_FMT_NONE};
};

#endif
