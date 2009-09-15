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

#ifndef __AARNI_MAIN_DIALOG_VIEW_H__
#define __AARNI_MAIN_DIALOG_VIEW_H__

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QProgressDialog>
#include <QVBoxLayout>

#include "encryptor.h"

namespace Aarni
{

struct EncryptionParameter;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = 0);
    ~MainDialog();

signals:
    void encryptRequested(EncryptionParameter& param);
    void encryptCanceled();

public slots:
    void setEncryptProgress(int progress);
    void setEncryptCompleted(int result);

private slots:
    void encrypt();
    void decrypt();

    void setSourcePath(const QString& path);
    void setDestinationPath(const QString& path);

    void showPassword(int state);

    void changePreferenceDescription(int index);

    void cancelEncrypt();

private:
    void requestEncrypt(CIPHER_OP_MODE mode);

    QVBoxLayout* layout_;
    QLineEdit* txtSource_;
    QLineEdit* txtDestination_;
    QLineEdit* txtPassword_;
    QCheckBox* chkRemove_;
    QComboBox* cmbPreference_;
    QLabel* lblPreference_;
    QProgressDialog* dlgProgress_;
};

}

#endif // __AARNI_MAIN_DIALOG_VIEW_H__
