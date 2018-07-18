// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file qtcommon.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#include "qtcommon.h"

#include <QApplication>
#include <QMessageBox>

void 
fatalError(const QString& err) 
{
  QMessageBox::critical(0, "Erreur interne", err,
			QMessageBox::Ok | QMessageBox::Default, 0, 0);
  QApplication::exit(1);
}

// vi:sw=2:ts=2:et:co=80:
