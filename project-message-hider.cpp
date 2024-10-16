#include "framework.h"
#include "project-message-hider.h"
#include <string>

#include <shellscalingapi.h>

#pragma comment(lib, "Shcore.lib")

#include "FontManager.h"
#include "Theme.h"
#include "Button.h"
#include "TextComponent.h"
#include "BoxComponent.h"

#define MAX_LOADSTRING 100

// Ressources
#define ICON_INPUT_LIGHT "ressources\\icons\\input-icon-light.png"
#define ICON_INPUT_DARK "ressources\\icons\\input-icon-dark.png"
#define ICON_OUTPUT_LIGHT "ressources\\icons\\output-icon-light.png"
#define ICON_OUTPUT_DARK "ressources\\icons\\output-icon-dark.png"
#define ICON_SET_LIGHT "ressources\\icons\\set-icon-light.png"
#define ICON_SET_DARK "ressources\\icons\\set-icon-dark.png"

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Déclaration de classes
FontManager fontManager;
Theme theme;

static TextComponent* titleText;

static Button* btnPrimary;
static Button* btnSecondary;

static Button* btnSubmit;
static Button* btnDownload;
static Button* btnReset;

static BoxComponent* stepBox;
static TextComponent* stepText;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    srand(time(0));
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECTMESSAGEHIDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTMESSAGEHIDER));

    MSG msg;
    /*HDC hdc = GetDC(NULL);
    MessageManager messManager;
    messManager.HideMessage(L"C:/Users/fifam/Pictures/Screenshots/game.png", "Super Secret messs", hdc);
    OutputDebugStringA(messManager.GetMessage(L"EncryptedImg.png", hdc).c_str());*/

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_MYICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECTMESSAGEHIDER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDC_MYICON));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(
       szWindowClass,        // Nom de la classe de fenêtre
       L"PixHide",              // Titre de la fenêtre
       WS_OVERLAPPEDWINDOW,  // Style de la fenêtre classique
       CW_USEDEFAULT,        // Position X
       CW_USEDEFAULT,        // Position Y
       1280,                 // Largeur par défaut
       720,                  // Hauteur par défaut
       nullptr,              // Pas de fenêtre parent
       nullptr,              // Pas de menu
       hInstance,            // Instance de l'application
       nullptr               // Paramètres supplémentaires
   );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_SHOWMAXIMIZED);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HFONT hFontTitle, hFontSubtitle, hFontParagraph, hFontLead, hFontLarge, hFontSmall, hFontMuted;
    static DWORD fontCountTitle = 0, fontCountSubtitle = 0, fontCountParagraph = 0, fontCountLead = 0, fontCountLarge = 0, fontCountSmall = 0, fontCountMuted = 0;

    switch (message)
    {
    case WM_CREATE:
        {
		    fontManager.LoadFont(hWnd);
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 101:
            btnPrimary->OnClick();
            break;
        case 102:
            btnSecondary->OnClick();
            break;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // Utils
            RECT rect;

			// Background color
            COLORREF backgroundColor = theme.GetColor(50);
            HBRUSH hBrush = CreateSolidBrush(backgroundColor);
            GetClientRect(hWnd, &rect);
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            //Text
            titleText = new TextComponent(hdc, L"The best steganography tool to hide or\nextract a message in an image.", 48, 129, 916, fontManager.GetFontTitle(), theme.GetColor(950));

            // Step 1
			stepBox = new BoxComponent(hdc, 48, 293, 576, 200, theme.GetColor(900));
            stepText = new TextComponent(hdc, L"1. Upload the image to start editing it.", 169, 422, 334, fontManager.GetFontLarge(), theme.GetColor(50));
            
			// Step 2
            stepBox = new BoxComponent(hdc, 672, 293, 576, 200, theme.GetColor(200));
			stepText = new TextComponent(hdc, L"2. Hide or extract the message.", 821, 422, 278, fontManager.GetFontLarge(), theme.GetColor(950));

            // Step 3
			stepBox = new BoxComponent(hdc, 1296, 293, 576, 200, theme.GetColor(200));
			stepText = new TextComponent(hdc, L"3. Check the result.", 1496, 422, 176, fontManager.GetFontLarge(), theme.GetColor(950));

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void AdjustWindowSize(HWND hwnd, UINT imageWidth, UINT imageHeight)
{
    // Récupérer la taille de la fenêtre et du client
    RECT rcClient, rcWindow;
    GetClientRect(hwnd, &rcClient);
    GetWindowRect(hwnd, &rcWindow);

    // Calculer la taille de la bordure de la fenêtre
    int borderWidth = (rcWindow.right - rcWindow.left) - rcClient.right;
    int borderHeight = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;


    // Ajuster la taille de la fenêtre en fonction de l'image
    SetWindowPos(hwnd, nullptr, 0, 0, imageWidth + borderWidth, imageHeight + borderHeight, SWP_NOMOVE | SWP_NOZORDER);
}
