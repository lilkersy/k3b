/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWINDOWSPIPEWRITER_P_H
#define QWINDOWSPIPEWRITER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <qt_windows.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

#ifndef QT_NO_THREAD

#define SLEEPMIN 10
#define SLEEPMAX 500

class QIncrementalSleepTimer
{

public:
    QIncrementalSleepTimer(int msecs)
        : totalTimeOut(msecs)
        , nextSleep(qMin(SLEEPMIN, totalTimeOut))
    {
        if (totalTimeOut == -1)
            nextSleep = SLEEPMIN;
        timer.start();
    }

    int nextSleepTime()
    {
        int tmp = nextSleep;
        nextSleep = qMin(nextSleep * 2, qMin(SLEEPMAX, timeLeft()));
        return tmp;
    }

    int timeLeft() const
    {
        if (totalTimeOut == -1)
            return SLEEPMAX;
        return qMax(totalTimeOut - timer.elapsed(), 0);
    }

    bool hasTimedOut() const
    {
        if (totalTimeOut == -1)
            return false;
        return timer.elapsed() >= totalTimeOut;
    }

    void resetIncrements()
    {
        nextSleep = qMin(SLEEPMIN, timeLeft());
    }

private:
    QTime timer;
    int totalTimeOut;
    int nextSleep;
};

class Q_CORE_EXPORT QWindowsPipeWriter : public QThread
{
    Q_OBJECT

Q_SIGNALS:
    void canWrite();

public:
    QWindowsPipeWriter(HANDLE writePipe, QObject * parent = 0);
    ~QWindowsPipeWriter();

    bool waitForWrite(int msecs);
    qint64 write(const char *data, qint64 maxlen);

    qint64 bytesToWrite() const
    {
        QMutexLocker locker(&lock);
        return data.size();
    }

    bool hadWritten() const
    {
        return hasWritten;
    }

protected:
   void run();

private:
    QByteArray data;
    QWaitCondition waitCondition;
    mutable QMutex lock;
    HANDLE writePipe;
    volatile bool quitNow;
    bool hasWritten;
};

#endif //QT_NO_THREAD

QT_END_NAMESPACE

QT_END_HEADER

#endif // QT_NO_PROCESS
