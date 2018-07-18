// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file GwNetTab.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */


#include "GwNetTab.h"
#include "ConfProfile.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QRadioButton>
#include <QSpinBox>

#define ADMIN_EXT 1
#define ADMIN_INT 2

#define UPDATE_EXT 1
#define UPDATE_INT 2

GwNetTab::GwNetTab(QWidget *parent, ConfProfile *profile)
  : ConfTab(parent, profile)
{ 
  spbNumIfs = new QSpinBox(this);
  spbNumIfs->setMinimum(2);
  spbNumIfs->setValue(2);
  spbNumIfs->setMaximum(3);
  
  btnAdminExt = new QRadioButton("Interface externe (eth0 + IPsec)", this);
  btnAdminInt = new QRadioButton("Interface interne (eth1)", this);

  chkAdminGateway = new QCheckBox("Passerelle de téléadministration "
                        "(réseau client)", this);
  chkAdminEth2 = new QCheckBox("Interface d'administration dédiée "
                        "(eth2, en clair)", this);


  bgAdmin = new QButtonGroup;
  bgAdmin->setExclusive(true);
  bgAdmin->addButton(btnAdminExt, ADMIN_EXT);
  bgAdmin->addButton(btnAdminInt, ADMIN_INT);

  btnUpdateExt = new QRadioButton("Interface externe (eth0 + IPsec)", this);
  btnUpdateInt = new QRadioButton("Interface interne (eth1)", this);

  bgUpdate = new QButtonGroup;
  bgUpdate->setExclusive(true);
  bgUpdate->addButton(btnUpdateExt, UPDATE_EXT);
  bgUpdate->addButton(btnUpdateInt, UPDATE_INT);

  connect(spbNumIfs, SIGNAL(valueChanged(int)), this, SLOT(setNumIfs(int)));


  // Horizontal layouts 
  QHBoxLayout *hbt = new QHBoxLayout;
  hbt->setSpacing(4);
  hbt->addWidget(new QLabel("Nombre d'interfaces : ", this));
  hbt->addSpacing(10);
  hbt->addWidget(spbNumIfs);
  hbt->addStretch(2);

  // Vertical layout
  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(4);

  vb->addLayout(hbt);
  vb->addSpacing(10);
  QHBoxLayout *hb;

#define add_widget(space, widget) do {\
  vb->addSpacing(5); \
  hb = new QHBoxLayout; \
  hb->addSpacing(space); \
  hb->addWidget(widget); \
  hb->addStretch(5); \
  vb->addLayout(hb); \
} while (0)

  vb->addWidget(new QLabel("<b><u>Interface de téléadministration</u> :</b>",
                          this));
  add_widget(15, btnAdminExt);
  add_widget(15, btnAdminInt);
  add_widget(15, chkAdminGateway);
  add_widget(15, chkAdminEth2);

  vb->addSpacing(10);
  vb->addWidget(new QLabel("<b><u>Interface de mise à jour</u> :</b>",
                          this));
  add_widget(15, btnUpdateExt);
  add_widget(15, btnUpdateInt);
  vb->addStretch(2);
}

bool 
GwNetTab::saveCurrentProfile()
{
  if (spbNumIfs->value() == 1)
    return true; // this is a client

  profile->setNameVal(GW_NET_CONF, "IF_NUMBER", 
                                QString::number(spbNumIfs->value()));
  int user_if = 0, update_if = 0;
  bool admin_noipsec = false, update_noipsec = false; 

  switch (bgAdmin->checkedId()) {
    case ADMIN_EXT:
      user_if = 0;
      admin_noipsec = false;
      break;
    case ADMIN_INT:
      user_if = 1;
      admin_noipsec = true;
      break;
  }

  switch (bgUpdate->checkedId()) {
    case UPDATE_EXT:
      update_if = 0;
      update_noipsec = false;
      break;
    case UPDATE_INT:
      update_if = 1;
      update_noipsec = true;
      break;
  }

  profile->setNameVal(GW_NET_CONF, "USER_IF", QString::number(user_if));
  if (chkAdminEth2->isChecked())
    profile->setNameVal(GW_NET_CONF, "ADMIN_IF", QString::number(2));
  else
    profile->setNameVal(GW_NET_CONF, "ADMIN_IF", "");

  profile->setNameVal(GW_NET_CONF, "UPDATE_IF", QString::number(update_if));
  profile->setNameVal(GW_NET_CONF, "ADMIN_NOIPSEC", 
                                (admin_noipsec) ? yes_str : "");
  profile->setNameVal(GW_NET_CONF, "UPDATE_NOIPSEC", 
                                (update_noipsec) ? yes_str : "");

  profile->setNameVal(GW_NET_CONF, "ADMIN_GTW", 
                          (chkAdminGateway->isChecked()) ? yes_str : "");
      
  return true;
}

bool 
GwNetTab::loadCurrentProfile()
{
  QString tmp;
  int numUser, numAdm, numUpd, numIfs;
  tmp = profile->getNameVal(GW_NET_CONF, "IF_NUMBER", num_rx);
  numIfs = tmp.toInt();
  if (numIfs >=2 && numIfs <= 3)
    spbNumIfs->setValue(numIfs);

  tmp = profile->getNameVal(GW_NET_CONF, "UPDATE_IF", num_rx);
  numUpd = tmp.toInt();
  if (numUpd != 0 && numUpd != 1)
    numUpd = 0;
  if (!numUpd)
    btnUpdateExt->setChecked(true);
  else
    btnUpdateInt->setChecked(true);

  tmp = profile->getNameVal(GW_NET_CONF, "USER_IF", num_rx);
  numUser = tmp.toInt();
  if (numUser < 0 || numUser > numIfs)
    numUser = 0;

  switch (numUser) {
    case 0:
      btnAdminExt->setChecked(true);
      break;
    case 1:
      btnAdminInt->setChecked(true);
      break;
  }

  tmp = profile->getNameVal(GW_NET_CONF, "ADMIN_IF", num_rx);
  if (!tmp.isEmpty()) {
    numAdm = tmp.toInt();
    if (numAdm == 2) 
      chkAdminEth2->setChecked(true);
    else {
      chkAdminEth2->setChecked(false);
      qDebug("Warning: ignoring ADMIN_IF on interface != eth2");
    }
  }

  if (!profile->getNameVal(GW_NET_CONF, "ADMIN_NOIPSEC", yes_rx).isEmpty()){
    if (numUser == 0) 
      qDebug("Warning: ignoring ADMIN_NOIPSEC on external interface");
  }

  if (!profile->getNameVal(GW_NET_CONF, "UPDATE_NOIPSEC", yes_rx).isEmpty()){
    if (numUpd == 0) 
      qDebug("Warning: ignoring UPDATE_NOIPSEC on external interface");
  }

  tmp = profile->getNameVal(GW_NET_CONF, "ADMIN_GTW", yes_rx);
  chkAdminGateway->setChecked(!tmp.isEmpty());

  return true;
}

bool 
GwNetTab::initValues()
{
  return true;
}

void
GwNetTab::setNumIfs(int num)
{
  if (num == 3) {
    chkAdminEth2->setEnabled(true);
  } else {
    chkAdminEth2->setEnabled(false);
  }
}

void
GwNetTab::enableJails(int mask)
{
  if (mask != JAIL_CLIP) {
    // Client
    spbNumIfs->setMinimum(1);
    spbNumIfs->setValue(1);
    spbNumIfs->setMaximum(1);
  } else {
    // Gw
    spbNumIfs->setMinimum(2);
    spbNumIfs->setValue(2);
    spbNumIfs->setMaximum(3);
  }
}

// vi:sw=2:ts=2:et:co=80:
