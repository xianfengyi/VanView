/********************************************************************************
** Form generated from reading UI file 'vanview.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VANVIEW_H
#define UI_VANVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VanViewClass
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox_2;
    QFrame *previewFrame;
    QPushButton *videoPreViewButton;
    QPushButton *playButton;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_28;
    QComboBox *crossbarclassTestcomboBox;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_27;
    QComboBox *crossbarlistTestcomboBox;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_29;
    QComboBox *videosizeTestcomboBox;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_26;
    QComboBox *webcamDeviceTestComboBox;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_13;
    QComboBox *audioOutputTestBox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *audioOutputTestStartButton;
    QPushButton *audioOutputTestStopButton;
    QGroupBox *frame_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_14;
    QComboBox *audioInputTest_InputDeviceBox;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_16;
    QComboBox *audioInputTest_OutputDeviceBox;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *audioInputTestStartButton;
    QPushButton *audioInputTestStopButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VanViewClass)
    {
        if (VanViewClass->objectName().isEmpty())
            VanViewClass->setObjectName(QStringLiteral("VanViewClass"));
        VanViewClass->resize(786, 752);
        centralWidget = new QWidget(VanViewClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 50, 761, 521));
        groupBox_2->setAlignment(Qt::AlignCenter);
        previewFrame = new QFrame(groupBox_2);
        previewFrame->setObjectName(QStringLiteral("previewFrame"));
        previewFrame->setGeometry(QRect(10, 20, 741, 461));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(previewFrame->sizePolicy().hasHeightForWidth());
        previewFrame->setSizePolicy(sizePolicy);
        previewFrame->setMinimumSize(QSize(171, 101));
        previewFrame->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        previewFrame->setFrameShape(QFrame::StyledPanel);
        previewFrame->setFrameShadow(QFrame::Raised);
        videoPreViewButton = new QPushButton(groupBox_2);
        videoPreViewButton->setObjectName(QStringLiteral("videoPreViewButton"));
        videoPreViewButton->setGeometry(QRect(50, 490, 111, 23));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(videoPreViewButton->sizePolicy().hasHeightForWidth());
        videoPreViewButton->setSizePolicy(sizePolicy1);
        playButton = new QPushButton(groupBox_2);
        playButton->setObjectName(QStringLiteral("playButton"));
        playButton->setGeometry(QRect(480, 490, 75, 23));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(620, 10, 149, 22));
        horizontalLayout_11 = new QHBoxLayout(layoutWidget);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        label_28 = new QLabel(layoutWidget);
        label_28->setObjectName(QStringLiteral("label_28"));

        horizontalLayout_11->addWidget(label_28);

        crossbarclassTestcomboBox = new QComboBox(layoutWidget);
        crossbarclassTestcomboBox->setObjectName(QStringLiteral("crossbarclassTestcomboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(crossbarclassTestcomboBox->sizePolicy().hasHeightForWidth());
        crossbarclassTestcomboBox->setSizePolicy(sizePolicy2);

        horizontalLayout_11->addWidget(crossbarclassTestcomboBox);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(430, 10, 171, 22));
        horizontalLayout_10 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_27 = new QLabel(layoutWidget1);
        label_27->setObjectName(QStringLiteral("label_27"));

        horizontalLayout_10->addWidget(label_27);

        crossbarlistTestcomboBox = new QComboBox(layoutWidget1);
        crossbarlistTestcomboBox->setObjectName(QStringLiteral("crossbarlistTestcomboBox"));
        sizePolicy2.setHeightForWidth(crossbarlistTestcomboBox->sizePolicy().hasHeightForWidth());
        crossbarlistTestcomboBox->setSizePolicy(sizePolicy2);

        horizontalLayout_10->addWidget(crossbarlistTestcomboBox);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(250, 10, 171, 22));
        horizontalLayout_12 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        label_29 = new QLabel(layoutWidget2);
        label_29->setObjectName(QStringLiteral("label_29"));

        horizontalLayout_12->addWidget(label_29);

        videosizeTestcomboBox = new QComboBox(layoutWidget2);
        videosizeTestcomboBox->setObjectName(QStringLiteral("videosizeTestcomboBox"));
        sizePolicy2.setHeightForWidth(videosizeTestcomboBox->sizePolicy().hasHeightForWidth());
        videosizeTestcomboBox->setSizePolicy(sizePolicy2);

        horizontalLayout_12->addWidget(videosizeTestcomboBox);

        layoutWidget3 = new QWidget(centralWidget);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(20, 10, 221, 22));
        horizontalLayout_9 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_26 = new QLabel(layoutWidget3);
        label_26->setObjectName(QStringLiteral("label_26"));

        horizontalLayout_9->addWidget(label_26);

        webcamDeviceTestComboBox = new QComboBox(layoutWidget3);
        webcamDeviceTestComboBox->setObjectName(QStringLiteral("webcamDeviceTestComboBox"));
        sizePolicy2.setHeightForWidth(webcamDeviceTestComboBox->sizePolicy().hasHeightForWidth());
        webcamDeviceTestComboBox->setSizePolicy(sizePolicy2);

        horizontalLayout_9->addWidget(webcamDeviceTestComboBox);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 580, 331, 121));
        groupBox->setAlignment(Qt::AlignCenter);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_3->addWidget(label_13);

        audioOutputTestBox = new QComboBox(groupBox);
        audioOutputTestBox->setObjectName(QStringLiteral("audioOutputTestBox"));
        sizePolicy2.setHeightForWidth(audioOutputTestBox->sizePolicy().hasHeightForWidth());
        audioOutputTestBox->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(audioOutputTestBox);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        audioOutputTestStartButton = new QPushButton(groupBox);
        audioOutputTestStartButton->setObjectName(QStringLiteral("audioOutputTestStartButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(audioOutputTestStartButton->sizePolicy().hasHeightForWidth());
        audioOutputTestStartButton->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(audioOutputTestStartButton);

        audioOutputTestStopButton = new QPushButton(groupBox);
        audioOutputTestStopButton->setObjectName(QStringLiteral("audioOutputTestStopButton"));
        sizePolicy3.setHeightForWidth(audioOutputTestStopButton->sizePolicy().hasHeightForWidth());
        audioOutputTestStopButton->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(audioOutputTestStopButton);


        verticalLayout_2->addLayout(horizontalLayout_4);

        frame_3 = new QGroupBox(centralWidget);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(440, 580, 331, 121));
        frame_3->setAlignment(Qt::AlignCenter);
        verticalLayout_3 = new QVBoxLayout(frame_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_14 = new QLabel(frame_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_6->addWidget(label_14);

        audioInputTest_InputDeviceBox = new QComboBox(frame_3);
        audioInputTest_InputDeviceBox->setObjectName(QStringLiteral("audioInputTest_InputDeviceBox"));
        sizePolicy2.setHeightForWidth(audioInputTest_InputDeviceBox->sizePolicy().hasHeightForWidth());
        audioInputTest_InputDeviceBox->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(audioInputTest_InputDeviceBox);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_16 = new QLabel(frame_3);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_7->addWidget(label_16);

        audioInputTest_OutputDeviceBox = new QComboBox(frame_3);
        audioInputTest_OutputDeviceBox->setObjectName(QStringLiteral("audioInputTest_OutputDeviceBox"));
        sizePolicy2.setHeightForWidth(audioInputTest_OutputDeviceBox->sizePolicy().hasHeightForWidth());
        audioInputTest_OutputDeviceBox->setSizePolicy(sizePolicy2);

        horizontalLayout_7->addWidget(audioInputTest_OutputDeviceBox);


        verticalLayout_3->addLayout(horizontalLayout_7);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        audioInputTestStartButton = new QPushButton(frame_3);
        audioInputTestStartButton->setObjectName(QStringLiteral("audioInputTestStartButton"));
        sizePolicy3.setHeightForWidth(audioInputTestStartButton->sizePolicy().hasHeightForWidth());
        audioInputTestStartButton->setSizePolicy(sizePolicy3);

        horizontalLayout_5->addWidget(audioInputTestStartButton);

        audioInputTestStopButton = new QPushButton(frame_3);
        audioInputTestStopButton->setObjectName(QStringLiteral("audioInputTestStopButton"));
        sizePolicy3.setHeightForWidth(audioInputTestStopButton->sizePolicy().hasHeightForWidth());
        audioInputTestStopButton->setSizePolicy(sizePolicy3);

        horizontalLayout_5->addWidget(audioInputTestStopButton);


        verticalLayout_3->addLayout(horizontalLayout_5);

        VanViewClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VanViewClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 786, 23));
        VanViewClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VanViewClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VanViewClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VanViewClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VanViewClass->setStatusBar(statusBar);

        retranslateUi(VanViewClass);

        QMetaObject::connectSlotsByName(VanViewClass);
    } // setupUi

    void retranslateUi(QMainWindow *VanViewClass)
    {
        VanViewClass->setWindowTitle(QApplication::translate("VanViewClass", "VanView", 0));
        groupBox_2->setTitle(QApplication::translate("VanViewClass", "\350\247\206\351\242\221\351\242\204\350\247\210", 0));
        videoPreViewButton->setText(QApplication::translate("VanViewClass", "\351\242\204\350\247\210", 0));
        playButton->setText(QApplication::translate("VanViewClass", "\346\222\255\346\224\276", 0));
        label_28->setText(QApplication::translate("VanViewClass", "\351\253\230\346\270\205\350\276\223\345\205\245\347\261\273\345\236\213", 0));
        label_27->setText(QApplication::translate("VanViewClass", "\351\253\230\346\270\205\350\276\223\345\205\245\350\256\276\345\244\207", 0));
        label_29->setText(QApplication::translate("VanViewClass", "\350\247\206\351\242\221\345\233\276\345\203\217\345\244\247\345\260\217", 0));
        label_26->setText(QApplication::translate("VanViewClass", "\350\247\206\351\242\221\351\200\211\346\213\251\350\256\276\345\244\207", 0));
        groupBox->setTitle(QApplication::translate("VanViewClass", "\345\243\260\351\237\263\350\276\223\345\207\272\350\256\276\345\244\207\346\265\213\350\257\225", 0));
        label_13->setText(QApplication::translate("VanViewClass", "\350\276\223\345\207\272\350\256\276\345\244\207", 0));
        audioOutputTestStartButton->setText(QApplication::translate("VanViewClass", "\345\274\200\345\247\213\346\265\213\350\257\225", 0));
        audioOutputTestStopButton->setText(QApplication::translate("VanViewClass", "\345\201\234\346\255\242", 0));
        frame_3->setTitle(QApplication::translate("VanViewClass", "\345\243\260\351\237\263\350\276\223\345\205\245\350\256\276\345\244\207\346\265\213\350\257\225", 0));
        label_14->setText(QApplication::translate("VanViewClass", "\350\276\223\345\205\245\350\256\276\345\244\207", 0));
        label_16->setText(QApplication::translate("VanViewClass", "\350\276\223\345\207\272\350\256\276\345\244\207", 0));
        audioInputTestStartButton->setText(QApplication::translate("VanViewClass", "\345\274\200\345\247\213\346\265\213\350\257\225", 0));
        audioInputTestStopButton->setText(QApplication::translate("VanViewClass", "\345\201\234\346\255\242", 0));
    } // retranslateUi

};

namespace Ui {
    class VanViewClass: public Ui_VanViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VANVIEW_H
