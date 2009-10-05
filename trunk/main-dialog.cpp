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
#include <QGridLayout>
#include <QMessageBox>
#include <QUrl>

#include "main-dialog.h"

namespace Aarni
{

MainDialog::MainDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
    retranslateUI();

    enc_ = new Encryptor;

    connect(encryptButton_, SIGNAL(clicked()), encWizard_, SLOT(show()));
    connect(decryptButton_, SIGNAL(clicked()), decWizard_, SLOT(show()));
    connect(browseButton_, SIGNAL(clicked()), this, SLOT(openHomePage()));
    connect(aboutButton_, SIGNAL(clicked()), aboutDialog_, SLOT(show()));
    connect(encWizard_, SIGNAL(encryptionRequested(EncryptionParameter)),
            this, SLOT(requestEncryption(EncryptionParameter)));
    connect(decWizard_, SIGNAL(decryptionRequested(EncryptionParameter)),
            this, SLOT(requestEncryption(EncryptionParameter)));
    connect(enc_, SIGNAL(encryptionCompleted(quint32)), this, SLOT(completeEncryption(quint32)));
}

void MainDialog::retranslateUI()
{
    setWindowTitle(tr("MAIN_DIALOG_TITLE"));

    titleLabel_->setText(tr("MAIN_SUBTITLE_LABEL"));
    encryptButton_->setText(tr("MAIN_ENCRYPT_BUTTON"));
    decryptButton_->setText(tr("MAIN_DECRYPT_BUTTON"));
    browseButton_->setText(tr("MAIN_BROWSE_BUTTON"));
    aboutButton_->setText(tr("MAIN_ABOUT_BUTTON"));

    aboutDialog_->retranslateUI();
    decWizard_->retranslateUI();
    encWizard_->retranslateUI();
}

void MainDialog::openHomePage()
{
    QDesktopServices::openUrl(QUrl("http://xizhizhu.blogspot.com/"));
}

void MainDialog::requestEncryption(EncryptionParameter param)
{
    enc_->encrypt(param);

    encryptButton_->setEnabled(false);
    decryptButton_->setEnabled(false);
    browseButton_->setEnabled(false);
    aboutButton_->setEnabled(false);
    QMessageBox::information(this, tr("MESSAGE_ENCRYPT_TITLE"), tr("MESSAGE_ENCRYPT_TEXT"));
}

void MainDialog::completeEncryption(quint32 result)
{
    encryptButton_->setEnabled(true);
    decryptButton_->setEnabled(true);
    browseButton_->setEnabled(true);
    aboutButton_->setEnabled(true);

    switch (result)
    {
    case ERROR_SUCCESS:
        QMessageBox::information(this, tr("MESSAGE_COMPLETE_TITLE"), tr("MESSAGE_COMPLETE_TEXT"));
        break;
    case ERROR_CORRUPTED_FILE:
        QMessageBox::information(this, tr("MESSAGE_COMPLETE_TITLE"), tr("MESSAGE_COMPLETE_CORRUPTED_FILE"));
        break;
    case ERROR_INVALID_FILE_FORMAT:
        QMessageBox::information(this, tr("MESSAGE_COMPLETE_TITLE"), tr("MESSAGE_COMPLETE_INVALID_FORMAT"));
        break;
    default:
        QMessageBox::information(this, tr("MESSAGE_COMPLETE_TITLE"), tr("MESSAGE_COMPLETE_UNKNOWN"));
    }
}

void MainDialog::setupUI()
{
    setWindowIcon(QIcon(":/images/logo.png"));

    QGridLayout* layout = new QGridLayout;
    setLayout(layout);

    logoLabel_ = new QLabel;
    logoLabel_->setPixmap(QPixmap(":/images/logo.png"));
    layout->addWidget(logoLabel_, 0, 0);

    titleLabel_ = new QLabel;
    layout->addWidget(titleLabel_, 0, 1, 1, 3);

    encryptButton_ = new QPushButton;
    encryptButton_->setIcon(QIcon(":/images/encrypt.png"));
    encryptButton_->setIconSize(QSize(48, 48));
    encryptButton_->setFixedSize(150, 60);
    layout->addWidget(encryptButton_, 1, 0, 1, 2);

    decryptButton_ = new QPushButton;
    decryptButton_->setIcon(QIcon(":/images/decrypt.png"));
    decryptButton_->setIconSize(QSize(48, 48));
    decryptButton_->setFixedSize(150, 60);
    layout->addWidget(decryptButton_, 1, 2, 1, 2);

    browseButton_ = new QPushButton;
    browseButton_->setIcon(QIcon(":/images/home.png"));
    browseButton_->setIconSize(QSize(48, 48));
    browseButton_->setFixedSize(150, 60);
    layout->addWidget(browseButton_, 2, 0, 1, 2);

    aboutButton_ = new QPushButton;
    aboutButton_->setIcon(QIcon(":/images/about.png"));
    aboutButton_->setIconSize(QSize(48, 48));
    aboutButton_->setFixedSize(150, 60);
    layout->addWidget(aboutButton_, 2, 2, 1, 2);

    aboutDialog_ = new AboutDialog;
    decWizard_ = new DecWizard;
    encWizard_ = new EncWizard;

    show();
}

}
