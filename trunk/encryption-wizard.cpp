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
#include <QList>
#include <QVariant>

#include "encryption-wizard.h"

namespace Aarni
{

EncWizard::EncWizard(QWidget* parent)
    : QWizard(parent)
{
    setupUI();
    retranslateUI();
}

void EncWizard::retranslateUI()
{
    setWindowTitle(tr("ENCRYPT_WIZARD_TITLE"));

    QList<int> ids = pageIds();
    EncWizardPage* p = NULL;
    for (int i = 0; i < ids.size(); i++)
    {
        p = dynamic_cast<EncWizardPage*>(page(i));
        if (p)
        {
            p->retranslateUI();
        }
    }
}

void EncWizard::accept()
{
    EncryptionParameter param;
    param.isEncryption_ = true;
    param.source_ = field("source").toString();
    param.destination_ = field("destination").toString();
    param.password_ = field("password").toString();
    emit encryptionRequested(param);

    restart();

    QDialog::accept();
}

void EncWizard::setupUI()
{
    addPage(new EncSourcePage);
    addPage(new EncDestinationPage);
    addPage(new EncPasswordPage);
    addPage(new EncConclusionPage);

    setModal(true);
    setWindowIcon(QIcon(":/images/encrypt.png"));
}


EncSourcePage::EncSourcePage(QWidget* parent)
    : EncWizardPage(parent)
{
    setupUI();
    retranslateUI();

    registerField("source", sourceLineEdit_);
    connect(sourceLineEdit_, SIGNAL(textChanged(QString)), this, SLOT(validate()));
    connect(fileBrowseButton_, SIGNAL(clicked()), this, SLOT(browseFile()));
    connect(dirBrowseButton_, SIGNAL(clicked()), this, SLOT(browseDir()));
}

bool EncSourcePage::isComplete() const
{
    return QFile::exists(sourceLineEdit_->text());
}

void EncSourcePage::retranslateUI()
{
    setTitle(tr("ENCRYPT_SOURCE_PAGE"));
    setSubTitle(tr("ENCRYPT_SOURCE_SUBTITLE"));

    sourceLabel_->setText(tr("ENCRYPT_SOURCE_LABEL"));
    fileBrowseButton_->setText(tr("ENCRYPT_FILE_BROWSE_BUTTON"));
    dirBrowseButton_->setText(tr("ENCRYPT_DIR_BROWSE_BUTTON"));
}

void EncSourcePage::browseDir()
{
    sourceLineEdit_->setText(QFileDialog::getExistingDirectory());
}

void EncSourcePage::browseFile()
{
    sourceLineEdit_->setText(QFileDialog::getOpenFileName());
}

void EncSourcePage::validate()
{
    if (!QFile::exists(sourceLineEdit_->text()))
    {
        warningLabel_->setText(tr("ENCRYPT_SOURCE_NOT_EXIST"));
    }
    else
    {
        warningLabel_->setText(QString());
    }

    emit completeChanged();
}

void EncSourcePage::setupUI()
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

    dirBrowseButton_ = new QPushButton;
    layout->addWidget(dirBrowseButton_, 1, 2);
}


EncDestinationPage::EncDestinationPage(QWidget* parent)
    : EncWizardPage(parent)
{
    setupUI();
    retranslateUI();

    registerField("destination", destinationLineEdit_);
    connect(destinationLineEdit_, SIGNAL(textChanged(QString)), this, SLOT(validate()));
    connect(destinationBrowseButton_, SIGNAL(clicked()), this, SLOT(browseDestination()));
}

bool EncDestinationPage::isComplete() const
{
    return !destinationLineEdit_->text().isEmpty();
}

void EncDestinationPage::retranslateUI()
{
    setTitle(tr("ENCRYPT_DESTINATION_PAGE"));
    setSubTitle(tr("ENCRYPT_DESTINATION_SUBTITLE"));
    
    destinationLabel_->setText(tr("ENCRYPT_DESTINATION_LABEL"));
    destinationBrowseButton_->setText(tr("ENCRYPT_DESTINATION_BROWSE_BUTTON"));
}

void EncDestinationPage::browseDestination()
{
    destinationLineEdit_->setText(QFileDialog::getSaveFileName());
}

void EncDestinationPage::validate()
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

void EncDestinationPage::setupUI()
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


EncPasswordPage::EncPasswordPage(QWidget* parent)
    : EncWizardPage(parent)
{
    setupUI();
    retranslateUI();

    registerField("password", passwordLineEdit_);
    connect(passwordLineEdit_, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(passwordCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(showPassword(int)));
}

bool EncPasswordPage::isComplete() const
{
    return !passwordLineEdit_->text().isEmpty();
}

void EncPasswordPage::retranslateUI()
{
    setTitle(tr("ENCRYPT_PASSWORD_PAGE"));
    setSubTitle(tr("ENCRYPT_PASSWORD_SUBTITLE"));

    passwordLabel_->setText(tr("ENCRYPT_PASSWORD_LABEL"));
    passwordCheckBox_->setText(tr("ENCRYPT_PASSWORD_CHECKBOX"));
}

void EncPasswordPage::showPassword(int state)
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

void EncPasswordPage::setupUI()
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


EncConclusionPage::EncConclusionPage(QWidget* parent)
    : EncWizardPage(parent)
{
    setupUI();
    retranslateUI();
}

void EncConclusionPage::initializePage()
{
    QString source = wizard()->field("source").toString();
    sourceLabel_->setText(source);
    destinationLabel_->setText(wizard()->field("destination").toString());

    // calculate the size of the source
    QFileInfo file;
    qint64 size = 0;
    file.setFile(source);
    if (file.isFile())
    {
        size = file.size();
    }
    else if (file.isDir())
    {
        QDirIterator di(source, QDirIterator::Subdirectories);
        while(di.hasNext()) {
            file.setFile(di.next());
            if (file.isFile())
            {
                size += file.size();
            }
        }
    }
    sizeLabel_->setText(QString().setNum(size));

    QWizardPage::initializePage();
}

void EncConclusionPage::retranslateUI()
{
    setTitle(tr("ENCRYPT_CONCLUSION_PAGE"));
    setSubTitle(tr("ENCRYPT_CONCLUSION_SUBTITLE"));

    sourceDescLabel_->setText(tr("ENCRYPT_CONCLUSION_SOURCE"));
    sizeDescLabel_->setText(tr("ENCRYPT_CONCLUSION_SIZE"));
    destinationDescLabel_->setText(tr("ENCRYPT_CONCLUSION_DESTINATION"));

}

void EncConclusionPage::setupUI()
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
