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

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "encryptor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator trans;
    if (!trans.load(":/trans/lang_" + QLocale::system().name()))
    {
        trans.load(":/trans/lang_en");
    }
    app.installTranslator(&trans);

    Aarni::Encryptor encryptor;

    return app.exec();
}
