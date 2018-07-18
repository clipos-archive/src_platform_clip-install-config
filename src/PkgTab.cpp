// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file PkgTab.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#include <QLayout>
#include <QLabel>
#include <QScrollArea>

#include "PkgTab.h"
#include "ConfProfile.h"
#include "PkgTable.h"


PkgTab::PkgTab(QWidget *parent, ConfProfile *profile, 
                  conffile_t file, const QString &name)
  : ConfTab(parent, profile), file(file)
{ 
  jail = name.toLower();
  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(5);
  pkgs = NULL;
  sa = new QScrollArea(this);
  sa->setBackgroundRole(QPalette::Background);
  //sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  //sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  vb->addWidget(new QLabel(QString("<b>Liste des paquetages optionnels "
                "pour ") + name + "</b>", this));
  vb->addWidget(new QLabel(QString("<font color=blue><b>Bleu</b> : "
                "sélectionné</font> / <b>Noir</b> : disponible "), this));
  vb->addWidget(sa, 1);
  vb->addSpacing(8);
  //setMinimumWidth(700);
}

bool 
PkgTab::saveCurrentProfile()
{
  if (!pkgs)
    return false;
  profile->setVal(file, pkgs->selected());
  return true;
}

bool 
PkgTab::loadCurrentProfile()
{
  /* Yeah, this sucks, but changing the widget is the only way I can make
   * the scrollarea refresh the viewport() widget.
   * Note that setWidget() automatically deletes the previous widget 
   * if any.
   */
  pkgs = new PkgTable(this, jail, profilePath() + "/conf/optional." + jail);
  sa->setWidget(pkgs);
  /* Needed if scrollarea is visible when we set the widget */
  pkgs->show();
  return true;
}

bool 
PkgTab::initValues()
{
  //pkgs->setValue("");
  return true;
}

// vi:sw=2:ts=2:et:co=80:
