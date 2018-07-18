// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file consts.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#include "consts.h"

const QString PkgListCommand(PREFIX"/bin/list_optional.pl");
const QString PkgListConfBase(PREFIX"/share/clip-install-config/apt.conf.");
QString mirrorPath("/mnt/cdrom/mirrors");

const QString yes_str("yes");
const QRegExp yes_rx("^yes$");
const QString no_str("no");

const QRegExp any_rx(".+");

const QRegExp profilename_rx("[0-9a-zA-Z_\\-]+");
const QRegExp pkgname_rx("[0-9a-zA-Z_\\-]+");
const QRegExp username_rx("[a-z_][0-9a-z_\\-]{0,7}");

const QRegExp one_rx("1");
const QRegExp num_rx("\\d+");

const QRegExp ip_rx("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
const QRegExp mask_rx("\\d{1,2}");

const QRegExp host_rx("[a-zA-Z0-9./:-_,]+");
const QRegExp virtual_rx("^virtual$");
// vi:sw=2:ts=2:et:co=80:
