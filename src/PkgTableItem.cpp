// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file PkgTableItem.cpp
 * clip-config single item in pkg table.
 * @author Olivier Levillain <clipos@ssi.gouv.fr>
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#include "PkgTableItem.h"
#include <QLabel>
#include <QCheckBox>

PkgTableItem::PkgTableItem(QWidget *parent, const QString &txt, bool b)
  : QHBoxLayout(), txt(txt), changed(false)
{
  setSpacing(6);
  l = new QLabel(parent);
  cb = new QCheckBox(parent);
  cb->setChecked(b);
  l->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  if (b)
    l->setText(QString("<b><font color=blue>") 
        + txt + "</font></b>");
  else
    l->setText(QString("<b>") + txt + "</b>");
  connect(cb, SIGNAL(toggled(bool)), this, SLOT(toggle(bool)));
  
  addWidget(cb, 0, Qt::AlignCenter|Qt::AlignVCenter);
  addSpacing(8);
  addWidget(l, 1, Qt::AlignLeft|Qt::AlignVCenter);
}

PkgTableItem::~PkgTableItem(void)
{
  takeAt(0);
  takeAt(0);
  delete l;
  delete cb;
}

void PkgTableItem::toggle(bool b)
{
  changed = !changed;
  if (b)
    l->setText(QString("<b><font color=blue>") 
        + txt + "</font></b>");
  else
    l->setText(QString("<b>") + txt + "</b>");
}

bool PkgTableItem::set(void)
{
  return cb->isChecked();
}

// vi:sw=2:ts=2:et:co=80:
