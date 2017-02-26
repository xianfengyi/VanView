#ifndef _TESTMEDIA_H_
#define _TESTMEDIA_H_

#include <QtCore/QtCore>
#include "TestMediaComman.h"

typedef void*  HANDLE;

class TestMedia :public QObject
{
	Q_OBJECT
public:
	TestMedia(const QString mediaID);
	~TestMedia();

public:// PROPERTIES
	public Q_SLOTS : // METHODS
		void Destroy();
	QByteArray GetMediaInfo(int info_type);
	void Pause();
	void Run();
	void SetMediaInfo(int action_id, const QByteArray &input_garray);
	void Stop();
Q_SIGNALS: // SIGNALS
	void RunningStateChanged(const QString &media_id, int current_state);

private:

	int CreateAudioTestGraph(const TestMediaInfo& info);
	int CreateVideoTestGraph(const VideoPreviewInfo &info);


private:
	TestMediaState		_currentState;
	QString			_mediaID;

	///保留本示例的指针
	TestMedia*	_instance;
};

#endif // !_TESTMEDIA_H_