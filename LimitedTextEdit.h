#pragma once
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QKeyEvent>
#include <QMimeData>
#include <QDir>
#include <QDebug>
#include <QPushButton>

class LimitedTextEdit : public QTextEdit
{
public:
    LimitedTextEdit(int maxChars, QWidget *parent = nullptr)
        : QTextEdit(parent), m_max(maxChars)
    {
        setAcceptRichText(false);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

protected:
    void keyPressEvent(QKeyEvent *e) override;

    void insertFromMimeData(const QMimeData *source) override;

private:
    int m_max;
};