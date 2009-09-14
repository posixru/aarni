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

#include "aes.h"
#include "arc4.h"
#include "cipher.h"

namespace Aarni
{

// Create a cipher with the specified algorithm name.
Cipher::Cipher(const QString& algorithm, QObject* parent)
    : QObject(parent), algorithmName_(algorithm),
      key_(NULL), initialized_(false)
{
    // do nothing
}

// Return a Cipher object that implements the specified algorithm.
Cipher* Cipher::getInstance(const QString& algorithm)
{
    if (ALGORITHM_CIPHER_AES == algorithm)
    {
        return new AES;
    }
    else if (ALGORITHM_CIPHER_ARC4 == algorithm)
    {
        return new Arc4;
    }

    return NULL;
}

// Initialize the cipher.
void Cipher::init(const CIPHER_OP_MODE opmode, const QByteArray& key)
{
    opmode_ = opmode;
    key_ = key;
    initialized_ = true;
    reset();
    scheduleKey();
}

// Return a string that identifies the algorithm.
QString Cipher::getAlgorithm() const
{
    return algorithmName_;
}

}
