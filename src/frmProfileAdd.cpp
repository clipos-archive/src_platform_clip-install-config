// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file frmProfileAdd.cpp
 * clip-install-config new profile form.
 * @author Olivier Levillain <clipos@ssi.gouv.fr>
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2012 ANSSI
 * @n
 * All rights reserved.
 */

#include "consts.h"
#include "frmProfileAdd.h"

#include <QCheckBox>
#include <QDir>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExpValidator>



// frmProfileAdd constructor
//-----------------------

frmProfileAdd::frmProfileAdd(bool pendingChanges, const QString& currentProfile)
  : QDialog (0, Qt::Dialog | Qt::SubWindow),
    newName(QString::null), result(ADD_CANCEL)
{
  setWindowTitle("Ajout de profil");
  //setWindowIcon(QIcon(ICONPATH"applications-internet.png"));

  edtNewName = new QLineEdit("", this);
  edtNewName->setValidator(new QRegExpValidator (profilename_rx, this));
   
  chkSave = 0;

  QPushButton* btnOK = new QPushButton(//QIcon(ICONPATH"dialog-ok.png"),
                                       "OK", this);
  connect(btnOK, SIGNAL(clicked()), this, SLOT(OKPressed()));

  QPushButton* btnCancel = new QPushButton(//QIcon(ICONPATH"dialog-cancel.png"), 
                                           "Annuler", this);
  connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

  QHBoxLayout* layButtons = new QHBoxLayout;
  layButtons->setSpacing(4);

  layButtons->addSpacerItem(new QSpacerItem(50, 0, 
                  QSizePolicy::Expanding, QSizePolicy::Minimum));
  layButtons->addWidget(btnOK);
  layButtons->addSpacerItem(new QSpacerItem(2, 0, 
                  QSizePolicy::Expanding, QSizePolicy::Minimum));
  layButtons->addWidget(btnCancel);
  layButtons->addItem(new QSpacerItem(4, 0, 
                  QSizePolicy::Expanding, QSizePolicy::Minimum));

  QVBoxLayout* lv = new QVBoxLayout(this);
  lv->setSpacing(4);

  lv->addSpacerItem(new QSpacerItem(0, 10, 
                  QSizePolicy::Minimum, QSizePolicy::Expanding));
  lv->addWidget(new QLabel("Nom du nouveau profil :", this));
  lv->addWidget(edtNewName);

  if (pendingChanges) {
    lv->addSpacerItem(new QSpacerItem(0, 10, 
                  QSizePolicy::Minimum, QSizePolicy::Expanding));
    lv->addWidget(new QLabel("Attention, le profil en cours d'édition "
                      "contient des modifications non sauvegardées.", this));

    chkSave = new QCheckBox("Sauvegarder le profil " + currentProfile + 
                                " avant la création du nouveau profil", this);
    chkSave->setChecked(false);
    lv->addWidget(chkSave);
  }

  lv->addSpacerItem(new QSpacerItem(0, 10, 
                  QSizePolicy::Minimum, QSizePolicy::Expanding));
  lv->addLayout(layButtons);
}

void 
frmProfileAdd::OKPressed() 
{
  QString profileName = edtNewName->text();

  if (!profilename_rx.exactMatch(profileName)) {
    QMessageBox::warning (0, "Nom de profil incorrect",
			  "Le nom " + profileName + " n'est pas un nom de profil valide.",
			  QMessageBox::Ok | QMessageBox::Default, 0, 0);
    return;
  }
    
  QDir dir;
  QString path = confBaseDir + "/" + profileName;
  
  if (dir.exists(path)) {
    QMessageBox::warning (0, "Nom de profil existant",
			  "Le profil " + profileName + " existe déjà. Pour l'écraser, "
        "veuillez d'abord le supprimer.",
			  QMessageBox::Ok | QMessageBox::Default, 0, 0);
    return;
  }
    
  newName = profileName;
  result = ADD_OK | ((chkSave != 0 && chkSave->isChecked()) ? 
                                        ADD_SAVE_BEFORE : ADD_DONTSAVE);
  accept();
}

// vi:sw=2:ts=2:et:co=80:
