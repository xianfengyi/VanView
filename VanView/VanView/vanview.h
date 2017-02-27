#ifndef VANVIEW_H
#define VANVIEW_H

#include <QtWidgets/QMainWindow>
#include "ui_vanview.h"

class TestMedia;
class VanPlayer;
class VanView : public QMainWindow
{
	Q_OBJECT

public:
	VanView(QWidget *parent = 0);
	~VanView();

	void ReadDevices();

Q_SIGNALS:
	void serverTestSignal();

	private slots:
	//Video
	void preViewWebcamChangedSlot(const QString& webcamName);
	void preViewCrossbarChangedSlot(const QString& crossbar);
	void videoPreViewSlot();

	//Audio
	void startAudioInputTest();
	void stopAudioInputTest();
	void startAudioOutputTest();
	void stopAudioOutputTest();


    void playVideoFile();

private:
	Ui::VanViewClass ui;
	TestMedia *testmedia_instance;
	bool isVideoPreview;


    VanPlayer* player;
    
	
};

#endif // VANVIEW_H
