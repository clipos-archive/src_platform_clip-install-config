// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file DeviceTab.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010-2011 SGDSN/ANSSI
 * @n
 * All rights reserved.
 */

#ifndef DEVICETAB_H
#define DEVICETAB_H

#include "ConfTab.h"
#include "consts.h"

class QPushButton;
class QCheckBox;
class QProcess;

class DeviceTab : public ConfTab
{
  Q_OBJECT
 
 public:
  DeviceTab(QWidget *parent, ConfProfile *profile);

 private:
  int jails;
  QProcess *cc;

  QCheckBox *chkClrUsbCLIP;
  QCheckBox *chkClrUsbRMH;
  QCheckBox *chkClrUsbRMB;
  QCheckBox *chkRwUsbAdmin;
  QCheckBox *chkRwUsbAudit;
  QCheckBox *chkRwUsbUpdate;
  QCheckBox *chkRwUsbRMH;
  QCheckBox *chkRwUsbRMB;
  QCheckBox *chkRoUsbAdmin;
  QCheckBox *chkRoUsbAudit;
  QCheckBox *chkRoUsbUpdate;
  QCheckBox *chkRoUsbRMH;
  QCheckBox *chkRoUsbRMB;

  QPushButton *btnRun;

 public slots:
  virtual bool saveCurrentProfile();
  virtual bool loadCurrentProfile();
  virtual bool initValues();
  virtual void enableJails(int mask);
 private slots:
  void runClipConfig();
  void clipConfigFinished(int status);
};

#endif // DEVICETAB_H

// vi:sw=2:ts=2:et:co=80:
