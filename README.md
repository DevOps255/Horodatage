# Horodatage
# horodatage

> Modifier les métadonnées temporelles d'un fichier Windows depuis la ligne de commande — date de création, dernier accès, dernière modification.

---

## Ce que ça fait

Tu rentres un chemin de fichier et une date. Le programme ouvre le fichier, traduit ta date au format que Windows comprend réellement (un entier de 64 bits comptant les intervalles de 100 nanosecondes depuis le 1er janvier 1601), et réécrit les trois timestamps simultanément sur le disque.

```
Entrez le chemin complet du fichier à modifier:
C:\Users\hp\Documents\rapport.pdf

Entrez l'année que vous voulez remplacer : 2024
Entrez le mois que vous voulez remplacer (1-12) : 06
Entrez le jour que vous voulez remplacer (1-30) : 15
Entrez l'heure que vous voulez modifier (0-23) : 14
Entrez la minute que vous voulez modifier (0-59) : 30

OK!! tout as été enregistré
```

---

## Prérequis

- Windows 10 ou supérieur
- CMake 4.x
- Un compilateur C++ compatible MSVC ou MinGW (le projet utilise `<windows.h>`)

---

## Installation

```bash
git clone https://github.com/ton-username/horodatage.git
cd horodatage
```

```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
cmake --build .
```

L'exécutable `horodatage.exe` apparaît dans `cmake-build-debug/`.

---

## Utilisation

```bash
./horodatage.exe
```

Le programme est interactif — il te demande le chemin du fichier puis la date souhaitée champ par champ.

> **Note :** Le fichier cible doit exister. Le programme ne crée pas de fichier, il modifie uniquement les métadonnées d'un fichier existant.

---

## Structure du projet

```
horodatage/
├── CMakeLists.txt
├── README.md
├── main.cpp              # Point d'entrée — saisie utilisateur + orchestration
├── TimeManager.h         # Déclaration de ConvertToMachineTime()
├── TimeManager.cpp       # Conversion SYSTEMTIME → FILETIME via l'API Windows
├── FileManager.h         # Déclaration de OpenFileHandle()
└── FileManager.cpp       # Ouverture du fichier avec les bons flags d'accès
```

Chaque module a une seule responsabilité :

- **TimeManager** — traduit une date humaine (année, mois, jour, heure, minute) en `FILETIME`, le format 64 bits qu'exige le noyau Windows
- **FileManager** — ouvre le fichier avec `CreateFileA` en mode `FILE_WRITE_ATTRIBUTES` + `OPEN_EXISTING`, retourne un `HANDLE` valide ou signale l'erreur
- **main** — collecte les entrées utilisateur, appelle les deux modules, exécute `SetFileTime`, ferme le handle

---

## Comment ça marche

### 1. La saisie

`main.cpp` collecte le chemin du fichier et les 5 champs de la date (année, mois, jour, heure, minute) via `std::cin`.

### 2. La conversion — `TimeManager`

Windows n'accepte pas une date au format humain pour écrire dans le système de fichiers NTFS. Il faut passer par deux structures :

- `SYSTEMTIME` — la représentation lisible : année, mois, jour, heure, minute, seconde, milliseconde
- `FILETIME` — un entier de 64 bits. Il compte les intervalles de 100 nanosecondes écoulés depuis le **1er janvier 1601** (début du cycle grégorien de 400 ans choisi par les architectes de Windows NT)

La fonction `SystemTimeToFileTime()` fait la traduction. C'est le pont entre les deux mondes.

```cpp
// TimeManager.cpp
FILETIME ConvertToMachineTime(const int year, const int month, const int day,
                               const int hour, const int minute) {
    SYSTEMTIME systime;
    systime.wMilliseconds = 0;
    systime.wSecond       = 0;
    systime.wDayOfWeek    = 0;

    systime.wYear   = year;
    systime.wMonth  = month;
    systime.wDay    = day;
    systime.wHour   = hour;
    systime.wMinute = minute;

    FILETIME ft;
    if (const bool convert = SystemTimeToFileTime(&systime, &ft); !convert) {
        std::cerr << "Erreur lors de la conversion\n";
    }
    return ft;
}
```

### 3. L'ouverture du fichier — `FileManager`

Pour modifier les métadonnées d'un fichier, Windows exige un accès spécifique. Pas `GENERIC_WRITE` (qui donne accès au contenu) — uniquement `FILE_WRITE_ATTRIBUTES`.

```cpp
// FileManager.cpp
HANDLE OpenFileHandle(const std::string &fileName) {
    const HANDLE hFile = CreateFileA(
        fileName.c_str(),
        FILE_WRITE_ATTRIBUTES,   // accès aux métadonnées uniquement
        FILE_SHARE_READ,         // d'autres processus peuvent lire en parallèle
        nullptr,
        OPEN_EXISTING,           // le fichier doit exister
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening file " << fileName << std::endl;
    }
    return hFile;
}
```

### 4. L'écriture — `SetFileTime`

`SetFileTime` prend le handle et trois pointeurs vers des `FILETIME` — un pour la date de création, un pour le dernier accès, un pour la dernière modification. On passe la même valeur pour les trois.

```cpp
// main.cpp (extrait)
FILETIME BinaryTime = ConvertToMachineTime(YearInput, MonthInput, DayInput,
                                            HourInput, MinuteInput);
HANDLE file = OpenFileHandle(path);

if (file == INVALID_HANDLE_VALUE) {
    std::cerr << "Accès refusé!\n";
    return 1;
}

if (!SetFileTime(file, &BinaryTime, &BinaryTime, &BinaryTime)) {
    std::cerr << "Erreur lors de la mise à jour des dates du fichier.\n";
    CloseHandle(file);
    return 1;
}

std::cout << "Horodatage modifié avec succès.\n";
CloseHandle(file);
```

---

## Ce qui arrive si ça échoue

| Situation | Message affiché | Cause probable |
|---|---|---|
| Fichier introuvable | `Error opening file <chemin>` | Le chemin est incorrect ou le fichier n'existe pas |
| Accès refusé | `Accès refusé!` | Droits insuffisants — relancer en administrateur |
| Conversion échouée | `Erreur lors de la conversion` | Date invalide (ex: mois 13, jour 32) |
| Écriture échouée | `Erreur lors de la mise à jour des dates du fichier.` | Fichier verrouillé par un autre processus |

---

## Limitations actuelles

- Windows uniquement (`<windows.h>`)
- Les chemins avec espaces doivent être entourés de guillemets
- Les secondes et millisecondes sont fixées à `0`
- Pas encore d'interface graphique

---

## Roadmap

- [ ] Support des chemins avec espaces sans guillemets
- [ ] Option pour modifier un seul timestamp au lieu des trois
- [ ] Meilleure validation des entrées (date invalide, chemin vide)
- [ ] Interface graphique cross-platform avec Flutter + FFI

---

## Contexte

Ce projet est né d'une question posée en cours : comment modifierait-on la date de modification d'un fichier si on en avait besoin ? Trois fonctions de l'API Windows plus tard — `GetFileTime`, `SystemTimeToFileTime`, `SetFileTime` — le script Python de 10 lignes est devenu un projet C++ structuré avec modules, CMake, et gestion d'erreurs.

---
