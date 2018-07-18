// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file frmConfMain.h
 * clip-config main network configuration form header.
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef FRMCONFMAIN_H
#define FRMCONFMAIN_H

#include <QDialog>
#include <QLinkedList>
#include <QMap>
#include <QStringList>

class ConfProfile;
class ConfTab;

class DeviceTab;
class DownloadTab;
class GwNetTab;
class JailsTab;
class NetTab;
class PkgTab;
class UserTab;

class QPushButton;
class QListWidget;
class QListWidgetItem;
class QCheckBox;
class QTabWidget;
class QLabel;

typedef enum {
  TAB_JAILS = 0,
  TAB_GW_NET,
  TAB_DEVICES,
  TAB_USERS,
  TAB_NET,
  TAB_OPTIONAL_RMH,
  TAB_OPTIONAL_RMB,
  TAB_DOWNLOAD,
  TAB_MAX,
} TabIdx;

class frmConfMain : public QDialog
{
  Q_OBJECT

public:
  frmConfMain();

 private:
  QListWidget *profiles;
  ConfProfile *profile;
  QStringList knownProfiles;
  QListWidgetItem *currentProfile;

  QPushButton *btnChangeConfPath;
  QPushButton *btnAddProfile;
  QPushButton *btnDelProfile;
  QPushButton *btnSaveProfile;

  QPushButton *btnQuit;
  //QCheckBox   *chkAdvanced;

  QTabWidget *tabs;

  QLinkedList<ConfTab *> tabList;
  QString tabCaptions[TAB_MAX];
  bool tabPresent[TAB_MAX];
  int tabIndexes[TAB_MAX];

  JailsTab *jailsTab;
  GwNetTab *gwNetTab;
  DeviceTab *deviceTab;
  UserTab *userTab;
  NetTab *netTab;
  PkgTab *pkgTabRMH;
  PkgTab *pkgTabRMB;
  DownloadTab *downloadTab;

 protected:
  virtual void keyPressEvent(QKeyEvent* e);

 private:
  void listProfiles();
  void loadProfile();
  void updateProfile();
  bool savePending(QString action);
  
  void showTab(TabIdx num, ConfTab *tab);
  void hideTab(TabIdx num);
  void enableTab(TabIdx num, ConfTab *tab, bool on);

 signals:
  void setActive(const QString &);
  void jailsEnabled(int mask);

 private slots:
  void profileSelected(QListWidgetItem *);
  void changeConfPath();
  void saveProfile();
  void addProfile();
  void deleteProfile();
  
  void showAdvanced(bool);
  void enableJails(int mask);

  void quit();
};

#endif // FRMCONFMAIN_H

// vi:sw=2:ts=2:et:co=80:
