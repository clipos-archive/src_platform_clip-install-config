// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file ConfTab.cpp
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */


#include "ConfTab.h"
#include "ConfProfile.h"

ConfTab::ConfTab(QWidget *parent, ConfProfile *profile)
  : QFrame(parent), advanced(false), switchable(true), profile(profile)
{ }

void
ConfTab::showAdvanced(bool on)
{
  advanced = on;
}

void
ConfTab::setProfile(ConfProfile *prof)
{
  profile = prof;
}

QString
ConfTab::profilePath(void) const
{
  return profile->getPath();
}

bool
ConfTab::isSwitchable(void) const
{
  return switchable;
}

// vi:sw=2:ts=2:et:co=80:
