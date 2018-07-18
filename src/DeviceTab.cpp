// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file DeviceTab.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010-2011 SGDSN/ANSSI
 * @n
 * All rights reserved.
 */


#include "DeviceTab.h"
#include "ConfProfile.h"

#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>

DeviceTab::DeviceTab(QWidget *parent, ConfProfile *profile)
  : ConfTab(parent, profile), jails(JAIL_RMH|JAIL_RMB)
{ 
  cc = new QProcess(this);
  connect(cc, SIGNAL(finished(int, QProcess::ExitStatus)),
              this, SLOT(clipConfigFinished(int)));

  // Buttons
  chkClrUsbCLIP = new QCheckBox("CLIP", this);
  chkClrUsbRMH = new QCheckBox("RMH", this);
  chkClrUsbRMB = new QCheckBox("RMB", this);

  chkRwUsbAdmin = new QCheckBox("ADMIN", this);
  chkRwUsbAudit = new QCheckBox("AUDIT", this);
  chkRwUsbUpdate = new QCheckBox("UPDATE", this);
  chkRwUsbRMH = new QCheckBox("RMH", this);
  chkRwUsbRMB = new QCheckBox("RMB", this);

  chkRoUsbAdmin = new QCheckBox("ADMIN", this);
  chkRoUsbAudit = new QCheckBox("AUDIT", this);
  chkRoUsbUpdate = new QCheckBox("UPDATE", this);
  chkRoUsbRMH = new QCheckBox("RMH", this);
  chkRoUsbRMB = new QCheckBox("RMB", this);

  btnRun = new QPushButton("Configurer les attributions dynamiques", this);
  connect(btnRun, SIGNAL(clicked()), this, SLOT(runClipConfig()));

  // Horizontal layouts 
  QHBoxLayout *chb = new QHBoxLayout;
  chb->setSpacing(4);
  chb->addSpacing(10);
  chb->addWidget(chkClrUsbCLIP);
  chb->addSpacing(10);
  chb->addWidget(chkClrUsbRMH);
  chb->addSpacing(10);
  chb->addWidget(chkClrUsbRMB);
  chb->addStretch(2);

  QHBoxLayout *rwhb = new QHBoxLayout;
  rwhb->setSpacing(4);
  rwhb->addSpacing(10);
  rwhb->addWidget(chkRwUsbAdmin);
  rwhb->addSpacing(10);
  rwhb->addWidget(chkRwUsbAudit);
  rwhb->addSpacing(10);
  rwhb->addWidget(chkRwUsbUpdate);
  rwhb->addSpacing(10);
  rwhb->addWidget(chkRwUsbRMH);
  rwhb->addSpacing(10);
  rwhb->addWidget(chkRwUsbRMB);
  rwhb->addStretch(2);

  QHBoxLayout *rohb = new QHBoxLayout;
  rohb->setSpacing(4);
  rohb->addSpacing(10);
  rohb->addWidget(chkRoUsbAdmin);
  rohb->addSpacing(10);
  rohb->addWidget(chkRoUsbAudit);
  rohb->addSpacing(10);
  rohb->addWidget(chkRoUsbUpdate);
  rohb->addSpacing(10);
  rohb->addWidget(chkRoUsbRMH);
  rohb->addSpacing(10);
  rohb->addWidget(chkRoUsbRMB);
  rohb->addStretch(2);


  // Vertical layout
  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(4);

  vb->addWidget(new QLabel("<b><u>Autorisation des supports USB "
                           "signés non chiffrés</u> :</b>", this));
  vb->addSpacing(5);
  vb->addLayout(chb);
  vb->addSpacing(10);

  vb->addWidget(new QLabel("<b><u>Accès en écriture aux supports USB "
                           "non signés</u> :</b>", this));
  vb->addSpacing(5);
  vb->addLayout(rwhb);
  vb->addSpacing(10);

  vb->addWidget(new QLabel("<b><u>Accès en lecture aux supports USB "
                           "non signés</u> :</b>", this));
  vb->addSpacing(5);
  vb->addLayout(rohb);
  vb->addSpacing(10);


  vb->addWidget(new QLabel("<b><u>Périphériques à attribution dynamique"
                           "<u> :<b>", this));
  vb->addSpacing(5);
  vb->addWidget(btnRun);
  vb->addStretch(2);
}

bool 
DeviceTab::saveCurrentProfile()
{
  QStringList clr, rw, ro;
  
  if (chkClrUsbCLIP->isChecked())
    clr << "clip";
  if (chkClrUsbRMH->isChecked())
    clr << "rm_h";
  if (chkClrUsbRMB->isChecked())
    clr << "rm_b";

  if (chkRwUsbAdmin->isChecked())
    rw << "admin";
  if (chkRwUsbAudit->isChecked())
    rw << "audit";
  if (chkRwUsbAdmin->isChecked())
    rw << "update";
  if (chkRwUsbRMH->isChecked())
    rw << "rm_h";
  if (chkRwUsbRMB->isChecked())
    rw << "rm_b";

  if (chkRoUsbAdmin->isChecked())
    ro << "admin";
  if (chkRoUsbAudit->isChecked())
    ro << "audit";
  if (chkRoUsbUpdate->isChecked())
    ro << "update";
  if (chkRoUsbRMH->isChecked())
    ro << "rm_h";
  if (chkRoUsbRMB->isChecked())
    ro << "rm_b";

  profile->setNameVal(USBKEYS_CONF, "CLEARTEXT_LEVELS", clr.join(" "));
  profile->setNameVal(USBKEYS_CONF, "RW_LEVELS", rw.join(" "));
  profile->setNameVal(USBKEYS_CONF, "RO_LEVELS", ro.join(" "));
  // Hardcoded for now
  profile->setNameVal(USBKEYS_CONF, "CDROM_DEVICE", "/dev/sr0");

  return true;
}

bool 
DeviceTab::loadCurrentProfile()
{
  QString clr = profile->getNameVal(USBKEYS_CONF, 
                                        "CLEARTEXT_LEVELS", any_rx);
  QString rw = profile->getNameVal(USBKEYS_CONF, "RW_LEVELS", any_rx);
  QString ro = profile->getNameVal(USBKEYS_CONF, "RO_LEVELS", any_rx);

  chkClrUsbCLIP->setChecked(clr.contains("clip"));
  chkClrUsbRMH->setChecked(clr.contains("rm_h"));
  chkClrUsbRMB->setChecked(clr.contains("rm_b"));

  chkRwUsbAdmin->setChecked(rw.contains("admin"));
  chkRwUsbAudit->setChecked(rw.contains("audit"));
  chkRwUsbUpdate->setChecked(rw.contains("update"));
  chkRwUsbRMH->setChecked(rw.contains("rm_h"));
  chkRwUsbRMB->setChecked(rw.contains("rm_b"));

  chkRoUsbAdmin->setChecked(ro.contains("admin"));
  chkRoUsbAudit->setChecked(ro.contains("audit"));
  chkRoUsbUpdate->setChecked(ro.contains("update"));
  chkRoUsbRMH->setChecked(ro.contains("rm_h"));
  chkRoUsbRMB->setChecked(ro.contains("rm_b"));

  return true;
}

bool 
DeviceTab::initValues()
{
  return true;
}

#define enable_btn(btn, on) do {\
  btn->setEnabled(on); \
  if (!on) \
    btn->setChecked(false); \
} while(0)

void
DeviceTab::enableJails(int mask)
{
  bool rmh = ((mask & JAIL_RMH) != 0);
  bool rmb = ((mask & JAIL_RMB) != 0);

  jails = mask;

  enable_btn(chkClrUsbRMH, rmh);
  enable_btn(chkRwUsbRMH, rmh);
  enable_btn(chkRoUsbRMH, rmh);

  enable_btn(chkClrUsbRMB, rmb);
  enable_btn(chkRwUsbRMB, rmb);
  enable_btn(chkRoUsbRMB, rmb);
}

void
DeviceTab::runClipConfig()
{
  if (cc->state() != QProcess::NotRunning) {
    qDebug("Multiple launch of clip-config");
    return;
  }

  QStringList args;
  QString path = profilePath() + "/conf/devices";
  args << "-D" << "-c" << path;
  if (jails & JAIL_RMH)
    args << "-h";
  if (jails & JAIL_RMB)
    args << "-b";

  cc->start("clip-config", args);
  if (!cc->waitForStarted()) {
    QMessageBox::warning (0, "Echec",
          "Impossible de lancer clip-config.\n"
          "Vérifiez votre installation.",
          QMessageBox::Ok | QMessageBox::Default, 0, 0);
    return;
  }
  btnRun->setEnabled(false);
  switchable = false;
}

void
DeviceTab::clipConfigFinished(int status)
{
  btnRun->setEnabled(true);
  switchable = true;
}

// vi:sw=2:ts=2:et:co=80:
