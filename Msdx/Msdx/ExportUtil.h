#pragma  once
/************************************************************************/
/* ����Graph API���������п��ܰ���һЩȫ�ֺ���                                */
/************************************************************************/
#include "msdx_global.h"

//typedef unsigned char bool; //������ʾ�������ͣ�C�����в�û��bool����

#ifdef __cplusplus
extern "C"
{
#endif

	enum msdxErrors{
		MSDX_ERROR,
		MSDX_OUTOFMEM,
		MSDX_BADARG,
		MSDX_NOVIDEODEVICE,
		MSDX_NOAUDIODEVICE,
		MSDX_GRAPHFAIL,
		MSDX_CONNECTFAIL,
		MSDX_NOFILTER
	};

	/************************************************************************/
	/* ȫ��API����                                                          */
	/************************************************************************/

	int MSDXAPI testRoute();

	// Formal Interface

	int MSDXAPI msdx_initial(); 	
	void MSDXAPI msdx_uninitial();
	
	void MSDXAPI msdx_set_default_config();
	/*config*/
	void MSDXAPI msdx_config_set_video_device(const char * deviceid,const char *crossbar,const char* cb_input);   //junLin.chen 2010-07-20
	void MSDXAPI msdx_config_set_audio_format(int SampleRate,int Channels,int BitsPerSample);

	///������Ƶ�豸...
	void MSDXAPI msdx_config_set_audio_device(const char* inputDevice , const char* outputDevice );

	//�Ƿ���������Ӧ���ʵ���
	void MSDXAPI msdx_send_enable_adaptive_coderate(bool enable);


	/************************************************************************/
	/* ��������Ƶ����                                                         */
	/************************************************************************/

	enum AudioTestType
	{
		MSDX_LOCAL_ATYPE_DEVICE,
		MSDX_LOCAL_ATYPE_FILE
	};

	int MSDXAPI msdx_create_audio_test_graph(const AudioTestType type, const char * inputSrc, const char * outputDevice);
	void MSDXAPI msdx_destroy_audio_test_graph();

	int MSDXAPI msdx_create_video_test_graph(const char * deviceid, const char*crossbar, const char* cb_input, int width, int height, int fps);
	int MSDXAPI msdx_set_video_test_preview_window(HWND WndHandle, long left, long top, long width, long height);
	int MSDXAPI msdx_run_video_test_graph();
	void MSDXAPI msdx_destroy_video_test_graph();

#ifdef __cplusplus
}

#endif

