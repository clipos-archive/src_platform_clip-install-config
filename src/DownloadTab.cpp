// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file DownloadTab.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */


#include <QLayout>

#include "DownloadTab.h"
#include "ConfProfile.h"
#include "DownloadSourcesFrame.h"

DownloadTab::DownloadTab(QWidget *parent, ConfProfile *profile)
  : ConfTab(parent, profile)
{ 
  jails = JAIL_CLIP;
  srcCLIP = new DownloadSourcesFrame(this, "clip", "clip4-rm-dpkg");
  srcRMH = new DownloadSourcesFrame(this, "rm_h", "clip4-rm-dpkg");
  srcRMB = new DownloadSourcesFrame(this, "rm_b", "clip4-rm-dpkg");
  // Vertical layout
  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(4);

  vb->addSpacing(5);
  vb->addWidget(srcCLIP);
  vb->addSpacing(10);
  vb->addWidget(srcRMH);
  vb->addSpacing(10);
  vb->addWidget(srcRMB);
  vb->addSpacing(10);

  vb->addStretch(2);

}

bool 
DownloadTab::saveCurrentProfile()
{

  profile->setVal(DOWNLOAD_CLIP, srcCLIP->writeContent());
  if (jails & JAIL_RMH) {
    profile->setVal(DOWNLOAD_RMH, srcRMH->writeContent());
  }
  if (jails & JAIL_RMB) {
    profile->setVal(DOWNLOAD_RMB, srcRMB->writeContent());
  }
  return true;
}

bool 
DownloadTab::loadCurrentProfile()
{
  QString val;

  val = profile->getVal(DOWNLOAD_CLIP);
  srcCLIP->readContent(val);

  val = profile->getVal(DOWNLOAD_RMH);
  srcRMH->readContent(val);

  val = profile->getVal(DOWNLOAD_RMB);
  srcRMB->readContent(val);

  return true;
}

bool 
DownloadTab::initValues()
{
  return true;
}

void
DownloadTab::enableJails(int mask)
{
  jails = mask;

  srcRMH->setEnabled((mask & JAIL_RMH));
  srcRMB->setEnabled((mask & JAIL_RMB));

  if (jails & (JAIL_RMH | JAIL_RMB))
    srcCLIP->setBase("clip4-rm-dpkg");
  else
    srcCLIP->setBase("clip4-gtw-dpkg");
}

// vi:sw=2:ts=2:et:co=80:
