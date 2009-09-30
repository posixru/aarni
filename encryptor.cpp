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

#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QtEndian>

#include "const.h"
#include "encryptor.h"

#include <iostream>
using namespace std;

namespace Aarni
{

Encryptor::Encryptor(QObject* parent)
    : QThread(parent)
{
    // do nothing
}

// Encrypt the files according to the parameters.
quint32 Encryptor::encrypt(const EncryptionParameter& param)
{
    if (isRunning())
    {
        return ERROR_ALREADY_RUNNING;
    }
    if (param.password_.isEmpty())
    {
        return ERROR_EMPTY_PASSWORD;
    }
    if (!QFile::exists(param.source_))
    {
        return ERROR_SOURCE_NOT_EXIST;
    }

    quint32 result = ERROR_SUCCESS;
    if (QFile::exists(param.destination_))
    {
        result = ERROR_DESTINATION_EXISTED;
    }

    param_ = param;

    start();

    return result;
}

// Check if the specific file is encrypted.
bool Encryptor::isEncryptedFile(const QString& name)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    FileHeader header;
    file.read((char*)&header, sizeof(header));
    if (qToLittleEndian<quint16>(FILE_MAGIC) != header.magic_)
    {
        return false;
    }

    return true;
}

// Encrypt/decrypt the given files based on the parameter.
void Encryptor::run()
{
    directory_.clear();

    prepareKey();

    if (param_.isEncryption_)
    {
        encrypt();
    }
    else
    {
        decrypt();
    }
}

// Prepare the key from the password.
void Encryptor::prepareKey()
{
    sha_.update(param_.password_.toAscii());
    QByteArray out;
    sha_.digest(out);

    // the first 128 bits as the key for plaintext,
    // the second 128 bits as the key for tweak,
    // the third 128 bits as the initial tweak value
    aes_.init(QByteArray(out.data(), 32), QByteArray(out.data() + 32, 16));
}

// Encrypt.
void Encryptor::encrypt()
{
    // reserve the space for the header
    QFile file(param_.destination_);
    file.open(QIODevice::ReadWrite);
    file.resize(sizeof(FileHeader));

    // encrypt files
    aes_.reset();
    directory_.clear();
    QDirIterator di(param_.source_, QDirIterator::Subdirectories);
    while(di.hasNext()) {
        QString tmp = di.next();
        if (di.fileInfo().isFile() && di.fileName() != "." && di.fileName() != "..")
        {
            encryptFile(tmp);
        }
    }

    // write the header
    FileHeader header;
    header.magic_ = qToLittleEndian<quint16>(FILE_MAGIC);
    header.version_ = FILE_VERSION;
    header.flags_ = FILE_FLAG_NONE;
    header.dirOffset_ = qToLittleEndian<quint64>(file.size());
    int size = directory_.size();
    header.dirEntryNum_ = qToLittleEndian<quint32>(directory_.size());
    QByteArray digest;
    sha_.digest(digest);
    memcpy(header.digest_, digest.data(), 48);
    file.seek(0);
    file.write((char*)&header, sizeof(header));

    // compress, encrypt and write the directory
    quint64 tmp;
    QByteArray buf1;
    file.seek(file.size());
    for (int i = 0; i < size; i++)
    {
        tmp = qToLittleEndian<quint64>(directory_[i].offset_);
        buf1.append((char*)&tmp, 8);

        tmp = qToLittleEndian<quint64>(directory_[i].length_);
        buf1.append((char*)&tmp, 8);

        tmp = qToLittleEndian<quint64>(directory_[i].name_.toAscii().length());
        buf1.append((char*)&tmp, 8);

        buf1.append(directory_[i].name_);
    }

    QByteArray buf2 = qCompress(buf1, 9);
    aes_.reset();
    aes_.encrypt(buf2, buf1);
    file.write(buf1);

    // encryption completes
    emit encryptionCompleted(ERROR_SUCCESS);
}

// Decrypt.
void Encryptor::decrypt()
{
    // verify the header
    if (!isEncryptedFile(param_.source_))
    {
        emit encryptionCompleted(ERROR_INVALID_FILE_FORMAT);
        return;
    }

    QFile src(param_.source_);
    src.open(QIODevice::ReadOnly);
    FileHeader header;

    // decrypt, uncompress, and read the directory
    src.read((char*)&header, sizeof(header));
    quint64 tmp64 = qFromLittleEndian<quint64>(header.dirOffset_);
    src.seek(tmp64);
    QByteArray buf1 = src.read(src.size() - tmp64);
    QByteArray buf2;
    aes_.reset();
    aes_.decrypt(buf1, buf2);
    buf1 = qUncompress(buf2);

    DirectoryEntry entry;
    quint32 tmp32 = qFromLittleEndian<quint32>(header.dirEntryNum_);
    tmp64 = 0;
    for (quint32 i = 0; i < tmp32; i++)
    {
        entry.offset_ = qFromLittleEndian<quint64>((uchar*)buf1.data() + tmp64);
        tmp64 += 8;

        entry.length_ = qFromLittleEndian<quint64>((uchar*)buf1.data() + tmp64);
        tmp64 += 8;

        quint64 tmp =  qFromLittleEndian<quint64>((uchar*)buf1.data() + tmp64);
        tmp64 += 8;

        buf2.clear();
        buf2.append(buf1.data() + tmp64, tmp);
        tmp64 += tmp;
        entry.name_ = param_.destination_ + QDir::separator() + buf2;

        directory_.push_back(entry);
    }

    // decrypt files
    aes_.reset();
    int size = directory_.size();
    cout << size << endl;
    for (int i = 0; i < size; i++)
    {
        // decrypt and decompress
        src.seek(directory_[i].offset_);
        buf1 = src.read(directory_[i].length_);
        aes_.decrypt(buf1, buf2);
        buf1 = qUncompress(buf2);

        // remove the padding
        buf1.resize(buf1.length() - buf1[buf1.length() - 1]);

        // compute the digest
        sha_.update(buf1);

        // write the file
        QDir().mkpath(QFileInfo(directory_[i].name_).absolutePath());
        QFile dst(directory_[i].name_);
        dst.open(QIODevice::WriteOnly);
        dst.write(buf1);
    }

    // verify the digest
    QByteArray digest;
    sha_.digest(digest);
    for (int i = 0; i < digest.length(); i++)
    {
        if ((quint8)digest.at(i) != header.digest_[i])
        {
            emit encryptionCompleted(ERROR_CORRUPTED_FILE);
            return;
        }
    }

    emit encryptionCompleted(ERROR_SUCCESS);
}

// Encrypt a single file.
void Encryptor::encryptFile(const QString& file)
{
    // in case the destination is in the directory
    if (param_.destination_ == file)
    {
        return;
    }

    // read the file
    QFile src(file);
    src.open(QIODevice::ReadOnly);
    QByteArray buf1 = src.readAll();

    // compute digest
    sha_.update(buf1);

    // pad the file
    quint8 tmp = 16 - buf1.length() % 16;
    buf1.append(QByteArray(tmp, tmp));

    // compress and encrypt
    QByteArray buf2 = qCompress(buf1, 9);
    aes_.encrypt(buf2, buf1);

    // write the processed data to the end of the file
    QFile dst(param_.destination_);
    dst.open(QIODevice::ReadWrite);
    quint64 offset = dst.size();
    dst.seek(offset);
    dst.write(buf1);

    // add the information to the directory
    DirectoryEntry entry;
    int length = file.length() - param_.source_.length();
    if (0 < length)
    {
        entry.name_ = file.right(length);
    }
    else
    {
        entry.name_ = QFileInfo(file).fileName();
    }
    entry.offset_ = offset;
    entry.length_ = buf1.length();
    directory_.push_back(entry);
}

}
