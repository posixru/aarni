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
#include "ripemd160.h"

namespace Aarni
{

// Create a message digest using RIPEMD-160.
Ripemd160::Ripemd160(QObject* parent)
    : MessageDigest(ALGORITHM_HASH_RIPEMD160, parent)
{
    reset();
}

// Reset the digest to the initial state.
void Ripemd160::reset()
{
    h0_ = 0x67452301;
    h1_ = 0xEFCDAB89;
    h2_ = 0x98BADCFE;
    h3_ = 0x10325476;
    h4_ = 0xC3D2E1F0;
    bufLen_ = 0;
    total_ = 0;
}

// Update the digest.
void Ripemd160::update(const QByteArray& in)
{
    int len = in.length();
    int left = len + bufLen_;
    char buffer[64];
    const char* input = in.data();

    if (64 > left)
    {
        // we have less than 64 bytes (a block) to hash
        memcpy(buffer_ + bufLen_, input, len);
        bufLen_ = left;
        return ;
    }

    if (0 < bufLen_)
    {
        // we have data left in the context buffer
        int tmp = 64 - bufLen_;
        memcpy(buffer, buffer_, bufLen_);
        memcpy(buffer + bufLen_, input, tmp);
        hashBlock(buffer);
        input += tmp;
        left -= 64;
        total_ += 64;
    }

    while (64 <= left)
    {
        // hash the data block by block
        memcpy(buffer, input, 64);
        hashBlock(buffer);
        input += 64;
        left -= 64;
        total_ += 64;
    }

    // copy the data left to the context buffer
    if (0 != left)
    {
        memcpy(buffer_, input, left);
    }
    bufLen_ = left;
}

// Complete the hash computation and return the digest.
void Ripemd160::digest(QByteArray& out)
{
    // hash the last block
    total_ += bufLen_;
    memset(buffer_ + bufLen_, 0, 64 - bufLen_);
    buffer_[bufLen_] ^= 0x80;

    if (55 < bufLen_)
    {
        hashBlock(buffer_);
        memset(buffer_, 0, 64);
    }

    *(unsigned int*)(buffer_ + 56) = total_ << 3;
    *(unsigned int*)(buffer_ + 60) = total_ >> 29;
    hashBlock(buffer_);

    // generate the digest
    out.resize(20);

    memcpy(out.data(), &h0_, 4);
    memcpy(out.data() + 4, &h1_, 4);
    memcpy(out.data() + 8, &h2_, 4);
    memcpy(out.data() + 12, &h3_, 4);
    memcpy(out.data() + 16, &h4_, 4);
}

// Return the length of the digest in bytes.
int Ripemd160::getDigestLength() const
{
    return 20;
}

// Compute the digest of a single block.
void Ripemd160::hashBlock(const char* const in)
{
    unsigned int a, b, c, d, e;
    unsigned int aa, bb, cc, dd, ee;
    const unsigned int* const inbuf = (const unsigned int* const)in;

// cyclic left shift x over n positions
#define ROL(x, n)   (((x) << (n)) | ((x) >> (32-(n))))

// nonlinear functions for each round
#define F1(x, y, z) ((x) ^ (y) ^ (z))
#define F2(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define F3(x, y, z) (((x) | ~(y)) ^ (z))
#define F4(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define F5(x, y, z) ((x) ^ ((y) | ~(z)))

// operations for each round
#define RND(a, b, c, d, e, f, x, s, k) \
    (a) += f(b, c, d) + (x) + (k); \
    (a) = ROL(a, s) + (e); \
    (c) = ROL(c, 10);

    // left rounds
    a = h0_;
    b = h1_;
    c = h2_;
    d = h3_;
    e = h4_;

    // round 1
    RND(a, b, c, d, e, F1, inbuf[ 0], 11, 0);
    RND(e, a, b, c, d, F1, inbuf[ 1], 14, 0);
    RND(d, e, a, b, c, F1, inbuf[ 2], 15, 0);
    RND(c, d, e, a, b, F1, inbuf[ 3], 12, 0);
    RND(b, c, d, e, a, F1, inbuf[ 4],  5, 0);
    RND(a, b, c, d, e, F1, inbuf[ 5],  8, 0);
    RND(e, a, b, c, d, F1, inbuf[ 6],  7, 0);
    RND(d, e, a, b, c, F1, inbuf[ 7],  9, 0);
    RND(c, d, e, a, b, F1, inbuf[ 8], 11, 0);
    RND(b, c, d, e, a, F1, inbuf[ 9], 13, 0);
    RND(a, b, c, d, e, F1, inbuf[10], 14, 0);
    RND(e, a, b, c, d, F1, inbuf[11], 15, 0);
    RND(d, e, a, b, c, F1, inbuf[12],  6, 0);
    RND(c, d, e, a, b, F1, inbuf[13],  7, 0);
    RND(b, c, d, e, a, F1, inbuf[14],  9, 0);
    RND(a, b, c, d, e, F1, inbuf[15],  8, 0);

    // round 2
    RND(e, a, b, c, d, F2, inbuf[ 7],  7, 0x5A827999);
    RND(d, e, a, b, c, F2, inbuf[ 4],  6, 0x5A827999);
    RND(c, d, e, a, b, F2, inbuf[13],  8, 0x5A827999);
    RND(b, c, d, e, a, F2, inbuf[ 1], 13, 0x5A827999);
    RND(a, b, c, d, e, F2, inbuf[10], 11, 0x5A827999);
    RND(e, a, b, c, d, F2, inbuf[ 6],  9, 0x5A827999);
    RND(d, e, a, b, c, F2, inbuf[15],  7, 0x5A827999);
    RND(c, d, e, a, b, F2, inbuf[ 3], 15, 0x5A827999);
    RND(b, c, d, e, a, F2, inbuf[12],  7, 0x5A827999);
    RND(a, b, c, d, e, F2, inbuf[ 0], 12, 0x5A827999);
    RND(e, a, b, c, d, F2, inbuf[ 9], 15, 0x5A827999);
    RND(d, e, a, b, c, F2, inbuf[ 5],  9, 0x5A827999);
    RND(c, d, e, a, b, F2, inbuf[ 2], 11, 0x5A827999);
    RND(b, c, d, e, a, F2, inbuf[14],  7, 0x5A827999);
    RND(a, b, c, d, e, F2, inbuf[11], 13, 0x5A827999);
    RND(e, a, b, c, d, F2, inbuf[ 8], 12, 0x5A827999);

    // round 3
    RND(d, e, a, b, c, F3, inbuf[ 3], 11, 0x6ED9EBA1);
    RND(c, d, e, a, b, F3, inbuf[10], 13, 0x6ED9EBA1);
    RND(b, c, d, e, a, F3, inbuf[14],  6, 0x6ED9EBA1);
    RND(a, b, c, d, e, F3, inbuf[ 4],  7, 0x6ED9EBA1);
    RND(e, a, b, c, d, F3, inbuf[ 9], 14, 0x6ED9EBA1);
    RND(d, e, a, b, c, F3, inbuf[15],  9, 0x6ED9EBA1);
    RND(c, d, e, a, b, F3, inbuf[ 8], 13, 0x6ED9EBA1);
    RND(b, c, d, e, a, F3, inbuf[ 1], 15, 0x6ED9EBA1);
    RND(a, b, c, d, e, F3, inbuf[ 2], 14, 0x6ED9EBA1);
    RND(e, a, b, c, d, F3, inbuf[ 7],  8, 0x6ED9EBA1);
    RND(d, e, a, b, c, F3, inbuf[ 0], 13, 0x6ED9EBA1);
    RND(c, d, e, a, b, F3, inbuf[ 6],  6, 0x6ED9EBA1);
    RND(b, c, d, e, a, F3, inbuf[13],  5, 0x6ED9EBA1);
    RND(a, b, c, d, e, F3, inbuf[11], 12, 0x6ED9EBA1);
    RND(e, a, b, c, d, F3, inbuf[ 5],  7, 0x6ED9EBA1);
    RND(d, e, a, b, c, F3, inbuf[12],  5, 0x6ED9EBA1);

    // round 4
    RND(c, d, e, a, b, F4, inbuf[ 1], 11, 0x8F1BBCDC);
    RND(b, c, d, e, a, F4, inbuf[ 9], 12, 0x8F1BBCDC);
    RND(a, b, c, d, e, F4, inbuf[11], 14, 0x8F1BBCDC);
    RND(e, a, b, c, d, F4, inbuf[10], 15, 0x8F1BBCDC);
    RND(d, e, a, b, c, F4, inbuf[ 0], 14, 0x8F1BBCDC);
    RND(c, d, e, a, b, F4, inbuf[ 8], 15, 0x8F1BBCDC);
    RND(b, c, d, e, a, F4, inbuf[12],  9, 0x8F1BBCDC);
    RND(a, b, c, d, e, F4, inbuf[ 4],  8, 0x8F1BBCDC);
    RND(e, a, b, c, d, F4, inbuf[13],  9, 0x8F1BBCDC);
    RND(d, e, a, b, c, F4, inbuf[ 3], 14, 0x8F1BBCDC);
    RND(c, d, e, a, b, F4, inbuf[ 7],  5, 0x8F1BBCDC);
    RND(b, c, d, e, a, F4, inbuf[15],  6, 0x8F1BBCDC);
    RND(a, b, c, d, e, F4, inbuf[14],  8, 0x8F1BBCDC);
    RND(e, a, b, c, d, F4, inbuf[ 5],  6, 0x8F1BBCDC);
    RND(d, e, a, b, c, F4, inbuf[ 6],  5, 0x8F1BBCDC);
    RND(c, d, e, a, b, F4, inbuf[ 2], 12, 0x8F1BBCDC);

    // round 5
    RND(b, c, d, e, a, F5, inbuf[ 4],  9, 0xA953FD4E);
    RND(a, b, c, d, e, F5, inbuf[ 0], 15, 0xA953FD4E);
    RND(e, a, b, c, d, F5, inbuf[ 5],  5, 0xA953FD4E);
    RND(d, e, a, b, c, F5, inbuf[ 9], 11, 0xA953FD4E);
    RND(c, d, e, a, b, F5, inbuf[ 7],  6, 0xA953FD4E);
    RND(b, c, d, e, a, F5, inbuf[12],  8, 0xA953FD4E);
    RND(a, b, c, d, e, F5, inbuf[ 2], 13, 0xA953FD4E);
    RND(e, a, b, c, d, F5, inbuf[10], 12, 0xA953FD4E);
    RND(d, e, a, b, c, F5, inbuf[14],  5, 0xA953FD4E);
    RND(c, d, e, a, b, F5, inbuf[ 1], 12, 0xA953FD4E);
    RND(b, c, d, e, a, F5, inbuf[ 3], 13, 0xA953FD4E);
    RND(a, b, c, d, e, F5, inbuf[ 8], 14, 0xA953FD4E);
    RND(e, a, b, c, d, F5, inbuf[11], 11, 0xA953FD4E);
    RND(d, e, a, b, c, F5, inbuf[ 6],  8, 0xA953FD4E);
    RND(c, d, e, a, b, F5, inbuf[15],  5, 0xA953FD4E);
    RND(b, c, d, e, a, F5, inbuf[13],  6, 0xA953FD4E);

    // right rounds
    aa = h0_;
    bb = h1_;
    cc = h2_;
    dd = h3_;
    ee = h4_;

    // round 1
    RND(aa, bb, cc, dd, ee, F5, inbuf[ 5],  8, 0x50A28BE6);
    RND(ee, aa, bb, cc, dd, F5, inbuf[14],  9, 0x50A28BE6);
    RND(dd, ee, aa, bb, cc, F5, inbuf[ 7],  9, 0x50A28BE6);
    RND(cc, dd, ee, aa, bb, F5, inbuf[ 0], 11, 0x50A28BE6);
    RND(bb, cc, dd, ee, aa, F5, inbuf[ 9], 13, 0x50A28BE6);
    RND(aa, bb, cc, dd, ee, F5, inbuf[ 2], 15, 0x50A28BE6);
    RND(ee, aa, bb, cc, dd, F5, inbuf[11], 15, 0x50A28BE6);
    RND(dd, ee, aa, bb, cc, F5, inbuf[ 4],  5, 0x50A28BE6);
    RND(cc, dd, ee, aa, bb, F5, inbuf[13],  7, 0x50A28BE6);
    RND(bb, cc, dd, ee, aa, F5, inbuf[ 6],  7, 0x50A28BE6);
    RND(aa, bb, cc, dd, ee, F5, inbuf[15],  8, 0x50A28BE6);
    RND(ee, aa, bb, cc, dd, F5, inbuf[ 8], 11, 0x50A28BE6);
    RND(dd, ee, aa, bb, cc, F5, inbuf[ 1], 14, 0x50A28BE6);
    RND(cc, dd, ee, aa, bb, F5, inbuf[10], 14, 0x50A28BE6);
    RND(bb, cc, dd, ee, aa, F5, inbuf[ 3], 12, 0x50A28BE6);
    RND(aa, bb, cc, dd, ee, F5, inbuf[12],  6, 0x50A28BE6);

    // round 2
    RND(ee, aa, bb, cc, dd, F4, inbuf[ 6],  9, 0x5C4DD124);
    RND(dd, ee, aa, bb, cc, F4, inbuf[11], 13, 0x5C4DD124);
    RND(cc, dd, ee, aa, bb, F4, inbuf[ 3], 15, 0x5C4DD124);
    RND(bb, cc, dd, ee, aa, F4, inbuf[ 7],  7, 0x5C4DD124);
    RND(aa, bb, cc, dd, ee, F4, inbuf[ 0], 12, 0x5C4DD124);
    RND(ee, aa, bb, cc, dd, F4, inbuf[13],  8, 0x5C4DD124);
    RND(dd, ee, aa, bb, cc, F4, inbuf[ 5],  9, 0x5C4DD124);
    RND(cc, dd, ee, aa, bb, F4, inbuf[10], 11, 0x5C4DD124);
    RND(bb, cc, dd, ee, aa, F4, inbuf[14],  7, 0x5C4DD124);
    RND(aa, bb, cc, dd, ee, F4, inbuf[15],  7, 0x5C4DD124);
    RND(ee, aa, bb, cc, dd, F4, inbuf[ 8], 12, 0x5C4DD124);
    RND(dd, ee, aa, bb, cc, F4, inbuf[12],  7, 0x5C4DD124);
    RND(cc, dd, ee, aa, bb, F4, inbuf[ 4],  6, 0x5C4DD124);
    RND(bb, cc, dd, ee, aa, F4, inbuf[ 9], 15, 0x5C4DD124);
    RND(aa, bb, cc, dd, ee, F4, inbuf[ 1], 13, 0x5C4DD124);
    RND(ee, aa, bb, cc, dd, F4, inbuf[ 2], 11, 0x5C4DD124);

    // round 3
    RND(dd, ee, aa, bb, cc, F3, inbuf[15],  9, 0x6D703EF3);
    RND(cc, dd, ee, aa, bb, F3, inbuf[ 5],  7, 0x6D703EF3);
    RND(bb, cc, dd, ee, aa, F3, inbuf[ 1], 15, 0x6D703EF3);
    RND(aa, bb, cc, dd, ee, F3, inbuf[ 3], 11, 0x6D703EF3);
    RND(ee, aa, bb, cc, dd, F3, inbuf[ 7],  8, 0x6D703EF3);
    RND(dd, ee, aa, bb, cc, F3, inbuf[14],  6, 0x6D703EF3);
    RND(cc, dd, ee, aa, bb, F3, inbuf[ 6],  6, 0x6D703EF3);
    RND(bb, cc, dd, ee, aa, F3, inbuf[ 9], 14, 0x6D703EF3);
    RND(aa, bb, cc, dd, ee, F3, inbuf[11], 12, 0x6D703EF3);
    RND(ee, aa, bb, cc, dd, F3, inbuf[ 8], 13, 0x6D703EF3);
    RND(dd, ee, aa, bb, cc, F3, inbuf[12],  5, 0x6D703EF3);
    RND(cc, dd, ee, aa, bb, F3, inbuf[ 2], 14, 0x6D703EF3);
    RND(bb, cc, dd, ee, aa, F3, inbuf[10], 13, 0x6D703EF3);
    RND(aa, bb, cc, dd, ee, F3, inbuf[ 0], 13, 0x6D703EF3);
    RND(ee, aa, bb, cc, dd, F3, inbuf[ 4],  7, 0x6D703EF3);
    RND(dd, ee, aa, bb, cc, F3, inbuf[13],  5, 0x6D703EF3);

    // round 4
    RND(cc, dd, ee, aa, bb, F2, inbuf[ 8], 15, 0x7A6D76E9);
    RND(bb, cc, dd, ee, aa, F2, inbuf[ 6],  5, 0x7A6D76E9);
    RND(aa, bb, cc, dd, ee, F2, inbuf[ 4],  8, 0x7A6D76E9);
    RND(ee, aa, bb, cc, dd, F2, inbuf[ 1], 11, 0x7A6D76E9);
    RND(dd, ee, aa, bb, cc, F2, inbuf[ 3], 14, 0x7A6D76E9);
    RND(cc, dd, ee, aa, bb, F2, inbuf[11], 14, 0x7A6D76E9);
    RND(bb, cc, dd, ee, aa, F2, inbuf[15],  6, 0x7A6D76E9);
    RND(aa, bb, cc, dd, ee, F2, inbuf[ 0], 14, 0x7A6D76E9);
    RND(ee, aa, bb, cc, dd, F2, inbuf[ 5],  6, 0x7A6D76E9);
    RND(dd, ee, aa, bb, cc, F2, inbuf[12],  9, 0x7A6D76E9);
    RND(cc, dd, ee, aa, bb, F2, inbuf[ 2], 12, 0x7A6D76E9);
    RND(bb, cc, dd, ee, aa, F2, inbuf[13],  9, 0x7A6D76E9);
    RND(aa, bb, cc, dd, ee, F2, inbuf[ 9], 12, 0x7A6D76E9);
    RND(ee, aa, bb, cc, dd, F2, inbuf[ 7],  5, 0x7A6D76E9);
    RND(dd, ee, aa, bb, cc, F2, inbuf[10], 15, 0x7A6D76E9);
    RND(cc, dd, ee, aa, bb, F2, inbuf[14],  8, 0x7A6D76E9);

    // round 5
    RND(bb, cc, dd, ee, aa, F1, inbuf[12] ,  8, 0);
    RND(aa, bb, cc, dd, ee, F1, inbuf[15] ,  5, 0);
    RND(ee, aa, bb, cc, dd, F1, inbuf[10] , 12, 0);
    RND(dd, ee, aa, bb, cc, F1, inbuf[ 4] ,  9, 0);
    RND(cc, dd, ee, aa, bb, F1, inbuf[ 1] , 12, 0);
    RND(bb, cc, dd, ee, aa, F1, inbuf[ 5] ,  5, 0);
    RND(aa, bb, cc, dd, ee, F1, inbuf[ 8] , 14, 0);
    RND(ee, aa, bb, cc, dd, F1, inbuf[ 7] ,  6, 0);
    RND(dd, ee, aa, bb, cc, F1, inbuf[ 6] ,  8, 0);
    RND(cc, dd, ee, aa, bb, F1, inbuf[ 2] , 13, 0);
    RND(bb, cc, dd, ee, aa, F1, inbuf[13] ,  6, 0);
    RND(aa, bb, cc, dd, ee, F1, inbuf[14] ,  5, 0);
    RND(ee, aa, bb, cc, dd, F1, inbuf[ 0] , 15, 0);
    RND(dd, ee, aa, bb, cc, F1, inbuf[ 3] , 13, 0);
    RND(cc, dd, ee, aa, bb, F1, inbuf[ 9] , 11, 0);
    RND(bb, cc, dd, ee, aa, F1, inbuf[11] , 11, 0);

// undefine the macros used
#undef ROL
#undef F1
#undef F2
#undef F3
#undef F4
#undef F5
#undef RND

    // combine the two parts
    dd += h1_ + c;
    h1_ = h2_ + d + ee;
    h2_ = h3_ + e + aa;
    h3_ = h4_ + a + bb;
    h4_ = h0_ + b + cc;
    h0_ = dd;
}

}
