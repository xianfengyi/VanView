/************************************************************************/
/* ��ÿ��filter�Ĺ����װ���࣬���㸴��                                 */
/* ע�⣺���ʱû�п����ظ���ӻ�����filter���йط����ظ����ý�������Ϊ */
/* ����Ԥ��.                                                            */
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

//�ɼ�������
class CrossbarContainer
{
public:
    CrossbarContainer();
    ~CrossbarContainer();

    //���ｫgraph builder��COMָ��ת���˾���ָ�룬�д�����COMָ�룬��ô����û�����
    //��ΪCOMָ�벻��STL����ָ�룬COMָ��������ü����Ƕ��ڲ�����ָ��ִ��p->AddRef()
    //ʵ�����ü���������pָ��Ķ����ڡ�COMָ���ά������������
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

//��Ƶ��������
class VideoSourceContainer
{
public:
    VideoSourceContainer();
    ~VideoSourceContainer();

    dxStatus AddFilter(IGraphBuilder *owner_graph, std::string device_path);
    IBaseFilter *GetFilter() { return video_source_filter_; }
    IPin *GetOutputPin() { return video_pin_out_; }
    const CMediaType &GetType() { return candidate_type_; }
    //������Ƶ��ʽ��֡�ʵ�ǰ��δʹ��
    dxStatus SetFormat(unsigned int width, unsigned int height, unsigned int fps);

private:
    void SeekRealFormat(IPin * pPin);

private:
    CComPtr<IGraphBuilder> graph_builder_;
    CComPtr<IBaseFilter> video_source_filter_;
    CComPtr<IPin> video_pin_out_;
    CMediaType candidate_type_;
};

//��Ƶ��������
class AudioSourceContainer
{
public:
    virtual ~AudioSourceContainer() {};

    //��Ȼ����Ƶ�����豸�������������ȿ���Ҫ�õ�����豸
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

    //����豸��Ч
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

//ע�⣺AEC�Ǽ̳���MicrophoneSourceContainer�ģ���Ϊֻ��Ҫ��дAddFilter����
class AECSourceContainer : public MicrophoneSourceContainer
{
public:
    AECSourceContainer();
    ~AECSourceContainer();

    //����豸��Ч
    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, 
        std::string input_device, std::string output_device);
};


//��Ƶ����������

class AudioEncoderContainer
{
public:
    virtual ~AudioEncoderContainer() {};

    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt) = 0;
    virtual IBaseFilter *GetFilter() = 0;

    struct EncoderParam
    {
        //��ʱ�ò������õ��Ժ���˵
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


//��Ƶ����������

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

    //��ֻ�����Ⱦ������������Ԥ�����ڣ�����ܣ���ʹ��EVRʱ������graphʱ���Զ�����һ������
    virtual dxStatus AddFilter(IGraphBuilder *owner_graph, IPin *upstream_pin, bool use_evr) = 0;
    virtual dxStatus SetPreviewWindow(const VideoWindow &window) = 0;
};

//�Զ���Ƶ��Ⱦ��ͨ��IGraphBuilder::Render��ɣ��ʺ�ֻ��һ����ʾ���ڵ�����
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

//������Ƶrenderer�����ɣ�auto��ʾĬ�ϵ�renderer
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

    //name��ѡ�����ڹ��õ�filter�����ָ��һ��name������
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
