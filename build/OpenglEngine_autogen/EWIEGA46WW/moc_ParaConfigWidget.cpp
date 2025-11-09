/****************************************************************************
** Meta object code from reading C++ file 'ParaConfigWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ParaConfigWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ParaConfigWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ParaConfigWidget_t {
    QByteArrayData data[7];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ParaConfigWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ParaConfigWidget_t qt_meta_stringdata_ParaConfigWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ParaConfigWidget"
QT_MOC_LITERAL(1, 17, 11), // "CheckBoxSig"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 7), // "checked"
QT_MOC_LITERAL(4, 38, 10), // "EngineType"
QT_MOC_LITERAL(5, 49, 4), // "type"
QT_MOC_LITERAL(6, 54, 12) // "CheckBoxSlot"

    },
    "ParaConfigWidget\0CheckBoxSig\0\0checked\0"
    "EngineType\0type\0CheckBoxSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParaConfigWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   29,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 4,    3,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void ParaConfigWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ParaConfigWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->CheckBoxSig((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< EngineType(*)>(_a[2]))); break;
        case 1: _t->CheckBoxSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ParaConfigWidget::*)(bool , EngineType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ParaConfigWidget::CheckBoxSig)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ParaConfigWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ParaConfigWidget.data,
    qt_meta_data_ParaConfigWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ParaConfigWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParaConfigWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ParaConfigWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ParaConfigWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ParaConfigWidget::CheckBoxSig(bool _t1, EngineType _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
