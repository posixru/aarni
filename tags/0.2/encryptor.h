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

#include <QThread>
#include <QVector>

#include "aes.h"
#include "sha384.h"

namespace Aarni
{

// This structure describes the parameters for encryption.
struct EncryptionParameter
{
    bool isEncryption_;
    QString source_;
    QString destination_;
    QString password_;
};

// This structure describes the file header.
struct FileHeader
{
    quint16 magic_;
    quint8 version_;
    quint8 flags_;
    quint32 dirEntryNum_;
    quint64 dirOffset_;
    quint8 digest_[48];
};

// This structure describes the directory entry.
struct DirectoryEntry
{
    quint64 offset_;
    quint64 length_;
    QString name_;
};

// This class encrypts/decrypts the given file/directory in a separate thread.
class Encryptor : public QThread
{
    Q_OBJECT

public:
    Encryptor(QObject* parent = NULL);

    // Encrypt the files according to the parameters.
    //
    // Parameter:
    //  param - the encryption parameter
    //
    // Return value:
    //  ERROR_SUCCESS - success
    //  ERROR_ALREADY_RUNNING - the thread is already running
    //  ERROR_EMPTY_PASSWORD - the password is empty
    //  ERROR_SOURCE_NOT_EXIST - the source doesn't exist
    //  ERROR_DESTINATION_EXISTED - the destination already existed but still initialized
    quint32 encrypt(const EncryptionParameter& param);

    // Check if the specific file is encrypted.
    //
    // Parameter:
    //  name - the file to be checked
    //
    // Return value:
    //  true - the file is encrypted
    //  false - the file is not encrypted
    static bool isEncryptedFile(const QString& name);

signals:
    // This signal is emitted when the encryption is completed.
    void encryptionCompleted(quint32 result);

protected:
    // Encrypt/decrypt the given files based on the parameter.
    void run();

private:
    // Prepare the key from the password.
    void prepareKey();

    // Encrypt/decrypt.
    void encrypt();
    void decrypt();

    // Traverse the directory for encryption.
    void traverse(const QString& path);

    // Encrypt a single file.
    void encryptFile(const QString& file);

    // The variables used.
    AES aes_;
    SHA384 sha_;
    EncryptionParameter param_;
    QVector<DirectoryEntry> directory_;
};

}

#endif // __AARNI_ENCRYPTOR_H__
