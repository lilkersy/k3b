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

#include "k3baudiocdview.h"
#include "k3baudiorippingdialog.h"

#include <k3b.h>
#include <device/k3btoc.h>
#include <device/k3bdiskinfo.h>
#include <tools/k3blistview.h>
#include <cddb/k3bcddbresult.h>
#include <device/k3bmsf.h>
#include <cddb/k3bcddb.h>
#include <cddb/k3bcddbquery.h>
#include <k3btoolbox.h>
#include <kcutlabel.h>
#include <k3bstdguiitems.h>

#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kmessagebox.h>
#include <kconfig.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <kstandarddirs.h>
#include <kdialogbase.h>

#include <qlayout.h>
#include <qheader.h>
#include <qlabel.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qfont.h>


class K3bAudioCdView::AudioTrackViewItem : public QCheckListItem
{
public:
  AudioTrackViewItem( QListView* parent, 
		      QListViewItem* after,
		      int _trackNumber,
		      const K3b::Msf& length) 
    : QCheckListItem( parent,
		      after,
		      QString::null,
		      QCheckListItem::CheckBox ) {
    setText( 1, QString::number(_trackNumber).rightJustify( 2, ' ' ) );
    setText( 3, i18n("Track %1").arg(_trackNumber) );
    setText( 4, length.toString() );
    setText( 5, KIO::convertSize( length.audioBytes() ) );

    trackNumber = _trackNumber;
    setOn(true);
  }

  int trackNumber;

  void updateCddbData( const K3bCddbResultEntry& entry ) {
    setText( 2, entry.artists[trackNumber-1] );
    setText( 3, entry.titles[trackNumber-1] );
  }
};


K3bAudioCdView::K3bAudioCdView( QWidget* parent, const char *name )
  : K3bCdContentsView( parent, name )
{
  QGridLayout* mainGrid = new QGridLayout( this );
  mainGrid->setMargin( 2 );

  // header
  // ----------------------------------------------------------------------------------
  QFrame* headerFrame = K3bStdGuiItems::purpleFrame( this );
  QHBoxLayout* headerLayout = new QHBoxLayout( headerFrame ); 
  headerLayout->setMargin( 2 );  // to make sure the frame gets displayed
  QLabel* pixmapLabel1 = new QLabel( headerFrame, "pixmapLabel1" );
  pixmapLabel1->setPixmap( QPixmap(locate( "appdata", "pics/diskinfo_left.png" )) );
  pixmapLabel1->setScaledContents( FALSE );
  headerLayout->addWidget( pixmapLabel1 );

  m_labelTitle = new KCutLabel( headerFrame, "m_labelTitle" );
  m_labelTitle->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 0, m_labelTitle->sizePolicy().hasHeightForWidth() ) );
  m_labelTitle->setPaletteBackgroundColor( QColor( 205, 210, 255 ) );
  QFont m_labelTitle_font( m_labelTitle->font() );
  m_labelTitle_font.setPointSize( 12 );
  m_labelTitle_font.setBold( TRUE );
  m_labelTitle->setFont( m_labelTitle_font ); 
  headerLayout->addWidget( m_labelTitle );

  QLabel* pixmapLabel2 = new QLabel( headerFrame, "pixmapLabel2" );
  pixmapLabel2->setPixmap( QPixmap(locate( "appdata", "pics/diskinfo_audio.png" )) );
  pixmapLabel2->setScaledContents( FALSE );
  headerLayout->addWidget( pixmapLabel2 );


  // toolbox
  // ----------------------------------------------------------------------------------
  QHBoxLayout* toolBoxLayout = new QHBoxLayout( 0, 0, 0, "toolBoxLayout" );
  m_toolBox = new K3bToolBox( this );
  toolBoxLayout->addWidget( m_toolBox );
  QSpacerItem* spacer = new QSpacerItem( 10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
  toolBoxLayout->addItem( spacer );
  m_labelLength = new QLabel( this );
  m_labelLength->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
  toolBoxLayout->addWidget( m_labelLength );


  // the track view
  // ----------------------------------------------------------------------------------
  m_trackView = new K3bListView( this );
  m_trackView->setFullWidth(true);
  m_trackView->setSorting(-1);
  m_trackView->setAllColumnsShowFocus( true );
  m_trackView->setSelectionMode( QListView::Single );
  m_trackView->setDragEnabled( true );
  m_trackView->addColumn( "" );
  m_trackView->addColumn( "" );
  m_trackView->addColumn( i18n("Artist") );
  m_trackView->addColumn( i18n("Title") );
  m_trackView->addColumn( i18n("Length") );
  m_trackView->addColumn( i18n("Size") );

  m_trackView->header()->setClickEnabled(false);
  m_trackView->setColumnWidthMode( 0, QListView::Manual );
  m_trackView->setColumnWidth( 0, 20 );
  m_trackView->header()->setResizeEnabled( false,0 );

  m_trackView->setItemsRenameable(true);
  m_trackView->setRenameable(0, false);
  m_trackView->setRenameable(1, false);
  m_trackView->setRenameable(2, true);
  m_trackView->setRenameable(3, true);

  connect( m_trackView, SIGNAL(itemRenamed(QListViewItem*, const QString&, int)),
	   this, SLOT(slotItemRenamed(QListViewItem*, const QString&, int)) );
  connect( m_trackView, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
	   this, SLOT(slotContextMenu(KListView*, QListViewItem*, const QPoint&)) );
  connect( m_trackView, SIGNAL(selectionChanged(QListViewItem*)), 
	   this, SLOT(slotTrackSelectionChanged(QListViewItem*)) );

  mainGrid->addWidget( headerFrame, 0, 0 );
  mainGrid->addLayout( toolBoxLayout, 1, 0 );
  mainGrid->addWidget( m_trackView, 2, 0 );


  m_cddb = new K3bCddb( this );

  connect( m_cddb, SIGNAL(infoMessage(const QString&)),
	   k3bMain(), SLOT(showBusyInfo(const QString&)) );
  connect( m_cddb, SIGNAL(queryFinished(int)),
	   k3bMain(), SLOT(endBusy()) );
  connect( m_cddb, SIGNAL(queryFinished(int)),
	   this, SLOT(slotCddbQueryFinished(int)) );

  initActions();
  slotTrackSelectionChanged(0);
}


K3bAudioCdView::~K3bAudioCdView()
{
}


void K3bAudioCdView::setDisk( const K3bCdDevice::DiskInfo& info )
{
  m_diskInfo = info;
  reload();
}


void K3bAudioCdView::reload()
{
  m_trackView->clear();

  // initialize cddb info for editing
  m_cddbInfo = K3bCddbResultEntry();
  for( int i = 0; i < (int)m_diskInfo.toc.count(); ++i ) {
    m_cddbInfo.titles.append("");
    m_cddbInfo.artists.append("");
    m_cddbInfo.extInfos.append("");
  }

  // create a listviewItem for every audio track
  int index = 1;
  for( K3bToc::const_iterator it = m_diskInfo.toc.begin(); 
       it != m_diskInfo.toc.end(); ++it ) {

    // for now skip data tracks since we are not able to rip them to iso
    if( (*it).type() == K3bTrack::AUDIO ) {
      K3b::Msf length( (*it).length() );
      (void)new AudioTrackViewItem( m_trackView,
				    m_trackView->lastItem(),
				    index,
				    length );
    }

    index++;
  }

  updateDisplay();

  queryCddb();
}


void K3bAudioCdView::initActions()
{
  m_actionCollection = new KActionCollection( this );

  KAction* actionSelectAll = KStdAction::selectAll( this, SLOT(slotSelectAll()), 
						    m_actionCollection, "select_all" );
  KAction* actionDeselectAll = KStdAction::deselect( this, SLOT(slotDeselectAll()), 
						     m_actionCollection, "deselect_all" );
  KAction* actionSelect = new KAction( i18n("Select Track"), 0, 0, this,
				       SLOT(slotSelect()), actionCollection(),
				       "select_track" );
  KAction* actionDeselect = new KAction( i18n("Deselect Track"), 0, 0, this,
					 SLOT(slotDeselect()), actionCollection(),
					 "deselect_track" );
  KAction* actionEditTrackCddbInfo = new KAction( i18n("Edit cddb Info"), "edit", 0, this,
						  SLOT(slotEditTrackCddb()), actionCollection(),
						  "edit_track_cddb" );
  KAction* actionEditAlbumCddbInfo = new KAction( i18n("Edit Album cddb Info"), "edit", 0, this,
						  SLOT(slotEditAlbumCddb()), actionCollection(),
						  "edit_album_cddb" );

  KAction* actionStartRip = new KAction( i18n("Start Ripping"), "run", 0, this,
					 SLOT(startRip()), actionCollection(), "start_rip" );

  KAction* actionQueryCddb = new KAction( i18n("Query cddb"), "reload", 0, this,
					  SLOT(queryCddb()), actionCollection(), "query_cddb" );

  // TODO: set the actions tooltips and whatsthis infos

  // setup the popup menu
  m_popupMenu = new KActionMenu( actionCollection(), "popup_menu" );
  KAction* separator = new KActionSeparator( actionCollection(), "separator" );
  m_popupMenu->insert( actionSelect );
  m_popupMenu->insert( actionDeselect );
  m_popupMenu->insert( actionSelectAll );
  m_popupMenu->insert( actionDeselectAll );
  m_popupMenu->insert( separator );
  m_popupMenu->insert( actionEditTrackCddbInfo );
  m_popupMenu->insert( actionEditAlbumCddbInfo );
  m_popupMenu->insert( separator );
  m_popupMenu->insert( actionStartRip );

  // setup the toolbox
  m_toolBox->addButton( actionQueryCddb );
  m_toolBox->addButton( actionEditTrackCddbInfo );
  m_toolBox->addButton( actionEditAlbumCddbInfo );
  m_toolBox->addButton( actionStartRip );
}


void K3bAudioCdView::slotContextMenu( KListView*, QListViewItem*, const QPoint& p )
{
  m_popupMenu->popup(p);
}


void K3bAudioCdView::slotItemRenamed( QListViewItem* item, const QString& str, int col )
{
  AudioTrackViewItem* a = (AudioTrackViewItem*)item;
  if( col == 2 )
    m_cddbInfo.artists[a->trackNumber-1] = str;
  else if( col == 3 )
    m_cddbInfo.titles[a->trackNumber-1] = str;
  else if( col == 6 )
    m_cddbInfo.extInfos[a->trackNumber-1] = str;
}


void K3bAudioCdView::slotTrackSelectionChanged( QListViewItem* item )
{
  actionCollection()->action("edit_track_cddb")->setEnabled( item != 0 );
  actionCollection()->action("select_track")->setEnabled( item != 0 );
  actionCollection()->action("deselect_track")->setEnabled( item != 0 );
}


void K3bAudioCdView::startRip()
{
  QValueList<int> trackNumbers;
  for( QListViewItemIterator it( m_trackView ); it.current(); ++it ) {
    AudioTrackViewItem* a = (AudioTrackViewItem*)it.current();
    if( a->isOn() )
      trackNumbers.append( a->trackNumber );
  }

  if( trackNumbers.count() == 0 ) {
    KMessageBox::error( this, i18n("Please select the tracks to rip."),
			i18n("No Tracks Selected") );
  }
  else {
    K3bAudioRippingDialog rip( m_diskInfo, 
			       m_cddbInfo, 
			       trackNumbers, 
			       this );
    rip.exec();
  }
}


void K3bAudioCdView::slotEditTrackCddb()
{
  if( QListViewItem* item = m_trackView->selectedItem() ) {
    AudioTrackViewItem* a = (AudioTrackViewItem*)item;
    KDialogBase d( this, "trackCddbDialog", true, i18n("Cddb Track %1").arg(a->trackNumber),
		   KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, true);
    QWidget* w = new QWidget( &d );

    KLineEdit* editTitle = new KLineEdit( m_cddbInfo.titles[a->trackNumber-1], w );
    KLineEdit* editArtist = new KLineEdit( m_cddbInfo.artists[a->trackNumber-1], w );
    KLineEdit* editExtInfo = new KLineEdit( m_cddbInfo.extInfos[a->trackNumber-1], w );
    QFrame* line = new QFrame( w );
    line->setFrameShape( QFrame::HLine );
    line->setFrameShadow( QFrame::Sunken );

    QGridLayout* grid = new QGridLayout( w );
    grid->setSpacing( KDialog::spacingHint() );

    grid->addWidget( new QLabel( i18n("Title:"), w ), 0, 0 );
    grid->addWidget( editTitle, 0, 1 );
    grid->addMultiCellWidget( line, 1, 1, 0, 1 );
    grid->addWidget( new QLabel( i18n("Artist:"), w ), 2, 0 );
    grid->addWidget( editArtist, 2, 1 );
    grid->addWidget( new QLabel( i18n("Extra info:"), w ), 3, 0 );
    grid->addWidget( editExtInfo, 3, 1 );
    grid->setRowStretch( 4, 1 );

    d.setMainWidget(w);
    d.resize(260, d.sizeHint().height());

    if( d.exec() == QDialog::Accepted ) {
      m_cddbInfo.titles[a->trackNumber-1] = editTitle->text();
      m_cddbInfo.artists[a->trackNumber-1] = editArtist->text();
      m_cddbInfo.extInfos[a->trackNumber-1] = editExtInfo->text();
      a->updateCddbData( m_cddbInfo );
    }
  }
}


void K3bAudioCdView::slotEditAlbumCddb()
{
  KDialogBase d( this, "trackCddbDialog", true, i18n("Album Cddb"),
		 KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, true);
  QWidget* w = new QWidget( &d );

  KLineEdit* editTitle = new KLineEdit( m_cddbInfo.cdTitle, w );
  KLineEdit* editArtist = new KLineEdit( m_cddbInfo.cdArtist, w );
  KLineEdit* editExtInfo = new KLineEdit( m_cddbInfo.cdExtInfo, w );
  KLineEdit* editGenre = new KLineEdit( m_cddbInfo.genre, w );
  QSpinBox* spinYear = new QSpinBox( 0, 9999, 1, w );
  spinYear->setValue( m_cddbInfo.year );
  QFrame* line = new QFrame( w );
  line->setFrameShape( QFrame::HLine );
  line->setFrameShadow( QFrame::Sunken );
  KComboBox* comboCat = new KComboBox( w );
  comboCat->insertStringList( K3bCddbQuery::categories() );

  // set the category
  for( int i = 0; i < comboCat->count(); ++i )
    if( comboCat->text(i) == m_cddbInfo.category ) {
      comboCat->setCurrentItem(i);
      break;
    }

  QGridLayout* grid = new QGridLayout( w );
  grid->setSpacing( KDialog::spacingHint() );

  grid->addWidget( new QLabel( i18n("Title:"), w ), 0, 0 );
  grid->addWidget( editTitle, 0, 1 );
  grid->addMultiCellWidget( line, 1, 1, 0, 1 );
  grid->addWidget( new QLabel( i18n("Artist:"), w ), 2, 0 );
  grid->addWidget( editArtist, 2, 1 );
  grid->addWidget( new QLabel( i18n("Extra info:"), w ), 3, 0 );
  grid->addWidget( editExtInfo, 3, 1 );
  grid->addWidget( new QLabel( i18n("Genre:"), w ), 4, 0 );
  grid->addWidget( editGenre, 4, 1 );
  grid->addWidget( new QLabel( i18n("Year:"), w ), 5, 0 );
  grid->addWidget( spinYear, 5, 1 );
  grid->addWidget( new QLabel( i18n("Category:"), w ), 6, 0 );
  grid->addWidget( comboCat, 6, 1 );
  grid->setRowStretch( 7, 1 );

  d.setMainWidget(w);
  d.resize(260, d.sizeHint().height());

  if( d.exec() == QDialog::Accepted ) {
    m_cddbInfo.cdTitle = editTitle->text();
    m_cddbInfo.cdArtist = editArtist->text();
    m_cddbInfo.cdExtInfo = editExtInfo->text();
    m_cddbInfo.category = comboCat->currentText();
    m_cddbInfo.genre = editGenre->text();
    m_cddbInfo.year = spinYear->value();

    updateDisplay();
  }
}


void K3bAudioCdView::queryCddb()
{
  KConfig* c = kapp->config();
  c->setGroup("Cddb");

  m_cddb->readConfig( c );

  if( c->readBoolEntry( "use local cddb query", true ) || 
      c->readBoolEntry( "use remote cddb", false ) ) {

    // disable all interaction
    m_trackView->setEnabled(false);
    m_toolBox->setEnabled(false);

    m_cddb->query( m_diskInfo.toc );
  }
}


void K3bAudioCdView::slotCddbQueryFinished( int error )
{
  k3bMain()->endBusy();

  if( error == K3bCddbQuery::SUCCESS ) {
    m_cddbInfo = m_cddb->result();

    // save the entry locally
    // K3bCddb only saves if it is configured, otherwise does nothing
    m_cddb->saveEntry( m_cddbInfo );

    //       kdDebug() << "cddb info:" << endl;
    //       kdDebug() << "DTITLE:  '" << m_cddbInfo.cdTitle << "'" << endl;
    //       kdDebug() << "DARTIST: '" << m_cddbInfo.cdArtist << "'" << endl;
    //       kdDebug() << "DEXT:    '" << m_cddbInfo.cdExtInfo << "'" << endl;
    //       kdDebug() << "DISCID:  '" << m_cddbInfo.discid << "'" << endl;

    //       for( QStringList::const_iterator it = m_cddbInfo.titles.begin();
    // 	   it != m_cddbInfo.titles.end(); ++it ) {
    // 	kdDebug() << "TTITLE:  '" << *it << "'" << endl;
    //       }
    //       for( QStringList::const_iterator it = m_cddbInfo.artists.begin();
    // 	   it != m_cddbInfo.artists.end(); ++it ) {
    // 	kdDebug() << "TARTIST: '" << *it << "'" << endl;
    //       }
    //       for( QStringList::const_iterator it = m_cddbInfo.extInfos.begin();
    // 	   it != m_cddbInfo.extInfos.end(); ++it ) {
    // 	kdDebug() << "TEXT:    '" << *it << "'" << endl;
    //       }

    // now update the listview
    for( QListViewItemIterator it( m_trackView ); it.current(); ++it ) {
      AudioTrackViewItem* item = (AudioTrackViewItem*)it.current();
      item->updateCddbData( m_cddbInfo );
    }

    updateDisplay();
  }
  else if( error == K3bCddbQuery::NO_ENTRY_FOUND ) {
    KMessageBox::information( this, i18n("No CDDB entry found"), i18n("CDDB") );
  }
  else {
    KMessageBox::information( this, m_cddb->errorString(), i18n("Cddb error") );
  }

  m_trackView->setEnabled(true);
  m_toolBox->setEnabled(true);
}


void K3bAudioCdView::slotSelectAll()
{
  for( QListViewItemIterator it( m_trackView ); it.current(); ++it )
    ((AudioTrackViewItem*)it.current())->setOn(true);
}

void K3bAudioCdView::slotDeselectAll()
{
  for( QListViewItemIterator it( m_trackView ); it.current(); ++it )
    ((AudioTrackViewItem*)it.current())->setOn(false);
}

void K3bAudioCdView::slotSelect()
{
  if( QListViewItem* sel = m_trackView->selectedItem() )
    ((AudioTrackViewItem*)sel)->setOn(true);
}

void K3bAudioCdView::slotDeselect()
{
  if( QListViewItem* sel = m_trackView->selectedItem() )
    ((AudioTrackViewItem*)sel)->setOn(false);
}

void K3bAudioCdView::updateDisplay()
{
  if( !m_cddbInfo.cdTitle.isEmpty() ) {
    QString s = m_cddbInfo.cdTitle;
    if( !m_cddbInfo.cdArtist.isEmpty() )
      s += " (" + m_cddbInfo.cdArtist + ")";
    m_labelTitle->setText( s );
  }
  else
    m_labelTitle->setText( i18n("Audio CD") );

  m_labelLength->setText( i18n("1 track (%1)", 
			       "%n tracks (%1)", 
			       m_diskInfo.toc.count()).arg(K3b::Msf(m_diskInfo.toc.length()).toString()) );
}

#include "k3baudiocdview.moc"
