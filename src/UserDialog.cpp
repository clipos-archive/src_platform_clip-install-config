// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file UserDialog.cpp
 * Based on clip-config:frmUsersAdd.
 * @author Olivier Levillain <clipos@ssi.gouv.fr>
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#include "UserDialog.h"
#include "consts.h"

#include <QButtonGroup>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExpValidator>
#include <QSlider>

static inline const long 
min(long a, long b)
{
  return (a < b) ? a : b;
}

enum {
  ADMIN = 0,
  AUDIT = 1,
  USER = 2,
  PRIV_USER = 3,
  NOMAD_USER = 4,
};

#define NAME_OFFSET   0
#define TYPE_OFFSET   1
#define PWD_OFFSET    2
#define RMH_OFFSET    3
#define RMB_OFFSET    4

const char *type_str[] = {
         "Administrateur",
         "Auditeur",
         "Utilisateur",
         "Utilisateur privilégié",
         "Utilisateur nomade"
};

const char *type_vals[] = {
         "admin",
         "audit",
         "user",
         "privuser",
         "nomad"
};
        
// UserDialog constructor
//----------------------

UserDialog::UserDialog(int jails, bool new_p, QString &input, bool *save)
  : jails(jails), line(input), save(save)
{
  QStringList vals = input.split(QRegExp("\\s+"));
  if (new_p) {
    setWindowTitle("Ajouter un compte");
  } else {
    setWindowTitle(QString("Modification du compte %1").arg(vals.at(0)));
  }
  *save = false;

  QVBoxLayout *vb = new QVBoxLayout(this);
  vb->setSpacing(7);

  QHBoxLayout *hbName = new QHBoxLayout;
  hbName->setSpacing(7);

  hbName->addWidget(new QLabel("Nom : ", this));
  hbName->addStretch(2);
  hbName->addWidget(edtName = new QLineEdit("", this));
  edtName->setValidator(new QRegExpValidator(username_rx, this));
  vb->addLayout(hbName);

  QHBoxLayout *hbPasswd = new QHBoxLayout;
  hbPasswd->setSpacing(7);
  lblPasswd = new QLabel("Mot de passe", this);
  hbPasswd->addWidget(lblPasswd);
  hbPasswd->addStretch(2);
  hbPasswd->addWidget(edtPasswd = new QLineEdit("", this));
  edtPasswd->setValidator(new QRegExpValidator(any_rx, this));
  vb->addLayout(hbPasswd);
  vb->addSpacing(10);

  QGridLayout *glType = new QGridLayout;
  glType->setVerticalSpacing(7);
  glType->setHorizontalSpacing(7);
  
  btnGroup = new QGroupBox("Type de compte", this);
  btnType = new QButtonGroup(this);
  btnType->setExclusive(true);

  int index = 0;
  btnAdmin = new QRadioButton(type_str[ADMIN], btnGroup);
  btnType->addButton(btnAdmin, ADMIN);
  glType->addWidget(btnAdmin, index / 2, index % 2);
  index++;

  btnAudit = new QRadioButton(type_str[AUDIT], btnGroup);
  btnType->addButton(btnAudit, AUDIT);
  glType->addWidget(btnAudit, index / 2, index % 2);
  index++;
  
  if (jails != JAIL_CLIP) {
    btnUser = new QRadioButton(type_str[USER], btnGroup);
    btnType->addButton(btnUser, USER);
    glType->addWidget(btnUser, index / 2, index % 2);
    index++;
  }
  
  btnPrivUser = new QRadioButton(type_str[PRIV_USER], btnGroup);
  btnType->addButton(btnPrivUser, PRIV_USER);
  glType->addWidget(btnPrivUser, index / 2, index % 2);
  index++;
  
  if (jails != JAIL_CLIP) {
    btnNomUser = new QRadioButton(type_str[NOMAD_USER], btnGroup);
    btnType->addButton(btnNomUser, NOMAD_USER);
    glType->addWidget(btnNomUser, index / 2, index % 2);
    index++;
  }


  if (jails != JAIL_CLIP)
    btnUser->setChecked(true);
  else
    btnAdmin->setChecked(true);

  btnGroup->setLayout(glType);
  vb->addWidget(btnGroup);

  if (jails != JAIL_CLIP) {
    QHBoxLayout *hbTotalSize = new QHBoxLayout;
    hbTotalSize->setSpacing(7);
    hbTotalSize->addWidget(new QLabel("Taille totale allouée : ", this));
    hbTotalSize->addStretch(2);
    hbTotalSize->addWidget (lblTotalSize = new QLabel ("", this));
    vb->addLayout (hbTotalSize);

    sldTotalSize = new QSlider(this);
    sldTotalSize->setMinimum(128);
    sldTotalSize->setMaximum(120*1024);
    sldTotalSize->setPageStep(16);
    sldTotalSize->setSliderPosition(4096);
    sldTotalSize->setOrientation(Qt::Horizontal);
    sldTotalSize->setTracking(true);
    vb->addWidget(sldTotalSize);
  }

  if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
    QHBoxLayout *hbRepartition = new QHBoxLayout;
    hbRepartition->setSpacing(7);
    hbRepartition->addWidget(new QLabel("Répartition : ", this));
    hbRepartition->addStretch(2);
    hbRepartition->addWidget(new QLabel ("RM_B = ", this));
    hbRepartition->addWidget(lblRMBSize = new QLabel ("", this));
    hbRepartition->addStretch(2);
    hbRepartition->addWidget(new QLabel ("RM_H = ", this));
    hbRepartition->addWidget(lblRMHSize = new QLabel("", this));
    hbRepartition->addStretch(2);
    hbRepartition->addWidget(btnReset = new QPushButton("RàZ", this));
    hbRepartition->addStretch(2);
    vb->addLayout(hbRepartition);

    sldRepartition = new QSlider(this); 
    sldRepartition->setMinimum(64);
    sldRepartition->setMaximum(sldTotalSize->value() - 64);
    sldRepartition->setPageStep(16);
    sldRepartition->setSliderPosition(sldTotalSize->value() / 2 );
    sldRepartition->setOrientation(Qt::Horizontal);
    sldRepartition->setTracking(true);
    vb->addWidget(sldRepartition);
  }

  QHBoxLayout *hbButtons = new QHBoxLayout;
  hbButtons->setSpacing(7);
  hbButtons->addStretch(2);
  hbButtons->addWidget(btnConfirm = new QPushButton(
                              (new_p) ? "Ajouter" : "Modifier", this));
  btnConfirm->setEnabled(false);
  hbButtons->addStretch(2);
  hbButtons->addWidget(btnCancel = new QPushButton("Annuler", this));
  hbButtons->addStretch(2);
  vb->addLayout(hbButtons);

  // Connections
  //------------

  connect(btnConfirm, SIGNAL(clicked()), this, SLOT(updateValues()));
  connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

  connect(edtName, SIGNAL(textChanged(const QString &)), 
                                          this, SLOT(updateOk()));
  connect(edtPasswd, SIGNAL(textChanged(const QString &)), 
                                          this, SLOT(updateOk()));

  if (jails != JAIL_CLIP) {
    connect(btnType, SIGNAL(buttonClicked(int)), this, SLOT(updateWidgets()));
    connect(sldTotalSize, SIGNAL(valueChanged(int)), 
                                           this, SLOT(updateWidgets()));
  }
  if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
    connect(sldRepartition, SIGNAL(valueChanged(int)), 
                                           this, SLOT(updateWidgets()));
    connect(btnReset, SIGNAL(clicked()), this, SLOT(resetRepartition()));
  }

  if (!new_p)
    readInput();
  updateWidgets();
}

UserDialog::~UserDialog()
{
}

/*************************************************************/
/*                      Private slots                        */
/*************************************************************/

void
UserDialog::readInput()
{
  QStringList vals = line.split(QRegExp("\\s+"));

  if (vals.at(RMB_OFFSET).isEmpty()) {
    qDebug("Incorrect user line: %s", line.toStdString().c_str());
    return;
  }

  QString name = vals.at(NAME_OFFSET);

  QString type = vals.at(TYPE_OFFSET);
  QRadioButton *btn;

  if (type == "user") {
    btn = btnUser;
  } else if (type == "privuser") {
    btn = btnPrivUser;
  } else if (type == "nomad") {
    btn = btnNomUser;
  } else if (type == "admin") {
    btn = btnAdmin;
  } else if (type == "audit") {
    btn = btnAudit;
  } else {
    qDebug("Unsupported type: %s", type.toStdString().c_str());
    return;
  }

  QString pwd = vals.at(PWD_OFFSET);

  bool ok;
  rmhSize = vals.at(RMH_OFFSET).toInt(&ok);
  if (!ok) {
    qDebug("Unsupported RMH size: %s", 
              vals.at(RMH_OFFSET).toStdString().c_str());
    return;
  }
  rmbSize = vals.at(RMB_OFFSET).toInt(&ok);
  if (!ok) {
    qDebug("Unsupported RMB size: %s", 
              vals.at(RMB_OFFSET).toStdString().c_str());
    return;
  }
  edtName->setText(name);
  edtPasswd->setText(pwd);
  btn->setChecked(true);
  if (jails != JAIL_CLIP) {
    sldTotalSize->setValue(rmhSize + rmbSize);
    if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
      sldRepartition->setValue(rmhSize);
    }
  }
  updateOk();
}

void 
UserDialog::updateOk() 
{
  bool newok = true;

  if (edtPasswd->text().isEmpty()) {
    newok = false;
  } 
  
  if (edtName->text().isEmpty()) {
    newok = false;
  }

  btnConfirm->setEnabled(newok);
}

void 
UserDialog::updateWidgets() 
{
  if ((jails != JAIL_CLIP) && (btnType->checkedId() == USER 
                  || btnType->checkedId() == PRIV_USER 
                  || btnType->checkedId() == NOMAD_USER)) {
    sldTotalSize->setEnabled(true);
    if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
      sldRepartition->setEnabled(true);
      btnReset->setEnabled(true);
    }

    int totalSize = sldTotalSize->value();
    if ((jails != JAIL_CLIP) && (btnType->checkedId() == USER 
                    || btnType->checkedId() == PRIV_USER 
                    || btnType->checkedId() == NOMAD_USER)) {
      if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
        rmbSize = sldRepartition->value();
        rmhSize = totalSize - rmbSize;
      } else {
        if ((jails & JAIL_RMH) != 0)
          rmhSize = totalSize;
          rmbSize = 0;
        if ((jails & JAIL_RMB) != 0)
          rmbSize = totalSize;
          rmhSize = 0;
      }
    } 

    lblTotalSize->setText(QString::number(totalSize));

    if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
      lblRMHSize->setText(QString::number(rmhSize));
      lblRMBSize->setText(QString::number(rmbSize));

      sldRepartition->setMinimum(64);
      sldRepartition->setMaximum(totalSize - 64);
    }
  } else {
    rmhSize = rmbSize = 0;
    if (jails != JAIL_CLIP) {
      sldTotalSize->setEnabled(false);
    }
    if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
      sldRepartition->setEnabled(false);
      btnReset->setEnabled(false);
    }
  }
}

void 
UserDialog::resetRepartition() 
{
  if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
    sldRepartition->setSliderPosition(sldTotalSize->value() / 2);
    updateWidgets();
  }
}

void 
UserDialog::updateValues() 
{
  // Préparation des données pour la requête
  if (!edtName->hasAcceptableInput()) {
    QMessageBox::warning (0, "Nom de compte invalide",
        "Veuillez entrer un nom de compte valide s'il vous plaît.",
        QMessageBox::Ok | QMessageBox::Default, 0, 0);
    return;
  }

  bool checked = true;
  const char *ptr;
  unsigned int i, l;
  ptr = edtPasswd->text().toStdString().c_str();
  l = edtPasswd->text().length();
  for (i = 0; i < l; i++) {
      if (!isascii(*(ptr+i)) || !isprint(*(ptr+i)) || isspace(*(ptr+i))) {
        checked = false;
        break;
      }
  }
  if (!checked) {
    QMessageBox::warning (0, "Saisie invalide",
                   "Le mot de passe saisit contient des caractères non "
                   "supportés.\nEn particulier, les caractères accentués et "
                   "les espaces ne sont pas autorisés.",
                   QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape, 0);
    return;
  }
    
  int totalSize = sldTotalSize->value();
  if ((jails != JAIL_CLIP) && (btnType->checkedId() == USER 
                  || btnType->checkedId() == PRIV_USER 
                  || btnType->checkedId() == NOMAD_USER)) {
    if ((jails & (JAIL_RMH|JAIL_RMB)) == (JAIL_RMH|JAIL_RMB)) {
      rmbSize = sldRepartition->value();
      rmhSize = totalSize - rmbSize;
    } else {
      if ((jails & JAIL_RMH) != 0) {
        rmhSize = totalSize;
        rmbSize = 0;
      } 
      if ((jails & JAIL_RMB) != 0) {
        rmbSize = totalSize;
        rmhSize = 0;
      }
    }
  }

  QString newline = QString("%1 %2 %3 %4 %5").arg(edtName->text())
                          .arg(type_vals[btnType->checkedId()])
                          .arg(edtPasswd->text())
                          .arg(rmhSize)
                          .arg(rmbSize);

  line = newline;
  *save = true;
  emit(accept());
}

// vi:sw=2:ts=2:et:co=80:
