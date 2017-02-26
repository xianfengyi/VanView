/************************************************************************/
/* ���ļ��ж�����MSDXģ������ֹ��õĶ��壬�Լ����Ⱪ©�Ķ���           */
/************************************************************************/

#ifndef DX_DEFINITIONS_H
#define DX_DEFINITIONS_H

#include <string>

namespace msdx
{

//������
enum dxStatus
{
    DX_ERR_NULL_PTR       = -1,
    DX_ERR_MEMORY_ALLOC   = -2,
    DX_ERR_SYS            = -3, //ϵͳAPI���ش���
    DX_ERR_ABORT          = -4,
    DX_ERR_UNSUPPORT      = -5,
    DX_ERR_BAD_DEVICE     = -6, //��Ч�豸
    DX_ERR_BAD_INTERFACE  = -7, //��Ч�Ľӿ�
    DX_ERR_BAD_PARAM      = -8,

    DX_ERR_NONE           = 0,

    DX_WARN_NOT_IMPL      = 1,
    DX_WARN_OP_IGNORED    = 2,
};

#define STATUS_ERR(status) (status) < 0
#define STATUS_OK(status) (status) >= 0

//Graph״̬
enum dxGraphStatus
{
    DX_GRAPH_STATUS_UNKNOWN = -1,
    DX_GRAPH_STATUS_INITIALIZED = 0,
    DX_GRAPH_STATUS_RUNNING,
    DX_GRAPH_STATUS_STOPPED,
    DX_GRAPH_STATUS_PAUSED
};

//��Ƶ���뷽ʽ
enum dxAudioCodec
{
    DX_AUDIO_CODEC_UNKNOWN,
    DX_AUDIO_CODEC_SPEEX,
    DX_AUDIO_CODEC_AAC
};

//��Ƶ���뷽ʽ
enum dxVideoCodec
{
    DX_VIDEO_CODEC_UNKNOWN,
    DX_VIDEO_CODEC_H264,
};

//����ɼ�����������δ����ʱ��ֵ
//ע�ⲻҪ��ϵͳö��tagPhysicalConnectorType�е��κ�ֵ�ظ�
//http://msdn.microsoft.com/en-us/library/windows/desktop/dd390991(v=vs.85).aspx
const int PhysConn_Auto = -1;


//���緢�ͻ����Ŀ��
typedef struct
{
    std::string ip; //ipv4 or ipv6
    unsigned short port;
} NetAddr;

//��ƵԤ������
typedef struct
{
    unsigned long window_handle;
    long position_left; //ͼ��λ�ڴ��ڵ�λ��
    long position_top;
    long width; //��ʾͼ��Ĵ�С
    long height;
} VideoWindow;


} // end namespace msdx

#endif