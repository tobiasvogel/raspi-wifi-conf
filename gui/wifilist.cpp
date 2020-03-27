#include "wifilist.h"
#ifdef USE_NETWORK
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>

WifiList::WifiList(QWidget *parent)
{

    widgetLayout = new QVBoxLayout(this);
    label = new QLabel(nullptr);
    label->setText(tr("Select any of the Networks to be used:"));
    widgetLayout->addWidget(label);
    wifiListWidget = new QTreeWidget(nullptr);
    wifiListWidget->setHeaderLabels({"#", "SSID"});
    wifiListWidget->setColumnCount(2);
    wifiListWidget->setColumnWidth(0, 35);
    wifiListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    widgetLayout->addWidget(wifiListWidget);
    buttonLayout = new QHBoxLayout();
    widgetLayout->addLayout(buttonLayout);
    cancelButton = new QPushButton(nullptr);
    applyButton = new QPushButton(nullptr);
    buttonSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Ignored);
    buttonLayout->addSpacerItem(buttonSpacer);
    buttonLayout->addWidget(cancelButton);
    buttonSeparator = new QSpacerItem(4,20,QSizePolicy::Fixed,QSizePolicy::Ignored);
    buttonLayout->addSpacerItem(buttonSeparator);
    buttonLayout->addWidget(applyButton);
    cancelButton->setText(tr("&Cancel"));
    applyButton->setText(tr("&Use"));
    QObject::connect(applyButton, SIGNAL(clicked()), this, SLOT(loadWiFiSSID()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelled()));
    QObject::connect(wifiListWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(wifiDoubleClicked(QTreeWidgetItem*, int)));

    int i = 1;

    auto nc = ncm.allConfigurations();

    for (auto &x : nc) {
        if (x.bearerType() == QNetworkConfiguration::BearerWLAN) {
            QTreeWidgetItem * wifiItem = new QTreeWidgetItem();
            wifiItem->setTextAlignment(0,Qt::AlignVCenter);
            wifiItem->setTextAlignment(1,Qt::AlignLeft);
            wifiItem->setText(0,QString::number(i));
            wifiItem->setText(1,x.name());
            wifiListWidget->addTopLevelItem(wifiItem);
            i++;
        }
    }

}

void WifiList::loadWiFiSSID(void)
{
    if (wifiListWidget->selectedItems().count() == 0) { return; }
    selectedSSID = wifiListWidget->selectedItems().first()->text(1);
    this->close();
}


void WifiList::cancelled()
{
    this->close();
}

void WifiList::wifiDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item == nullptr) { return; }
    selectedSSID = item->text(1);
    this->close();
}
#else
WifiList::WifiList(QWidget *parent) {}
#endif
