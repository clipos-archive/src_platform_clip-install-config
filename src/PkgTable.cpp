// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file PkgTable.cpp
 * clip-install-config package table.
 * @author Olivier Levillain <clipos@ssi.gouv.fr>
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010-2012 ANSSI
 * @n
 * All rights reserved.
 */

#include <QLabel>
#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <iostream>

#include "PkgTableItem.h"
#include "PkgTable.h"
#include "consts.h"

PkgTable::PkgTable(QWidget *parent, const QString &jail, const QString &path)
  : QFrame(parent), jail(jail)
{    
  gl = new QGridLayout(this);
  gl->setHorizontalSpacing(6);
  gl->setVerticalSpacing(6);
  gl->setColumnMinimumWidth(2, 400);
  gl->setColumnStretch(2, 1);
  setLayout(gl);

  QString plist;
  QStringList args;
  args <<  "-j" << jail;
  // TODO : optional packages for gateways ?
  if (jail.startsWith("rm_")) {
    args << "-conf" << PkgListConfBase + "rm";
    args << "-m" << mirrorPath + "/clip4-rm-dpkg/rm/rm-apps-conf";
  } else {
    args << "-conf" << PkgListConfBase + "clip";
    args << "-m" << mirrorPath + "/clip4-rm-dpkg/clip/clip-apps-conf";
  }
  args << "-o" << path;
  QString tmp = mirrorPath;
  QString cache = QString("/tmp/%1.%2.cache")
            .arg(tmp.replace("/", "_")).arg(jail);
  args << "-cache" << cache;
  QProcess lister;

  lister.setReadChannel(QProcess::StandardOutput);
  lister.start(PkgListCommand, args);
  lister.waitForFinished();

  plist = QString(lister.readAll());
  QStringList _pkgs = plist.split('\n', QString::SkipEmptyParts);
  nrows = _pkgs.count();

  if (nrows) {
    int i = 1;
    for (QStringList::Iterator it = _pkgs.begin(); it != _pkgs.end() ; ++it) {
      QString str = *it;
      if (str.startsWith('*')) {
        str = str.right(str.length() - 1);
        QLabel *l = new QLabel(this);
        l->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        l->setText(QString("<b><u>") + str + "</u></b>");
        QHBoxLayout *h = new QHBoxLayout;
        h->addStretch(2);
        h->addWidget(l);
        h->addStretch(5);
        gl->addLayout(h, i, 0, 1, 3);
        i++;
        gl->addWidget(new QLabel("<b><font color=gray>Paquetage</font></b>", 
                              this), i, 0, 1, 2);
        gl->addWidget(new 
          QLabel("<b><i><font color=gray>Description</font></i></b>", 
                  this), i, 2);
      } else {
        bool b = (str.at(0) == '+');
        str = str.right(str.length() - 1);
        QString pname = str.section(' ', 0, 0);
        QString pdesc = str.section(' ', 1);
        PkgTableItem *it = new PkgTableItem(this, pname, b);
        gl->addLayout(it, i, 0);
        pkgs.push_front(it);
        gl->addItem(new QSpacerItem(5, 0), i, 1);
        QLabel *l = new QLabel(QString("<i>") + pdesc + "</i>", this);
        l->setWordWrap(true);
        gl->addWidget(l, i, 2);
      }
      i++;
    }
    gl->setColumnStretch(0, 1);
    gl->setColumnStretch(1, 1);
    gl->setColumnStretch(2, 2);
  } else {
    gl->addWidget(new QLabel("<i>Aucun paquetage optionnel n'est "
                         "disponible pour " + jail.toUpper() + ".</i>", this));
  }
}

PkgTable::~PkgTable()
{
  QLayoutItem *child;
  while ((child = gl->takeAt(0)) != 0) {
    delete child;
  }
  pkgs.clear();
}

QString
PkgTable::selected(void)
{
  QString toSave;
  for (QLinkedList<PkgTableItem *>::iterator it  = pkgs.begin(); 
                        it != pkgs.end(); ++it) {
    if ((*it)->set()) {
      toSave.append((*it)->getPkgName());
      toSave.append("\n");
    }
    (*it)->saved();
  }
  return toSave;
}

bool 
PkgTable::pending(void)
{
  for (QLinkedList<PkgTableItem *>::const_iterator it  = pkgs.constBegin(); 
                        it != pkgs.constEnd(); ++it) {
    if ((*it)->pending())
      return true;
  }
  return false;
}

// vi:sw=2:ts=2:et:co=80:
