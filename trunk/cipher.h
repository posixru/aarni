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

#ifndef __AARNI_CIPHER_H__
#define __AARNI_CIPHER_H__

#include <QObject>
#include <QString>

#include "const.h"

namespace Aarni
{

class Cipher : public QObject
{
protected:
    // Create a cipher with the specified algorithm name.
    Cipher(const QString& algorithm, QObject* parent = NULL);

public:
    // Return a Cipher object that implements the specified algorithm.
    static Cipher* getInstance(const QString& algorithm);

    // Initialize the cipher.
    void init(const CIPHER_OP_MODE opmode, const QByteArray& key);

    // Reset the digest to the initial state.
    virtual void reset() = 0;

    // Continue the encryption/decryption operation.
    virtual void update(const QByteArray& in, QByteArray& out) = 0;

    // Complete the encryption/decryption.
    virtual void doFinal(const QByteArray& in, QByteArray& out) = 0;

    // Complete the encryption/decryption.
    virtual void doFinal(QByteArray& out) = 0;

    // Return a string that identifies the algorithm.
    QString getAlgorithm() const;

protected:

    // the name of the algorithm used
    QString algorithmName_;

    // operation mode
    CIPHER_OP_MODE opmode_;

    // the key used
    QByteArray key_;

    // whether it has been initialized
    bool initialized_;

private:
    // Schedule the key into the encryption/decryption key.
    virtual void scheduleKey() = 0;
};

}

#endif // __AARNI_CIPHER_H__
