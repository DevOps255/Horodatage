#include <iostream>
#include "TimeManager.h"
#include <string>
#include "FileManager.h"

int main() {

    int YearInput, MonthInput, DayInput, HourInput, MinuteInput;
    std::string path;


    std::cout <<"salut!!!!!! alors si votre prof vous a donné un devoir et vous n'avez plus le droit de le modifier,\n"
                "sachez que j'ai la solution, elle s'apelle... j'ai pas trouvé de nom\n";


    std::cout << "entrez l'année que vous voulez remplacer (ex:2022): \n ";
    std::cin>>YearInput;

    std::cout <<"Entrez le mois que vous voulez remplacer(1-12):\n";
    std::cin>>MonthInput;

    std::cout << "Entrez le jour que vous voulez remplacer(1-30)\n";
    std::cin>>DayInput;

    std::cout << "Entrez l'heure que vous voulez modifier (0-23)\n";
    std::cin>>HourInput;

    std::cout <<"Entrez la minute que vous voulez modiifer (0-59)\n";
    std::cin>>MinuteInput;

    std::cout <<"OK!! tout as été enregistré\n";

    std::cout <<"Entrez le chemin complet du fichier à modifier:\n";
    std::cin >> path;

    //conversion du format systemtime au format binaire filetime

    FILETIME BinaryTime = ConvertToMachineTime(YearInput, MonthInput, DayInput, HourInput, MinuteInput);
    HANDLE file = OpenFileHandle(path);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr<<"Accès refusé!\n";
        return 1;
    }

    if (!SetFileTime(file, &BinaryTime, &BinaryTime, &BinaryTime)) {
        std::cerr << "Erreur lors de la mise à jour des dates du fichier.\n";
        CloseHandle(file);
        return 1;
    }

    std::cout << "Horodatage modifié avec succès.\n";
    CloseHandle(file);

    return 0;







}