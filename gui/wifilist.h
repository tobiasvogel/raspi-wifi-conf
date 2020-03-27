#ifndef WIFILIST_H
#define WIFILIST_H

#include <QDialog>
#include <QTreeWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#ifdef USE_NETWORK
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>


class WifiList : public QDialog
{
    Q_OBJECT
public:
    explicit WifiList(QWidget *parent = nullptr);

    QString selectedSSID = "";


signals:

public slots:
    void loadWiFiSSID(void);
    void cancelled(void);
    void wifiDoubleClicked(QTreeWidgetItem*, int);

protected:
    QLabel *label;
    QVBoxLayout *widgetLayout;
    QTreeWidget *wifiListWidget;
    QPushButton *cancelButton;
    QPushButton *applyButton;
    QHBoxLayout *buttonLayout;
    QSpacerItem *buttonSpacer;
    QSpacerItem *buttonSeparator;

    QNetworkConfiguration cfg;
    QNetworkConfigurationManager ncm;
};

#else
class WifiList : public QDialog
{
    Q_OBJECT
public:
    explicit WifiList(QWidget *parent = nullptr);

    QString selectedSSID = "";


signals:

};
#endif

#endif // WIFILIST_H
