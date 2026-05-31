#include "TimeManager.h"
#include <iostream>

FILETIME ConvertToMachineTime(const int year, const int month, const int day, const int hour, const int minute) {
    SYSTEMTIME systime;
    //Tout ce qu'on va pas demander à l'utilisateur, on le met à zéro
    systime.wMilliseconds = 0;
    systime.wSecond=0;
    systime.wDayOfWeek=0;

    //données à remplir

    systime.wYear= year;
    systime.wMonth= month;
    systime.wDay= day;
    systime.wHour= hour;
    systime.wMinute= minute;

    FILETIME ft;


    if (const bool convert = SystemTimeToFileTime(&systime, &ft); !convert) {
        std::cerr<<"Erreur  lors de la conversion\n";

    }


    return ft;
}
