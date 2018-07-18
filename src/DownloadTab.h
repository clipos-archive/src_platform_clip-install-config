// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file DownloadTab.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef DOWNLOADTAB_H
#define DOWNLOADTAB_H

#include "ConfTab.h"
#include "consts.h"

class DownloadSourcesFrame;

class DownloadTab : public ConfTab
{
  Q_OBJECT
 
 public:
  DownloadTab(QWidget *parent, ConfProfile *profile);

 private:
  int jails;
  DownloadSourcesFrame *srcCLIP;
  DownloadSourcesFrame *srcRMH;
  DownloadSourcesFrame *srcRMB;

 public slots:
  virtual bool saveCurrentProfile();
  virtual bool loadCurrentProfile();
  virtual bool initValues();
  virtual void enableJails(int mask);
};

#endif // DOWNLOADTAB_H

// vi:sw=2:ts=2:et:co=80:
