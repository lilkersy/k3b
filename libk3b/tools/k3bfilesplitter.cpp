/*
 *
 * $Id: sourceheader 511311 2006-02-19 14:51:05Z trueg $
 * Copyright (C) 2006 Sebastian Trueg <trueg@k3b.org>
 *
 * This file is part of the K3b project.
 * Copyright (C) 1998-2007 Sebastian Trueg <trueg@k3b.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * See the file "COPYING" for the exact licensing terms.
 */

#include "k3bfilesplitter.h"
#include "k3bfilesysteminfo.h"

#include <kdebug.h>

#include <qfile.h>


class K3bFileSplitter::Private
{
public:
    Private( K3bFileSplitter* splitter )
        : m_splitter( splitter ) {
    }

    QString filename;
    QFile file;
    int counter;

    qint64 maxFileSize;

    qint64 currentOverallPos;
    qint64 currentFilePos;

    void determineMaxFileSize() {
        if( maxFileSize == 0 ) {
            if( K3bFileSystemInfo( filename ).type() == K3bFileSystemInfo::FS_FAT )
                maxFileSize = 1024ULL*1024ULL*1024ULL; // 1GB
            else
                maxFileSize = 1024ULL*1024ULL*1024ULL*1024ULL*1024ULL;  // incredibly big, 1024 TB
        }
    }

    QString buildFileName( int counter ) {
        if( counter > 0 )
            return filename + '.' + QString::number(counter).rightJustified( 3, '0' );
        else
            return filename;
    }

    QString currentFileName() {
        return buildFileName( counter );
    }

    bool openPrevFile() {
        return openFile( --counter );
    }

    bool openNextFile() {
        return openFile( ++counter );
    }

    bool openFile( int counter ) {
        file.close();
        file.setFileName( buildFileName( counter ) );
        currentFilePos = 0;
        if( file.open( m_splitter->openMode() ) ) {
            return true;
        }
        else {
            m_splitter->close();
            return false;
        }
    }

private:
    K3bFileSplitter* m_splitter;
};


K3bFileSplitter::K3bFileSplitter()
{
    d = new Private( this );
}


K3bFileSplitter::K3bFileSplitter( const QString& filename )
{
    d = new Private( this );
    setName( filename );
}


K3bFileSplitter::~K3bFileSplitter()
{
    delete d;
}


const QString& K3bFileSplitter::name() const
{
    return d->filename;
}


void K3bFileSplitter::setName( const QString& filename )
{
    close();
    d->maxFileSize = 0;
    d->filename = filename;
}


bool K3bFileSplitter::open( OpenMode mode )
{
    close();

    d->determineMaxFileSize();

    d->counter = 0;
    d->currentFilePos = 0;
    d->currentOverallPos = 0;
    if ( open( mode ) ) {
        return d->openFile( 0 );
    }
    else {
        return false;
    }
}


void K3bFileSplitter::close()
{
    d->file.close();
    d->counter = 0;
    d->currentFilePos = 0;
    d->currentOverallPos = 0;
}


int K3bFileSplitter::handle() const
{
    // FIXME: use a K3bPipe to simulate this
    return -1;
}



void K3bFileSplitter::flush()
{
    d->file.flush();
}


qint64 K3bFileSplitter::size() const
{
    // not implemented due to Offset size limitations
    return 0;
}


qint64 K3bFileSplitter::pos() const
{
    return d->currentOverallPos;
}


bool K3bFileSplitter::seek( qint64 pos )
{
    Q_UNUSED( pos );
    // FIXME: implement me (although not used yet)
    return false;
}


bool K3bFileSplitter::atEnd() const
{
    return d->file.atEnd() && !QFile::exists( d->buildFileName( d->counter+1 ) );
}


qint64 K3bFileSplitter::readData( char *data, qint64 maxlen )
{
    qint64 r = d->file.read( data, maxlen );
    if( r == 0 ) {
        if( atEnd() ) {
            return r;
        }
        else if( d->openNextFile() ) {
            // recursively call us
            return readData( data, maxlen );
        }
    }
    else if( r > 0 ) {
        d->currentOverallPos += r;
        d->currentFilePos += r;
    }

    return r;
}


qint64 K3bFileSplitter::writeData( const char *data, qint64 len )
{
    // We cannot rely on QFile::at since it uses long on most copmpilations
    qint64 max = qMin( len, d->maxFileSize - d->currentFilePos );

    qint64 r = d->file.write( data, max );

    if( r < 0 )
        return r;

    d->currentOverallPos += r;
    d->currentFilePos += r;

    // recursively call us
    if( r < len ) {
        if( d->openNextFile() )
            return r + writeData( data+r, len-r );
        else
            return -1;
    }
    else
        return r;
}


// int K3bFileSplitter::getch()
// {
//     int r = d->file.getch();
//     if( r == -1 ) {
//         if( !d->file.atEnd() ) {
//             return -1;
//         }
//         else if( !atEnd() ) {
//             if( !d->openNextFile() )
//                 return -1;
//             else
//                 return getch();
//         }
//     }

//     d->currentOverallPos++;
//     d->currentFilePos++;

//     return r;
// }


// int K3bFileSplitter::putch( int c )
// {
//     if( d->currentFilePos < d->maxFileSize ) {
//         d->currentOverallPos++;
//         d->currentFilePos++;
//         return d->file.putch( c );
//     }
//     else if( d->openNextFile() ) {
//         // recursively call us
//         return putch( c );
//     }
//     else
//         return -1;
// }


// int K3bFileSplitter::ungetch( int c )
// {
//     if( d->currentFilePos > 0 ) {
//         int r = d->file.ungetch( c );
//         if( r != -1 ) {
//             d->currentOverallPos--;
//             d->currentFilePos--;
//         }
//         return r;
//     }
//     else if( d->counter > 0 ) {
//         // open prev file
//         if( d->openPrevFile() ) {
//             // seek to the end
//             d->file.at( d->file.size() );
//             d->currentFilePos = d->file.at();
//             return getch();
//         }
//         else
//             return -1;
//     }
//     else
//         return -1;
// }


void K3bFileSplitter::remove()
{
    close();
    while( QFile::exists( d->buildFileName( d->counter ) ) )
        QFile::remove( d->buildFileName( d->counter++ ) );
}


void K3bFileSplitter::setMaxFileSize( qint64 size )
{
    d->maxFileSize = size;
}
