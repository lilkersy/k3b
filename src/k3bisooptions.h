#ifndef K3B_ISO_OPTIONS_H
#define K3B_ISO_OPTIONS_H

#include <qstring.h>

class K3bIsoOptions
{
 public:
  K3bIsoOptions()
    : m_inputCharset( "iso8859-1" ) {
    m_bForceInputCharset = false;

    m_createRockRidge = true;
    m_createJoliet = false;
    m_ISOallowLowercase = false;
    m_ISOallowPeriodAtBegin = false;
    m_ISOallow31charFilenames = true;
    m_ISOomitVersionNumbers = false;
    m_ISOomitTrailingPeriod = false;
    m_ISOmaxFilenameLength = false;
    m_ISOrelaxedFilenames = false;
    m_ISOnoIsoTranslate = false;
    m_ISOallowMultiDot = false;
    m_ISOuntranslatedFilenames = false;
    m_noDeepDirectoryRelocation = false;
    m_followSymbolicLinks = false;
    m_hideRR_MOVED = false;
    m_createTRANS_TBL = false;
    m_hideTRANS_TBL = false;
    m_isoLevel = 3;
  }

  bool forceInputCharset() const { return m_bForceInputCharset; }
  const QString& inputCharset() const { return m_inputCharset; }

  void setForceInputCharset( bool b ) { m_bForceInputCharset = b; }
  void setInputCharset( const QString& cs ) { m_inputCharset = cs; }

	
  // -- mkisofs-options ----------------------------------------------------------------------
  bool createRockRidge() const { return m_createRockRidge; }
  bool createJoliet() const { return m_createJoliet; }
  bool ISOallowLowercase() const { return m_ISOallowLowercase; }
  bool ISOallowPeriodAtBegin() const { return m_ISOallowPeriodAtBegin; }
  bool ISOallow31charFilenames() const { return m_ISOallow31charFilenames; }
  bool ISOomitVersionNumbers() const { return m_ISOomitVersionNumbers; }
  bool ISOomitTrailingPeriod() const { return m_ISOomitTrailingPeriod; }
  bool ISOmaxFilenameLength() const { return m_ISOmaxFilenameLength; }
  bool ISOrelaxedFilenames() const { return m_ISOrelaxedFilenames; }
  bool ISOnoIsoTranslate() const { return m_ISOnoIsoTranslate; }
  bool ISOallowMultiDot() const { return m_ISOallowMultiDot; }
  bool ISOuntranslatedFilenames() const { return m_ISOuntranslatedFilenames; }
  bool noDeepDirectoryRelocation() const { return m_noDeepDirectoryRelocation; }
  bool followSymbolicLinks() const { return m_followSymbolicLinks; }
  bool hideRR_MOVED() const { return m_hideRR_MOVED; }
  bool createTRANS_TBL() const { return m_createTRANS_TBL; }
  bool hideTRANS_TBL() const { return m_hideTRANS_TBL; }

  int ISOLevel() const { return m_isoLevel; }
  const QString& systemId() const { return m_systemId; }
  const QString& applicationID() const { return m_applicationID; }
  const QString& volumeID() const { return m_volumeID; }
  const QString& volumeSetId() const { return m_volumeSetId; }
  const QString& publisher() const { return m_publisher; }
  const QString& preparer() const { return m_preparer; }
	
  void setCreateRockRidge( bool b ) { m_createRockRidge = b; }
  void setCreateJoliet( bool b ) {  m_createJoliet = b; }
  void setISOallowLowercase( bool b ) {  m_ISOallowLowercase = b; }
  void setISOallowPeriodAtBegin( bool b ) {  m_ISOallowPeriodAtBegin = b; }
  void setISOallow31charFilenames( bool b ) {  m_ISOallow31charFilenames = b; }
  void setISOomitVersionNumbers( bool b ) {  m_ISOomitVersionNumbers = b; }
  void setISOomitTrailingPeriod( bool b ) {  m_ISOomitTrailingPeriod = b; }
  void setISOmaxFilenameLength( bool b ) {  m_ISOmaxFilenameLength = b; }
  void setISOrelaxedFilenames( bool b ) {  m_ISOrelaxedFilenames = b; }
  void setISOnoIsoTranslate( bool b ) {  m_ISOnoIsoTranslate = b; }
  void setISOallowMultiDot( bool b ) {  m_ISOallowMultiDot = b; }
  void setISOuntranslatedFilenames( bool b ) {  m_ISOuntranslatedFilenames = b; }
  void setNoDeepDirectoryRelocation( bool b ) {  m_noDeepDirectoryRelocation = b; }
  void setFollowSymbolicLinks( bool b ) {  m_followSymbolicLinks = b; }
  void setHideRR_MOVED( bool b ) {  m_hideRR_MOVED = b; }
  void setCreateTRANS_TBL( bool b ) {  m_createTRANS_TBL = b; }
  void setHideTRANS_TBL( bool b ) {  m_hideTRANS_TBL = b; }
	
  void setISOLevel( int i ) { m_isoLevel = i; }
  void setSystemId( const QString& s ) { m_systemId = s; }
  void setApplicationID( const QString& s ) { m_applicationID = s; }
  void setVolumeID( const QString& s ) { m_volumeID = s; }
  void setVolumeSetId( const QString& s ) { m_volumeSetId = s; }
  void setPublisher( const QString& s ) { m_publisher = s; }
  void setPreparer( const QString& s ) { m_preparer = s; }
	
  // ----------------------------------------------------------------- mkisofs-options -----------

 private:
  // volume descriptor
  QString m_volumeID;
  QString m_applicationID;
  QString m_preparer;
  QString m_publisher;
  QString m_systemId;
  QString m_volumeSetId;
	

  bool m_bForceInputCharset;
  QString m_inputCharset;

  // mkisofs options -------------------------------------
  bool m_createRockRidge;    // -r or -R
  bool m_createJoliet;             // -J
  bool m_ISOallowLowercase;   // -allow-lowercase
  bool m_ISOallowPeriodAtBegin;   // -L
  bool m_ISOallow31charFilenames;  // -I
  bool m_ISOomitVersionNumbers;   // -N
  bool m_ISOomitTrailingPeriod;   // -d
  bool m_ISOmaxFilenameLength;     // -max-iso9660-filenames (forces -N)
  bool m_ISOrelaxedFilenames;      // -relaxed-filenames
  bool m_ISOnoIsoTranslate;        // -no-iso-translate
  bool m_ISOallowMultiDot;          // -allow-multidot
  bool m_ISOuntranslatedFilenames;   // -U (forces -d, -I, -L, -N, -relaxed-filenames, -allow-lowercase, -allow-multidot, -no-iso-translate)
  bool m_noDeepDirectoryRelocation;   // -D
  bool m_followSymbolicLinks;       // -f
  bool m_hideRR_MOVED;  // -hide-rr-moved
  bool m_createTRANS_TBL;    // -T
  bool m_hideTRANS_TBL;    // -hide-joliet-trans-tbl
	
  int m_isoLevel;
};

#endif
