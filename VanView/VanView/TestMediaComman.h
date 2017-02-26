#ifndef TEST_MEDIA_H
#define TEST_MEDIA_H

#include <Qtcore/QString>
#include <QtCore/QDataStream>

#define		TEST_MEDIA_SERVICE_NAME			"com.dcampus.coolview.channel.type.testMedia"
#define		TEST_MEDIA_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/channel/type/testMedia"

#define TEST_MEDIA_PROC_NAME	"CvTestMedia"

#define AUDIOOUTPUT_TEST_MEDIA_ID   "TestAudioOutput"
#define AUDIODINPUT_TEST_MEDIA_ID   "TestAudioCapture"
#define VIDEO_PREVIEW_MEDIA_ID      "TestVideoCapture"


inline QString getTestMediaServiceName(const QString& mediaID)
{
	return QString(TEST_MEDIA_SERVICE_NAME) + QString(".") + mediaID;
}

inline QString getTestMediaObjectPath(const QString& mediaID)
{
	return QString(TEST_MEDIA_SERVICE_OBJECT_PATH) + QString("/") + mediaID;
}


enum TestMediaState
{
	TestMedia_NoInitial,
	TestMedia_Running,
	TestMedia_Pause,
	TestMedia_Stop,
	TestMedia_Destroyed
};


enum TestMediaActionID
{
	TestMediaAction_InitialDeviceTest,
	TestMediaAction_InitialAudioTest,
	TestMediaAction_InitialVideoPreviewTest,
	TestMediaAction_ShowConsoleWindow,
};

class TestMediaInfo
{
public:

	///�����豸���ƻ������ļ��豸
	QString		audioIutputDevice;
	//��������豸����
	QString     audioOutputDevice;


	friend QDataStream& operator>>(QDataStream& in, TestMediaInfo& data)
	{
		in >> data.audioIutputDevice >> data.audioOutputDevice;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const TestMediaInfo& data)
	{
		out << data.audioIutputDevice << data.audioOutputDevice;
		return out;
	}
};

class VideoPreviewInfo
{
public:

	QString     videoCaptureDevice;
	QString		videoCrossbar;		        //crossbar Name������֧�ָ���
	QString		videoCrossbarType;          //crossbarInputType,����crossbar����������
	unsigned short	width;			        //��Ƶ֡�Ŀ�ȣ���λΪ����
	unsigned short	height;			        //��Ƶ֡�ĸ߶ȣ���λΪ����
	unsigned short	fps;			        //��Ƶ��֡�ʣ���λΪ֡/��
	int			preview_wnd;				//Ԥ���Ĳ��Ŵ���


	friend QDataStream& operator>>(QDataStream& in, VideoPreviewInfo& data)
	{
		in >> data.videoCaptureDevice >> data.videoCrossbar >> data.videoCrossbarType
			>> data.width >> data.height >> data.fps >> data.preview_wnd;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const VideoPreviewInfo& data)
	{
		out << data.videoCaptureDevice << data.videoCrossbar << data.videoCrossbarType
			<< data.width << data.height << data.fps << data.preview_wnd;

		return out;
	}
};



#endif