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

#ifndef __AARNI_ARC4_H__
#define __AARNI_ARC4_H__

#include "cipher.h"
#include "const.h"

namespace Aarni
{

class Arc4 : public Cipher
{
public:
    // Create a cipher using ARC4.
    Arc4(QObject* parent = NULL);

    // Reset the digest to the initial state.
    virtual void reset();

    // Continue the encryption/decryption operation.
    virtual void update(const QByteArray& in, QByteArray& out);

    // Complete the encryption/decryption.
    virtual void doFinal(const QByteArray& in, QByteArray& out);

    // Complete the encryption/decryption.
    virtual void doFinal(QByteArray& out);

private:
    // Schedule the key into the encryption/decryption key.
    virtual void scheduleKey();

    // the internal state
    char state_[256];
};

}

#endif // __AARNI_ARC4_H__
