// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file NetTab.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef NETTAB_H
#define NETTAB_H

#include "ConfTab.h"
#include "consts.h"

class QPushButton;
class IPField;
class QCheckBox;
class QProcess;

class NetTab : public ConfTab
{
  Q_OBJECT
 
 public:
  NetTab(QWidget *parent, ConfProfile *profile);

 public slots:
  virtual bool saveCurrentProfile();
  virtual bool loadCurrentProfile();
  virtual bool initValues();
  virtual void enableJails(int mask);

 private slots:
  void runClipConfig(void);
  void clipConfigFinished(int status);
  void enableAdminVirt(int);
  void enableAuditVirt(int);
  void enableRmhVirt(int);
  void enableUpdateVirt(int);

 private:
  int jails;

  QProcess *cc;
  QPushButton *btnRun;

  QCheckBox *chkAdminVirt;
  QCheckBox *chkAuditVirt;
  QCheckBox *chkRmhVirt;
  QCheckBox *chkUpdateVirt;

  IPField *adminAddr;
  IPField *auditAddr;
  IPField *rmhAddr;
  IPField *updateAddr;
};

#endif // NETTAB_H

// vi:sw=2:ts=2:et:co=80:
