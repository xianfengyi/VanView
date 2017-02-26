#include <string>
#include <vector>
using std::vector;
using std::string;

#include "vanview.h"
#include "DeviceManager/DeviceManager.h"
#include "msdx/config.h"
#include "VanViewComman.h"
#include "TestMediaComman.h"

#include "TestMedia.h"
#include <msdx/ExportUtil.h>
#include <Windows.h>

VanView::VanView(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ReadDevices();

	testmedia_instance = NULL;

	isVideoPreview = false;

	//Video
	connect(ui.webcamDeviceTestComboBox, SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(preViewWebcamChangedSlot(const QString&)));
	connect(ui.crossbarlistTestcomboBox, SIGNAL(currentIndexChanged(const QString&)),
		this, SLOT(preViewCrossbarChangedSlot(const QString&)));
	connect(ui.videoPreViewButton, SIGNAL(clicked()), this, SLOT(videoPreViewSlot()));

	//Audio
	connect(ui.audioInputTestStartButton, SIGNAL(clicked()), this, SLOT(startAudioInputTest()));
	connect(ui.audioOutputTestStartButton, SIGNAL(clicked()), this, SLOT(startAudioOutputTest()));
	connect(ui.audioInputTestStopButton, SIGNAL(clicked()), this, SLOT(stopAudioInputTest()));
	connect(ui.audioOutputTestStopButton, SIGNAL(clicked()), this, SLOT(stopAudioOutputTest()));
}

VanView::~VanView()
{
	SAFE_DELETE(testmedia_instance);
}


void VanView::ReadDevices()
{
	QStringList devices;

	//video
	ui.webcamDeviceTestComboBox->clear();
	{
		VCapDeviceList webcamList;
		DeviceManager::GetInstatnce()->GetVideoCaptureDevices(&webcamList);
		for (VCapDeviceList::iterator it = webcamList.begin(); it != webcamList.end(); ++it)
		{
			QVariant data(QString::fromStdString(it->devicePath));
			ui.webcamDeviceTestComboBox->addItem(QString::fromStdString(it->friendlyName), data);
		}
	}
	//当前设备初始化分辨率
	vector<std::string> videoSizeList;
	DeviceManager::GetInstatnce()->GetVideoDeviceMediaType(ui.webcamDeviceTestComboBox->currentText().toLocal8Bit().constData(), &videoSizeList);
	QStringList itemList;
	TransformStringVectorToQStringList(&videoSizeList, &itemList);
	ui.videosizeTestcomboBox->addItems(itemList);

	//Audio
	vector<std::string> audioInputList;
	DeviceManager::GetInstatnce()->GetAudioCaptureDevices(&audioInputList);

	ui.audioInputTest_InputDeviceBox->clear();
	TransformStringVectorToQStringList(&audioInputList, &devices);
	ui.audioInputTest_InputDeviceBox->addItems(devices);

	vector<std::string> audioOutputList;
	DeviceManager::GetInstatnce()->GetAudioOutputDevices(&audioOutputList);

	ui.audioInputTest_OutputDeviceBox->clear();
	TransformStringVectorToQStringList(&audioOutputList, &devices);
	ui.audioInputTest_OutputDeviceBox->addItems(devices);

	ui.audioOutputTestBox->clear();
	TransformStringVectorToQStringList(&audioOutputList, &devices);
	ui.audioOutputTestBox->addItems(devices);

	TransformStringVectorToQStringList(&audioOutputList, &devices);

}

void VanView::preViewWebcamChangedSlot(const QString& webcamName)
{
	//设备属性
	ui.videosizeTestcomboBox->clear();
	vector<std::string> videoSizeList;
	DeviceManager::GetInstatnce()->GetVideoDeviceMediaType(webcamName.toLocal8Bit().constData(), &videoSizeList);
	QStringList itemList;
	TransformStringVectorToQStringList(&videoSizeList, &itemList);
	ui.videosizeTestcomboBox->addItems(itemList);

	if (webcamName.contains("webcam", Qt::CaseInsensitive)
		|| webcamName.contains("e2eSoft", Qt::CaseInsensitive))
	{
		//如果是网络摄像头则清空采集卡配置
		ui.crossbarlistTestcomboBox->clear();
		ui.crossbarclassTestcomboBox->clear();
	}
	else
	{
		//高清采集卡
		ui.crossbarlistTestcomboBox->clear();
		vector<std::string> crossbarList;
		DeviceManager::GetInstatnce()->GetCrossbarDeviceList(webcamName.toLocal8Bit().constData(), &crossbarList);
		ui.crossbarlistTestcomboBox->addItem(QString::fromLocal8Bit("不使用"), QVariant(QString("")));
		ui.crossbarlistTestcomboBox->addItem(QString::fromLocal8Bit("自动选择"),
			QVariant(QString::fromLocal8Bit(MSDX_CONF_CROSSBAR_AUTO)));
		for (int i = 0; i < crossbarList.size(); i += 2)
		{
			//显示friendlyName
			QString item_text = QString::fromLocal8Bit(crossbarList[i].c_str());
			//内部保留displayName
			QString item_data = QString::fromLocal8Bit(crossbarList[i + 1].c_str());
			ui.crossbarlistTestcomboBox->addItem(item_text, QVariant(item_data));
		}
	}
}

void VanView::preViewCrossbarChangedSlot(const QString& crossbar)
{
	ui.crossbarclassTestcomboBox->clear();
	int index = ui.crossbarlistTestcomboBox->currentIndex();
	if (index <= 0)
	{
		//0为不使用
		return;
	}
	vector<std::string> crossbarType;
	QString crossbar_displayname = ui.crossbarlistTestcomboBox->itemData(index).toString();
	DeviceManager::GetInstatnce()->GetCrossbarInputType(crossbar_displayname.toLocal8Bit().constData(), &crossbarType);
	QStringList itemList;
	TransformStringVectorToQStringList(&crossbarType, &itemList);
	ui.crossbarclassTestcomboBox->addItems(itemList);
}

void VanView::videoPreViewSlot()
{
	testmedia_instance = new TestMedia(VIDEO_PREVIEW_MEDIA_ID);
	if (!isVideoPreview)
	{
		VideoPreviewInfo videoInfo;
		int select_index = -1;
		//cam
		select_index = ui.webcamDeviceTestComboBox->currentIndex();
		if (select_index >= 0)
			videoInfo.videoCaptureDevice = ui.webcamDeviceTestComboBox->itemData(select_index).toString();

		//crossbar
		select_index = ui.crossbarlistTestcomboBox->currentIndex();
		if (select_index >= 0)
		{
			videoInfo.videoCrossbar = ui.crossbarlistTestcomboBox->itemData(select_index).toString();
		}
		videoInfo.videoCrossbarType = ui.crossbarclassTestcomboBox->currentText();

		//视频宽高
		QStringList sizes = ui.videosizeTestcomboBox->currentText().split("X");
		if (sizes.size() >= 2)
		{
			videoInfo.width = sizes.at(0).toUInt();
			videoInfo.height = sizes.at(1).toUInt();
			videoInfo.fps = 30;
			if (sizes.size() == 3)
				videoInfo.fps = sizes.at(2).toUInt();
		}
		else
		{
			videoInfo.width = 0;
			videoInfo.height = 0;
			videoInfo.fps = 0;
		}

		videoInfo.preview_wnd = (int)ui.previewFrame->winId();

		ui.videoPreViewButton->setText(QString::fromLocal8Bit("停止预览"));
		isVideoPreview = !isVideoPreview;

		QByteArray output_bytes;
		QDataStream out(&output_bytes, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_4);
		out << videoInfo;

		testmedia_instance->SetMediaInfo(2, output_bytes);
	}
	else
	{
		testmedia_instance->Destroy();
		ui.videoPreViewButton->setText(QString::fromLocal8Bit("预览"));
		isVideoPreview = !isVideoPreview;
	}

}

void VanView::startAudioInputTest()
{
	testmedia_instance = new TestMedia(AUDIODINPUT_TEST_MEDIA_ID);
	QString outPutDevice = ui.audioInputTest_OutputDeviceBox->currentText();
	QString inputPutDevice = ui.audioInputTest_InputDeviceBox->currentText();
	if (outPutDevice == "" || inputPutDevice == "")
		return;

	ui.audioInputTestStopButton->setEnabled(true);
	ui.audioInputTestStartButton->setEnabled(false);
	ui.audioOutputTestStartButton->setEnabled(false);
	ui.audioOutputTestStopButton->setEnabled(false);

	TestMediaInfo testInfo;

	testInfo.audioIutputDevice = inputPutDevice;
	testInfo.audioOutputDevice = outPutDevice;

	QByteArray output_bytes;
	QDataStream out(&output_bytes, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_4);
	out << testInfo;

	testmedia_instance->SetMediaInfo(TestMediaAction_InitialAudioTest, output_bytes);

}

void VanView::stopAudioInputTest()
{
	ui.audioInputTestStopButton->setEnabled(false);
	ui.audioInputTestStartButton->setEnabled(true);
	ui.audioOutputTestStartButton->setEnabled(true);
	ui.audioOutputTestStopButton->setEnabled(false);

	testmedia_instance->Destroy();
}

void VanView::startAudioOutputTest()
{
	testmedia_instance = new TestMedia(AUDIOOUTPUT_TEST_MEDIA_ID);

	QString outPutDevice = ui.audioOutputTestBox->currentText();
	if (outPutDevice == "")
		return;

	ui.audioInputTestStopButton->setEnabled(false);
	ui.audioInputTestStartButton->setEnabled(false);
	ui.audioOutputTestStartButton->setEnabled(false);
	ui.audioOutputTestStopButton->setEnabled(true);

	QString appDir = QApplication::applicationDirPath();
	appDir.replace("/", "\\");
	QString audioFilePath = appDir + "\\Resources\\AudioTest.mp3";

	TestMediaInfo testInfo;

	testInfo.audioIutputDevice = audioFilePath;
	testInfo.audioOutputDevice = outPutDevice;

	QByteArray output_bytes;
	QDataStream out(&output_bytes, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_4);
	out << testInfo;

	testmedia_instance->SetMediaInfo(TestMediaAction_InitialAudioTest, output_bytes);
}

void VanView::stopAudioOutputTest()
{
	ui.audioInputTestStopButton->setEnabled(false);
	ui.audioInputTestStartButton->setEnabled(true);
	ui.audioOutputTestStartButton->setEnabled(true);
	ui.audioOutputTestStopButton->setEnabled(false);

	testmedia_instance->Destroy();
}