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

#ifndef __AARNI_DECRYPTION_WIZARD_H__
#define __AARNI_DECRYPTION_WIZARD_H__

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWizard>
#include <QWizardPage>

#include "encryptor.h"

namespace Aarni
{

class DecWizard : public QWizard
{
    Q_OBJECT

public:
    DecWizard(QWidget* parent = 0);

    void retranslateUI();

signals:
    void decryptionRequested(EncryptionParameter param);

public slots:
    // Reimplement QDialog::accept(), which is called when the user clicks the
    // Finish button.
    virtual void accept();

private:
    void setupUI();
};

class DecWizardPage: public QWizardPage
{
    Q_OBJECT

public:
    DecWizardPage(QWidget* parent = 0) : QWizardPage(parent) {}

    virtual void retranslateUI() = 0;
};

class DecSourcePage : public DecWizardPage
{
    Q_OBJECT

public:
    DecSourcePage(QWidget* parent = 0);

    // Reimplement QWizardPage::isComplete(), which determines whether the Next
    // or Finish button should be enabled or disabled.
    virtual bool isComplete() const;

    virtual void retranslateUI();

private slots:
    void browseFile();
    void validate();

private:
    void setupUI();

    QLabel* sourceLabel_;
    QLabel* warningLabel_;
    QLineEdit* sourceLineEdit_;
    QPushButton* fileBrowseButton_;
 };

class DecDestinationPage : public DecWizardPage
{
    Q_OBJECT

public:
    DecDestinationPage(QWidget* parent = 0);

    // Reimplement QWizardPage::isComplete(), which determines whether the Next
    // or Finish button should be enabled or disabled.
    virtual bool isComplete() const;

    virtual void retranslateUI();

private slots:
    void browseDestination();
    void validate();

private:
    void setupUI();

    QLabel* destinationLabel_;
    QLabel* warningLabel_;
    QLineEdit* destinationLineEdit_;
    QPushButton* destinationBrowseButton_;
 };

class DecPasswordPage : public DecWizardPage
{
    Q_OBJECT

public:
    DecPasswordPage(QWidget* parent = 0);

    // Reimplement QWizardPage::isComplete(), which determines whether the Next
    // or Finish button should be enabled or disabled.
    virtual bool isComplete() const;

    virtual void retranslateUI();

private slots:
    void showPassword(int state);

private:
    void setupUI();

    QCheckBox* passwordCheckBox_;
    QLabel* passwordLabel_;
    QLineEdit* passwordLineEdit_;
 };

class DecConclusionPage : public DecWizardPage
{
    Q_OBJECT

public:
    DecConclusionPage(QWidget* parent = NULL);

    // Reimplement the QWizardPage::initializePage() function, which is called
    // just before the page is shown.
    virtual void initializePage();

    virtual void retranslateUI();

private:
    void setupUI();

    QLabel* sourceDescLabel_;
    QLabel* sizeDescLabel_;
    QLabel* destinationDescLabel_;
    QLabel* sourceLabel_;
    QLabel* sizeLabel_;
    QLabel* destinationLabel_;
 };

}

#endif // __AARNI_DECRYPTION_WIZARD_H__
