#ifndef INFOLABEL_H
#define INFOLABEL_H

#include <QLabel>


class InfoLabel : public QLabel {
            Q_OBJECT
            Q_PROPERTY(QString helpText READ helpText WRITE setHelpText)

        public:
            explicit InfoLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
            ~InfoLabel();

            void setHelpText(QString helpText) {
                m_helpText = helpText;
            }
            QString helpText() const
            { return m_helpText; }

        signals:
            void clicked();

        protected:
            void mousePressEvent(QMouseEvent* event);

        private:
            QString m_helpText;
        };


#endif // INFOLABEL_H
