/* 
 *
 * $Id$
 * Copyright (C) 2003 Christian Kvasny <chris@k3b.org>
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


#ifndef _K3B_VIDEOCD_RIPPING_DIALOG_H_
#define _K3B_VIDEOCD_RIPPING_DIALOG_H_

#include <k3binteractiondialog.h>

#include <qstringlist.h>

#include <device/k3bdiskinfo.h>

class KListView;
class QCheckBox;
class QLabel;
class QSpinBox;
class QComboBox;
class QToolButton;
class KURLRequester;
class K3bTempDirSelectionWidget;

class K3bVideoCdRippingDialog : public K3bInteractionDialog
{
  Q_OBJECT

 public: 
  K3bVideoCdRippingDialog( const long size, QWidget* parent = 0, const char* name = 0 );
  ~K3bVideoCdRippingDialog();


 private:
  void setupGui();
  void setupContextHelp();
  void setVideoCdSize( long s) { m_videocdsize = s; };

  K3bTempDirSelectionWidget* m_tempDirSelectionWidget;

  KURLRequester* m_editDirectory;

  QLabel* m_labelFreeSpace;
  QLabel* m_labelNecessarySize;
  QCheckBox* m_ignoreExt;
  QCheckBox* m_sector2336;
  QCheckBox* m_extractXML;

  long m_videocdsize;
  
 private slots:
  void slotStartClicked();

  void slotLoadK3bDefaults();
  void slotLoadUserDefaults();
  void slotSaveUserDefaults();

  void slotToggleAll();
};

#endif
