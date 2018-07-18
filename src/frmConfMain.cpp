// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file frmConfMain.cpp
 * clip-config main network configuration form.
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#include <QApplication>
#include <QCheckBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QProcess>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>

#include <unistd.h>

#include "ConfProfile.h"
#include "ConfTab.h"
#include <ClipConfig/CCException.h>
#include "qtcommon.h"
#include "consts.h"
#include "frmProfileAdd.h"

#include "JailsTab.h"
#include "GwNetTab.h"
#include "DeviceTab.h"
#include "DownloadTab.h"
#include "NetTab.h"
#include "PkgTab.h"
#include "UserTab.h"

#include "frmConfMain.h"

frmConfMain::frmConfMain()
  : profile(NULL), currentProfile(NULL)
{
  setWindowTitle("Gestion des configurations d'installation");
  setMinimumHeight(550);
  setMinimumWidth(700);

  tabCaptions[TAB_JAILS] = "Cages RM";
  tabCaptions[TAB_GW_NET] = "Configuration passerelle";
  tabCaptions[TAB_DEVICES] = "Gestion des périphériques";
  tabCaptions[TAB_USERS] = "Comptes utilisateurs";
  tabCaptions[TAB_NET] = "Paramètres réseau";
  tabCaptions[TAB_OPTIONAL_RMH] = "Paquetages optionnels RM_H";
  tabCaptions[TAB_OPTIONAL_RMB] = "Paquetages optionnels RM_B";
  tabCaptions[TAB_DOWNLOAD] = "Sources de mises à jour";

  for (int i = 0; i < TAB_MAX; i++) {
    tabIndexes[i] = 0;
    tabPresent[i] = false;
  }

  /****** Profile selection *******/

  profiles = new QListWidget(this);
  profiles->setMinimumWidth(120);
  profiles->setMaximumWidth(150);

  btnChangeConfPath = new QPushButton("Répertoire de profils", this);
  btnChangeConfPath->setDefault(false);
  btnChangeConfPath->setAutoDefault(false);
  btnChangeConfPath->setToolTip("Configurer le chemin "
                                  "du répertoire de profils");
  
  btnAddProfile = new QPushButton("+", this);
  btnAddProfile->setDefault(false);
  btnAddProfile->setAutoDefault(false);
  btnAddProfile->setToolTip("Créer un nouveau profil (initialement\n"
                            "une copie du profil courant)");
  
  btnDelProfile = new QPushButton("-", this);
  btnDelProfile->setDefault(false);
  btnDelProfile->setAutoDefault(false);
  btnDelProfile->setToolTip("Supprimer le profil courant");

  btnSaveProfile = new QPushButton("Sauver", this);
  btnSaveProfile->setDefault(false);
  btnSaveProfile->setAutoDefault(false);
  btnSaveProfile->setToolTip("Sauver le profil courant");

  connect(profiles, SIGNAL(currentItemChanged(QListWidgetItem *, 
                                                      QListWidgetItem *)), 
                    this, SLOT(profileSelected(QListWidgetItem *))); 
  connect(this, SIGNAL(jailsEnabled(int)), this, SLOT(enableJails(int)));
   
  connect(btnAddProfile, SIGNAL(clicked()), 
                    this, SLOT(addProfile()));
  connect (btnDelProfile, SIGNAL(clicked()), 
                    this, SLOT(deleteProfile()));
  connect(btnSaveProfile, SIGNAL(clicked()), 
                    this, SLOT(saveProfile()));
  connect(btnChangeConfPath, SIGNAL(clicked()), 
                    this, SLOT(changeConfPath()));

  /****** Bottom ******/
  //chkAdvanced = new QCheckBox("Options avancées", this);
  //chkAdvanced->setToolTip("Montrer les options avancées");
  btnQuit = new QPushButton ("Quitter", this);
  btnQuit->setDefault(false);
  btnQuit->setAutoDefault(false);

  connect(btnQuit, SIGNAL(clicked()), this, SLOT(quit()));
  //connect(chkAdvanced, SIGNAL(toggled(bool)), this, SLOT(showAdvanced(bool)));

  /****** Tabs ******/

  tabs = new QTabWidget(this);
  tabList.clear();

  jailsTab = new JailsTab(this, profile);
  tabList.push_back(jailsTab);
  showTab(TAB_JAILS, jailsTab);
  connect(jailsTab, SIGNAL(jailsEnabled(int)), this, SIGNAL(jailsEnabled(int)));

  gwNetTab = new GwNetTab(this, profile);
  tabList.push_back(gwNetTab);
  showTab(TAB_GW_NET, gwNetTab);

  deviceTab = new DeviceTab(this, profile);
  tabList.push_back(deviceTab);
  showTab(TAB_DEVICES, deviceTab);

  userTab = new UserTab(this, profile);
  tabList.push_back(userTab);
  showTab(TAB_USERS, userTab);

  netTab = new NetTab(this, profile);
  tabList.push_back(netTab);
  showTab(TAB_NET, netTab);

  pkgTabRMH = new PkgTab(this, profile, OPTIONAL_RMH, "RM_H");
  tabList.push_back(pkgTabRMH);
  showTab(TAB_OPTIONAL_RMH, pkgTabRMH);
    
  pkgTabRMB = new PkgTab(this, profile, OPTIONAL_RMB, "RM_B");
  tabList.push_back(pkgTabRMB);
  showTab(TAB_OPTIONAL_RMB, pkgTabRMB);
    
  downloadTab = new DownloadTab(this, profile);
  tabList.push_back(downloadTab);
  showTab(TAB_DOWNLOAD, downloadTab);

  for (QLinkedList<ConfTab *>::iterator it = tabList.begin(); 
                                      it != tabList.end(); it++)
    connect(this, SIGNAL(jailsEnabled(int)), (*it), SLOT(enableJails(int)));

  /****** Layout ******/
  QVBoxLayout *vbLeft = new QVBoxLayout;
  vbLeft->addSpacing(10);
  vbLeft->addWidget(new QLabel("<b>Liste des profils</b>", this));
  vbLeft->addSpacing(10);
  vbLeft->addWidget(profiles);
  vbLeft->addSpacing(10);
  vbLeft->addWidget(btnChangeConfPath);
  vbLeft->addSpacing(10);

  QHBoxLayout *hbTop = new QHBoxLayout;
  hbTop->setSpacing(4);
  hbTop->addSpacing(10);
  hbTop->addLayout(vbLeft);
  hbTop->addSpacing(10);
  hbTop->addWidget(tabs);
  hbTop->addSpacing(10);

  QHBoxLayout *hbBottom = new QHBoxLayout;
  hbBottom->setSpacing(4);
  hbBottom->addStretch(2);
  hbBottom->addSpacing(20);
  //hbBottom->addWidget(chkAdvanced);
  //hbBottom->addSpacing(10);
  hbBottom->addWidget(btnAddProfile);
  hbBottom->addSpacing(10);
  hbBottom->addWidget(btnDelProfile);
  hbBottom->addSpacing(30);
  hbBottom->addWidget(btnSaveProfile);
  hbBottom->addSpacing(20);
  hbBottom->addWidget(btnQuit);
  hbBottom->addStretch(2);


  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(4);
  vb->addLayout(hbTop);
  vb->addSpacing(10);
  vb->addLayout(hbBottom);

  /****** Initialization ******/
  
  listProfiles();
  loadProfile();
  //showAdvanced(chkAdvanced->isChecked());
}

/*************************************************************/
/*                    Protected slots                        */
/*************************************************************/

void 
frmConfMain::keyPressEvent(QKeyEvent* e)
{
  if (e->key() == Qt::Key_Escape) {
    e->accept();
    quit();
  }
}

/*************************************************************/
/*                      Private slots                        */
/*************************************************************/

void
frmConfMain::changeConfPath(void)
{
  QString tmp = QFileDialog::getExistingDirectory(this,
                  "Chemin du répertoire de profils", 
                  confBaseDir);

  if (tmp.isEmpty())
    return;

  confBaseDir = tmp;

  listProfiles();
  loadProfile();
}

void
frmConfMain::profileSelected(QListWidgetItem *item)
{
  if (!item || currentProfile == item)
    return;

  for (QLinkedList<ConfTab *>::iterator it = tabList.begin(); 
                                      it != tabList.end(); it++) {
    if (!(*it)->isSwitchable()) {
      QMessageBox::warning (0, "Edition en cours",
          "Le profil courant est en cours d'édition.\n"
          "Veuillez fermer les applications externes ouvertes\n"
          "sur ce profil avant de passer à un autre profil.",
          QMessageBox::Ok | QMessageBox::Default, 0, 0);

      profiles->setCurrentItem(currentProfile, 
                              QItemSelectionModel::ToggleCurrent);
      return;
    }
  }

  updateProfile();
  if (!profile || !profile->pendingChanges() ||
      savePending("Voulez-vous les sauvegarder avant d'éditer "
                         "un autre profil ?")) {
    loadProfile();
  } else {
    if (currentProfile)
      profiles->setCurrentItem(currentProfile);
  }
}

void
frmConfMain::saveProfile()
{
  updateProfile();
  profile->writeProfile();
}

void
frmConfMain::addProfile()
{
  updateProfile();
  if (!profile) {
    QMessageBox::warning(0, "Création impossible",
          "Aucun profil n'est actuellement sélectionné. La création de\n"
          "profil n'est possible à ce stade que par copie d'un profil\n"
          "existant.",
          QMessageBox::Ok | QMessageBox::Default, 0, 0);
    return;
  }

  frmProfileAdd dialog(profile->pendingChanges(), currentProfile->text());

  if ((dialog.exec () == Accepted) && ((dialog.result & ADD_OK) != 0)) {
    if (dialog.result & ADD_SAVE_BEFORE)
      profile->writeProfile();

    QString oldPath = confBaseDir + "/" + currentProfile->text();
    QString newPath = confBaseDir + "/" + dialog.newName;

    QStringList args;
    args << "-r" << oldPath << newPath;
    if (QProcess::execute("cp", args)) {
      QMessageBox::warning(0, "Echec de la copie", 
          "La copie de profil " + currentProfile->text() + 
          "vers " + dialog.newName + "a échoué.",
          QMessageBox::Ok | QMessageBox::Default, 0, 0);
      return;
    }

    profiles->addItem(dialog.newName);
    profiles->setCurrentRow(profiles->count() - 1);

    loadProfile();
  }
}

void
frmConfMain::deleteProfile()
{
  if (!currentProfile)
    return;
  QString pname = currentProfile->text();
  if (pname.isEmpty())
    throw CCException("deleteProfile: empty name");

  if (QMessageBox::question (0, "Supprimer un profil ?",
           QString ("Êtes-vous certain de vouloir supprimer le profil ")
           + pname + " ?",
           QMessageBox::Yes | QMessageBox::Default,
           QMessageBox::No | QMessageBox::Escape) == QMessageBox::Yes) {

    QListWidgetItem *item = profiles->takeItem(profiles->currentRow());

    if (!profile->deleteProfile()) {
      QMessageBox::warning (0, "Erreur lors de la suppression",
          "Le profil " + pname + " n'a pas pu être complètement effacé.\n"
          "Il vous faudra supprimer les fichiers restants manuellement.\n"
          "Les profils sont stockés dans le répértoire " 
          + confBaseDir + ".",
          QMessageBox::Ok | QMessageBox::Default, 0, 0);
    }
    delete item;
    profiles->setCurrentRow(0);
    loadProfile();
  }
}

void
frmConfMain::showAdvanced(bool on)
{
  for (QLinkedList<ConfTab *>::iterator it = tabList.begin(); 
                                      it != tabList.end(); it++)
    (*it)->showAdvanced(on);
}

void
frmConfMain::quit()
{
  updateProfile();
  if (!profile) {
    QApplication::exit(0);
    return;
  }

  if (!profile->pendingChanges() ||
        savePending("Voulez-vous les sauvegarder avant de quitter ?")) {
    delete(profile);
    QApplication::exit(0);
  }
}

void
frmConfMain::enableJails(int mask)
{
  enableTab(TAB_OPTIONAL_RMH, pkgTabRMH, (mask & JAIL_RMH));
  enableTab(TAB_OPTIONAL_RMB, pkgTabRMB, (mask & JAIL_RMB));
  enableTab(TAB_GW_NET, gwNetTab, (mask == JAIL_CLIP));
}

/*************************************************************/
/*                          Private                          */
/*************************************************************/
void
frmConfMain::listProfiles()
{
  QDir d(confBaseDir, QString::null, QDir::Name, QDir::Dirs);

  if (!d.exists()) {
   QMessageBox::warning (0, "Erreur interne",
       "Le répertoire " + confBaseDir + " n'existe pas.\n",
       QMessageBox::Ok | QMessageBox::Default, 0, 0);
   return;
  }

  QStringList candidates = d.entryList();
  QStringList::iterator i;

  profiles->clear();
  for (i = candidates.begin(); i != candidates.end(); ++i) {
    if (profilename_rx.exactMatch(*i)) {
      profiles->addItem(*i);
    }
  }
  if (profiles->count() > 0)
    profiles->setCurrentRow(0);
}

void
frmConfMain::loadProfile()
{
  tabs->setCurrentIndex(0);

  if (profile) {
    delete profile;
    profile = NULL;
  }

  // Empty profiles directory
  if (!profiles->count())
    return;

  currentProfile = profiles->currentItem();
  QString pname = currentProfile->text();

  if (pname.isEmpty())
    throw CCException("loadProfile: empty profile name");

  profile = new ConfProfile(pname);

  QLinkedList<ConfTab *>::iterator it;

  for (it = tabList.begin(); it != tabList.end(); ++it) 
    (*it)->setProfile(profile);

  for (it = tabList.begin(); it != tabList.end(); ++it) 
    (*it)->loadCurrentProfile();

  /* When parameters are added to the config, the program might ask to
     save pending changes the user has not made. To avoid this, we
     preemptively save the profile we just loaded. This way, the
     semantics is left unchanged but the parameters are all saved. */
  saveProfile();
}


void
frmConfMain::updateProfile()
{
  if (!profile)
    return;

  QLinkedList<ConfTab *>::iterator it;
  for (it = tabList.begin(); it != tabList.end(); it++) 
    (*it)->saveCurrentProfile();
}

bool
frmConfMain::savePending(QString question)
{
  QString msg1("Le profil "+ profile->getProfileName() +
            " contient des modifications non sauvegardées.\n");

  int answer = QMessageBox::question(0, "Sauver les modifications ?", 
              msg1 + question,
              QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
              QMessageBox::Cancel | QMessageBox::Escape);
  switch (answer) {
    case QMessageBox::Yes:
      saveProfile();
      return true;
    case QMessageBox::No:
      return true;
    default:
      return false;
  }
}

/* 
 * Yeah, I need all this junk to insert / remove tabs in the right 
 * order... Or else I missed something...
 */
inline void
frmConfMain::showTab(TabIdx num, ConfTab *tab)
{
  if (!tabPresent[num]) {
    tabs->insertTab(tabIndexes[num], tab, tabCaptions[num]);
    tabPresent[num] = true;
    for (int _i = num + 1; _i < TAB_MAX; _i++)
      tabIndexes[_i] += 1;
  }
}

inline void
frmConfMain::hideTab(TabIdx num)
{
  if (tabPresent[num]) {
    tabs->removeTab(tabIndexes[num]);
    tabPresent[num] = false;
    for (int _i = num + 1; _i < TAB_MAX; _i++)
      tabIndexes[_i] -= 1;
  }
}

inline void
frmConfMain::enableTab(TabIdx num, ConfTab *tab, bool on)
{
  if (on)
    showTab(num, tab);
  else
    hideTab(num);
  tab->setEnabled(on);
}

// vi:sw=2:ts=2:et:co=80:
