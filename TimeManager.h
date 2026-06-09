#ifndef HORODATAGE_TIMEMANAGER_H
#define HORODATAGE_TIMEMANAGER_H

#include <windows.h>

FILETIME ConvertToMachineTime(int year, int month, int day, int hour, int minute);

#endif //HORODATAGE_TIMEMANAGER_H
