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

#include <QGridLayout>
#include <QIcon>

#include "about-dialog.h"

namespace Aarni
{

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
    retranslateUI();
}

void AboutDialog::retranslateUI()
{
    setWindowTitle(tr("ABOUT_DIALOG_TITLE"));

    descLabel_->setText(tr("DESCRIPTION_LABEL"));
}

void AboutDialog::setupUI()
{
    setModal(true);
    setWindowIcon(QIcon(":/images/logo.png"));

    QGridLayout* layout = new QGridLayout;
    setLayout(layout);

    logoLabel_ = new QLabel;
    logoLabel_->setPixmap(QPixmap(":/images/logo.png"));
    layout->addWidget(logoLabel_, 0, 0);

    descLabel_ = new QLabel;
    descLabel_->setWordWrap(true);
    descLabel_->setOpenExternalLinks(true);
    layout->addWidget(descLabel_, 0, 1, 2, 1);
}

}
