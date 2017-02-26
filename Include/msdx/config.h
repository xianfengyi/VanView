#ifndef CONFIG_H
#define CONFIG_H

//ͨ�����Է����ڴ�����Ƶ�������Ļ����ϲ�֧��320�ĸ�ʽ��ֻ��֧��384��
//�ڸ���1280x720�Ŀ�߱ȵó�216�ĸ�...
//��������Ϊ������ֱ������Renderʱ��Ҫ��Surface Stride����
//���������Calculating Surface Stride��
//http://msdn.microsoft.com/en-us/library/windows/desktop/dd318229(v=vs.85).aspx

#define MSDX_CONF_SMALL_VIDEO_WIDTH     384
#define MSDX_CONF_SMALL_VIDEO_HEIGHT    216

//����Ƶ��ѡ���ø��ĺ���ͬʱ�޸�CoolView��RunningConfig���豸����ҳ�洦���߼�

#define MSDX_CONF_AUDIO_SAMPLERATE_DEFAULT    16000
#define MSDX_CONF_AUDIO_CHANNEL_DEFAULT           1
#define MSDX_CONF_AUDIO_BITSPERSAMPLE_DEFAULT    16

//�������������豸����
#define MSDX_CONF_AUDIO_INPUT_NOAEC 0
#define MSDX_CONF_AUDIO_INPUT_WINAEC 1
#define MSDX_CONF_AUDIO_INPUT_WEBRTC 2

//�����Զ�ѡ��ɼ����ı�ʾ
#define MSDX_CONF_CROSSBAR_AUTO "Auto"

//���屾��ý�����˿ڻ���
#define LOCALPRE_VIDEO_PORT_BASE 8000
#define LOCALREC_AUDIO_PORT_BASE 9000
#define LOCALREC_VIDEO_PORT_BASE 9100

#endif