// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file GwNetTab.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef GWNETTAB_H
#define GWNETTAB_H

#include "ConfTab.h"
#include "consts.h"

class QButtonGroup;
class QRadioButton;
class QCheckBox;
class QSpinBox;

class GwNetTab : public ConfTab
{
  Q_OBJECT
 
 public:
  GwNetTab(QWidget *parent, ConfProfile *profile);

 private:
  QSpinBox *spbNumIfs;

  QButtonGroup *bgAdmin;
  QButtonGroup *bgUpdate;

  QRadioButton *btnAdminExt;
  QRadioButton *btnAdminInt;

  QRadioButton *btnUpdateExt;
  QRadioButton *btnUpdateInt;
  
  QCheckBox *chkAdminGateway;
  QCheckBox *chkAdminEth2;

 public slots:
  virtual bool saveCurrentProfile();
  virtual bool loadCurrentProfile();
  virtual bool initValues();
  virtual void enableJails(int mask);

 private slots:
  void setNumIfs(int num);

 signals:
  void numIfsChanged(int num);
};

#endif // GWNETTAB_H

// vi:sw=2:ts=2:et:co=80:
