// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file ConfProfile.h
 * clip-config net profile header.
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef CONFPROFILE_H
#define CONFPROFILE_H

#include "consts.h"
#include <QStringList>
#include <ClipConfig/SimpleConfigFile.h>
#include <ClipConfig/ClipConfigFile.h>
#include <ClipConfig/ClipProfile.h>

class ConfProfile : public ClipProfile
{
 public:
  ConfProfile(const QString& profileName);
  bool createSubDirs();
  bool removeSubDirs();
  void writeProfileAs(const QString& clone);
  QString getPath() const {return path; };

 private:
  QStringList dirs;
  ConfigFile *newFile(conffile_t num);
};

#endif // CONFPROFILE_H

// vi:sw=2:ts=2:et:co=80: 
