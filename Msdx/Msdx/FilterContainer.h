/************************************************************************/
/* 将每种filter的管理包装成类，方便复用                                 */
/* 注意：设计时没有考虑重复添加或连接filter，有关方法重复调用将导致行为 */
/* 不可预料.                                                            */
/************************************************************************/

#ifndef FILTER_CONTAINER_H
#define FILTER_CONTAINER_H

#include <atlbase.h>
#include <streams.h>
#include <string>
#include <list>
#include <map>

#include "DxDefinitions.h"

namespace msdx
{

//采集卡容器
class CrossbarContainer
{
public:
    CrossbarContainer();
    ~CrossbarContainer();

    //这里将graph builder的COM指针转成了经典指针，有存入了COM指针，这么做是没问题的
    //因为COM指针不像STL智能指针，COM指针添加引用计数是对内部经典指针执行p->AddRef()
    //实际引用计数保存在p指向的对象内。COM指针仅维护计数的增减
    dxStatus AddFilter(IGraphBuilder *owner_graph, std::string display_name);
    dxStatus AddFilterAuto(ICaptureGraphBuilder2 *capture, IBaseFilter *source_filter);
    IBaseFilter *GetFilter() { return video_crossbar_filter_; }
    dxStatus SetPhysicalType(long type);
    dxStatus ConnectToVideoSource(IBaseFilter *filter);

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> video_crossbar_filter_;
    long physical_type_;
};

//视频输入容器
class VideoSourceContainer
{
public:
    VideoSourceContainer();
    ~VideoSourceContainer();

    dxStatus AddFilter(IGraphBuilder *owner_graph, std::string device_path);
    IBaseFilter *GetFilter() { return video_source_filter_; }
    IPin *GetOutputPin() { return video_pin_out_; }
    const CMediaType &GetType() { return candidate_type_; }
    //设置视频格式，帧率当前并未使用
    dxStatus SetFormat(unsigned int width, unsigned int height, unsigned int fps);

private:
    void SeekRealFormat(IPin * pPin);

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> video_source_filter_;
    CComPtr<IPin> video_pin_out_;
    CMediaType candidate_type_;
};

//音频输入容器
class AudioSourceContainer
{
public:
    virtual ~AudioSourceContainer() {};

    //虽然是音频输入设备，但回声对消等可能要用到输出设备
    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, 
        std::string input_device, std::string output_device) = 0;

    virtual dxStatus SetFormat(unsigned int sample_rate, unsigned int channel_count, 
        unsigned int bits_per_sample, unsigned int latency) = 0;

    virtual IBaseFilter *GetFilter() = 0;
    virtual IPin *GetOutputPin() = 0;
    virtual const CMediaType &GetType() = 0;
};

class MicrophoneSourceContainer : public AudioSourceContainer
{
public:
    MicrophoneSourceContainer();
    ~MicrophoneSourceContainer();

    //输出设备无效
    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, 
        std::string input_device, std::string output_device);

    virtual dxStatus SetFormat(unsigned int sample_rate, unsigned int channel_count, 
        unsigned int bits_per_sample, unsigned int latency);

    virtual IBaseFilter *GetFilter() { return audio_source_filter_; }
    virtual IPin *GetOutputPin() { return audio_pin_out_; }
    virtual const CMediaType &GetType() { return candidate_type_; }

protected:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> audio_source_filter_;
    CComPtr<IPin> audio_pin_out_;
    CMediaType candidate_type_;
};

//注意：AEC是继承自MicrophoneSourceContainer的，因为只需要重写AddFilter方法
class AECSourceContainer : public MicrophoneSourceContainer
{
public:
    AECSourceContainer();
    ~AECSourceContainer();

    //输出设备无效
    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, 
        std::string input_device, std::string output_device);
};


//音频编码器容器

class AudioEncoderContainer
{
public:
    virtual ~AudioEncoderContainer() {};

    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt) = 0;
    virtual IBaseFilter *GetFilter() = 0;

    struct EncoderParam
    {
        //暂时用不到，用到以后再说
    };
    virtual dxStatus Config(EncoderParam &param) { return DX_WARN_NOT_IMPL; };
};

class SpeexEncoderContainer : public AudioEncoderContainer
{
public:
    ~SpeexEncoderContainer();

    virtual dxStatus AddFilter( IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt );
    virtual IBaseFilter * GetFilter() { return audio_encoder_filter_; };

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> audio_encoder_filter_;
};

class AACEncoderContainer : public AudioEncoderContainer
{
public:
    ~AACEncoderContainer();

    virtual dxStatus AddFilter( IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt );
    virtual IBaseFilter * GetFilter() { return audio_encoder_filter_; };

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> audio_encoder_filter_;
};


//音频解码器容器

class AudioDecoderContainer
{
public:
    virtual ~AudioDecoderContainer() {}

    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt) = 0;
    virtual IBaseFilter *GetFilter() = 0;
};

class SpeexDecoderContainer : public AudioDecoderContainer
{
public:
    ~SpeexDecoderContainer();

    dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt);
    IBaseFilter *GetFilter() {return audio_decoder_filter_; }

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> audio_decoder_filter_;
};

class AACDecoderContainer : public AudioDecoderContainer
{
public:
    ~AACDecoderContainer();

    dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt);
    IBaseFilter *GetFilter() {return audio_decoder_filter_; }

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> audio_decoder_filter_;
};


//video renderer

class VideoRendererContainer
{
public:
    virtual ~VideoRendererContainer() {};

    //若只添加渲染器，而不设置预览窗口，则可能（不使用EVR时）运行graph时会自动创建一个窗口
    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, bool use_evr) = 0;
    virtual dxStatus SetPreviewWindow(const VideoWindow &window) = 0;
};

//自动视频渲染，通过IGraphBuilder::Render完成，适合只有一个显示窗口的情形
class AutoVideoRendererContainer : public VideoRendererContainer
{
public:
    virtual ~AutoVideoRendererContainer();

    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, bool use_evr);
    virtual dxStatus SetPreviewWindow(const VideoWindow &window);

protected:
    dxStatus SetVMRWindow(unsigned long preview_window, 
        long left, long top, long width, long height);
    dxStatus SetEVRWindow(unsigned long preview_window, 
        long left, long top, long width, long height);

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> evr_renderer_filter_;
};



//audio renderer
class AudioRendererContainer
{
public:
    virtual ~AudioRendererContainer(){};

    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, 
        std::string display_name) = 0;
};

//沿用视频renderer命名吧，auto表示默认的renderer
class AutoAudioRendererContainer : public AudioRendererContainer
{
public:
    ~AutoAudioRendererContainer();

    dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, 
        std::string display_name);

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> audio_renderer_filter_;
};


//infinit_pin_tee container

class InfPinTeeContainer
{
public:
    ~InfPinTeeContainer();

    //name可选，对于共用的filter，最好指定一个name以区分
    dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, const wchar_t *name);
    IBaseFilter * GetFilter() { return inf_pin_tee_filter_; };
    IPin * GetUnconnectedPin();

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> inf_pin_tee_filter_;
    std::wstring filter_name_;
};


//file source container

class FileSourceContainer
{
public:
    ~FileSourceContainer();

    dxStatus AddFilter(IGraphBuilder *owner_graph, std::string file);
    IBaseFilter *GetFilter() { return file_source_filter_; }
    IPin *GetOutputPin() { return pin_out_; }

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> file_source_filter_;
    CComPtr<IPin> pin_out_;
};


}

#endif // !FILTER_CONTAINER_H
