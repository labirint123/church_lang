#include "LimitedTextEdit.h"

void LimitedTextEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Home:
    case Qt::Key_End:
    case Qt::Key_Tab:
    case Qt::Key_Return:
    case Qt::Key_Enter:
        QTextEdit::keyPressEvent(e);
        return;
    default:
        break;
    }

    QString txt = e->text();
    if (txt.isEmpty())
    {
        QTextEdit::keyPressEvent(e);
        return;
    }

    QChar c = txt.at(0);
    if (!c.isDigit())
    {
        return;
    }

    int selLen = textCursor().selectedText().length();
    int curLen = toPlainText().length() - selLen;
    if (curLen + 1 <= m_max)
    {
        QTextEdit::keyPressEvent(e);
    }
}

void LimitedTextEdit::insertFromMimeData(const QMimeData *source)
{
    QString text = source->text();
    QString filtered;
    for (QChar c : text)
        if (c.isDigit())
            filtered.append(c);

    int selLen = textCursor().selectedText().length();
    int curLen = toPlainText().length() - selLen;
    int avail = m_max - curLen;
    if (avail <= 0)
        return;
    if (filtered.length() > avail)
        filtered.truncate(avail);

    QMimeData md;
    md.setText(filtered);
    QTextEdit::insertFromMimeData(&md);
}