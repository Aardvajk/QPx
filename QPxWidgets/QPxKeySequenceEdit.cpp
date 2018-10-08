#include "QPxKeySequenceEdit.h"

#include <QtCore/QVector>
#include <QtCore/QMap>

#include <QtGui/QKeyEvent>

#include <pcx/join_str.h>

namespace
{

class Cache
{
public:
    Cache() : lock(false) { }

    QVector<QKeySequence> seqs;
    bool lock;
};

QKeySequence keyEventToKeySequence(QKeyEvent *event)
{
    QVector<Qt::Key> ignores = { Qt::Key_unknown, Qt::Key_Control, Qt::Key_Shift, Qt::Key_Alt, Qt::Key_Meta, Qt::Key_Menu };
    QMap<Qt::KeyboardModifier, Qt::Modifier> modifiers = { { Qt::ShiftModifier, Qt::SHIFT }, { Qt::ControlModifier, Qt::CTRL }, { Qt::AltModifier, Qt::ALT }, { Qt::MetaModifier, Qt::META } };

    int key = event->key();

    if(ignores.contains(static_cast<Qt::Key>(key)))
    {
        return QKeySequence();
    }

    foreach(Qt::KeyboardModifier mod, modifiers.keys())
    {
        if(event->modifiers() & mod) key += modifiers[mod];
    }

    return QKeySequence(key);
}

QKeySequence sequenceListToKeySequence(const QVector<QKeySequence> &seqs)
{
    return QKeySequence(pcx::join_str(seqs, ", ", [](const QKeySequence &s){ return s.toString(QKeySequence::NativeText).toStdString(); }).c_str());
}

}

QPx::KeySequenceEdit::KeySequenceEdit(QWidget *parent) : QLineEdit(parent)
{
    cache.alloc<Cache>();
    setClearButtonEnabled(true);

    connect(this, SIGNAL(textChanged(QString)), SLOT(internalTextChanged(QString)));
}

QKeySequence QPx::KeySequenceEdit::KeySequence() const
{
    return sequenceListToKeySequence(cache.get<Cache>().seqs);
}

void QPx::KeySequenceEdit::setKeySequence(const QKeySequence &sequence)
{
    auto &c = cache.get<Cache>();

    if(sequenceListToKeySequence(c.seqs) != sequence)
    {
        c.seqs.clear();
        for(int i = 0; i < sequence.count(); ++i)
        {
            c.seqs.append(QKeySequence(sequence[i]));
        }
    
        c.lock = true;
        setText(sequenceListToKeySequence(c.seqs).toString(QKeySequence::NativeText));
        c.lock = false;
    
        emit keySequenceChanged(sequence);
    }
}

void QPx::KeySequenceEdit::keyPressEvent(QKeyEvent *event)
{
    auto &c = cache.get<Cache>();

    if(isEnabled())
    {
        auto s = keyEventToKeySequence(event);
        if(!s.isEmpty())
        {
            if(!c.seqs.contains(s))
            {
                c.seqs.append(s);

                c.lock = true;
                setText(sequenceListToKeySequence(c.seqs).toString(QKeySequence::NativeText));
                c.lock = false;

                emit keySequenceChanged(QKeySequence(sequenceListToKeySequence(c.seqs)));
            }
        }
    }
}

void QPx::KeySequenceEdit::internalTextChanged(const QString &text)
{
    auto &c = cache.get<Cache>();

    if(!c.lock && text.isEmpty())
    {
        c.seqs.clear();
        emit keySequenceChanged(QKeySequence());
    }
}

