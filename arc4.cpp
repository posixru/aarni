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

#include "arc4.h"

namespace Aarni
{

// Create a cipher using ARC4.
Arc4::Arc4(QObject* parent)
    : Cipher(ALGORITHM_CIPHER_ARC4, parent)
{
    // do nothing
}

// Reset the digest to the initial state.
void Arc4::reset()
{
    if (!initialized_)
    {
        return;
    }

    scheduleKey();

}

// Continue the encryption/decryption operation.
void Arc4::update(const QByteArray& in, QByteArray& out)
{
    if (!initialized_)
    {
        out.resize(0);
        return;
    }

    int length = in.length();
    out.resize(length);

    int i = 0;
    int j = 0;
    for (int k = 0; k < length; k++)
    {
        i++;
        i &= 255;
        j += state_[i];
        j &= 255;
        char tmp = state_[i];
        state_[i] = state_[j];
        state_[j] = tmp;
        out[k] = in[k] ^ state_[(state_[i] + state_[j]) & 255];
    }
}

// Complete the encryption/decryption.
void Arc4::doFinal(const QByteArray& in, QByteArray& out)
{
    update(in, out);
}

// Complete the encryption/decryption.
void Arc4::doFinal(QByteArray& out)
{
    out.resize(0);
}

// Schedule the key into the encryption/decryption key.
void Arc4::scheduleKey()
{
    int keyLength = key_.length();
    for (int i = 0; i < 256; i++)
    {
        state_[i] = i;
    }
    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        j += state_[i] + key_[i % keyLength];
        j &= 255;
        char tmp = state_[i];
        state_[i] = state_[j];
        state_[j] = tmp;
    }
}

}
