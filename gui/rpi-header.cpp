#include "rpi-header.h"

const QStringList RaspberryPiHeader::header {

"ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev",
"update_config=1",
"country={{COUNTRYCODE}}",

};
