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

#ifndef _K3B_BOOT_ITEM_H_
#define _K3B_BOOT_ITEM_H_

#include "k3bfileitem.h"

class K3bBootItem : public K3bFileItem
{
 public:
  K3bBootItem( const QString& fileName, K3bDataDoc* doc, K3bDirItem* dir, const QString& k3bName = 0 );
  ~K3bBootItem();

  bool isHideable() const { return false; }

  enum imageType { FLOPPY, HARDDISK, NONE };

  void setNoBoot( bool b ) { m_noBoot = b; }
  void setBootInfoTable( bool b ) { m_bootInfoTable = b; }
  void setLoadSegment( int s ) { m_loadSegment = s; }
  void setLoadSize( int s ) { m_loadSize = s; }
  void setImageType( int t ) { m_imageType = t; }

  bool noBoot() const { return m_noBoot; }
  bool bootInfoTable() const { return m_bootInfoTable; }
  int loadSegment() const { return m_loadSegment; }
  int loadSize() const { return m_loadSize; }
  int imageType() const { return m_imageType; }

 private:
  bool m_noBoot;
  bool m_bootInfoTable;
  int m_loadSegment;
  int m_loadSize;
  int m_imageType;
};

#endif
