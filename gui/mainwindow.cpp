#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "infolabel.h"
#include "editheaders.h"
#include "wifilist.h"
#include "../incl/wpa_passphrase.h"

#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QObject>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("WPA Configuration"));
#ifndef USE_NETWORK
    ui->wifiScanPushButton->hide();
#endif
    raspiHeaders = RaspberryPiHeader::header;
    ui->countryCodeLineEdit->setText(QLocale().name().split('_').at(1));
    ui->countryCodeLineEdit->setPlaceholderText(QLocale().name().split('_').at(1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::getRaspiHeaders()
{
    return raspiHeaders;
}

void MainWindow::setRaspiHeaders(QString headerString)
{
    QStringList headerList;
    headerList = headerString.split(QString("\n"), QString::SkipEmptyParts);

    this->setRaspiHeaders(headerList);
}

void MainWindow::setRaspiHeaders(QStringList newHeaders)
{
    raspiHeaders = newHeaders;
}

void MainWindow::unhidePSK()
{
    if (!pskIsHidden()) { return; }
    ui->pskLineEdit->setEchoMode(QLineEdit::Normal);
    ui->hidePskPushButton->setIcon(QIcon(":/hide-icon"));
}

void MainWindow::hidePSK()
{
    if (pskIsHidden()) { return; }
    ui->pskLineEdit->setEchoMode(QLineEdit::Password);
    ui->hidePskPushButton->setIcon(QIcon(":/unhide-icon"));
}

bool MainWindow::pskIsHidden()
{
    if (ui->pskLineEdit->echoMode() == QLineEdit::Password) {
        return true;
    } else {
        return false;
    }
}

void MainWindow::togglePskVisibility()
{
    if (pskIsHidden()) {
        unhidePSK();
    } else {
        hidePSK();
    }
}

void MainWindow::toggleNoPassword(bool toggle)
{
    if (toggle) {
        if (!pskIsHidden()) { hidePSK(); }
        ui->pskLineEdit->setEnabled(false);
        ui->hidePskPushButton->setEnabled(false);
        ui->hidePlainPasswordCheckBox->setEnabled(false);
    } else {
        ui->pskLineEdit->setEnabled(true);
        ui->hidePskPushButton->setEnabled(true);
        ui->hidePlainPasswordCheckBox->setEnabled(true);
    }
}

void MainWindow::toggleNoRaspiHeaders(bool toggle)
{

        ui->editHeaderPushButton->setEnabled(!toggle);
}

void MainWindow::toggleNetworkIdentification(bool toggle)
{
ui->networkIdentificationLineEdit->setEnabled(toggle);
}

void MainWindow::toggleNetworkPriority(bool toggle)
{
ui->networkPrioritySpinBox->setEnabled(toggle);
}

void MainWindow::displayHelpText()
{
    QVariant getVal = this->sender()->property("helpText");
    QString helpText = "";
    if (getVal.canConvert<QString>()) {
        helpText = getVal.toString();
        QMessageBox msgBox;
        msgBox.setText(helpText);
        msgBox.setWindowTitle(tr("Help"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::editHeaders()
{
    EditHeaders headerEditor;
    headerEditor.injectLines(getRaspiHeaders());
    headerEditor.setMinimumSize(this->width(), 260);
    headerEditor.setModal(true);
    headerEditor.exec();
    if (!headerEditor.wasAborted()) {
        this->setRaspiHeaders(headerEditor.getModifiedHeaders());
    }
}

void MainWindow::browseForOutputDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::current().path(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty()) {
        return;
    } else {
        ui->outputLineEdit->setText(dir);
    }
}

void MainWindow::scanForNetworks()
{

#ifdef USE_NETWORK
    WifiList scanList;
    scanList.setMinimumSize(this->width(), 260);
    scanList.setModal(true);
    scanList.exec();
    if (!scanList.selectedSSID.isEmpty()) {
        ui->ssidLineEdit->setText(scanList.selectedSSID);
    }
#else
    return;
#endif
}

void MainWindow::generateOutput(void) {

        if (!this->preFlightCheck()) { return; }

    output.clear();
    QString encryptedPSK = "";
    QString unencryptedPSK = ui->pskLineEdit->text();
    QString countryCode = ui->countryCodeLineEdit->text();
    bool fileExists;

    QMessageBox msgBox;
    bool error = false;

    QFile configFile(QDir().toNativeSeparators(ui->outputLineEdit->text()).append(QDir::separator()).append("wpa_supplicant.conf"));
    QFile sshFile(QDir().toNativeSeparators(ui->outputLineEdit->text()).append(QDir::separator()).append("ssh"));

    if (ui->sshCheckBox->isChecked()) {
        if (sshFile.exists()) {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("'ssh'-File already exists in the Directory specified."));
            msgBox.exec();
        } else {
            if (!sshFile.open(QFile::ReadWrite)) {
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText(tr("Error creating the 'ssh'-File. Check your File-Permissions."));
                msgBox.exec();
                error = true;
            } else {
                sshFile.close();
            }
        }
    }

    fileExists = configFile.exists();

    std::string str_ssid = ui->ssidLineEdit->text().toStdString();
    std::string str_passphrase = ui->pskLineEdit->text().toStdString();

    const char *char_ssid = str_ssid.c_str();
    const char *char_passphrase = str_passphrase.c_str();

    size_t ssid_len = ui->ssidLineEdit->text().length();
    size_t passphrase_len = ui->pskLineEdit->text().length();

    char *char_encodedPSK = encode(char_ssid, ssid_len, char_passphrase, passphrase_len);

    encryptedPSK = char_encodedPSK;

    if (!(fileExists && ui->appendCheckBox->isChecked())) {
    if (!ui->noRasbpiCheckBox->isChecked()) {
    foreach (QString hLine, getRaspiHeaders()) {
        hLine.replace(QString("{{COUNTRYCODE}}"), countryCode);
        output.append(hLine);
    }
    }
    output.append(QString(""));
    } else {
        output.append(QString(""));
    }
    output.append("network={");
    output.append(QString(("    ssid=\"")).append(ui->ssidLineEdit->text()).append("\""));
    if (ui->hiddenNetworkCheckBox->isChecked()) {
        output.append("    scan_ssid=1");
    }
    if (ui->noPasswordCheckBox->isChecked()) {
        output.append("    key_mgmt=NONE");
    } else {
        if (!ui->hidePlainPasswordCheckBox->isChecked()) {
            output.append(QString("    #psk=\"").append(unencryptedPSK).append("\""));
        }
        output.append(QString("    psk=").append(encryptedPSK.toLower()));
    }
    if (ui->assignPriorityCheckBox->isChecked()) {
        output.append(QString("    priority=%1").arg(ui->networkPrioritySpinBox->value()));
    }
    if (ui->provideIdentificationCheckBox->isChecked()) {
        output.append(QString("    id_str=\"").append(ui->networkIdentificationLineEdit->text()).append("\""));
    }
    output.append("}");

    if (!ui->appendCheckBox->isChecked() && fileExists) {
        msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Config File already exists. Check the corresponding Option if you want to Append another Network Configuration to it."));
            msgBox.exec();
            error = true;
    } else {
    if (!configFile.open(QIODevice::Append)) {
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setText(tr("Error opening the Config File. Check your File-Permissions."));
          msgBox.exec();
          error = true;
          msgBox.setIcon(QMessageBox::Question);
          msgBox.setText(tr("You can still Manually Copy the Generated Output to a File called \" wpa_supplicant.conf \".\nWould you like me to Display the generated File-Contents in order for you to Copy it?"));
          msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
          msgBox.setDefaultButton(QMessageBox::Yes);
          int ret = msgBox.exec();
          if (ret == QMessageBox::Yes) {
            this->showCopyPasteDialog(output);
          }
     } else {
        QTextStream qout(&configFile);
        foreach (QString line, output) {
            qout << line;
            qout << QChar('\n');
        }
        configFile.close();
    }
    }
    if (!error) {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("File(s) generated successfully!"));
        msgBox.exec();
    }
}

void MainWindow::showCopyPasteDialog(QStringList contentList)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QDialog *copyPasteDialog;
    copyPasteDialog = new QDialog(this);
    copyPasteDialog->setMinimumSize(this->width(), 360);
    QVBoxLayout layout(copyPasteDialog);
    QLabel *label;
    label = new QLabel(nullptr);
    label->setText(tr("Copy the Lines below (\" wpa_supplicant.conf \"):"));
    layout.addWidget(label);
    QTextEdit *content;
    content = new QTextEdit(nullptr);
    layout.addWidget(content);
    QHBoxLayout *buttonLayout;
    buttonLayout = new QHBoxLayout(nullptr);
    layout.addLayout(buttonLayout);
    QPushButton *copyToClipboard;
    copyToClipboard = new QPushButton(nullptr);
    copyToClipboard->setText(tr("Copy to Clipboard"));
    QPushButton *closeButton;
    closeButton = new QPushButton(nullptr);
    closeButton->setText(tr("Close"));
    QSpacerItem *buttonSpacer;
    buttonSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Ignored);
    QSpacerItem *buttonSeparator;
    buttonSeparator = new QSpacerItem(4,20,QSizePolicy::Fixed,QSizePolicy::Ignored);
    buttonLayout->addSpacerItem(buttonSpacer);
    buttonLayout->addWidget(copyToClipboard);
    buttonLayout->addSpacerItem(buttonSeparator);
    buttonLayout->addWidget(closeButton);
    foreach (QString mline, contentList) {
        content->append(mline);
    }
    content->setReadOnly(true);
    QObject::connect(copyToClipboard, &QPushButton::clicked,
                     [=]() { clipboard->setText(content->toPlainText(), QClipboard::Clipboard); QMessageBox::information(this, tr("Success"), tr("Successfully copied data to Clipboard."), QMessageBox::Ok); });
    QObject::connect(closeButton, &QPushButton::clicked,
                     [=]() { copyPasteDialog->accept(); });
    copyPasteDialog->exec();
}

void MainWindow::showAbout()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(tr("About"));
    QString aboutText = tr("This Program is released under the GPLv2 License.");
    aboutText.append("\n\n");
    aboutText.append(tr("The \"wpa_supplicant\" utility is released under the BSD License (see %1)").arg("http://w1.fi/wpa_supplicant/"));
    aboutText.append("\n\n");
    aboutText.append(tr("%1 2020 by %2").arg(QChar(0x00A9)).arg("Tobias X. Vogel"));
    msgBox.setText(aboutText);
    QString detailedText = tr("Please report Bugs at %1").arg("https://github.com/tobiasvogel/raspi-wifi-conf/issues");
    detailedText.append("\n\n");
    detailedText.append(tr("Contributions and Translations welcome! Please submit Pull Requests via GitHub at %1").arg("https://github.com/tobiasvogel/raspi-wifi-conf/pulls"));
    msgBox.setDetailedText(detailedText);
    msgBox.exec();
}

bool MainWindow::preFlightCheck()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    // non-empty ssid
    if (ui->ssidLineEdit->text().isEmpty()) {
        msgBox.setText(tr("No SSID ha-s been specified."));
        msgBox.exec();
        return false;
    }
    if (!(ui->noPasswordCheckBox->isChecked())) {
    // passphrase length
    if (ui->pskLineEdit->text().length() < 8) {
        msgBox.setText(tr("A WiFi-Passphrase must contain at least 8 characters."));
        msgBox.exec();
        return false;
    }
    if (ui->pskLineEdit->text().length() > 63) {
        msgBox.setText(tr("A WiFi-Passphrase cannot exceed 63 characters in length."));
        msgBox.exec();
        return false;
    }
    if (ui->outputLineEdit->text().isEmpty()) {
        msgBox.setText(tr("No Output directory specified."));
        msgBox.exec();
        return false;
    }
    // passphrase characters
    msgBox.setText(tr("The Passphrase you entered contains invalid characters."));
    for (int i = 0; i < ui->pskLineEdit->text().length(); i++) {
        if (ui->pskLineEdit->text().at(i).unicode() < 32) {
            msgBox.exec();
            return false;
        }
        if (ui->pskLineEdit->text().at(i).unicode() == 127) {
            msgBox.exec();
            return false;
        }
    }
    }
    // output directory
    if (!QDir().exists(ui->outputLineEdit->text())) {
        msgBox.setText(tr("The Output Directory specified does not exist."));
        msgBox.exec();
        return false;
    }

    return true;
}


