// header.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows
#include <windows.h>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")
#include <Shobjidl.h>
#pragma comment(lib, "Shell32.lib")
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

// Fichiers d'en-tête C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
// Fichiers d'en-tête classes
#include "ImageManager.h"
#include "MessageManager.h"
// Bibliothèques supplémentaires
#include <shellapi.h>