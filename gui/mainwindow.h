#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rpi-header.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    QStringList raspiHeaders;
    QStringList getRaspiHeaders(void);
    void setRaspiHeaders(QString);
    void setRaspiHeaders(QStringList);

private slots:
    void unhidePSK(void);
    void hidePSK(void);
    bool pskIsHidden(void);
    void togglePskVisibility(void);
    void toggleNoPassword(bool);
    void toggleNoRaspiHeaders(bool);
    void toggleNetworkIdentification(bool);
    void toggleNetworkPriority(bool);
    void displayHelpText(void);
    void editHeaders(void);
    void browseForOutputDir(void);
    void scanForNetworks(void);
    void generateOutput();
    void showCopyPasteDialog(QStringList);

    void showAbout(void);

    bool preFlightCheck(void);
private:
    Ui::MainWindow *ui;

    QStringList output;
};
#endif // MAINWINDOW_H
