// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file ConfTab.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef CONFTAB_H
#define CONFTAB_H

#include <QFrame>

class ConfProfile;

class ConfTab : public QFrame
{
  Q_OBJECT
 
 public:
  ConfTab(QWidget *parent = NULL, ConfProfile *profile = NULL);
  virtual ~ConfTab() { };
  QString profilePath() const;
  bool isSwitchable() const;

 public slots:
  virtual bool saveCurrentProfile() = 0;
  virtual bool loadCurrentProfile() = 0;
  virtual bool initValues() = 0;

  virtual void showAdvanced(bool on);
  virtual void enable(bool on) { };
  virtual void enableJails(int mask) { };

  void setProfile(ConfProfile *prof);
 
 protected:
  bool advanced;
  bool switchable;
  ConfProfile *profile;
};

#endif // CONFTAB_H

// vi:sw=2:ts=2:et:co=80:
