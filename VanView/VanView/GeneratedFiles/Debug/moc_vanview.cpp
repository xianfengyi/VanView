/****************************************************************************
** Meta object code from reading C++ file 'vanview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vanview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vanview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VanView_t {
    QByteArrayData data[13];
    char stringdata[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VanView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VanView_t qt_meta_stringdata_VanView = {
    {
QT_MOC_LITERAL(0, 0, 7), // "VanView"
QT_MOC_LITERAL(1, 8, 16), // "serverTestSignal"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 24), // "preViewWebcamChangedSlot"
QT_MOC_LITERAL(4, 51, 10), // "webcamName"
QT_MOC_LITERAL(5, 62, 26), // "preViewCrossbarChangedSlot"
QT_MOC_LITERAL(6, 89, 8), // "crossbar"
QT_MOC_LITERAL(7, 98, 16), // "videoPreViewSlot"
QT_MOC_LITERAL(8, 115, 19), // "startAudioInputTest"
QT_MOC_LITERAL(9, 135, 18), // "stopAudioInputTest"
QT_MOC_LITERAL(10, 154, 20), // "startAudioOutputTest"
QT_MOC_LITERAL(11, 175, 19), // "stopAudioOutputTest"
QT_MOC_LITERAL(12, 195, 13) // "playVideoFile"

    },
    "VanView\0serverTestSignal\0\0"
    "preViewWebcamChangedSlot\0webcamName\0"
    "preViewCrossbarChangedSlot\0crossbar\0"
    "videoPreViewSlot\0startAudioInputTest\0"
    "stopAudioInputTest\0startAudioOutputTest\0"
    "stopAudioOutputTest\0playVideoFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VanView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   60,    2, 0x08 /* Private */,
       5,    1,   63,    2, 0x08 /* Private */,
       7,    0,   66,    2, 0x08 /* Private */,
       8,    0,   67,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VanView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VanView *_t = static_cast<VanView *>(_o);
        switch (_id) {
        case 0: _t->serverTestSignal(); break;
        case 1: _t->preViewWebcamChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->preViewCrossbarChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->videoPreViewSlot(); break;
        case 4: _t->startAudioInputTest(); break;
        case 5: _t->stopAudioInputTest(); break;
        case 6: _t->startAudioOutputTest(); break;
        case 7: _t->stopAudioOutputTest(); break;
        case 8: _t->playVideoFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VanView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VanView::serverTestSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject VanView::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_VanView.data,
      qt_meta_data_VanView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VanView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VanView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VanView.stringdata))
        return static_cast<void*>(const_cast< VanView*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int VanView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void VanView::serverTestSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
