#ifndef LOGGER_H
#define LOGGER_H
//
#include <QTextCodec>
#include <QDebug>

class _log
{
public:
    static void log(QString m){
        #ifdef QT_DEBUG
        #ifdef Q_OS_LINUX
                QTextCodec* tc = QTextCodec::codecForName("UTF-8");
                qDebug() << tc->toUnicode(
                                m.toUtf8());
                return;
        #endif
                qDebug() << m;
        #endif
    }

    static void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "D: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtInfoMsg:
            fprintf(stderr, "I: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "W: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "C: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "F: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            abort();
        }
    }

};

#endif //LOGGER_H
