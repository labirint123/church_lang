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
    }

protected:
    void keyPressEvent(QKeyEvent *e) override
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

    void insertFromMimeData(const QMimeData *source) override
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

private:
    int m_max;
};

void CalculateName(LimitedTextEdit *numEdit,
                   LimitedTextEdit *limEdit,
                   LimitedTextEdit *cvvEdit,
                   QLabel *Out)
{
    QString numStr = numEdit->toPlainText();
    QString limStr = limEdit->toPlainText();
    QString cvvStr = cvvEdit->toPlainText();

    // …ваша логика подсчёта product…
    long long int seed = 1;
    auto multiplyDigits = [&](const QString &s)
    {
        for (QChar ch : s)
        {
            if (ch.isDigit())
            {
                seed *= static_cast<long long int>(ch.digitValue());
            }
        }
    };
    multiplyDigits(numStr);
    multiplyDigits(limStr);
    multiplyDigits(cvvStr);

    QString namesContent;
    {
        QFile f("Names.txt");
        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&f);
            in.setCodec("UTF-8");
            namesContent = in.readAll();
        }
        else
        {
            qWarning() << "Не удалось открыть Names.txt";
        }
    }

    QString secNamesContent;
    {
        QFile f("secNames.txt");
        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&f);
            in.setCodec("UTF-8");
            secNamesContent = in.readAll();
        }
        else
        {
            qWarning() << "Не удалось открыть secNames.txt";
        }
    }

    QStringList names = namesContent.split('\n', Qt::SkipEmptyParts);
    QStringList secondnames = secNamesContent.split('\n', Qt::SkipEmptyParts);

    qsrand(seed);

    QString outText;
    outText += names[qrand() % names.size()];
    outText += " ";
    outText += secondnames[qrand() % secondnames.size()];

    Out->setText(outText);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Russia));

    QWidget window;
    window.setFixedSize(650, 800);
    window.setStyleSheet("background-color: yellow;");

    QPixmap pix("church.png");
    if (pix.isNull())
        qWarning() << "Не удалось загрузить church.png из" << QDir::currentPath();

    QLabel *imageLabel = new QLabel;
    imageLabel->setPixmap(pix);
    imageLabel->setScaledContents(true);
    imageLabel->setFixedSize(322, 470);

    QLabel *topLabel = new QLabel("Твоё\nимя на\nязыке\nцеркви");
    QFont big = topLabel->font();
    big.setPointSize(64);
    topLabel->setFont(big);

    QVBoxLayout *mainL = new QVBoxLayout(&window);
    QHBoxLayout *topL = new QHBoxLayout;
    QGridLayout *bottomL = new QGridLayout;
    QHBoxLayout *Bottt = new QHBoxLayout;

    mainL->addLayout(topL);
    mainL->addLayout(bottomL);
    mainL->setAlignment(bottomL, Qt::AlignLeft | Qt::AlignTop);
    mainL->addLayout(Bottt);

    topL->addWidget(imageLabel);
    topL->addWidget(topLabel);

    bottomL->setContentsMargins(0, 0, 0, 0);
    bottomL->setHorizontalSpacing(8);
    bottomL->setVerticalSpacing(8);
    bottomL->setColumnStretch(3, 1);

    QLabel *lNum = new QLabel("Номер карты");
    LimitedTextEdit *eNum = new LimitedTextEdit(16);

    QLabel *lExp = new QLabel("Действительна до");
    LimitedTextEdit *eMonth = new LimitedTextEdit(2);
    QLabel *slash = new QLabel("/");
    LimitedTextEdit *eYear = new LimitedTextEdit(2);

    QLabel *lCVC = new QLabel("CVC2/CVV2");
    LimitedTextEdit *eCVC = new LimitedTextEdit(3);

    eNum->setFixedHeight(25);
    eMonth->setFixedHeight(25);
    slash->setFixedHeight(25);
    eYear->setFixedHeight(25);
    eCVC->setFixedHeight(25);

    bottomL->addWidget(lNum, 0, 0);
    bottomL->addWidget(eNum, 0, 1, 1, 3);

    bottomL->addWidget(lExp, 1, 0);
    bottomL->addWidget(eMonth, 1, 1);
    bottomL->addWidget(slash, 1, 2, Qt::AlignCenter);
    bottomL->addWidget(eYear, 1, 3);

    bottomL->addWidget(lCVC, 3, 0);
    bottomL->addWidget(eCVC, 3, 1, 1, 3);

    QPushButton *Enter = new QPushButton("Ввод");
    QLabel *Out = new QLabel("Вывод");
    QFont t = Out->font();
    t.setPointSize(24);
    t.setBold(1);
    Out->setFont(t);
    Bottt->addWidget(Enter);
    Bottt->addWidget(Out);

    QObject::connect(Enter, &QPushButton::clicked, [&]()
                     { CalculateName(eNum, eMonth, eCVC, Out); });

    window.show();
    return app.exec();
}
