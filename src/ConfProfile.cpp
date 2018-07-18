// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file ConfProfile.cpp
 * clip-config net profile. 
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#include "consts.h"
#include "ConfProfile.h"
#include <ClipConfig/CCException.h>

#include <QDir>

ConfigFile *
ConfProfile::newFile(conffile_t num) 
{
  ConfigFile *f;
  switch (num) {
    case OPTIONAL_RMB:
    case OPTIONAL_RMH:
    case USERS_CONF:
    case DOWNLOAD_CLIP:
    case DOWNLOAD_RMB:
    case DOWNLOAD_RMH:
      f = new SimpleConfigFile();
      break;

    case GW_NET_CONF:
    case NET_CONF:
      /* No quotes in that one, for some stupid historical reason... */
      f = new ClipConfigFile();
      break;
    case PRINTERS_CONF:
    case SOUND_CONF:
    case JAILS_CONF:
    case USBKEYS_CONF:
      f = new ClipConfigFile();
      f->setQuotes("\"");
      break;

    default:
      throw CCException(QString("ConfProfile had to deal with "
                                "an unexpected file number: %1").arg(num));
  }

  if (!addFile(num, f))
      throw CCException(QString("ConfProfile already has a file at index "
                                " %1.").arg(num));
  return f;
}

ConfProfile::ConfProfile(const QString& profileName)
  : ClipProfile(profileName, confBaseDir)
{
  ConfigFile *f;
  dirs << "params" 
        << "params/conf.d" 
        << "params/ike2_cert"
        << "params/update_keys"
        << "params/usb_keys";
  dirs << "conf"
        << "conf/cert"
        << "conf/clip_download"
        << "conf/ike2_cert"
        << "conf/netconf.d"
        << "conf/netconf.d/default";

  for (int i = 0; i < N_CONFFILES; i++) {
    f = newFile((conffile_t)i);
    f->readFile(path + "/" + filenames[i]);
  }
}

#define _mkdir(path) (dir.exists(path) || dir.mkdir(path))

bool ConfProfile::createSubDirs()
{
  QDir dir;
  QStringList::const_iterator it;
  for (it = dirs.constBegin(); it != dirs.constEnd(); it++) {
    if (!_mkdir(path + "/" + (*it)))
      return false;
  }
  return true;
}

bool ConfProfile::removeSubDirs()
{
  QDir dir;
  QStringList::const_iterator it = dirs.constEnd();
  for (;;) {
    --it;
    if (!dir.rmdir(path + "/" + (*it)))
      return false;
    if (it == dirs.constBegin())
      break;
  }
  return true;
}

void ConfProfile::writeProfileAs(const QString &clone)
{
  ClipProfile::writeProfileAs(clone, confBaseDir);
}
// vi:sw=2:ts=2:et:co=80: 
