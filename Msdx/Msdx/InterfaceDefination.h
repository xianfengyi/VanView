#include "streams.h"

typedef  UINT32 uint_32;
typedef void (*funcPtr)(void *pClass,uint_32 delaytime,uint_32 jitter, uint_32 lossrate,uint_32 ipAddr); //声明函数指针


// {BDFB8696-9F5F-42d4-964F-DC0855034067}
const CLSID IID_ISCUTNCIMAGESOURCE = {0xbdfb8696, 0x9f5f, 0x42d4, {0x96, 0x4f, 0xdc, 0x8, 0x55, 0x3, 0x40, 0x67}};
MIDL_INTERFACE("BDFB8696-9F5F-42d4-964F-DC0855034067")
ISCUTNCIMAGESOURCE : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE setPosition(int left,int up,int right,int down) = 0;
	virtual HRESULT STDMETHODCALLTYPE setFrameRate(int FramesPerSecond )= 0;
	virtual HRESULT STDMETHODCALLTYPE setJPEGQuality(int lever)=0;
};


//请同SPEEX编解码Filter项目中定义保持一致
//自定义的SPEEX类型GUID，Data1为MAKEFOURCC('S','P','X','R')，SPXR是参照VLC解码器得到的
//其余部分参照MEDIASUBTYPE_PCM
const CLSID MEDIASUBTYPE_SPEEX = {0x52585053, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};

//by dsh 10-07-29
// {08af6540-4f21-11cf-aacb-0020af0b1992}
const CLSID IID_SpeexEncIProp = {0x08af6540, 0x4f21, 0x11cf, {0xaa, 0xcb, 0x00, 0x20, 0xaf, 0x0b, 0x19, 0x92}};
MIDL_INTERFACE("08af6540-4f21-11cf-aacb-0020af0b1992")
SpeexEncIProp : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE put_SampleRate(UINT32 sampleRate) = 0;
	virtual HRESULT STDMETHODCALLTYPE get_SampleRate(UINT32* pSampleRate)= 0;
	virtual HRESULT STDMETHODCALLTYPE put_Quality(UINT32 quality)=0;
	virtual HRESULT STDMETHODCALLTYPE get_Quality(UINT32* pQuality)=0;
	virtual HRESULT STDMETHODCALLTYPE put_State(UINT32 state)=0;
	virtual HRESULT STDMETHODCALLTYPE get_State(UINT32* pState)=0;
};

//{782416A6-EB09-4c14-A836-55D4D4E44AA4}
const CLSID IID_SpeexDecIProp = {0x782416a6, 0xeb09, 0x4c14, {0xa8, 0x36, 0x55, 0xd4, 0xd4, 0xe4, 0x4a, 0xa4}};
MIDL_INTERFACE("782416a6-eb09-4c14-a836-55d4d4e44aa4")
SpeexDecIProp : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE put_SampleRate(UINT32 sampleRate) = 0;
	virtual HRESULT STDMETHODCALLTYPE get_SampleRate(UINT32* pSampleRate)= 0;
	virtual HRESULT STDMETHODCALLTYPE put_Quality(UINT32 quality)=0;
	virtual HRESULT STDMETHODCALLTYPE get_Quality(UINT32* pQuality)=0;
};
//end by dsh 10-07-29/////


const CLSID IID_IHWVideoEncoder = {0xA4912AE3,0x4AD1,0x4c7f,{0xB9,0xC6,0xAE,0x15,0x66,0xF8,0xDE,0x5D}};
MIDL_INTERFACE("A4912AE3-4AD1-4c7f-B9C6-AE1566F8DE5D")
HWVideoEncoder :  public IUnknown
{
public:
	virtual void STDMETHODCALLTYPE setEncoderState( bool work );
};//Patrick

// {081C50F4-D11F-43ED-B35A-B39728A2C118}
static const GUID IID_IHWVideoDecoder = {0x81c50f4, 0xd11f, 0x43ed, {0xb3, 0x5a, 0xb3, 0x97, 0x28, 0xa2, 0xc1, 0x18}};
MIDL_INTERFACE("081C50F4-D11F-43ED-B35A-B39728A2C118")
IHWVideoDecoder : public IUnknown
{
    STDMETHOD (GrabSample) ( const char *filename) = 0;
};

// {A7B65F66-5354-48c6-8E7E-E1D1154B45D1}
const CLSID IID_ISCUTVideoEncoder = {0xa7b65f66, 0x5354, 0x48c6, {0x8e, 0x7e, 0xe1, 0xd1, 0x15, 0x4b, 0x45, 0xd1}};
MIDL_INTERFACE("A7B65F66-5354-48c6-8E7E-E1D1154B45D1")
ISCUTVideoEncoder : public IUnknown
{
public:

	virtual void STDMETHODCALLTYPE setQuantizer( int quan )= 0;
	virtual int  STDMETHODCALLTYPE getQuantizer()= 0;

	/**
	 * @brief 设置编码器的工作状态
	 * @param work true则正常工作，false表示停止工作
	 */
	virtual void STDMETHODCALLTYPE setEncoderState( bool work );
	virtual bool STDMETHODCALLTYPE getEncoderState();

	//设置码率调整 陈俊霖 2011-5-20
	virtual void STDMETHODCALLTYPE QuantizerPlusOne()=0;
	virtual void STDMETHODCALLTYPE QuantizerMinerOne()=0; 

	//设置输出视频分辨率
	virtual void STDMETHODCALLTYPE setDestSize( int width, int height );

};

//by dsh 2010-10-18
//echo cancel
const CLSID IID_IAECProperty = {0x3ee64499, 0xf3c2, 0x4074, {0x8e, 0x41, 0xfe, 0xe9, 0x1c, 0x8b, 0xe3, 0x6f}};
MIDL_INTERFACE("3EE64499-F3C2-4074-8E41-FEE91C8BE36F")
IAECProperty : public IUnknown
{
public:

	STDMETHOD(put_CapDevFriendName) (wchar_t* capDevFriendName,int capNameSize,
			wchar_t* rendDevFriendName,int rendNameSize)=0;
	STDMETHOD(put_PreviewHandle)(HWND hPreviewHandle)=0;

};

//VideoScalerControl ..zhenHua.sun 2011-04-12
const CLSID IID_IVideoScalerControl = {0x8f3594f4, 0x59d3, 0x47ea, {0xb0, 0x90, 0xcb, 0x5f, 0x6c, 0x67, 0xfa, 0x93}};
MIDL_INTERFACE("8F3594F4-59D3-47ea-B090-CB5F6C67FA93")
IVideoScalerControl : public IUnknown
{
public:

	//设置视频缩放后的高和宽
	virtual HRESULT STDMETHODCALLTYPE SetOutputParams( const int width , const int height ) =0;

	//获取视频缩放后的高和宽
	virtual HRESULT STDMETHODCALLTYPE GetOutputParams( int* width , int* height )=0;
};



//FilePlayCtrl
const CLSID IID_IFilePlayCtrl = {0x238b36e2, 0x87fc, 0x4087, {0xa1, 0x9f, 0x3b, 0x8a, 0xc, 0x3d, 0x90, 0x13}};
MIDL_INTERFACE("238B36E2-87FC-4087-A19F-3B8A0C3D9013")
ISCUTFilePlayCtrl : public IUnknown
{
public:
    // sync id
    virtual HRESULT SetSyncId(const char *id) = 0;

    // initial timestamp offset
    virtual HRESULT SetInitialTsOffset(REFERENCE_TIME offset) = 0;

    // play operation code and param
    enum PlayOpCode { kOpPause, kOpResume, kOpSeek };
    union PlayOpParam {
        struct {
            long long t;
        } seek;
    };
    // play operation. each time call this method, op_id must larger than previous 
    virtual HRESULT Operate(unsigned long op_id, PlayOpCode op, PlayOpParam *param) = 0;

    enum VideoCodec { VIDEO_NONE = 0, H264 };
    enum AudioCodec { AUDIO_NONE = 0, AAC_LC };
    struct MediaFormat
    {
        VideoCodec video_codec;
        long video_width;
        long video_height;
        long video_fps;
        AudioCodec audio_codec;
        long audio_sample_rate;
        long audio_channel;
        long audio_bit_count;
    };
    //call this method to get media infomation
    virtual HRESULT GetMediaFormat(MediaFormat &fmt) = 0;

    enum PlayStatus { ST_STOP = 0, ST_PLAYING, ST_PAUSE, ST_WAIT, ST_EOS };
    struct ProgressInfo
    {
        PlayStatus status;
        long long position;
        long long duration;
    };
    //call this method to get play progress info
    virtual HRESULT GetProgress(ProgressInfo &info) = 0;
};
