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

#ifndef __AARNI_RIPEMD160_H__
#define __AARNI_RIPEMD160_H__

#include "message-digest.h"

namespace Aarni
{

class Ripemd160 : public MessageDigest
{
public:
    // Create a message digest using RIPEMD-160.
    Ripemd160(QObject* parent = NULL);

    // Reset the digest to the initial state.
    virtual void reset();

    // Update the digest.
    virtual void update(const QByteArray& in);

    // Complete the hash computation and return the digest.
    virtual void digest(QByteArray& out);

    // Return the length of the digest in bytes.
    virtual int getDigestLength() const;

private:
    // Compute the digest of a single block.
    void hashBlock(const char* const in);

    // the chaining variables.
    unsigned int h0_;
    unsigned int h1_;
    unsigned int h2_;
    unsigned int h3_;
    unsigned int h4_;

    // the buffer to hold the last block
    char buffer_[64];

    // the length of data in the buffer
    int bufLen_;

    // the total number of data hashed
    unsigned long long total_;
};

}

#endif // __AARNI_RIPEMD160_H__
