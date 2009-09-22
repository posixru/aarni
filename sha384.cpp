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

#include <QtEndian>

#include "sha384.h"

namespace Aarni
{

// round constants
quint64 SHA384::rcon_[80] = {
    Q_UINT64_C(0x428a2f98d728ae22), Q_UINT64_C(0x7137449123ef65cd),
    Q_UINT64_C(0xb5c0fbcfec4d3b2f), Q_UINT64_C(0xe9b5dba58189dbbc),
    Q_UINT64_C(0x3956c25bf348b538), Q_UINT64_C(0x59f111f1b605d019),
    Q_UINT64_C(0x923f82a4af194f9b), Q_UINT64_C(0xab1c5ed5da6d8118),
    Q_UINT64_C(0xd807aa98a3030242), Q_UINT64_C(0x12835b0145706fbe),
    Q_UINT64_C(0x243185be4ee4b28c), Q_UINT64_C(0x550c7dc3d5ffb4e2),
    Q_UINT64_C(0x72be5d74f27b896f), Q_UINT64_C(0x80deb1fe3b1696b1),
    Q_UINT64_C(0x9bdc06a725c71235), Q_UINT64_C(0xc19bf174cf692694),
    Q_UINT64_C(0xe49b69c19ef14ad2), Q_UINT64_C(0xefbe4786384f25e3),
    Q_UINT64_C(0x0fc19dc68b8cd5b5), Q_UINT64_C(0x240ca1cc77ac9c65),
    Q_UINT64_C(0x2de92c6f592b0275), Q_UINT64_C(0x4a7484aa6ea6e483),
    Q_UINT64_C(0x5cb0a9dcbd41fbd4), Q_UINT64_C(0x76f988da831153b5),
    Q_UINT64_C(0x983e5152ee66dfab), Q_UINT64_C(0xa831c66d2db43210),
    Q_UINT64_C(0xb00327c898fb213f), Q_UINT64_C(0xbf597fc7beef0ee4),
    Q_UINT64_C(0xc6e00bf33da88fc2), Q_UINT64_C(0xd5a79147930aa725),
    Q_UINT64_C(0x06ca6351e003826f), Q_UINT64_C(0x142929670a0e6e70),
    Q_UINT64_C(0x27b70a8546d22ffc), Q_UINT64_C(0x2e1b21385c26c926),
    Q_UINT64_C(0x4d2c6dfc5ac42aed), Q_UINT64_C(0x53380d139d95b3df),
    Q_UINT64_C(0x650a73548baf63de), Q_UINT64_C(0x766a0abb3c77b2a8),
    Q_UINT64_C(0x81c2c92e47edaee6), Q_UINT64_C(0x92722c851482353b),
    Q_UINT64_C(0xa2bfe8a14cf10364), Q_UINT64_C(0xa81a664bbc423001),
    Q_UINT64_C(0xc24b8b70d0f89791), Q_UINT64_C(0xc76c51a30654be30),
    Q_UINT64_C(0xd192e819d6ef5218), Q_UINT64_C(0xd69906245565a910),
    Q_UINT64_C(0xf40e35855771202a), Q_UINT64_C(0x106aa07032bbd1b8),
    Q_UINT64_C(0x19a4c116b8d2d0c8), Q_UINT64_C(0x1e376c085141ab53),
    Q_UINT64_C(0x2748774cdf8eeb99), Q_UINT64_C(0x34b0bcb5e19b48a8),
    Q_UINT64_C(0x391c0cb3c5c95a63), Q_UINT64_C(0x4ed8aa4ae3418acb),
    Q_UINT64_C(0x5b9cca4f7763e373), Q_UINT64_C(0x682e6ff3d6b2b8a3),
    Q_UINT64_C(0x748f82ee5defb2fc), Q_UINT64_C(0x78a5636f43172f60),
    Q_UINT64_C(0x84c87814a1f0ab72), Q_UINT64_C(0x8cc702081a6439ec),
    Q_UINT64_C(0x90befffa23631e28), Q_UINT64_C(0xa4506cebde82bde9),
    Q_UINT64_C(0xbef9a3f7b2c67915), Q_UINT64_C(0xc67178f2e372532b),
    Q_UINT64_C(0xca273eceea26619c), Q_UINT64_C(0xd186b8c721c0c207),
    Q_UINT64_C(0xeada7dd6cde0eb1e), Q_UINT64_C(0xf57d4f7fee6ed178),
    Q_UINT64_C(0x06f067aa72176fba), Q_UINT64_C(0x0a637dc5a2c898a6),
    Q_UINT64_C(0x113f9804bef90dae), Q_UINT64_C(0x1b710b35131c471b),
    Q_UINT64_C(0x28db77f523047d84), Q_UINT64_C(0x32caab7b40c72493),
    Q_UINT64_C(0x3c9ebe0a15c9bebc), Q_UINT64_C(0x431d67c49c100d4c),
    Q_UINT64_C(0x4cc5d4becb3e42b6), Q_UINT64_C(0x597f299cfc657e2a),
    Q_UINT64_C(0x5fcb6fab3ad6faec), Q_UINT64_C(0x6c44198c4a475817)
};

// Constructor.
SHA384::SHA384()
{
    reset();
}

// Reset the digest to the initial state.
void SHA384::reset()
{
    h0_ = Q_UINT64_C(0xcbbb9d5dc1059ed8);
    h1_ = Q_UINT64_C(0x629a292a367cd507);
    h2_ = Q_UINT64_C(0x9159015a3070dd17);
    h3_ = Q_UINT64_C(0x152fecd8f70e5939);
    h4_ = Q_UINT64_C(0x67332667ffc00b31);
    h5_ = Q_UINT64_C(0x8eb44a8768581511);
    h6_ = Q_UINT64_C(0xdb0c2e0d64f98fa7);
    h7_ = Q_UINT64_C(0x47b5481dbefa4fa4);
    bufLen_ = 0;
    total_ = 0;
}

// Update the digest.
void SHA384::update(const QByteArray& in)
{
    qint32 len = in.length();
    qint32 left = len + bufLen_;
    qint8 buffer[128];
    const qint8* input = (qint8*)in.data();

    if (128 > left)
    {
        // we have less than 128 bytes (a block) to hash
        memcpy(buffer_ + bufLen_, input, len);
        bufLen_ = left;
        return ;
    }

    if (0 < bufLen_)
    {
        // we have data left in the buffer
        qint32 tmp = 128 - bufLen_;
        memcpy(buffer, buffer_, bufLen_);
        memcpy(buffer + bufLen_, input, tmp);
        hashBlock(buffer);
        input += tmp;
        left -= 128;
        total_ += 128;
    }

    while (128 <= left)
    {
        // hash the data block by block
        memcpy(buffer, input, 128);
        hashBlock(buffer);
        input += 128;
        left -= 128;
        total_ += 128;
    }

    // copy the data left to the context buffer
    if (0 != left)
    {
        memcpy(buffer_, input, left);
    }
    bufLen_ = left;
}

// Complete the hash computation and return the digest.
void SHA384::digest(QByteArray& out)
{
    // hash the last block
    total_ += bufLen_;
    memset(buffer_ + bufLen_, 0, 128 - bufLen_);
    buffer_[bufLen_] ^= 0x80;

    if (111 < bufLen_)
    {
        hashBlock(buffer_);
        memset(buffer_, 0, 128);
    }

    qToBigEndian<quint64>(total_ >> 29, (uchar*)(buffer_ + 112));
    qToBigEndian<quint64>(total_ << 3, (uchar*)(buffer_ + 120));
    hashBlock(buffer_);

    // generate the digest
    out.resize(48);

    qToBigEndian<quint64>(h0_, (uchar*)out.data());
    qToBigEndian<quint64>(h1_, (uchar*)(out.data() + 8));
    qToBigEndian<quint64>(h2_, (uchar*)(out.data() + 16));
    qToBigEndian<quint64>(h3_, (uchar*)(out.data() + 24));
    qToBigEndian<quint64>(h4_, (uchar*)(out.data() + 32));
    qToBigEndian<quint64>(h5_, (uchar*)(out.data() + 40));

    // reset
    reset();
}

// Compute the digest of a single block.
void SHA384::hashBlock(const qint8* const in)
{
    quint64 a, b, c, d, e, f, g, h;
    quint64 w[80];

// macros used
#define ROTR(x,n) (((x) >> (n)) | ((x) << (64-(n))))
#define Ch(x,y,z) (((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SUM0(x) (ROTR((x), 28) ^ ROTR((x), 34) ^ ROTR((x), 39))
#define SUM1(x) (ROTR((x), 14) ^ ROTR((x), 18) ^ ROTR((x), 41))
#define SIGMA0(x) (ROTR((x), 1) ^ ROTR((x), 8) ^ ((x) >> 7))
#define SIGMA1(x) (ROTR((x), 19) ^ ROTR((x), 61) ^ ((x) >> 6))

    // prepare the message schedule
    for (qint32 i = 0; i < 16; i++)
    {
        qToBigEndian<quint64>(*(quint64*)(in + 8 * i), (uchar*)(w + i));
    }

    for (qint32 i = 16; i < 80; i++)
    {
        w[i] = SIGMA1(w[i - 2]) + w[i - 7] + SIGMA0(w[i - 15]) + w[i - 16];
    }

    // initialize the working variables with the chaining variables
    a = h0_; b = h1_; c = h2_; d = h3_;
    e = h4_; f = h5_; g = h6_; h = h7_;

    // 80 rounds
    for (qint32 i = 0; i < 80; i++)
    {
        quint64 t1, t2;

        t1 = h + SUM1(e) + Ch(e, f, g) + rcon_[i] + w[i];
        t2 = SUM0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // compute the updated chaining variables
    h0_ += a;
    h1_ += b;
    h2_ += c;
    h3_ += d;
    h4_ += e;
    h5_ += f;
    h6_ += g;
    h7_ += h;

#undef ROTR
#undef Ch
#undef Maj
#undef Sum0
#undef Sum1
#undef S0
#undef S1
}

}
