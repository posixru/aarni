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

#ifndef __AARNI_CONST_H__
#define __AARNI_CONST_H__

#include <QString>

namespace Aarni
{

// the magic number to identify the file
const char FILE_MAGIC_BYTE1 = 0x45;
const char FILE_MAGIC_BYTE2 = 0x41;

// current file version
const char FILE_VERSION = 0x01;

// algorithm names and ids
const QString ALGORITHM_NONE = "NONE";
const char ALGORITHM_NONE_ID = 0x00;

const QString ALGORITHM_HASH_RIPEMD160 = "RIPEMD160";
const char ALGORITHM_HASH_RIPEMD160_ID = 0x01;

const QString ALGORITHM_HASH_SHA256 = "SHA256";
const char ALGORITHM_HASH_SHA256_ID = 0x02;

const QString ALGORITHM_CIPHER_AES = "AES";
const char ALGORITHM_CIPHER_AES_ID = 0x01;

const QString ALGORITHM_CIPHER_ARC4 = "ARC4";
const char ALGORITHM_CIPHER_ARC4_ID = 0x02;

// the file is divided into blocks when encrypted
const int BLOCK_SIZE = 512;

// the length of the digest
const int DIGEST_SIZE = 20;

// operation mode
enum CIPHER_OP_MODE {ENCRYPT_MODE, DECRYPT_MODE};

// error code
const int ERROR_SUCCESS = 0;
const int ERROR_NO_PASSWORD = 1;
const int ERROR_ENCRYPTION_CANCELED = 2;
const int ERROR_FILE_OPEN_FAILED = 10;
const int ERROR_FILE_INVALID_FORMAT = 11;
const int ERROR_FILE_CORRUPTED = 12;

}

#endif // __AARNI_CONST_H__
