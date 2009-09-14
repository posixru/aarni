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

#include <QFile>

#include "cipher.h"
#include "encryptor.h"
#include "message-digest.h"

namespace Aarni
{

Encryptor::Encryptor(QObject* parent)
    : QObject(parent), ui_(new MainDialog)
{
    ui_->show();

    connect(ui_, SIGNAL(encryptRequested(EncryptionParameter&)),
            this, SLOT(encrypt(EncryptionParameter&)));
    connect(this, SIGNAL(encryptProgress(int)),
            ui_, SLOT(setEncryptProgress(int)));
    connect(this, SIGNAL(encryptCompleted(int)),
            ui_, SLOT(setEncryptCompleted(int)));
}

Encryptor::~Encryptor()
{
    delete ui_;
}

void Encryptor::encrypt(EncryptionParameter& param)
{
    // check password
    if (param.password_.isEmpty())
    {
        emit encryptCompleted(ERROR_NO_PASSWORD);
        return;
    }

    // open file
    QFile in(param.source_);
    QFile out(param.destination_);
    if (!in.open(QIODevice::ReadOnly) || !out.open(QIODevice::WriteOnly))
    {
        emit encryptCompleted(ERROR_FILE_OPEN_FAILED);
        return;
    }

    // write or verify the header
    FileHeader header;
    if (ENCRYPT_MODE == param.mode_)
    {
        // write the header
        header.magic_[0] = FILE_MAGIC_BYTE1;
        header.magic_[1] = FILE_MAGIC_BYTE2;
        header.version_ = FILE_VERSION;
        if (ALGORITHM_CIPHER_AES == param.cipher_)
        {
            header.cipher_ = ALGORITHM_CIPHER_AES_ID;
        }
        else
        {
            header.cipher_ = ALGORITHM_CIPHER_ARC4_ID;
        }
        header.hash_ = ALGORITHM_HASH_RIPEMD160_ID;
        header.compress_ = ALGORITHM_NONE_ID;
        header.encryptedBlockOffset_ = sizeof(header);
        header.digestOffset_ = header.encryptedBlockOffset_ + in.size();
        out.write((char*)&header, sizeof(header));
    }
    else
    {
        // verify the header
        in.read((char*)&header, sizeof(header));
        if (FILE_MAGIC_BYTE1 != header.magic_[0] || FILE_MAGIC_BYTE2 != header.magic_[1])
        {
            emit encryptCompleted(ERROR_FILE_INVALID_FORMAT);
            return;
        }

        // set up the algorithms
        switch (header.hash_)
        {
            case ALGORITHM_HASH_RIPEMD160_ID:
                param.hash_ = ALGORITHM_HASH_RIPEMD160;
                break;
            default:
                param.hash_ = ALGORITHM_HASH_RIPEMD160;
                break;
        }

        switch (header.cipher_)
        {
            case ALGORITHM_CIPHER_AES_ID:
                param.cipher_ = ALGORITHM_CIPHER_AES;
                break;
            case ALGORITHM_CIPHER_ARC4_ID:
                param.cipher_ = ALGORITHM_CIPHER_ARC4;
                break;
            default:
                param.cipher_ = ALGORITHM_CIPHER_AES;
                break;
        }
    }

    // initialize cipher
    QByteArray output;
    Cipher* cipher = Cipher::getInstance(param.cipher_);
    MessageDigest* md = MessageDigest::getInstance(param.hash_);
    md->digest(param.password_.toAscii(), output);
    if (ALGORITHM_CIPHER_AES == param.cipher_)
    {
        output.resize(32);
        for (int i = 31; i >= 16; i--)
        {
            output[i] = output[i - 12];
        }
    }
    cipher->init(param.mode_, output);

    // encrypt or decrypt
    QByteArray input;
    md->reset();
    long long size = in.size();
    long long left = size;
    if (DECRYPT_MODE == param.mode_)
    {
        left -= (DIGEST_SIZE + sizeof(FileHeader));
    }

    while (left > BLOCK_SIZE)
    {
        input = in.read(BLOCK_SIZE);
        cipher->update(input, output);
        if (ENCRYPT_MODE == param.mode_)
        {
            md->update(input);
        }
        else
        {
            md->update(output);
        }
        out.write(output);
        emit encryptProgress(((size - left) << 7) / size);
        left -= BLOCK_SIZE;
    }

    input = in.read(left);
    cipher->update(input, output);
    if (ENCRYPT_MODE == param.mode_)
    {
        md->update(input);
    }
    else
    {
        md->update(output);
    }
    out.write(output);

    // write or verify the digest
    md->digest(output);
    if (ENCRYPT_MODE == param.mode_)
    {
        // write the digest
        out.write(output);
    }
    else
    {
        // verify the digest
        input = in.read(DIGEST_SIZE);
        if (input != output)
        {
            emit encryptCompleted(ERROR_FILE_CORRUPTED);
            delete cipher;
            delete md;
            return;
        }
    }

    // delete the source file when required
    if (param.deleteSource_)
    {
        in.remove();
    }

    // encryption completed
    delete cipher;
    delete md;
    emit encryptProgress(128);
    emit encryptCompleted(ERROR_SUCCESS);
}

}
