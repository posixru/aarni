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

#ifndef __AARNI_MESSAGE_DIGEST_H__
#define __AARNI_MESSAGE_DIGEST_H__

#include <QByteArray>
#include <QObject>
#include <QString>

namespace Aarni
{

class MessageDigest : public QObject
{
protected:
    // Create a message digest with the specified algorithm name.
    MessageDigest(const QString& algorithm, QObject* parent = NULL);

public:
    // Return a MessageDigest object that implements the specified algorithm.
    static MessageDigest* getInstance(const QString& algorithm);

    // Reset the digest to the initial state.
    virtual void reset() = 0;

    // Update the digest.
    virtual void update(const QByteArray& in) = 0;

    // Complete the hash computation and return the digest.
    virtual void digest(QByteArray& out) = 0;

    // Perform a final update on the digest and complete the hash computation.
    void digest(const QByteArray& in, QByteArray& out);

    // Return a string that identifies the algorithm.
    QString getAlgorithm() const;

    // Return the length of the digest in bytes.
    virtual int getDigestLength() const = 0;

private:
    // the name of the algorithm used
    QString algorithmName_;
};

}

#endif // __AARNI_MESSAGE_DIGEST_H__
