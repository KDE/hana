/*
 * SPDX-FileCopyrightText: 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "framedecoder.h"

#include <QFileInfo>
#include <QImage>

extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/display.h>
#include <libavutil/imgutils.h>
}

using namespace std;

FrameDecoder::FrameDecoder(const QString &filename)
{
    initialize(filename);
}

FrameDecoder::~FrameDecoder()
{
    destroy();
}

void FrameDecoder::initialize(const QString &filename)
{
    QFileInfo fi(filename);

    if ((!m_formatContextWasGiven)
            && avformat_open_input(&m_formatContext, fi.absoluteFilePath().toLocal8Bit().data(), nullptr, nullptr) != 0) {
        qDebug() << "Could not open input file: " << fi.absoluteFilePath();
        return;
    }

    if (avformat_find_stream_info(m_formatContext, nullptr) < 0) {
        qDebug() << "Could not find stream information";
        return;
    }

    if (!initializeVideo()) {
        // It already printed a message
        return;
    }

    m_frame = av_frame_alloc();
    if (m_frame) {
        m_initialized = true;
    }
}

bool FrameDecoder::isInitialized()
{
    return m_initialized;
}

void FrameDecoder::destroy()
{
    deleteFilterGraph();
    if (m_videoCodecContext) {
        avcodec_free_context(&m_videoCodecContext);
        m_videoCodecContext = nullptr;
    }

    if ((!m_formatContextWasGiven) && m_formatContext) {
        avformat_close_input(&m_formatContext);
        m_formatContext = nullptr;
    }

    if (m_packet) {
        av_packet_unref(m_packet);
        delete m_packet;
        m_packet = nullptr;
    }

    if (m_frame) {
        av_frame_free(&m_frame);
        m_frame = nullptr;
    }

    if (m_frameBuffer) {
        av_free(m_frameBuffer);
        m_frameBuffer = nullptr;
    }
}

QString FrameDecoder::getCodec()
{
    QString codecName;
    if (m_videoCodec) {
        codecName = QString::fromLatin1(m_videoCodec->name);
    }
    return codecName;
}

bool FrameDecoder::initializeVideo()
{
    m_videoStream = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &m_videoCodec, 0);
    if (m_videoStream < 0) {
        qDebug() << "Could not find video stream";
        return false;
    }

    m_videoCodecContext = avcodec_alloc_context3(m_videoCodec);
    avcodec_parameters_to_context(m_videoCodecContext, m_formatContext->streams[m_videoStream]->codecpar);

    if (m_videoCodec == nullptr) {
        // set to nullptr, otherwise avcodec_close(m_pVideoCodecContext) crashes
        m_videoCodecContext = nullptr;
        qDebug() << "Video Codec not found";
        return false;
    }

    m_videoCodecContext->workaround_bugs = 1;

    if (avcodec_open2(m_videoCodecContext, m_videoCodec, nullptr) < 0) {
        qDebug() << "Could not open video codec";
        return false;
    }

    return true;
}

uint FrameDecoder::getWidth()
{
    if (m_videoCodecContext) {
        return m_videoCodecContext->width;
    }

    return 0;
}

uint FrameDecoder::getHeight()
{
    if (m_videoCodecContext) {
        return m_videoCodecContext->height;
    }

    return 0;
}

uint FrameDecoder::getDuration()
{
    if (m_formatContext) {
        return static_cast<int>(m_formatContext->duration / AV_TIME_BASE);
    }

    return 0;
}

void FrameDecoder::seek(uint timeInSeconds)
{
    if (!m_allowSeek) {
        return;
    }

    int64_t timestamp = timeInSeconds * AV_TIME_BASE;

    if (timestamp < 0) {
        timestamp = 0;
    }

    int i = m_videoStream;
    int ret = av_seek_frame(m_formatContext,
            i, timestamp * m_formatContext->streams[i]->time_base.den / AV_TIME_BASE / m_formatContext->streams[i]->time_base.num,
                  AVSEEK_FLAG_BACKWARD);
    // int ret = av_seek_frame(m_pFormatContext, -1, timestamp, 0);
    if (ret >= 0) {
        avcodec_flush_buffers(m_videoCodecContext);
    } else {
        qDebug() << "Seeking in video failed";
        return;
    }

    uint keyFrameAttempts = 0;
    bool gotFrame {false};

    while ((!gotFrame || m_frame->flags & AV_FRAME_FLAG_KEY) && keyFrameAttempts < 200) {
        uint count = 0;
        while (!gotFrame && count < 20) {
            getVideoPacket();
            gotFrame = decodeVideoPacket();
            ++count;
        }

        ++keyFrameAttempts;
    }

    if (!gotFrame) {
        qDebug() << "Seeking in video failed2";
    }
}

bool FrameDecoder::decodeVideoFrame()
{
    bool frameFinished = false;

    while (!frameFinished && getVideoPacket()) {
        frameFinished = decodeVideoPacket();
    }

    if (!frameFinished) {
        qDebug() << "decodeVideoFrame() failed: frame not finished";
    }

    return frameFinished;
}

bool FrameDecoder::decodeVideoPacket()
{
    if (m_packet->stream_index != m_videoStream) {
        return false;
    }

    av_frame_unref(m_frame);

    avcodec_send_packet(m_videoCodecContext, m_packet);
    int ret = avcodec_receive_frame(m_videoCodecContext, m_frame);
    if (ret == AVERROR(EAGAIN)) {
        return false;
    }

    return true;
}

bool FrameDecoder::getVideoPacket()
{
    bool framesAvailable = true;
    bool frameDecoded = false;

    uint attempts = 0;

    if (m_packet) {
        av_packet_unref(m_packet);
        delete m_packet;
    }

    m_packet = new AVPacket();

    while (framesAvailable && !frameDecoded && (attempts++ < 1000)) {
        framesAvailable = av_read_frame(m_formatContext, m_packet) >= 0;
        if (framesAvailable) {
            frameDecoded = m_packet->stream_index == m_videoStream;
            if (!frameDecoded) {
                av_packet_unref(m_packet);
            }
        }
    }

    return frameDecoded;
}

void FrameDecoder::deleteFilterGraph()
{
    if (m_filterGraph) {
        av_frame_free(&m_filterFrame);
        avfilter_graph_free(&m_filterGraph);
        m_filterGraph = nullptr;
    }
}

bool FrameDecoder::initFilterGraph(enum AVPixelFormat pixfmt, uint width, uint height)
{
    AVFilterInOut *inputs = nullptr, *outputs = nullptr;

    deleteFilterGraph();
    m_filterGraph = avfilter_graph_alloc();

    QByteArray arguments("buffer=");
    arguments += "video_size=" + QByteArray::number(width) + 'x' + QByteArray::number(height) + ':';
    arguments += "pix_fmt=" + QByteArray::number(pixfmt) + ':';
    arguments += "time_base=1/1:pixel_aspect=0/1[in];";
    arguments += "[in]yadif[out];";
    arguments += "[out]buffersink";

    int ret = avfilter_graph_parse2(m_filterGraph, arguments.constData(), &inputs, &outputs);
    if (ret < 0) {
        qWarning() << "Unable to parse filter graph";
        return false;
    }

    if (inputs || outputs) {
        return false;
    }

    ret = avfilter_graph_config(m_filterGraph, nullptr);
    if (ret < 0) {
        qWarning() << "Unable to validate filter graph";
        return false;
    }

    m_bufferSourceContext = avfilter_graph_get_filter(m_filterGraph, "Parsed_buffer_0");
    m_bufferSinkContext = avfilter_graph_get_filter(m_filterGraph, "Parsed_buffersink_2");
    if (!m_bufferSourceContext || !m_bufferSinkContext) {
        qWarning() << "Unable to get source or sink";
        return false;
    }
    m_filterFrame = av_frame_alloc();
    m_lastWidth = width;
    m_lastHeight = height;
    m_lastPixfmt = pixfmt;

    return true;
}

bool FrameDecoder::processFilterGraph(AVFrame *dst, const AVFrame *src, enum AVPixelFormat pixfmt, uint width, uint height)
{
    if (!m_filterGraph || width != m_lastWidth || height != m_lastHeight || pixfmt != m_lastPixfmt) {
        if (!initFilterGraph(pixfmt, width, height)) {
            return false;
        }
    }

    memcpy(m_filterFrame->data, src->data, sizeof(src->data));
    memcpy(m_filterFrame->linesize, src->linesize, sizeof(src->linesize));
    m_filterFrame->width = width;
    m_filterFrame->height = height;
    m_filterFrame->format = pixfmt;

    int ret = av_buffersrc_add_frame(m_bufferSourceContext, m_filterFrame);
    if (ret < 0) {
        return false;
    }

    ret = av_buffersink_get_frame(m_bufferSinkContext, m_filterFrame);
    if (ret < 0) {
        return false;
    }

    av_image_copy(dst->data, dst->linesize, (const uint8_t **)m_filterFrame->data, m_filterFrame->linesize, pixfmt, width, height);
    av_frame_unref(m_filterFrame);

    return true;
}

void FrameDecoder::getScaledVideoFrame(uint preferredSize, bool maintainAspectRatio, QImage &videoFrame)
{
    if (m_frame->flags & AV_FRAME_FLAG_INTERLACED) {
        processFilterGraph((AVFrame *)m_frame, (AVFrame *)m_frame, m_videoCodecContext->pix_fmt, m_videoCodecContext->width, m_videoCodecContext->height);
    }

    auto scaledSize = calculateDimensions(preferredSize, maintainAspectRatio);
    convertAndScaleFrame(AV_PIX_FMT_RGB24, scaledSize.width(), scaledSize.height());
    // .copy() since QImage otherwise assumes the memory will continue to be available.
    videoFrame = QImage(m_frame->data[0], scaledSize.width(), scaledSize.height(), m_frame->linesize[0], QImage::Format_RGB888).copy();
}

void FrameDecoder::convertAndScaleFrame(AVPixelFormat format, int scaledWidth, int scaledHeight)
{
    SwsContext *scaleContext = sws_getContext(m_videoCodecContext->width,
                                              m_videoCodecContext->height,
                                              m_videoCodecContext->pix_fmt,
                                              scaledWidth,
                                              scaledHeight,
                                              format,
                                              SWS_BICUBIC,
                                              nullptr,
                                              nullptr,
                                              nullptr);

    if (nullptr == scaleContext) {
        qDebug() << "Failed to create resize context";
        return;
    }

    AVFrame *convertedFrame = nullptr;
    uint8_t *convertedFrameBuffer = nullptr;

    createAVFrame(&convertedFrame, &convertedFrameBuffer, scaledWidth, scaledHeight, format);

    sws_scale(scaleContext, m_frame->data, m_frame->linesize, 0, m_videoCodecContext->height, convertedFrame->data, convertedFrame->linesize);
    sws_freeContext(scaleContext);

    av_frame_free(&m_frame);
    av_free(m_frameBuffer);

    m_frame = convertedFrame;
    m_frameBuffer = convertedFrameBuffer;
}

QSize FrameDecoder::calculateDimensions(uint preferredSize, bool maintainAspectRatio)
{
    int destWidth{0};
    int destHeight{0};
    if (!maintainAspectRatio) {
        destWidth = preferredSize;
        destHeight = preferredSize;
    } else {
        int srcWidth = m_videoCodecContext->width;
        int srcHeight = m_videoCodecContext->height;
        int ascpectNominator = m_videoCodecContext->sample_aspect_ratio.num;
        int ascpectDenominator = m_videoCodecContext->sample_aspect_ratio.den;

        if (ascpectNominator != 0 && ascpectDenominator != 0) {
            srcWidth = srcWidth * ascpectNominator / ascpectDenominator;
        }

        if (srcWidth > srcHeight) {
            destWidth = preferredSize;
            destHeight = int(static_cast<float>(preferredSize) / srcWidth * srcHeight);
        } else {
            destWidth = int(static_cast<float>(preferredSize) / srcHeight * srcWidth);
            destHeight = preferredSize;
        }
    }
    return QSize{destWidth, destHeight};
}

void FrameDecoder::createAVFrame(AVFrame **avFrame, quint8 **frameBuffer, uint width, uint height, AVPixelFormat format)
{
    *avFrame = av_frame_alloc();

    uint numBytes = av_image_get_buffer_size(format, width + 1, height + 1, 16);
    *frameBuffer = reinterpret_cast<quint8 *>(av_malloc(numBytes));
    av_image_fill_arrays((*avFrame)->data, (*avFrame)->linesize, *frameBuffer, format, width, height, 1);
}
