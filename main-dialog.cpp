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

#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QUrl>

#include "aes.h"
#include "main-dialog.h"
#include "sha384.h"

namespace Aarni
{

MainDialog::MainDialog(QObject* parent)
    : QObject(parent)
{
    ui_ = new MainDialogUI;
    enc_ = new Encryptor;

    connect(ui_->browseButton_, SIGNAL(clicked()), this, SLOT(viewHomePage()));

    connect(ui_->encryptBrowseSourceFileButton_, SIGNAL(clicked()), this, SLOT(browseEncryptionSourceFile()));
    connect(ui_->encryptBrowseSourceDirButton_, SIGNAL(clicked()), this, SLOT(browseEncryptionSourceDir()));
    connect(ui_->encryptBrowseDestinationButton_, SIGNAL(clicked()), this, SLOT(browseEncryptionDestination()));
    connect(ui_->encryptPasswordCheckbox_, SIGNAL(stateChanged(int)), this, SLOT(showEncryptionPassword(int)));
    connect(ui_->encryptWizard_->button(QWizard::FinishButton), SIGNAL(clicked()), this, SLOT(encrypt()));

    connect(ui_->decryptBrowseSourceButton_, SIGNAL(clicked()), this, SLOT(browseDecryptionSource()));
    connect(ui_->decryptBrowseDestinationButton_, SIGNAL(clicked()), this, SLOT(browseDecryptionDestination()));
    connect(ui_->decryptPasswordCheckbox_, SIGNAL(stateChanged(int)), this, SLOT(showEncryptionPassword(int)));
    connect(ui_->decryptWizard_->button(QWizard::FinishButton), SIGNAL(clicked()), this, SLOT(decrypt()));

    connect(enc_, SIGNAL(encryptionCompleted(quint32)), this, SLOT(completeEncryption(quint32)));

    ui_->show();
}

MainDialog::~MainDialog()
{
    delete ui_;
    delete enc_;
}

void MainDialog::viewHomePage()
{
    QDesktopServices::openUrl(QUrl("http://xizhizhu.blogspot.com/"));
}

void MainDialog::browseEncryptionSourceFile()
{
    ui_->encryptSourceEdit_->setText(QFileDialog::getOpenFileName());
}

void MainDialog::browseEncryptionSourceDir()
{
    ui_->encryptSourceEdit_->setText(QFileDialog::getExistingDirectory());
}

void MainDialog::browseEncryptionDestination()
{
    ui_->encryptDestinationEdit_->setText(QFileDialog::getSaveFileName());
}

void MainDialog::showEncryptionPassword(int state)
{
    if (Qt::Checked == state)
    {
        ui_->encryptPasswordEdit_->setEchoMode(QLineEdit::Normal);
    }
    else if (Qt::Unchecked == state)
    {
        ui_->encryptPasswordEdit_->setEchoMode(QLineEdit::Password);
    }
}

void MainDialog::browseDecryptionSource()
{
    ui_->decryptSourceEdit_->setText(QFileDialog::getOpenFileName());
}

void MainDialog::browseDecryptionDestination()
{
    ui_->decryptDestinationEdit_->setText(QFileDialog::getExistingDirectory());
}

void MainDialog::showDecryptionPassword(int state)
{
    if (Qt::Checked == state)
    {
        ui_->decryptPasswordEdit_->setEchoMode(QLineEdit::Normal);
    }
    else if (Qt::Unchecked == state)
    {
        ui_->decryptPasswordEdit_->setEchoMode(QLineEdit::Password);
    }
}



void MainDialog::encrypt()
{
    EncryptionParameter param;
    param.isEncryption_ = true;
    param.source_ = ui_->encryptSourceEdit_->text();
    param.destination_ = ui_->encryptDestinationEdit_->text();
    param.password_ = ui_->encryptPasswordEdit_->text();
    if (param.source_.isEmpty() || param.destination_.isEmpty() || param.password_.isEmpty())
    {
        return;
    }

    ui_->encryptButton_->setEnabled(false);
    ui_->decryptButton_->setEnabled(false);
    ui_->browseButton_->setEnabled(false);
    ui_->aboutButton_->setEnabled(false);

    enc_->encrypt(param);

    QMessageBox::information(ui_, tr("MESSAGE_ENCRYPTION_TITLE"), tr("MESSAGE_ENCRYPTION_TEXT"));
}

void MainDialog::decrypt()
{
    EncryptionParameter param;
    param.isEncryption_ = false;
    param.source_ = ui_->decryptSourceEdit_->text();
    param.destination_ = ui_->decryptDestinationEdit_->text();
    param.password_ = ui_->decryptPasswordEdit_->text();
    if (param.source_.isEmpty() || param.destination_.isEmpty() || param.password_.isEmpty())
    {
        return;
    }

    ui_->encryptButton_->setEnabled(false);
    ui_->decryptButton_->setEnabled(false);
    ui_->browseButton_->setEnabled(false);
    ui_->aboutButton_->setEnabled(false);

    enc_->encrypt(param);

    QMessageBox::information(ui_, tr("MESSAGE_DECRYPTION_TITLE"), tr("MESSAGE_DECRYPTION_TEXT"));
}

void MainDialog::completeEncryption(quint32 result)
{
    ui_->encryptButton_->setEnabled(true);
    ui_->decryptButton_->setEnabled(true);
    ui_->browseButton_->setEnabled(true);
    ui_->aboutButton_->setEnabled(true);

    switch (result)
    {
    case ERROR_SUCCESS:
        QMessageBox::information(ui_, tr("MESSAGE_COMPLETE_TITLE"), tr("MESSAGE_COMPLETE_TEXT"));
        break;
    case ERROR_CORRUPTED_FILE:
        QMessageBox::information(ui_, tr("MESSAGE_COMPLETE_TITLE"), tr("MESSAGE_COMPLETE_CORRUPTED_FILE"));
        break;
    case ERROR_INVALID_FILE_FORMAT:
        QMessageBox::information(ui_, tr("MESSAGE_COMPLETE_TITLE"), tr("MESSAGE_COMPLETE_INVALID_FORMAT"));
        break;
    default:
        QMessageBox::information(ui_, tr("MESSAGE_COMPLETE_TITLE"), tr("MESSAGE_COMPLETE_UNKNOWN"));
    }
}

}
