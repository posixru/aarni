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

#ifndef __AARNI_MAIN_DIALOG_H__
#define __AARNI_MAIN_DIALOG_H__

#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include "about-dialog.h"
#include "decryption-wizard.h"
#include "encryption-wizard.h"
#include "encryptor.h"

namespace Aarni
{

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget* parent = NULL);

    void retranslateUI();

private slots:
    void openHomePage();
    void requestEncryption(EncryptionParameter param);
    void completeEncryption(quint32 result);

private:
    void setupUI();

    QLabel* logoLabel_;
    QLabel* titleLabel_;
    QPushButton* encryptButton_;
    QPushButton* decryptButton_;
    QPushButton* browseButton_;
    QPushButton* aboutButton_;

    AboutDialog* aboutDialog_;
    DecWizard* decWizard_;
    EncWizard* encWizard_;

    Encryptor* enc_;
};

}

#endif // __AARNI_MAIN_DIALOG_H__
