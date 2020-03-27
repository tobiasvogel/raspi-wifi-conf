#ifndef EDITHEADERS_H
#define EDITHEADERS_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


class EditHeaders : public QDialog
{
    Q_OBJECT
public:
    explicit EditHeaders(QWidget *parent = nullptr);

    void injectLines(QStringList lines);

    bool wasAborted(void);

    QString getModifiedHeaders(void);

signals:

public slots:
    void editAborted(void);
    void editApplied(void);

private:
    QLabel *label;
    QTextEdit *editField;
    QVBoxLayout *widgetLayout;
    QPushButton *cancelButton;
    QPushButton *applyButton;
    QHBoxLayout *buttonLayout;
    QSpacerItem *buttonSpacer;
    QSpacerItem *buttonSeparator;

    bool aborted = true;
};

#endif // EDITHEADERS_H
