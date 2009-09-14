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

#include <QFileDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QTabWidget>

#include "main-dialog-view.h"

namespace Aarni
{

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QObject::tr("WINDOW_TITLE"));
    setWindowIcon(QIcon(":/images/lock.ico"));
    layout_ = new QVBoxLayout;
    setLayout(layout_);

    // initialize the tabs
    QTabWidget* tab = new QTabWidget();
    layout_->addWidget(tab);

    // encryption/decryption tab
    QWidget* widget = new QWidget;
    tab->addTab(widget, QObject::tr("ENCRYPTION_TAB"));
    QGridLayout* gridLayout = new QGridLayout;
    widget->setLayout(gridLayout);
    gridLayout->setAlignment(Qt::AlignTop);

    gridLayout->addWidget(new QLabel(QObject::tr("SOURCE_LABEL")), 0, 0);
    gridLayout->addWidget(new QLabel(QObject::tr("DESTINATION_LABEL")), 1, 0);
    gridLayout->addWidget(new QLabel(QObject::tr("PASSWORD_LABEL")), 2, 0);

    txtSource_ = new QLineEdit;
    gridLayout->addWidget(txtSource_, 0, 1, 1, 2);
    txtDestination_ = new QLineEdit;
    gridLayout->addWidget(txtDestination_, 1, 1, 1, 2);
    txtPassword_ = new QLineEdit;
    gridLayout->addWidget(txtPassword_, 2, 1, 1, 2);
    txtPassword_->setEchoMode(QLineEdit::Password);

    QPushButton* button = new QPushButton(QObject::tr("BROWSE_BUTTON"));
    gridLayout->addWidget(button, 0, 3);
    QFileDialog* dialog = new QFileDialog(this);
    connect(button, SIGNAL(clicked()), dialog, SLOT(show()));
    connect(dialog, SIGNAL(fileSelected(QString)), this, SLOT(setSourcePath(QString)));
    button = new QPushButton(QObject::tr("BROWSE_BUTTON"));
    gridLayout->addWidget(button, 1, 3);
    dialog = new QFileDialog(this);
    dialog->setFileMode(QFileDialog::Directory);
    connect(button, SIGNAL(clicked()), dialog, SLOT(show()));
    connect(dialog, SIGNAL(fileSelected(QString)), this, SLOT(setDestinationPath(QString)));
    button = new QPushButton(QObject::tr("ENCRYPT_BUTTON"));
    gridLayout->addWidget(button, 3, 1);
    connect(button, SIGNAL(clicked()), this, SLOT(encrypt()));
    button = new QPushButton(QObject::tr("DECRYPT_BUTTON"));
    gridLayout->addWidget(button, 3, 2);
    connect(button, SIGNAL(clicked()), this, SLOT(decrypt()));

    QCheckBox* checkBox = new QCheckBox(QObject::tr("PASSWORD_CHECKBOX"));
    gridLayout->addWidget(checkBox, 2, 3);
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(showPassword(int)));

    // advanced tab
    widget = new QWidget;
    tab->addTab(widget, QObject::tr("ADVANCED_TAB"));
    gridLayout = new QGridLayout;
    widget->setLayout(gridLayout);
    gridLayout->setAlignment(Qt::AlignTop);

    gridLayout->addWidget(new QLabel(QObject::tr("STRATEGY_LABEL")), 0, 0);
    gridLayout->addWidget(new QLabel(QObject::tr("MORE_SETTING_LABEL")), 2, 0);

    cmbPreference_ = new QComboBox;
    gridLayout->addWidget(cmbPreference_, 0, 1);
    cmbPreference_->addItem(QObject::tr("STRATEGY_SAFER"));
    cmbPreference_->addItem(QObject::tr("STRATEGY_FASTER"));

    lblPreference_ = new QLabel(QObject::tr("STRATEGY_SAFER_DESC"));
    gridLayout->addWidget(lblPreference_, 1, 1, 1, 3);
    connect(cmbPreference_, SIGNAL(currentIndexChanged(int)), this, SLOT(changePreferenceDescription(int)));

    chkRemove_ = new QCheckBox(QObject::tr("DELETE_ORIGINAL_CHECKBOX"));
    gridLayout->addWidget(chkRemove_, 2, 1, 1, 3);

    // about tab
    widget = new QWidget;
    tab->addTab(widget, QObject::tr("ABOUT_TAB"));
    gridLayout = new QGridLayout;
    widget->setLayout(gridLayout);
    gridLayout->setAlignment(Qt::AlignTop);

    QLabel* label = new QLabel(QObject::tr("DESCRIPTION_LABEL"));
    gridLayout->addWidget(label, 0, 0);
    label->setOpenExternalLinks(true);

    // set up the progress bar
    dlgProgress_ = new QProgressDialog(this);
    dlgProgress_->setRange(0, 128);
    dlgProgress_->setAutoClose(false);
    dlgProgress_->setAutoReset(false);
    dlgProgress_->setWindowModality(Qt::WindowModal);
    dlgProgress_->setWindowTitle(QObject::tr("WINDOW_TITLE"));
}

MainDialog::~MainDialog()
{
    // delete all items
    QLayoutItem *child;
    while (NULL != (child = layout_->takeAt(0)))
    {
        delete child->widget();
        delete child;
    }
}

void MainDialog::setEncryptProgress(int progress)
{
    dlgProgress_->setValue(progress);
}

void MainDialog::setEncryptCompleted(int result)
{
    dlgProgress_->setCancelButtonText(QObject::tr("OK"));

    switch (result)
    {
        case ERROR_SUCCESS:
            dlgProgress_->setLabelText(QObject::tr("ENCRYPTION_COMPLETED_LABEL"));
            break;
        case ERROR_NO_PASSWORD:
            dlgProgress_->setLabelText(QObject::tr("NO_PASSWORD_LABEL"));
            break;
        case ERROR_FILE_OPEN_FAILED:
            dlgProgress_->setLabelText(QObject::tr("FILE_OPEN_FAILED_LABEL"));
            break;
        case ERROR_FILE_INVALID_FORMAT:
            dlgProgress_->setLabelText(QObject::tr("FILE_INVALID_FORMAT_LABEL"));
            break;
        case ERROR_FILE_CORRUPTED:
            dlgProgress_->setLabelText(QObject::tr("FILE_CORRUPTED_LABEL"));
            break;
        default:
            dlgProgress_->setLabelText(QObject::tr("ERROR_UNKNOWN_LABEL"));
    }
}

void MainDialog::encrypt()
{
    dlgProgress_->setLabelText(QObject::tr("ENCRYPTING_LABEL"));

    requestEncrypt(ENCRYPT_MODE);
}

void MainDialog::decrypt()
{
    dlgProgress_->setLabelText(QObject::tr("DECRYPTING_LABEL"));

    requestEncrypt(DECRYPT_MODE);
}

void MainDialog::setSourcePath(const QString& path)
{
    txtSource_->setText(path);
    if (txtDestination_->text().isEmpty())
    {
        setDestinationPath(QFileInfo(path).path());
    }
}

void MainDialog::setDestinationPath(const QString& path)
{
    if (!txtSource_->text().isEmpty())
    {
        if (txtSource_->text().right(3) != ".ae")
        {
            txtDestination_->setText(path + "/" + QFileInfo(txtSource_->text()).fileName() + ".ae");
        }
        else
        {
            QString fileName = QFileInfo(txtSource_->text()).fileName();
            txtDestination_->setText(path + "/" + fileName.left(fileName.length() - 3));
        }
    }
    else
    {
        txtDestination_->setText(path);
    }
}

void MainDialog::showPassword(int state)
{
    if (Qt::Checked == state)
    {
        txtPassword_->setEchoMode(QLineEdit::Normal);
    }
    else if (Qt::Unchecked == state)
    {
        txtPassword_->setEchoMode(QLineEdit::Password);
    }
}

void MainDialog::changePreferenceDescription(int index)
{
    if (0 == index)
    {
        lblPreference_->setText(QObject::tr("STRATEGY_SAFER_DESC"));
    }
    else if (1 == index)
    {
        lblPreference_->setText(QObject::tr("STRATEGY_FASTER_DESC"));
    }
}

void MainDialog::requestEncrypt(CIPHER_OP_MODE mode)
{
    dlgProgress_->setCancelButtonText(QObject::tr("CANCEL_BUTTON"));
    dlgProgress_->show();

    EncryptionParameter param;
    param.mode_ = mode;
    param.source_ = txtSource_->text();
    param.destination_ = txtDestination_->text();
    param.password_ = txtPassword_->text();
    if (Qt::Checked == chkRemove_->checkState())
    {
        param.deleteSource_ = true;
    }
    else
    {
        param.deleteSource_ = false;
    }
    if (0 == cmbPreference_->currentIndex())
    {
        param.cipher_ = ALGORITHM_CIPHER_AES;
        param.hash_ = ALGORITHM_HASH_RIPEMD160;
    }
    else
    {
        param.cipher_ = ALGORITHM_CIPHER_ARC4;
        param.hash_ = ALGORITHM_HASH_RIPEMD160;
    }

    emit encryptRequested(param);
}

}
