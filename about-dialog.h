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

#ifndef __AARNI_ABOUT_DIALOG_H__
#define __AARNI_ABOUT_DIALOG_H__

#include <QDialog>
#include <QLabel>

namespace Aarni
{

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget* parent = NULL);

    void retranslateUI();

private:
    void setupUI();

    QLabel* descLabel_;
    QLabel* logoLabel_;
};

}

#endif // __AARNI_ABOUT_DIALOG_H__
