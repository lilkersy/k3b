/***************************************************************************
                          k3bvcdoptions.h  -  description
                             -------------------
    begin                : Sam Nov 23 2002
    copyright            : (C) 2002 by Sebastian Trueg
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

#ifndef K3B_VCD_OPTIONS_H
#define K3B_VCD_OPTIONS_H

#include <qstring.h>

class KConfig;


class K3bVcdOptions
{
 public:
  K3bVcdOptions();

  const QString& systemId() const { return m_systemId; }
  const QString& applicationId() const { return m_applicationId; }
  
  const QString& albumId() const { return m_albumID; }
  const QString& volumeId() const { return m_volumeID; }
  const QString& volumeSetId() const { return m_volumeSetId; }
  const QString& publisher() const { return m_publisher; }
  const QString& preparer() const { return m_preparer; }

  const int volumeCount() const { return m_volumeCount; }
  const int volumeNumber()const { return m_volumeNumber; }

  const bool BrokenSVcdMode() const { return m_brokensvcdmode; }
  const bool Sector2336() const { return m_sector2336; }

  void setAlbumId( const QString& s ) { m_albumID = s; }
  void setVolumeId( const QString& s ) { m_volumeID = s; }
  void setVolumeSetId( const QString& s ) { m_volumeSetId = s; }
  void setPublisher( const QString& s ) { m_publisher = s; }
  void setPreparer( const QString& s ) { m_preparer = s; }

  void setVolumeCount( const int& c ) { m_volumeCount = c; }
  void setVolumeNumber( const int n ) { m_volumeNumber = n; }
  
  void setBrokenSVcdMode( const bool& b ) { m_brokensvcdmode = b; }
  void setSector2336( const bool& b ) { m_sector2336 = b; }
  
  void save( KConfig* c );

  static K3bVcdOptions load( KConfig* c );
  static K3bVcdOptions defaults();

 private:
  // volume descriptor
  QString m_volumeID;
  QString m_albumID;
  QString m_volumeSetId;

  QString m_preparer;
  QString m_publisher;

  QString m_applicationId;
  QString m_systemId;
  
  int m_volumeCount;
  int m_volumeNumber;
    
  bool m_brokensvcdmode;
  bool m_sector2336;

};

#endif
