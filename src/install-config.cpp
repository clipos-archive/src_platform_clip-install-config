// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2010-2018 ANSSI. All Rights Reserved.
/**
 * @file install-config.cpp
 * install-config main.
 * @author Vincent Strubel <clipos@ssi.gouv.fr>
 *
 * Copyright (C) 2009 SGDN
 * Copyright (C) 2010 ANSSI
 * @n
 * All rights reserved.
 */

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QTextCodec>
#include <QTranslator>

#include <ClipWidgets/WaitDialog.h>
#include <QtSingleApplication>

#include <stdlib.h>

#include "frmConfMain.h"
#include "qtcommon.h"
#include "consts.h"

QString confBaseDir;

static int
initialize_pkgcache(const QString &jail)
{
  QStringList args;
  args <<  "-j" << jail;
  if (jail.startsWith("rm_")) {
    args << "-conf" << PkgListConfBase + "rm";
    args << "-m" << mirrorPath + "/clip4-rm-dpkg/rm/rm-apps-conf";
  } else {
    args << "-c" << PkgListConfBase + "clip";
    args << "-m" << mirrorPath + "/clip4-rm-dpkg/clip/clip-apps-conf";
  }
  args << "-o" << "/dev/null" << "-f";
  QString tmp = mirrorPath;
  QString cache = QString("/tmp/%1.%2.cache")
            .arg(tmp.replace("/", "_")).arg(jail);
  args << "-cache" << cache;

  QProcess lister;
  lister.start(PkgListCommand, args);
  WaitDialog waiter("Traitement en cours",
      "<p>Initialisation de la liste des paquetages optionnels pour "
      + jail.toUpper() + " en cours... Veuillez patienter.</p>",
      lister);
  waiter.exec();
}

int 
main(int argc, char** argv) 
{
  QString appId;

  if (argc < 2) {
    qDebug("Missing basedir argument");
    exit(EXIT_FAILURE);
  }

  confBaseDir = argv[1];

  if (!QDir::isAbsolutePath(confBaseDir)) {
    QDir d(confBaseDir);
    confBaseDir = d.absolutePath();
  }

  if (argc > 2)
    mirrorPath = argv[2];

  QtSingleApplication app(appId, argc, argv);
  if (app.sendMessage("Hi there") || app.isRunning())
    return 0;

  QTranslator qt(0);
  qt.load ("qt_fr.qm", PREFIX"/share/qt4/translations");
  app.installTranslator(&qt);
    
  QTextCodec *codec = QTextCodec::codecForName("utf8");
  QTextCodec::setCodecForCStrings(codec);

  if (QFile::exists(mirrorPath + "/clip4-rm-dpkg/rm/rm-apps-conf")) {
    initialize_pkgcache("rm_h");
    initialize_pkgcache("rm_b");
  }
  // Création et affichage de la fenêtre principale
  QWidget* window = new frmConfMain();

  app.setActivationWindow(window);
  QObject::connect(&app, SIGNAL(messageReceived(const QString &)), 
                                  &app, SLOT(activateWindow()));
  window->show();
  exit(app.exec());
}

// vi:sw=2:ts=2:et:co=80:
