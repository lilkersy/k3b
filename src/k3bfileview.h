/***************************************************************************
                          k3bfileview.h  -  description
                             -------------------
    begin                : Sun Oct 28 2001
    copyright            : (C) 2001 by Sebastian Trueg
    email                : trueg@informatik.uni-freiburg.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef K3BFILEVIEW_H
#define K3BFILEVIEW_H

#include <qwidget.h>
#include <qvbox.h>
#include <kfiledetailview.h>

class KDirOperator;
class QDragObject;
class KURL;
class KFileFilter;
class KFileViewItem;
class KActionCollection;



/**
  *@author Sebastian Trueg
  */
class K3bFileView : public QVBox
{
  Q_OBJECT

 public: 
  K3bFileView(QWidget *parent=0, const char *name=0);
  ~K3bFileView();
  void setUrl(const KURL &url, bool forward);

  KActionCollection* actionCollection() const;

 signals:
  void urlEntered( const KURL& url );

 public slots:
  void slotAudioFilePlay();
  void slotAudioFileEnqueue();
  void slotAddFilesToProject();

 private:
  class PrivateFileView;
  KDirOperator *m_dirOp;
  KFileFilter* m_filterWidget;

  void setupGUI();

 private slots:
  void slotFilterChanged();
  void slotFileHighlighted( const KFileViewItem* item );
  void slotCheckActions();
};


class K3bFileView::PrivateFileView : public KFileDetailView
{
  Q_OBJECT

 public:
  PrivateFileView( QWidget* parent, const char* name );

 protected:
  QDragObject* dragObject() const;
};


#endif
