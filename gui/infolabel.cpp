#include "infolabel.h"

InfoLabel::InfoLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

InfoLabel::~InfoLabel() {}

void InfoLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
