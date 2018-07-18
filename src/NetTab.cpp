// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file NetTab.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */


#include <ClipWidgets/IPField.h>
#include <QCheckBox>
#include <QFile>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QStringList>

#include "NetTab.h"
#include "ConfProfile.h"

NetTab::NetTab(QWidget *parent, ConfProfile *profile)
  : ConfTab(parent, profile)
{ 
  jails = JAIL_RMH | JAIL_RMB;

  cc = new QProcess(this);
  connect(cc, SIGNAL(finished(int, QProcess::ExitStatus)),
              this, SLOT(clipConfigFinished(int)));

  chkAdminVirt = new QCheckBox("Adresse dynamique", this);
  chkAuditVirt = new QCheckBox("Adresse dynamique", this);
  chkRmhVirt = new QCheckBox("Adresse dynamique", this);
  chkUpdateVirt = new QCheckBox("Adresse dynamique", this);

  adminAddr = new IPField(this, "Addresse ADMIN", 10, 10);
  auditAddr = new IPField(this, "Addresse Audit", 10, 10);
  rmhAddr = new IPField(this, "Addresse RM_H", 10, 10);
  updateAddr = new IPField(this, "Addresse UPDATE", 10, 10);

  connect(chkAdminVirt, SIGNAL(stateChanged(int)),
              this, SLOT(enableAdminVirt(int)));
  connect(chkAuditVirt, SIGNAL(stateChanged(int)),
              this, SLOT(enableAuditVirt(int)));
  connect(chkRmhVirt, SIGNAL(stateChanged(int)),
              this, SLOT(enableRmhVirt(int)));
  connect(chkUpdateVirt, SIGNAL(stateChanged(int)),
              this, SLOT(enableUpdateVirt(int)));

  btnRun = new QPushButton("Editer les profils réseau", this);
  connect(btnRun, SIGNAL(clicked()), this, SLOT(runClipConfig()));

  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(4);

  vb->addWidget(new QLabel("<b><u>Adresses internes des cages</u></b> "
                "(indépendantes du profil réseau)", this));
  QHBoxLayout *hb;
#define add_line(ipfield, chkbox) do {\
  hb = new QHBoxLayout;\
  hb->addSpacing(10);\
  hb->addWidget(ipfield);\
  hb->addSpacing(10);\
  hb->addWidget(chkbox);\
  hb->addSpacing(10);\
  vb->addSpacing(5);\
  vb->addLayout(hb); \
} while (0)

  add_line(updateAddr, chkUpdateVirt);
  add_line(adminAddr, chkAdminVirt);
  add_line(auditAddr, chkAuditVirt);
  add_line(rmhAddr, chkRmhVirt);

  vb->addSpacing(10);
  vb->addWidget(new QLabel("<b><u>Profils réseau</u></b>", this));
  vb->addSpacing(5);
  vb->addWidget(btnRun);
  vb->addSpacing(10);
  vb->addStretch(5);
}

void
NetTab::runClipConfig() 
{
  if (cc->state() != QProcess::NotRunning) {
    qDebug("Multiple launch of clip-config");
    return;
  }

  QStringList args;
  QString path = profilePath() + "/conf/netconf.d";
  args << "-n" << "-c" << path << "-i";
  if (!(jails & (JAIL_RMH|JAIL_RMB))) {
    // Three interfaces for gateways
    args << "-i" << "-i";
  } else {
    // Wifi for clients
    args << "-w";
    if (jails & JAIL_RMH)
      args << "-h";
    if (jails & JAIL_RMB)
      args << "-b";
  }
  cc->start("clip-config", args);
  if (!cc->waitForStarted()) {
    QMessageBox::warning (0, "Echec",
          "Impossible de lancer clip-config pour éditer les\n"
          "profils réseaux. Vérifiez votre installation.",
          QMessageBox::Ok | QMessageBox::Default, 0, 0);
    return;
  }
  btnRun->setEnabled(false);
  switchable = false;
}

void
NetTab::clipConfigFinished(int status)
{
  btnRun->setEnabled(true);
  switchable = true;
}

bool 
NetTab::saveCurrentProfile()
{
  QString ip;
#define set_val(ipfield, chkbox, varname) do {\
  if (chkbox->isChecked()) {\
    profile->setNameVal(NET_CONF, varname, "virtual");\
  } else {\
    ip = ipfield->returnIP();\
    if (ip != "0.0.0.0")\
      profile->setNameVal(NET_CONF, varname, ip);\
  }\
} while (0)

  set_val(adminAddr, chkAdminVirt, "ADMIN_ADDR");
  set_val(auditAddr, chkAuditVirt, "AUDIT_ADDR");
  set_val(updateAddr, chkUpdateVirt, "UPDATE_ADDR");
  if (jails & JAIL_RMH)
    set_val(rmhAddr, chkRmhVirt, "RMH_ADDR");

  return true;

#undef set_val
}

bool 
NetTab::loadCurrentProfile()
{
  bool virt;
#define get_val(ipfield, chkbox, varname) do {\
  virt = !profile->getNameVal(NET_CONF, varname, virtual_rx).isEmpty(); \
  if (virt) { \
    chkbox->setChecked(true); \
    ipfield->setNameValue("0.0.0.0"); \
  } else { \
    ipfield->setNameValue(profile->getNameVal(NET_CONF, varname, ip_rx)); \
    chkbox->setChecked(false); \
  } \
} while (0)

  get_val(updateAddr, chkUpdateVirt, "UPDATE_ADDR");
  get_val(adminAddr, chkAdminVirt, "ADMIN_ADDR");
  get_val(auditAddr, chkAuditVirt, "AUDIT_ADDR");
  get_val(rmhAddr, chkRmhVirt, "RMH_ADDR");

  return true;

#undef get_val
}

bool 
NetTab::initValues()
{
  return true;
}

void
NetTab::enableJails(int mask)
{
  jails = mask;
  bool rmh = ((mask & JAIL_RMH) != 0);

  rmhAddr->setEnabled(rmh);
}

void
NetTab::enableAdminVirt(int state)
{
  adminAddr->setEnabled((state != Qt::Checked));
}

void
NetTab::enableAuditVirt(int state)
{
  auditAddr->setEnabled((state != Qt::Checked));
}

void
NetTab::enableRmhVirt(int state)
{
  rmhAddr->setEnabled((state != Qt::Checked));
}

void
NetTab::enableUpdateVirt(int state)
{
  updateAddr->setEnabled((state != Qt::Checked));
}

// vi:sw=2:ts=2:et:co=80:
