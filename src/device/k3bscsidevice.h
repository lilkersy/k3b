
#ifndef K3B_SCSI_DEVICE_H
#define K3B_SCSI_DEVICE_H

#include "k3bdevice.h"

class ScsiIf;
struct cdrom_drive;

class K3bScsiDevice : public K3bDevice
{
 public:
  K3bScsiDevice( cdrom_drive* );
  ~K3bScsiDevice();

  int isReady() const;
  int isEmpty();
  bool init();
  bool block(bool) const;

  int interfaceType() const { return K3bDevice::SCSI; }

  void diskInfo();

 private:
  int getModePage( ScsiIf *_scsiIf, int pageCode, unsigned char *buf,
		   long bufLen, unsigned char *modePageHeader,
		   unsigned char *blockDesc, int showErrorMsg );
};

#endif
