#include "framework.h"
#include "project-message-hider.h"
#include <string>

#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")

#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")

// Fichiers d'en-tête classes
#include "FontManager.h"
#include "Theme.h"
#include "ButtonComponent.h"
#include "TextComponent.h"
#include "BoxComponent.h"
#include "ImageComponent.h"

ULONG_PTR gdiplusToken;

#define MAX_LOADSTRING 100

// Ressources
#define ICON_INPUT_LIGHT L"resources\\icons\\input-icon-light.png"
#define ICON_INPUT_DARK L"resources\\icons\\input-icon-dark.png"
#define ICON_OUTPUT_LIGHT L"resources\\icons\\output-icon-light.png"
#define ICON_OUTPUT_DARK L"resources\\icons\\output-icon-dark.png"
#define ICON_SET_LIGHT L"resources\\icons\\set-icon-light.png"
#define ICON_SET_DARK L"resources\\icons\\set-icon-dark.png"

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

MessageManager messManager;

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Déclaration de classes
FontManager fontManager;
Theme theme;

// Composants

// Counter
static TextComponent* counterText;
static BoxComponent* counter1Box;
static BoxComponent* counter2Box;
static BoxComponent* counter3Box;

// Title
static TextComponent* titleText;

// TODO : TO REMOVE, TEST ONLY
std::wstring counterTextContent = L"1 of 3";

static ButtonComponent* btnTest = nullptr;

// Buttons

// Step 1
static BoxComponent* step1Box;
static TextComponent* step1Text;
static ImageComponent* step1IconLight;
static ImageComponent* step1IconDark;

// Step 2
static BoxComponent* step2Box;
static TextComponent* step2Text;
static ImageComponent* step2IconLight;
static ImageComponent* step2IconDark;

// Step 3
static BoxComponent* step3Box;
static TextComponent* step3Text;
static ImageComponent* step3IconLight;
static ImageComponent* step3IconDark;

// Définition de la zone de drag and drop (gauche, haut, droite, bas)
RECT dragDropArea;
void CreateDragAndDropArea();
void DrawDragAndDropArea(HDC hdc);
bool IsPointInRect(RECT rect, POINT pt);

static void DrawButton(HDC hdc, int x, int y, int width, int height, const wchar_t* text) {
    // Dessiner le bouton
    HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200)); // Couleur de remplissage
    RECT rect = { x, y, x + width, y + height };
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // Dessiner le texte
    SetTextColor(hdc, RGB(0, 0, 0)); // Couleur du texte
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    srand(time(0));
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    step1IconLight = new ImageComponent(ICON_INPUT_LIGHT, 312, 342, 48, 48);
    step1IconDark = new ImageComponent(ICON_INPUT_DARK, 312, 342, 48, 48);

	step2IconLight = new ImageComponent(ICON_SET_LIGHT, 936, 342, 48, 48);
	step2IconDark = new ImageComponent(ICON_SET_DARK, 936, 342, 48, 48);

	step3IconLight = new ImageComponent(ICON_OUTPUT_LIGHT, 1560, 342, 48, 48);
	step3IconDark = new ImageComponent(ICON_OUTPUT_DARK, 1560, 342, 48, 48);

    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    // Initialisation de GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

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
    HDC hdc = GetDC(NULL);
    /*MessageManager messManager;
    //WCHAR currentDir[MAX_PATH];

    //GetCurrentDirectoryW(MAX_PATH, currentDir);
    messManager.HideMessage(std::wstring(currentDir) + L"\\TargetImg.png", "Super Secret messs", hdc);
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

    Gdiplus::GdiplusShutdown(gdiplusToken);

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
            // Autorise le drag-and-drop dans la fenêtre
            DragAcceptFiles(hWnd, TRUE);

            CreateDragAndDropArea();

            btnTest = new ButtonComponent(642, 802, 96, 36, L"Click Me", 1, true);
        }
        break;


    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        default :
            break;
        }
        break;

    // Est appelé quand on glisse un fichier sur la fenêtre
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;

        // Récupère la position du curseur lors du dépôt
        POINT pt;
        DragQueryPoint(hDrop, &pt);

        // Convertit la position du point à l'échelle de la fenêtre
        ScreenToClient(hWnd, &pt);

        // Vérifie si le curseur est dans la zone de drag and drop définie
        if (IsPointInRect(dragDropArea, pt))
        {
            // Récupère le chemin du premier fichier déposé
            wchar_t filePath[MAX_PATH];
            if (DragQueryFile(hDrop, 0, filePath, MAX_PATH))
            {
                ImageComponent uploadedImage;

                // Si le fichier déposé n'est pas un fichier accepté, affiche un message d'erreur
                if (!uploadedImage.IsValidFile(filePath))
                {
                    MessageBox(hWnd, L"Erreur : Format de fichier non valide.", L"Erreur de format", MB_OK | MB_ICONERROR);
                    DragFinish(hDrop);
                    break;
                }


                // Libérer la ressource précédente si une image était déjà chargée 
                // (pour éviter des problèmes d'affichage non voulus)
                if (uploadedImage.hBitmap)
                {
                    DeleteObject(uploadedImage.hBitmap);
                    uploadedImage.hBitmap = NULL;
                }

                // Récupère la fenêtre actuelle
                HDC hdc = GetDC(hWnd);

                // Charge et affiche l'image dans la fenêtre
                uploadedImage.PaintImage(hdc, hWnd, filePath);
                std::wstring path(filePath);
                std::wstring destPath(path.substr(0, path.find('.')) + L"_encrypted.png");
                if (messManager.HideMessage(filePath, "Super Secret messs", hdc))
                    MessageBox(hWnd, (L"You can find your encrypted file at " + destPath).c_str(), L"Succes", MB_OK | MB_ICONINFORMATION);
                OutputDebugStringA(messManager.GetMessage(destPath, hdc).c_str());
                ReleaseDC(hWnd, hdc);
            }
        }

        // Libérer les ressources du drag-and-drop
        DragFinish(hDrop);
    } break;
    case WM_LBUTTONDOWN: {
        // Vérifier si le clic est à l'intérieur du bouton
        if (btnTest && btnTest->HitTest(LOWORD(lParam), HIWORD(lParam)) && btnTest->GetId() == 1) {
            btnTest->OnClick();
			delete btnTest;
			btnTest = nullptr;
			btnTest->DeleteButton(hWnd);
        }
    } break;
    // Dessine la fenêtre et son contenu
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

            // Counter
            counter1Box = new BoxComponent(hdc, 48, 48, 48, 8, theme.GetColor(900));
            counter2Box = new BoxComponent(hdc, 100, 48, 48, 8, theme.GetColor(300));
            counter3Box = new BoxComponent(hdc, 152, 48, 48, 8, theme.GetColor(300));
            counterText = new TextComponent(hdc, counterTextContent, 48, 64, 37, fontManager.GetFontMuted(), theme.GetColor(950));

            //Text
            titleText = new TextComponent(hdc, L"The best steganography tool to hide or\nextract a message in an image.", 48, 129, 916, fontManager.GetFontTitle(), theme.GetColor(950));

            // Step 1
			step1Box = new BoxComponent(hdc, 48, 293, 576, 200, theme.GetColor(800));
            step1Text = new TextComponent(hdc, L"1. Upload the image to start editing it.", 169, 422, 334, fontManager.GetFontLarge(), theme.GetColor(50));
			step1IconLight->Draw(hdc);

			// Step 2
            step2Box = new BoxComponent(hdc, 672, 293, 576, 200, theme.GetColor(200));
			step2Text = new TextComponent(hdc, L"2. Hide or extract the message.", 821, 422, 278, fontManager.GetFontLarge(), theme.GetColor(950));
            step2IconDark->Draw(hdc);

            // Step 3
			step3Box = new BoxComponent(hdc, 1296, 293, 576, 200, theme.GetColor(200));
			step3Text = new TextComponent(hdc, L"3. Check the result.", 1496, 422, 176, fontManager.GetFontLarge(), theme.GetColor(950));
            step3IconDark->Draw(hdc);

            // Buttons
            if(btnTest) btnTest->Draw(hdc);

            DrawDragAndDropArea(hdc);

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
		delete btnTest;

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

void CreateDragAndDropArea()
{
    // Calcul de la hauteur de la barre de titre
    int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);

    // Calcul de la hauteur de la zone de travail
    RECT workArea;
    int workAreaHeight;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    workAreaHeight = workArea.bottom - workArea.top;

    // Définit la zone de drag and drop
    int left = 48;
    int right = 1333;
    int up = titleBarHeight + 541;
    int down = workAreaHeight - (titleBarHeight + 48);
    dragDropArea = { left, up, right, down };
}

void DrawDragAndDropArea(HDC hdc) 
{
    // Crée un stylo en pointillé
    HPEN hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Dessine le rectangle en pointillé pour délimiter la zone de drag-and-drop
    Rectangle(hdc, dragDropArea.left, dragDropArea.top, dragDropArea.right, dragDropArea.bottom);

    // Restaure l'ancien stylo et supprime le stylo créé
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

bool IsPointInRect(RECT rect, POINT pt) 
{
    return (pt.x >= rect.left && pt.x <= rect.right &&
        pt.y >= rect.top && pt.y <= rect.bottom);
}