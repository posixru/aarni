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

#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QIcon>
#include <QVariant>

#include "decryption-wizard.h"

namespace Aarni
{

DecWizard::DecWizard(QWidget* parent)
    : QWizard(parent)
{
    setupUI();
    retranslateUI();
}

void DecWizard::retranslateUI()
{
    setWindowTitle(tr("DECRYPT_WIZARD_TITLE"));

    QList<int> ids = pageIds();
    DecWizardPage* p = NULL;
    for (int i = 0; i < ids.size(); i++)
    {
        p = dynamic_cast<DecWizardPage*>(page(i));
        if (p)
        {
            p->retranslateUI();
        }
    }
}

void DecWizard::accept()
{
    EncryptionParameter param;
    param.isEncryption_ = false;
    param.source_ = field("source").toString();
    param.destination_ = field("destination").toString();
    param.password_ = field("password").toString();
    emit decryptionRequested(param);

    QDialog::accept();
}

void DecWizard::setupUI()
{
    addPage(new DecSourcePage);
    addPage(new DecDestinationPage);
    addPage(new DecPasswordPage);
    addPage(new DecConclusionPage);

    setModal(true);
    setWindowIcon(QIcon(":/images/decrypt.png"));
}


DecSourcePage::DecSourcePage(QWidget* parent)
    : DecWizardPage(parent)
{
    setupUI();
    retranslateUI();

    registerField("source", sourceLineEdit_);
    connect(sourceLineEdit_, SIGNAL(textChanged(QString)), this, SLOT(validate()));
    connect(fileBrowseButton_, SIGNAL(clicked()), this, SLOT(browseFile()));
}

bool DecSourcePage::isComplete() const
{
    return Encryptor::isEncryptedFile(sourceLineEdit_->text());
}

void DecSourcePage::retranslateUI()
{
    setTitle(tr("DECRYPT_SOURCE_PAGE"));
    setSubTitle(tr("DECRYPT_SOURCE_SUBTITLE"));

    sourceLabel_->setText(tr("DECRYPT_SOURCE_LABEL"));
    fileBrowseButton_->setText(tr("ENCRYPT_FILE_BROWSE_BUTTON"));
}

void DecSourcePage::browseFile()
{
    sourceLineEdit_->setText(QFileDialog::getOpenFileName());
}

void DecSourcePage::validate()
{
    if (!QFile::exists(sourceLineEdit_->text()))
    {
        warningLabel_->setText(tr("DECRYPT_SOURCE_NOT_EXIST"));
    }
    else
    {
        warningLabel_->setText(QString());
    }

    emit completeChanged();
}

void DecSourcePage::setupUI()
{
    QGridLayout* layout = new QGridLayout;
    setLayout(layout);

    sourceLabel_ = new QLabel;
    layout->addWidget(sourceLabel_, 0, 0);

    warningLabel_ = new QLabel;
    layout->addWidget(warningLabel_, 2, 1, 1, 3);

    sourceLineEdit_ = new QLineEdit;
    layout->addWidget(sourceLineEdit_, 0, 1, 1, 3);

    fileBrowseButton_ = new QPushButton;
    layout->addWidget(fileBrowseButton_, 1, 1);
}


DecDestinationPage::DecDestinationPage(QWidget* parent)
    : DecWizardPage(parent)
{
    setupUI();
    retranslateUI();

    registerField("destination", destinationLineEdit_);
    connect(destinationLineEdit_, SIGNAL(textChanged(QString)), this, SLOT(validate()));
    connect(destinationBrowseButton_, SIGNAL(clicked()), this, SLOT(browseDestination()));
}

bool DecDestinationPage::isComplete() const
{
    return !destinationLineEdit_->text().isEmpty();
}

void DecDestinationPage::retranslateUI()
{
    setTitle(tr("DECRYPT_DESTINATION_PAGE"));
    setSubTitle(tr("DECRYPT_DESTINATION_SUBTITLE"));

    destinationLabel_->setText(tr("DECRYPT_DESTINATION_LABEL"));
    destinationBrowseButton_->setText(tr("DECRYPT_DESTINATION_BROWSE_BUTTON"));
}

void DecDestinationPage::browseDestination()
{
    destinationLineEdit_->setText(QFileDialog::getSaveFileName());
}

void DecDestinationPage::validate()
{
    if (QFile::exists(destinationLineEdit_->text()))
    {
        warningLabel_->setText(tr("ENCRYPT_DESTINATION_EXIST"));
    }
    else
    {
        warningLabel_->setText(QString());
    }

    emit completeChanged();
}

void DecDestinationPage::setupUI()
{
    QGridLayout* layout = new QGridLayout;
    setLayout(layout);

    destinationLabel_ = new QLabel;
    layout->addWidget(destinationLabel_, 0, 0);

    warningLabel_ = new QLabel;
    layout->addWidget(warningLabel_, 2, 1, 1, 3);

    destinationLineEdit_ = new QLineEdit;
    layout->addWidget(destinationLineEdit_, 0, 1, 1, 3);

    destinationBrowseButton_ = new QPushButton;
    layout->addWidget(destinationBrowseButton_, 1, 1);
}


DecPasswordPage::DecPasswordPage(QWidget* parent)
    : DecWizardPage(parent)
{
    setupUI();
    retranslateUI();

    registerField("password", passwordLineEdit_);
    connect(passwordLineEdit_, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(passwordCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(showPassword(int)));
}

bool DecPasswordPage::isComplete() const
{
    return !passwordLineEdit_->text().isEmpty();
}

void DecPasswordPage::retranslateUI()
{
    setTitle(tr("DECRYPT_PASSWORD_PAGE"));
    setSubTitle(tr("DECRYPT_PASSWORD_SUBTITLE"));

    passwordLabel_->setText(tr("DECRYPT_PASSWORD_LABEL"));
    passwordCheckBox_->setText(tr("DECRYPT_PASSWORD_CHECKBOX"));
}

void DecPasswordPage::showPassword(int state)
{
    if (Qt::Checked == state)
    {
        passwordLineEdit_->setEchoMode(QLineEdit::Normal);
    }
    else if (Qt::Unchecked == state)
    {
        passwordLineEdit_->setEchoMode(QLineEdit::Password);
    }
}

void DecPasswordPage::setupUI()
{
    QGridLayout* layout = new QGridLayout;
    setLayout(layout);

    passwordLabel_ = new QLabel;
    layout->addWidget(passwordLabel_, 0, 0);

    passwordLineEdit_ = new QLineEdit;
    passwordLineEdit_->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordLineEdit_, 0, 1, 1, 3);

    passwordCheckBox_ = new QCheckBox;
    layout->addWidget(passwordCheckBox_, 1, 1);
}


DecConclusionPage::DecConclusionPage(QWidget* parent)
    : DecWizardPage(parent)
{
    setupUI();
    retranslateUI();
}

void DecConclusionPage::initializePage()
{
    QString source = wizard()->field("source").toString();
    sourceLabel_->setText(source);
    destinationLabel_->setText(wizard()->field("destination").toString());

    QFileInfo file(source);
    sizeLabel_->setText(QString().setNum(file.size()));

    QWizardPage::initializePage();
}

void DecConclusionPage::retranslateUI()
{
    setTitle(tr("DECRYPT_CONCLUSION_PAGE"));
    setSubTitle(tr("DECRYPT_CONCLUSION_SUBTITLE"));

    sourceDescLabel_->setText(tr("DECRYPT_CONCLUSION_SOURCE"));
    sizeDescLabel_->setText(tr("DECRYPT_CONCLUSION_SIZE"));
    destinationDescLabel_->setText(tr("DECRYPT_CONCLUSION_DESTINATION"));
}

void DecConclusionPage::setupUI()
{
    QGridLayout* layout = new QGridLayout;
    setLayout(layout);

    sourceDescLabel_ = new QLabel;
    layout->addWidget(sourceDescLabel_, 0, 0);

    sizeDescLabel_ = new QLabel;
    layout->addWidget(sizeDescLabel_, 1, 0);

    destinationDescLabel_ = new QLabel;
    layout->addWidget(destinationDescLabel_, 2, 0);

    sourceLabel_ = new QLabel;
    layout->addWidget(sourceLabel_, 0, 1, 1, 2);

    sizeLabel_ = new QLabel;
    layout->addWidget(sizeLabel_, 1, 1, 1, 2);

    destinationLabel_ = new QLabel;
    layout->addWidget(destinationLabel_, 2, 1, 1, 2);
}

}
