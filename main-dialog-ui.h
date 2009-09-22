#ifndef __MAIN_DIALOG_UI_H__
#define __MAIN_DIALOG_UI_H__

#include <QCheckBox>
#include <QDialog>
#include <QIcon>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSize>
#include <QPushButton>
#include <QWizard>
#include <QWizardPage>

namespace Aarni
{

class MainDialogUI : public QDialog
{
    Q_OBJECT

public:
    QGridLayout* layout_;
    QLabel* logoLabel_;
    QLabel* titleLabel_;
    QPushButton* encryptButton_;
    QPushButton* decryptButton_;
    QPushButton* browseButton_;
    QPushButton* aboutButton_;

    QWizard* encryptWizard_;
    QWizardPage* encryptSourcePage_;
    QWizardPage* encryptDestinationPage_;
    QWizardPage* encryptPasswordPage_;
    QGridLayout* encryptSourceLayout_;
    QGridLayout* encryptDestinationLayout_;
    QGridLayout* encryptPasswordLayout_;
    QLabel* encryptSourceLabel_;
    QLabel* encryptDestinationLabel_;
    QLabel* encryptPasswordLabel_;
    QPushButton* encryptBrowseSourceFileButton_;
    QPushButton* encryptBrowseSourceDirButton_;
    QPushButton* encryptBrowseDestinationButton_;
    QLineEdit* encryptSourceEdit_;
    QLineEdit* encryptDestinationEdit_;
    QLineEdit* encryptPasswordEdit_;
    QCheckBox* encryptPasswordCheckbox_;

    QWizard* decryptWizard_;
    QWizardPage* decryptSourcePage_;
    QWizardPage* decryptDestinationPage_;
    QWizardPage* decryptPasswordPage_;
    QGridLayout* decryptSourceLayout_;
    QGridLayout* decryptDestinationLayout_;
    QGridLayout* decryptPasswordLayout_;
    QLabel* decryptSourceLabel_;
    QLabel* decryptDestinationLabel_;
    QLabel* decryptPasswordLabel_;
    QPushButton* decryptBrowseSourceButton_;
    QPushButton* decryptBrowseDestinationButton_;
    QLineEdit* decryptSourceEdit_;
    QLineEdit* decryptDestinationEdit_;
    QLineEdit* decryptPasswordEdit_;
    QCheckBox* decryptPasswordCheckbox_;

    QDialog* aboutDialog_;
    QGridLayout* aboutDialogLayout_;
    QLabel* descLabel_;
    QLabel* logoAboutDialogLabel_;

    MainDialogUI(QWidget* parent = NULL)
        : QDialog(parent)
    {
        setupMainDialogUI();
        setupWizardUI();
        setupAboutDialogUI();

        retranslateUI();
    }

    void retranslateUI()
    {
        setWindowTitle(tr("DIALOG_TITLE"));

        titleLabel_->setText(tr("SUBTITLE_LABEL"));
        encryptButton_->setText(tr("ENCRYPT_BUTTON"));
        decryptButton_->setText(tr("DECRYPT_BUTTON"));
        browseButton_->setText(tr("BROWSE_BUTTON"));
        aboutButton_->setText(tr("ABOUT_BUTTON"));

        encryptWizard_->setWindowTitle(tr("ENCRYPT_WIZARD_TITLE"));
        encryptSourcePage_->setTitle(tr("ENCRYPT_SOURCE_PAGE"));
        encryptSourcePage_->setSubTitle(tr("ENCRYPT_SOURCE_SUBTITLE"));
        encryptDestinationPage_->setTitle(tr("ENCRYPT_DESTINATION_PAGE"));
        encryptDestinationPage_->setSubTitle(tr("ENCRYPT_DESTINATION_SUBTITLE"));
        encryptPasswordPage_->setTitle(tr("ENCRYPT_PASSWORD_PAGE"));
        encryptPasswordPage_->setSubTitle(tr("ENCRYPT_PASSWORD_SUBTITLE"));
        encryptSourceLabel_->setText(tr("ENCRYPT_SOURCE_LABEL"));
        encryptDestinationLabel_->setText(tr("ENCRYPT_DESTINATION_LABEL"));
        encryptPasswordLabel_->setText(tr("ENCRYPT_PASSWORD_LABEL"));
        encryptBrowseSourceFileButton_->setText(tr("ENCRYPT_BROWSE_SOURCE_FILE_BUTTON"));
        encryptBrowseSourceDirButton_->setText(tr("ENCRYPT_BROWSE_SOURCE_DIR_BUTTON"));
        encryptBrowseDestinationButton_->setText(tr("ENCRYPT_BROWSE_DESTINATION_BUTTON"));
        encryptPasswordCheckbox_->setText(tr("ENCRYPT_SHOW_PASSWORD_CHECKBOX"));

        decryptWizard_->setWindowTitle(tr("DECRYPT_WIZARD_TITLE"));
        decryptSourcePage_->setTitle(tr("DECRYPT_SOURCE_PAGE"));
        decryptSourcePage_->setSubTitle(tr("DECRYPT_SOURCE_SUBTITLE"));
        decryptDestinationPage_->setTitle(tr("DECRYPT_DESTINATION_PAGE"));
        decryptDestinationPage_->setSubTitle(tr("DECRYPT_DESTINATION_SUBTITLE"));
        decryptPasswordPage_->setTitle(tr("DECRYPT_PASSWORD_PAGE"));
        decryptPasswordPage_->setSubTitle(tr("DECRYPT_PASSWORD_SUBTITLE"));
        decryptSourceLabel_->setText(tr("DECRYPT_SOURCE_LABEL"));
        decryptDestinationLabel_->setText(tr("DECRYPT_DESTINATION_LABEL"));
        decryptPasswordLabel_->setText(tr("DECRYPT_PASSWORD_LABEL"));
        decryptBrowseSourceButton_->setText(tr("DECRYPT_BROWSE_SOURCE_DIR_BUTTON"));
        decryptBrowseDestinationButton_->setText(tr("DECRYPT_BROWSE_DESTINATION_BUTTON"));
        decryptPasswordCheckbox_->setText(tr("DECRYPT_SHOW_PASSWORD_CHECKBOX"));

        aboutDialog_->setWindowTitle(tr("ABOUT_DIALOG_TITLE"));
        descLabel_->setText(tr("DESCRIPTION_LABEL"));
    }

private:
    void setupMainDialogUI()
    {
        setWindowIcon(QIcon(":/images/logo.png"));

        layout_ = new QGridLayout;
        setLayout(layout_);

        logoLabel_ = new QLabel;
        logoLabel_->setPixmap(QPixmap(":/images/logo.png"));
        layout_->addWidget(logoLabel_, 0, 0);

        titleLabel_ = new QLabel;
        layout_->addWidget(titleLabel_, 0, 1, 1, 3);

        encryptButton_ = new QPushButton;
        encryptButton_->setIcon(QIcon(":/images/encrypt.png"));
        encryptButton_->setIconSize(QSize(48, 48));
        encryptButton_->setFixedSize(150, 60);
        layout_->addWidget(encryptButton_, 1, 0, 1, 2);

        decryptButton_ = new QPushButton;
        decryptButton_->setIcon(QIcon(":/images/decrypt.png"));
        decryptButton_->setIconSize(QSize(48, 48));
        decryptButton_->setFixedSize(150, 60);
        layout_->addWidget(decryptButton_, 1, 2, 1, 2);

        browseButton_ = new QPushButton;
        browseButton_->setIcon(QIcon(":/images/home.png"));
        browseButton_->setIconSize(QSize(48, 48));
        browseButton_->setFixedSize(150, 60);
        layout_->addWidget(browseButton_, 2, 0, 1, 2);

        aboutButton_ = new QPushButton;
        aboutButton_->setIcon(QIcon(":/images/about.png"));
        aboutButton_->setIconSize(QSize(48, 48));
        aboutButton_->setFixedSize(150, 60);
        layout_->addWidget(aboutButton_, 2, 2, 1, 2);
    }

    void setupWizardUI()
    {
        encryptWizard_ = new QWizard;
        encryptWizard_->setWindowIcon(QIcon(":/images/encrypt.png"));
        encryptWizard_->setModal(true);
        connect(encryptButton_, SIGNAL(clicked()), encryptWizard_, SLOT(show()));

        encryptSourcePage_ = new QWizardPage;
        encryptWizard_->addPage(encryptSourcePage_);
        encryptSourceLayout_ = new QGridLayout;
        encryptSourcePage_->setLayout(encryptSourceLayout_);
        encryptSourceLabel_ = new QLabel;
        encryptSourceLayout_->addWidget(encryptSourceLabel_, 0, 0);
        encryptSourceEdit_ = new QLineEdit;
        encryptSourceLayout_->addWidget(encryptSourceEdit_, 0, 1, 1, 3);
        encryptBrowseSourceFileButton_ = new QPushButton;
        encryptSourceLayout_->addWidget(encryptBrowseSourceFileButton_, 1, 1);
        encryptBrowseSourceDirButton_ = new QPushButton;
        encryptSourceLayout_->addWidget(encryptBrowseSourceDirButton_, 1, 2);

        encryptDestinationPage_ = new QWizardPage;
        encryptWizard_->addPage(encryptDestinationPage_);
        encryptDestinationLayout_ = new QGridLayout;
        encryptDestinationPage_->setLayout(encryptDestinationLayout_);
        encryptDestinationLabel_ = new QLabel;
        encryptDestinationLayout_->addWidget(encryptDestinationLabel_, 0, 0);
        encryptDestinationEdit_ = new QLineEdit;
        encryptDestinationLayout_->addWidget(encryptDestinationEdit_, 0, 1, 1, 3);
        encryptBrowseDestinationButton_ = new QPushButton;
        encryptDestinationLayout_->addWidget(encryptBrowseDestinationButton_, 1, 1);

        encryptPasswordPage_ = new QWizardPage;
        encryptWizard_->addPage(encryptPasswordPage_);
        encryptPasswordLayout_ = new QGridLayout;
        encryptPasswordPage_->setLayout(encryptPasswordLayout_);
        encryptPasswordLabel_ = new QLabel;
        encryptPasswordLayout_->addWidget(encryptPasswordLabel_, 0, 0);
        encryptPasswordEdit_ = new QLineEdit;
        encryptPasswordEdit_->setEchoMode(QLineEdit::Password);
        encryptPasswordLayout_->addWidget(encryptPasswordEdit_, 0, 1, 1, 3);
        encryptPasswordCheckbox_ = new QCheckBox;
        encryptPasswordLayout_->addWidget(encryptPasswordCheckbox_, 1, 1);

        decryptWizard_ = new QWizard;
        decryptWizard_->setWindowIcon(QIcon(":/images/decrypt.png"));
        decryptWizard_->setModal(true);
        connect(decryptButton_, SIGNAL(clicked()), decryptWizard_, SLOT(show()));

        decryptSourcePage_ = new QWizardPage;
        decryptWizard_->addPage(decryptSourcePage_);
        decryptSourceLayout_ = new QGridLayout;
        decryptSourcePage_->setLayout(decryptSourceLayout_);
        decryptSourceLabel_ = new QLabel;
        decryptSourceLayout_->addWidget(decryptSourceLabel_, 0, 0);
        decryptSourceEdit_ = new QLineEdit;
        decryptSourceLayout_->addWidget(decryptSourceEdit_, 0, 1, 1, 3);
        decryptBrowseSourceButton_ = new QPushButton;
        decryptSourceLayout_->addWidget(decryptBrowseSourceButton_, 1, 1);

        decryptDestinationPage_ = new QWizardPage;
        decryptWizard_->addPage(decryptDestinationPage_);
        decryptDestinationLayout_ = new QGridLayout;
        decryptDestinationPage_->setLayout(decryptDestinationLayout_);
        decryptDestinationLabel_ = new QLabel;
        decryptDestinationLayout_->addWidget(decryptDestinationLabel_, 0, 0);
        decryptDestinationEdit_ = new QLineEdit;
        decryptDestinationLayout_->addWidget(decryptDestinationEdit_, 0, 1, 1, 3);
        decryptBrowseDestinationButton_ = new QPushButton;
        decryptDestinationLayout_->addWidget(decryptBrowseDestinationButton_, 1, 1);

        decryptPasswordPage_ = new QWizardPage;
        decryptWizard_->addPage(decryptPasswordPage_);
        decryptPasswordLayout_ = new QGridLayout;
        decryptPasswordPage_->setLayout(decryptPasswordLayout_);
        decryptPasswordLabel_ = new QLabel;
        decryptPasswordLayout_->addWidget(decryptPasswordLabel_, 0, 0);
        decryptPasswordEdit_ = new QLineEdit;
        decryptPasswordEdit_->setEchoMode(QLineEdit::Password);
        decryptPasswordLayout_->addWidget(decryptPasswordEdit_, 0, 1, 1, 3);
        decryptPasswordCheckbox_ = new QCheckBox;
        decryptPasswordLayout_->addWidget(decryptPasswordCheckbox_, 1, 1);
    }

    void setupAboutDialogUI()
    {
        aboutDialog_ = new QDialog;
        aboutDialog_->setModal(true);
        aboutDialog_->setWindowIcon(QIcon(":/images/logo.png"));
        connect(aboutButton_, SIGNAL(clicked()), aboutDialog_, SLOT(show()));

        aboutDialogLayout_ = new QGridLayout;
        aboutDialog_->setLayout(aboutDialogLayout_);

        logoAboutDialogLabel_ = new QLabel;
        logoAboutDialogLabel_->setPixmap(QPixmap(":/images/logo.png"));
        aboutDialogLayout_->addWidget(logoAboutDialogLabel_, 0, 0);

        descLabel_ = new QLabel;
        descLabel_->setWordWrap(true);
        descLabel_->setOpenExternalLinks(true);
        aboutDialogLayout_->addWidget(descLabel_, 0, 1, 2, 1);
    }
};

}

#endif // __MAIN_DIALOG_UI_H__
