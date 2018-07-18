// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file consts.h
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#ifndef COMMON_H
#define COMMON_H

#include <QRegExp>
#include <QString>


// frmConfAdd
#define ADD_CANCEL 0x00
#define ADD_OK 0x01

#define ADD_DONTSAVE 0x00
#define ADD_SAVE_BEFORE 0x02

#define JAIL_CLIP 0x00
#define JAIL_RMH  0x01
#define JAIL_RMB  0x02

extern QString confBaseDir;

extern const QString PkgListCommand;
extern const QString PkgListConfBase;

extern QString mirrorPath;

// ConfProfile
typedef enum {
  JAILS_CONF,
  PRINTERS_CONF,
  SOUND_CONF,
  GW_NET_CONF,
  USBKEYS_CONF,
  USERS_CONF,
  NET_CONF,
  OPTIONAL_RMB,
  OPTIONAL_RMH,
  DOWNLOAD_CLIP,
  DOWNLOAD_RMB,
  DOWNLOAD_RMH,
  N_CONFFILES
} conffile_t;

static const char filenames[N_CONFFILES][64] = {
  "params/conf.d/clip",
  "params/conf.d/printers",
  "params/conf.d/sound",
  "params/conf.d/net",
  "params/usbkeys.conf",
  "params/users.list",
  "conf/net",
  "conf/optional.rm_b",
  "conf/optional.rm_h",
  "conf/clip_download/sources.list.clip",
  "conf/clip_download/sources.list.rm_b",
  "conf/clip_download/sources.list.rm_h",
};

// RegExps 
extern const QString yes_str;
extern const QRegExp yes_rx;
extern const QString no_str;

extern const QRegExp any_rx;
extern const QRegExp pkgname_rx;
extern const QRegExp profilename_rx;
extern const QRegExp username_rx;

extern const QRegExp one_rx;
extern const QRegExp num_rx;

extern const QRegExp ip_rx;
extern const QRegExp mask_rx;

extern const QRegExp host_rx;

extern const QRegExp virtual_rx;

#endif // COMMON_H

// vi:sw=2:ts=2:et:co=80:
