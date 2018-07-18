// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file JailsTab.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef JAILSTAB_H
#define JAILSTAB_H

#include "ConfTab.h"
#include "consts.h"

class QRadioButton;
class QButtonGroup;
class QLabel;

class JailsTab : public ConfTab
{
  Q_OBJECT
 
 public:
  JailsTab(QWidget *parent, ConfProfile *profile);

 private:
  QButtonGroup *choices;
  QRadioButton *btnNoRm;
  QRadioButton *btnRmh;
  QRadioButton *btnRmb;
  QRadioButton *btnRmhRmb;

 signals:
  void jailsEnabled(int mask);

 public slots:
  virtual bool saveCurrentProfile();
  virtual bool loadCurrentProfile();
  virtual bool initValues();
};

#endif // JAILSTAB_H

// vi:sw=2:ts=2:et:co=80:
