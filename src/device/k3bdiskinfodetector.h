/*
 *
 * $Id$
 * Copyright (C) 2003 Sebastian Trueg <trueg@k3b.org>
 *
 * This file is part of the K3b project.
 * Copyright (C) 1998-2003 Sebastian Trueg <trueg@k3b.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * See the file "COPYING" for the exact licensing terms.
 */



#ifndef K3BDISKINFO_DETECTOR_H
#define K3BDISKINFO_DETECTOR_H

#include <qobject.h>
#include <qthread.h>
#include <qwaitcondition.h>

typedef Q_INT32 size32;

#include "k3bdiskinfo.h"
#include "../rip/k3btcwrapper.h"


namespace K3bCdDevice
{

  class DiskInfoDetector : public QObject, public QThread
  {
    Q_OBJECT

  public:
    DiskInfoDetector( QObject* parent = 0 );
    ~DiskInfoDetector();
    virtual void run();

  public slots:
    void detect( CdDevice* dev );
    /**
     * no diskInfoReady signal will be emitted
     */
    void finish(bool success);

  signals:
    void diskInfoReady( const K3bCdDevice::DiskInfo& info );

  private slots:
    void fetchDiskInfo();
    void fetchTocInfo();
    void fetchIsoInfo();
    void calculateDiscId();
    void testForVideoDvd();
    void slotIsVideoDvd( bool dvd );

  private:
    CdDevice* m_device;
    DiskInfo m_info;
    K3bTcWrapper* m_tcWrapper;
    int m_cdfd;
    QWaitCondition ready;
  };
};

typedef K3bCdDevice::DiskInfoDetector K3bDiskInfoDetector;

#endif
