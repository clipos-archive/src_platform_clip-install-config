// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file UserTab.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef USERTAB_H
#define USERTAB_H

#include "ConfTab.h"
#include "consts.h"

class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;

class UserTab : public ConfTab
{
  Q_OBJECT
 
 public:
  UserTab(QWidget *parent, ConfProfile *profile);

 public slots:
  virtual bool saveCurrentProfile();
  virtual bool loadCurrentProfile();
  virtual bool initValues();
  virtual void enableJails(int mask);

 private slots:
  void addUser();
  void modUser();
  void delUser();
  void selectUser();

  bool readUser(const QString &line);
  QString writeUser(const QTreeWidgetItem *item) const;

 private:
  int jails;

  QTreeWidget *users;

  QPushButton *btnAdd;
  QPushButton *btnMod;
  QPushButton *btnDel;
};

#endif // USERTAB_H

// vi:sw=2:ts=2:et:co=80:
