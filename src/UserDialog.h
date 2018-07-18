// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file UserDialog.h
 * Based on clip-config:frmUsersAdd.
 * @author Olivier Levillain <clipos@ssi.gouv.fr>
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef USER_DIALOG_H
#define USER_DIALOG_H

#include <QDialog>
#include <QString>
#include <QThread>

class QButtonGroup;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QSlider;

class UserDialog : public QDialog
{
  Q_OBJECT

public:
  UserDialog(int jails, bool new_p, QString &line, bool *save);
  virtual ~UserDialog();

 private:
  int jails;
  QLineEdit *edtName;
  QLabel *lblPasswd;
  QLineEdit *edtPasswd;
  QGroupBox *btnGroup;
  QButtonGroup *btnType;
  QLabel *lblTotalSize;
  QSlider *sldTotalSize;
  QLabel *lblRMBSize;
  QLabel *lblRMHSize;
  QPushButton *btnReset;
  QSlider *sldRepartition;

  QPushButton *btnConfirm;
  QPushButton *btnCancel;

  QRadioButton *btnAdmin;
  QRadioButton *btnAudit;
  QRadioButton *btnUser;
  QRadioButton *btnPrivUser;
  QRadioButton *btnNomUser;

  QString &line;
  bool *save;

  int rmhSize;
  int rmbSize;

private slots:
  void readInput();
  void updateOk();
  void updateWidgets();
  void resetRepartition ();
  void updateValues();
};

#endif // USER_DIALOG_H

// vi:sw=2:ts=2:et:co=80:
