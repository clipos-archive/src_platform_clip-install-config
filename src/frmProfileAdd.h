// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file frmProfileAdd.h
 * clip-config new network profile form header.
 * @author Olivier Levillain <clipos@ssi.gouv.fr>
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2012 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef FRMPROFILEADD_H
#define FRMPROFILEADD_H

#include <QDialog>
#include <QString>

class QCheckBox;
class QLineEdit;


class frmProfileAdd : public QDialog
{
  Q_OBJECT

public:
  frmProfileAdd(bool pendingChanges, const QString& currentProfile);
  QString newName;
  int result;

 private:
   QCheckBox *chkSave;
   QLineEdit *edtNewName;

 private slots:
   void OKPressed();
};



#endif // FRMPROFILEADD_H

// vi:sw=2:ts=2:et:co=80:
