#include "mainwindow.h"
#include "editheaders.h"


EditHeaders::EditHeaders(QWidget *parent)
{
    widgetLayout = new QVBoxLayout(this);
    label = new QLabel(nullptr);
    widgetLayout->addWidget(label);
    label->setText(tr("Edit the lines below according to your needs. Be careful not to break the '{{COUNTRYCODE}}' placeholder."));
    editField = new QTextEdit(nullptr);
    widgetLayout->addWidget(editField);
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
    cancelButton->setText(tr("A&bort"));
    cancelButton->setIcon(QIcon(":/abort-icon"));
    applyButton->setText(tr("&Apply"));
    applyButton->setIcon(QIcon(":/apply-icon"));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(editAborted()));
    QObject::connect(applyButton, SIGNAL(clicked()), this, SLOT(editApplied()));
}

void EditHeaders::injectLines(QStringList lines)
{
    editField->clear();
    foreach (QString l, lines) {
        editField->append(l);
    }
}

bool EditHeaders::wasAborted()
{
    return aborted;
}

QString EditHeaders::getModifiedHeaders()
{
    return editField->toPlainText();
}

void EditHeaders::editAborted()
{
    aborted = true;
    this->close();
}

void EditHeaders::editApplied()
{
    aborted = false;
    this->close();
}
