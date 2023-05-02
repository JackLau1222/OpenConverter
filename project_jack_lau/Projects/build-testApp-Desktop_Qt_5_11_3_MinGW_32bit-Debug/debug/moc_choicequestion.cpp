/****************************************************************************
** Meta object code from reading C++ file 'choicequestion.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../testApp/choicequestion.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'choicequestion.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_choiceQuestion_t {
    QByteArrayData data[11];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_choiceQuestion_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_choiceQuestion_t qt_meta_stringdata_choiceQuestion = {
    {
QT_MOC_LITERAL(0, 0, 14), // "choiceQuestion"
QT_MOC_LITERAL(1, 15, 6), // "chosed"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 5), // "index"
QT_MOC_LITERAL(4, 29, 12), // "choieClicked"
QT_MOC_LITERAL(5, 42, 11), // "setQuestion"
QT_MOC_LITERAL(6, 54, 8), // "Question"
QT_MOC_LITERAL(7, 63, 9), // "addChoice"
QT_MOC_LITERAL(8, 73, 6), // "Choice"
QT_MOC_LITERAL(9, 80, 6), // "setPix"
QT_MOC_LITERAL(10, 87, 8) // "filename"

    },
    "choiceQuestion\0chosed\0\0index\0choieClicked\0"
    "setQuestion\0Question\0addChoice\0Choice\0"
    "setPix\0filename"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_choiceQuestion[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   42,    2, 0x0a /* Public */,
       5,    1,   45,    2, 0x0a /* Public */,
       7,    1,   48,    2, 0x0a /* Public */,
       9,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void choiceQuestion::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        choiceQuestion *_t = static_cast<choiceQuestion *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->chosed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->choieClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setQuestion((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->addChoice((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setPix((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (choiceQuestion::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&choiceQuestion::chosed)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject choiceQuestion::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_choiceQuestion.data,
      qt_meta_data_choiceQuestion,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *choiceQuestion::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *choiceQuestion::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_choiceQuestion.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int choiceQuestion::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void choiceQuestion::chosed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
