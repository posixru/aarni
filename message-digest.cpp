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

#include "const.h"
#include "message-digest.h"
#include "ripemd160.h"

namespace Aarni
{

// Create a message digest with the specified algorithm name.
MessageDigest::MessageDigest(const QString& algorithm, QObject* parent)
    : QObject(parent), algorithmName_(algorithm)
{
    // do nothing
}

// Return a MessageDigest object that implements the specified algorithm.
MessageDigest* MessageDigest::getInstance(const QString& algorithm)
{
    if (ALGORITHM_HASH_RIPEMD160 == algorithm)
    {
        return new Ripemd160;
    }

    return NULL;
}

// Return a string that identifies the algorithm.
QString MessageDigest::getAlgorithm() const
{
    return algorithmName_;
}

// Perform a final update on the digest and complete the hash computation.
void MessageDigest::digest(const QByteArray& in, QByteArray& out)
{
    update(in);
    digest(out);
}

}
