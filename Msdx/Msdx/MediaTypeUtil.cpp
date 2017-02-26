#include "MediaTypeUtil.h"
#include "3rd/faac/aac_types.h"
#include "InterfaceDefination.h" //for speex clsid
#include "Log.h"

using namespace msdx;

msdx::dxStatus msdx::MediaTypeUtil::GetVideoType( 
    VideoTypeParam param, CMediaType &media_type_out )
{
    //��ֹ0֡�ʣ�����Ҫ��������
    if (0 == param.fps) param.fps = 30;

    CMediaType	temp_media_type;
    temp_media_type.InitMediaType();
    temp_media_type.SetType(&MEDIATYPE_Video);
    temp_media_type.SetFormatType(&FORMAT_VideoInfo);

    VIDEOINFOHEADER info;
    ZeroMemory(&info, sizeof(VIDEOINFOHEADER));

    //��Ը�ʽ���ض���ʼ��
    if (param.media_subtype == MEDIASUBTYPE_H264)
    {
        temp_media_type.SetSubtype(&MEDIASUBTYPE_H264);
        info.bmiHeader.biCompression = MAKEFOURCC('H', '2', '6', '4'); //������һ��
        temp_media_type.SetTemporalCompression(true);
        info.bmiHeader.biBitCount = 16;
    } 
    else
    {
        return DX_ERR_UNSUPPORT;
    }

    info.AvgTimePerFrame = 10000000 / param.fps;  //ÿ֡����ʱ�䣨ns��= 10��7�η� / ֡�ʣ�fps����ע�ⵥλ������
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth  = param.width;
    info.bmiHeader.biHeight = param.height;
    info.bmiHeader.biSizeImage = info.bmiHeader.biWidth * info.bmiHeader.biHeight * (info.bmiHeader.biBitCount / 8);
    info.bmiHeader.biPlanes = 1;
    info.dwBitRate=(DWORD)(info.bmiHeader.biSizeImage * 8 * info.AvgTimePerFrame);

    temp_media_type.SetFormat((BYTE*)&info, sizeof(VIDEOINFOHEADER));
    temp_media_type.SetSampleSize(info.bmiHeader.biSizeImage);

    media_type_out = temp_media_type;

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::MediaTypeUtil::GetAudioType( 
    const AudioTypeParam &param, CMediaType &media_type_out )
{
    CMediaType	temp_media_type;
    temp_media_type.InitMediaType();
    temp_media_type.SetType(&MEDIATYPE_Audio);
    temp_media_type.SetSubtype(&param.media_subtype);
    temp_media_type.SetFormatType(&FORMAT_WaveFormatEx);
    temp_media_type.SetTemporalCompression(false);
    temp_media_type.bFixedSizeSamples = TRUE;

    //AACҪ�Ȼ����չ����
    unsigned long ext_size = 0;
    BYTE *ex_ptr = NULL;

    //��ʼ��WAVEFORMATEX���ݽṹ
    BYTE *wfx = new BYTE[sizeof(WAVEFORMATEX) + ext_size];
    ZeroMemory(wfx, sizeof(WAVEFORMATEX) + ext_size);

    WAVEFORMATEX &info = *((WAVEFORMATEX *)wfx);
    info.wFormatTag=(WORD)param.media_subtype.Data1;
    info.nSamplesPerSec = param.sample_rate;
    info.nChannels = param.channels;

    if (param.media_subtype == MEDIASUBTYPE_AAC)
    {
        info.nBlockAlign = 0;
        info.wBitsPerSample = 0;
        info.nAvgBytesPerSec = 0;
        info.cbSize = 2;
        // decoder specific info
        BYTE *ex = wfx + sizeof(WAVEFORMATEX);
        memcpy(ex, ex_ptr, ext_size);
    } 
    else if (param.media_subtype == MEDIASUBTYPE_SPEEX ||
             param.media_subtype == MEDIASUBTYPE_PCM)
    {
        info.wBitsPerSample = param.bits_per_sample;
        info.nBlockAlign = param.channels * (info.wBitsPerSample / 8);
        info.nAvgBytesPerSec = param.sample_rate * info.nBlockAlign;
        info.cbSize = 0;
    }
    else
    {
        return DX_ERR_UNSUPPORT;
    }

    //This method allocates memory for the format block and copies the buffer specified by pFormat into the new format block. 
    temp_media_type.SetFormat((BYTE*)&info, sizeof(WAVEFORMATEX) + ext_size);
    delete wfx;

    media_type_out = temp_media_type;

    return DX_ERR_NONE;
}
