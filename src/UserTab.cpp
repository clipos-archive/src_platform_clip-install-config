// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file UserTab.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */


#include <QFile>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QStringList>
#include <QTreeWidget>

#include "UserTab.h"
#include "UserDialog.h"
#include "ConfProfile.h"

#define NAME_OFFSET   0
#define TYPE_OFFSET   1
#define PWD_OFFSET    2
#define RMH_OFFSET    3
#define RMB_OFFSET    4

static const int columnCount = 5;

UserTab::UserTab(QWidget *parent, ConfProfile *profile)
  : ConfTab(parent, profile)
{ 
  jails = JAIL_RMH | JAIL_RMB;

  QStringList cols;
  cols << "Nom" << "Type" << "Mot de passe" << "Espace RM_H" << "Espace RM_B";

  users = new QTreeWidget(this);
  users->setColumnCount(columnCount);
  users->setSelectionMode(QAbstractItemView::SingleSelection);
  
  QTreeWidgetItem *item = new QTreeWidgetItem(cols);
  for (int i = 0; i < columnCount; i++)
    item->setTextAlignment(i, Qt::AlignHCenter);
  users->setHeaderItem(item);

  connect(users, 
            SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 
            this, SLOT(selectUser()));

  btnAdd = new QPushButton("Ajouter", this);
  btnMod = new QPushButton("Modifier", this);
  btnMod->setEnabled(false);
  btnDel = new QPushButton("Supprimer", this);
  btnDel->setEnabled(false);

  connect(btnAdd, SIGNAL(clicked()), this, SLOT(addUser()));
  connect(btnMod, SIGNAL(clicked()), this, SLOT(modUser()));
  connect(btnDel, SIGNAL(clicked()), this, SLOT(delUser()));


  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(4);
  vb->addWidget(new QLabel("Liste des comptes :", this));
  vb->addSpacing(10);
  vb->addWidget(users);
  vb->addSpacing(10);

  QHBoxLayout *hbb = new QHBoxLayout;
  hbb->setSpacing(6);
  hbb->addStretch(5);
  hbb->addWidget(btnAdd);
  hbb->addSpacing(20);
  hbb->addWidget(btnMod);
  hbb->addSpacing(20);
  hbb->addWidget(btnDel);
  hbb->addStretch(5);

  vb->addLayout(hbb);
}

QString 
UserTab::writeUser(const QTreeWidgetItem *item) const
{
  QString type;

  if (item->text(1) == "Utilisateur") {
    type = "user";
  } else if (item->text(1) == "Utilisateur privilégié") {
    type = "privuser";
  } else if (item->text(1) == "Utilisateur nomade") {
    type = "nomad";
  } else if (item->text(1) == "Administrateur") {
    type = "admin";
  } else if (item->text(1) == "Auditeur") {
    type = "audit";
  } else {
    qDebug("Unsupported type: %s", item->text(1).toStdString().c_str());
    return QString("");
  }

  QStringList vals;
  vals << item->text(0) 
        << type 
        << item->text(2) 
        << item->text(3)
        << item->text(4);

  return vals.join(" ");
}


bool 
UserTab::saveCurrentProfile()
{
  QString value, tmp;
  for (int i = 0; i < users->topLevelItemCount(); i++) {
    tmp = writeUser(users->topLevelItem(i));
    if (!tmp.isEmpty())
      value += tmp;
      value += "\n";
  }
  profile->setVal(USERS_CONF, value);
  return true;
}

bool
UserTab::readUser(const QString &line)
{
  QStringList vals = line.split(QRegExp("\\s+"));
  QStringList cols;

  if (vals.at(RMB_OFFSET).isEmpty()) {
    qDebug("Incorrect user line: %s", line.toStdString().c_str());
    return false;
  }

  cols << vals.at(NAME_OFFSET);

  QString type = vals.at(TYPE_OFFSET);
  if (type == "user") {
    cols << "Utilisateur";
  } else if (type == "privuser") {
    cols << "Utilisateur privilégié";
  } else if (type == "nomad") {
    cols << "Utilisateur nomade";
  } else if (type == "admin") {
    cols << "Administrateur";
  } else if (type == "audit") {
    cols << "Auditeur";
  } else {
    qDebug("Unsupported type: %s", type.toStdString().c_str());
    return false;
  }

  cols << vals.at(PWD_OFFSET);

  uint tmp;
  bool ok;
  tmp = vals.at(RMH_OFFSET).toUInt(&ok);
  if (!ok) {
    qDebug("Unsupported RMH size: %s", 
              vals.at(RMH_OFFSET).toStdString().c_str());
    return false;
  }
  tmp = vals.at(RMB_OFFSET).toUInt(&ok);
  if (!ok) {
    qDebug("Unsupported RMB size: %s", 
              vals.at(RMB_OFFSET).toStdString().c_str());
    return false;
  }

  cols << vals.at(RMH_OFFSET) << vals.at(RMB_OFFSET);

  QTreeWidgetItem *item = new QTreeWidgetItem(users, cols);
  for (int i = 0; i < columnCount; i++)
    item->setTextAlignment(i, Qt::AlignHCenter);

  return true;
}

bool 
UserTab::loadCurrentProfile()
{
  bool ret = true;
  users->clear();

  QString val = profile->getVal(USERS_CONF);
  QStringList lst = val.split("\n");

  for (QStringList::const_iterator i = lst.constBegin(); 
            i != lst.constEnd(); i++) {
    if ((*i).isEmpty())
      continue;
    if (!readUser((*i)))
      ret = false;
  }

  for (int i = 0; i < columnCount; i++)
    users->resizeColumnToContents(i);
  return ret;
}

bool 
UserTab::initValues()
{
  return true;
}

void
UserTab::enableJails(int mask)
{
  jails = mask;
}

void
UserTab::addUser()
{
  QString values("");
  bool ok = false;
  UserDialog dialog(jails, true, values, &ok);
  dialog.exec();
  if (ok && !values.isEmpty())
    readUser(values);
}

void
UserTab::modUser()
{
  QModelIndex index = users->currentIndex();
  const QTreeWidgetItem *item = users->topLevelItem(index.row());
  if (!item)
    return;
  QString values = writeUser(item);

  bool ok = false;
  UserDialog dialog(jails, false, values, &ok);
  dialog.exec();
  if (!ok || values.isEmpty())
    return;

  if (readUser(values))
    delete users->takeTopLevelItem(index.row());
}

void
UserTab::delUser()
{
  QTreeWidgetItem *item = users->takeTopLevelItem(users->currentIndex().row());
  if (item)
    delete item;
}

void
UserTab::selectUser()
{
  btnDel->setEnabled(true);
  btnMod->setEnabled(true);
}

// vi:sw=2:ts=2:et:co=80:
