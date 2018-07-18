// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file PkgTab.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef PKGTAB_H
#define PKGTAB_H

#include <QString>
#include "ConfTab.h"
#include "consts.h"

class PkgTable;
class QScrollArea;

class PkgTab : public ConfTab
{
  Q_OBJECT
 
 public:
  PkgTab(QWidget *parent, ConfProfile *profile,   
                  conffile_t file, const QString &name);

 private:
  conffile_t file;
  PkgTable *pkgs;
  QString jail;
  QScrollArea *sa;

 public slots:
  virtual bool saveCurrentProfile();
  virtual bool loadCurrentProfile();
  virtual bool initValues();
};

#endif // PKGTAB_H

// vi:sw=2:ts=2:et:co=80:
