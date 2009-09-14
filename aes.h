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

#ifndef __AARNI_AES_H__
#define __AARNI_AES_H__

#include "cipher.h"

namespace Aarni
{

// This class implements AES-128 [1] in the XTS mode [2]. The implementation
// is based on rijndael-alg-fst.c version 3.0.
//
// The best attack against AES-128 so far is on 7 rounds out of 10, with a
// complexity of 2^128 - 2^119 [3].
//
// References:
//  [1] http://www.csrc.nist.gov/publications/fips/fips197/fips-197.pdf
//  [2] https://siswg.net/
//  [4] http://www.springerlink.com/content/3tlphkvyk2tefla3/
class AES : public Cipher
{
public:
    // Create a cipher using AES.
    AES(QObject* parent = NULL);

    // Reset the digest to the initial state.
    virtual void reset();

    // Continue the encryption/decryption operation.
    virtual void update(const QByteArray& in, QByteArray& out);

    // Complete the encryption/decryption.
    virtual void doFinal(const QByteArray& in, QByteArray& out);

    // Complete the encryption/decryption.
    virtual void doFinal(QByteArray& out);

//private:
    // Schedule the key into the encryption/decryption key.
    virtual void scheduleKey();

    // The following functions implement XTS-AES.

    // Encrypt a data unit in the XTS mode.
    void xtsEncryptUnit(const QByteArray& in, QByteArray& out);

    // Decrypt a data unit in the XTS mode.
    void xtsDecryptUnit(const QByteArray& in, QByteArray& out);

    // Encrypt a block in the XTS mode.
    void xtsEncryptBlock(const unsigned char tweak[16], const unsigned char in[16], unsigned char out[16]);

    // Decrypt a block in the XTS mode.
    void xtsDecryptBlock(const unsigned char tweak[16], const unsigned char in[16], unsigned char out[16]);

    // Compute the tweak for the encryption of the next block.
    void xtsComputeTweak(unsigned char tweak[16]);


    // The following functions implement AES-128.

    // Expand the key into encryption/decryption key schedule.
    void aesExpandEncryptionKey(const unsigned char key[16], unsigned int expandedKey[44]);
    void aesExpandDecryptionKey(const unsigned char key[16], unsigned int expandedKey[44]);

    // Encrypt a single block using AES-128.
    void aesEncryptBlock(const unsigned int expandedKey[44], const unsigned char in[16], unsigned char out[16]);

    // Decrypt a single block using AES-128.
    void aesDecryptBlock(const unsigned int expandedKey[44], const unsigned char in[16], unsigned char out[16]);

    // The constants uses for AES-128.
    static const unsigned int aesTe0[256];
    static const unsigned int aesTe1[256];
    static const unsigned int aesTe2[256];
    static const unsigned int aesTe3[256];
    static const unsigned int aesTe4[256];
    static const unsigned int aesTd0[256];
    static const unsigned int aesTd1[256];
    static const unsigned int aesTd2[256];
    static const unsigned int aesTd3[256];
    static const unsigned int aesTd4[256];
    static const unsigned int aesRCon[10];


    // The variables used.

    // the 128-bit tweak
    unsigned char tweak_[16];

    // the expanded encryption key for the plaintext and the tweak
    unsigned int expandedKey1_[44];

    // the expanded encryption/decryption key for the tweak
    unsigned int expandedKey2_[44];
};

}

#endif // __AARNI_AES_H__
