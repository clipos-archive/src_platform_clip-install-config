// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file JailsTab.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */


#include <QButtonGroup>
#include <QLabel>
#include <QLayout>
#include <QRadioButton>

#include "JailsTab.h"
#include "ConfProfile.h"

JailsTab::JailsTab(QWidget *parent, ConfProfile *profile)
  : ConfTab(parent, profile)
{ 
  choices = new QButtonGroup();
  choices->setExclusive(true);

  btnNoRm = new QRadioButton("Pas de cages RM (passerelle)", this);
  choices->addButton(btnNoRm, JAIL_CLIP);

  btnRmh = new QRadioButton("Mononiveau - niveau haut uniquement", this);
  choices->addButton(btnRmh, JAIL_CLIP|JAIL_RMH);

  btnRmb = new QRadioButton("Mononiveau - niveau bas uniquement", this);
  choices->addButton(btnRmb, JAIL_CLIP|JAIL_RMB);

  btnRmhRmb = new QRadioButton("Biniveau - niveaux haut et bas", this);
  choices->addButton(btnRmhRmb, JAIL_CLIP|JAIL_RMH|JAIL_RMB);

  connect(choices, SIGNAL(buttonClicked(int)), 
                                  this, SIGNAL(jailsEnabled(int)));

  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(4);

  vb->addWidget(new QLabel("<b><u>Choix de la configuration installée "
                         "(nombre de cages RM)</u> :</b>", this));
  vb->addSpacing(10);

  vb->addWidget(btnNoRm);
  vb->addSpacing(10);
  vb->addWidget(btnRmh);
  vb->addSpacing(10);
  vb->addWidget(btnRmb);
  vb->addSpacing(10);
  vb->addWidget(btnRmhRmb);
  vb->addSpacing(10);
  vb->addStretch(5);
}

bool 
JailsTab::saveCurrentProfile()
{
  QStringList jails;
  int id = choices->checkedId();
  if (id & JAIL_RMH)
    jails << "rm_h";
  if (id & JAIL_RMB)
    jails << "rm_b";
  profile->setNameVal(JAILS_CONF, "CLIP_JAILS", jails.join(" "));
  profile->setNameVal(JAILS_CONF, "CLIP_LAYOUT", "mixborder");

  return true;
}

bool 
JailsTab::loadCurrentProfile()
{
  QString val = profile->getNameVal(JAILS_CONF, "CLIP_JAILS", any_rx);
  int id = JAIL_CLIP;
  if (val.contains("rm_h"))
    id |= JAIL_RMH;
  if (val.contains("rm_b"))
    id |= JAIL_RMB;

  switch (id) {
    case JAIL_CLIP:
      btnNoRm->setChecked(true); 
      break;
    case JAIL_CLIP|JAIL_RMH:
      btnRmh->setChecked(true); 
      break;
    case JAIL_CLIP|JAIL_RMB:
      btnRmb->setChecked(true); 
      break;
    case JAIL_CLIP|JAIL_RMH|JAIL_RMB:
      btnRmhRmb->setChecked(true); 
      break;
    default:
      qDebug("WTF? jails id is %d", id);
      return false;
  }
  emit(jailsEnabled(id));
  return true;
}

bool 
JailsTab::initValues()
{
  return true;
}

// vi:sw=2:ts=2:et:co=80:
