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

#ifndef __AARNI_ENCRYPTOR_H__
#define __AARNI_ENCRYPTOR_H__

#include <QObject>
#include <QString>

#include "const.h"
#include "main-dialog-view.h"

namespace Aarni
{

class MainDialog;

struct EncryptionParameter
{
    CIPHER_OP_MODE mode_;
    QString cipher_;
    QString hash_;
    QString source_;
    QString destination_;
    QString password_;
    bool deleteSource_;
};

struct FileHeader
{
    char magic_[2];
    char version_;
    char cipher_;
    char hash_;
    char compress_;
    unsigned int encryptedBlockOffset_;
    unsigned int digestOffset_;
};

class Encryptor : public QObject
{
    Q_OBJECT

public:
    Encryptor(QObject* parent = 0);
    ~Encryptor();

signals:
    void encryptStarted();
    void encryptProgress(int progress);
    void encryptCompleted(int result);

private slots:
    void encrypt(EncryptionParameter& param);
    void cancelEncrypt();

private:
    MainDialog* ui_;
    bool canceled_;
};

}

#endif // __AARNI_ENCRYPTOR_H__
