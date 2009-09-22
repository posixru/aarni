/*
 * Aarni is a simple and easy-to-use file encryptor.
 * Copyright (C) 2009 Xizhi Zhu <xizhi.zhu@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation,
 * or attached in the package as the LICENSE file.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifndef __AARNI_MAINDIALOG_H_
#define __AARNI_MAINDIALOG_H_

#include <QObject>

#include "encryptor.h"
#include "main-dialog-ui.h"

namespace Aarni
{

class MainDialog : public QObject
{
    Q_OBJECT

public:
    MainDialog(QObject* parent = NULL);
    ~MainDialog();

private slots:
    void viewHomePage();
    void browseEncryptionSourceFile();
    void browseEncryptionSourceDir();
    void browseEncryptionDestination();
    void showEncryptionPassword(int state);
    void browseDecryptionSource();
    void browseDecryptionDestination();
    void showDecryptionPassword(int state);
    void encrypt();
    void decrypt();
    void completeEncryption(quint32 result);

private:
    MainDialogUI* ui_;
    Encryptor* enc_;
};

}

#endif // __AARNI_MAINDIALOG_H_
