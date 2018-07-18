// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file PkgTable.h
 * clip-install-config package table.
 * @author Olivier Levillain <clipos@ssi.gouv.fr>
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef _PKG_TABLE_H
#define _PKG_TABLE_H

#include <QFrame>
#include <QLayout>
#include <QLinkedList>

class QString;
class PkgTableItem;

class PkgTable : public QFrame
{
  Q_OBJECT

public:
  PkgTable(QWidget *parent, const QString &jail, const QString &path);
  virtual ~PkgTable();
  QString selected(void);
  bool pending(void);

private:
  QString jail;
  int nrows;
  QGridLayout *gl;
  QLinkedList<PkgTableItem *> pkgs;
};

#endif

// vi:sw=2:ts=2:et:co=80:
