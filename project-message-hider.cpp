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

// State
int state = 1;

// Composants

// Counter
static TextComponent* counterText = nullptr;
static BoxComponent* counter1Box = nullptr;
static BoxComponent* counter2Box = nullptr;
static BoxComponent* counter3Box = nullptr;

// Title
static TextComponent* titleText = nullptr;

// TODO : TO REMOVE, TEST ONLY
static ImageComponent* uploadedImage = nullptr;

static ButtonComponent* btnTest = nullptr;

// Buttons
static ButtonComponent* btnSubmit = nullptr;
static ButtonComponent* btnReset= nullptr;
static ButtonComponent* btnDownload= nullptr;

// Step 1
static BoxComponent* step1Box = nullptr;
static TextComponent* step1Text = nullptr;
static ImageComponent* step1IconLight = nullptr;
static ImageComponent* step1IconDark = nullptr;

// Step 2
static BoxComponent* step2Box = nullptr;
static TextComponent* step2Text = nullptr;
static ImageComponent* step2IconLight = nullptr;
static ImageComponent* step2IconDark = nullptr;

// Step 3
static BoxComponent* step3Box = nullptr;
static TextComponent* step3Text = nullptr;
static ImageComponent* step3IconLight = nullptr;
static ImageComponent* step3IconDark = nullptr;

// Définition de la zone de drag and drop (gauche, haut, droite, bas)
RECT dragDropArea;
static TextComponent* dragAndDropAreaText;
int dNDCenterX;
int dNDCenterY;
int offsetX = -118;

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

    dNDCenterX = dragDropArea.left + ((dragDropArea.right - dragDropArea.left) / 2);
    dNDCenterY = dragDropArea.top + ((dragDropArea.bottom - dragDropArea.top) / 2);
    
    dragAndDropAreaText = new TextComponent(fontManager.GetFontParagraph(), L"Drag and Drop the image here.", (dNDCenterX + offsetX), dNDCenterY, 278, theme.GetColor(950));
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

	if(dragAndDropAreaText) dragAndDropAreaText->Draw(hdc);
}

bool IsPointInRect(RECT rect, POINT pt)
{
    return (pt.x >= rect.left && pt.x <= rect.right &&
        pt.y >= rect.top && pt.y <= rect.bottom);
}

static void UpdateState(HWND hWnd, int newState)
{
    state = newState;

    switch (state)
    {
    case 1:
        // Counter
        counter1Box->SetColor(hWnd, theme.GetColor(900));
        counter2Box->SetColor(hWnd, theme.GetColor(300));
        counter3Box->SetColor(hWnd, theme.GetColor(300));

        // Steps
		step1Box->SetColor(hWnd, theme.GetColor(900));
		step2Box->SetColor(hWnd, theme.GetColor(200));
		step3Box->SetColor(hWnd, theme.GetColor(200));

        step1Text->SetColor(hWnd, theme.GetColor(50));
        step2Text->SetColor(hWnd, theme.GetColor(950));
        step3Text->SetColor(hWnd, theme.GetColor(950));

		delete step1IconDark;
		delete step3IconLight;

		step1IconDark = nullptr;
		step3IconLight = nullptr;

		step1IconLight = new ImageComponent(ICON_INPUT_LIGHT, 312, 342, 48, 48);
		step3IconDark = new ImageComponent(ICON_OUTPUT_DARK, 1560, 342, 48, 48);

        break;
    case 2:
        // Counter
		counter1Box->SetColor(hWnd, theme.GetColor(300));
        counter2Box->SetColor(hWnd, theme.GetColor(900));
        counter3Box->SetColor(hWnd, theme.GetColor(300));

        // Steps
        step1Box->SetColor(hWnd, theme.GetColor(200));
        step2Box->SetColor(hWnd, theme.GetColor(900));
        step3Box->SetColor(hWnd, theme.GetColor(200));

		step1Text->SetColor(hWnd, theme.GetColor(950));
		step2Text->SetColor(hWnd, theme.GetColor(50));
		step3Text->SetColor(hWnd, theme.GetColor(950));

		delete step1IconLight;
		delete step2IconDark;

        step1IconLight = nullptr;
		step2IconDark = nullptr;

        step1IconDark = new ImageComponent(ICON_INPUT_DARK, 312, 342, 48, 48);
        step2IconLight = new ImageComponent(ICON_SET_LIGHT, 936, 342, 48, 48);

        break;
    case 3:
        // Counter
        counter1Box->SetColor(hWnd, theme.GetColor(300));
        counter2Box->SetColor(hWnd, theme.GetColor(300));
        counter3Box->SetColor(hWnd, theme.GetColor(900));

        // Steps
		step1Box->SetColor(hWnd, theme.GetColor(200));
		step2Box->SetColor(hWnd, theme.GetColor(200));
		step3Box->SetColor(hWnd, theme.GetColor(900));

        step1Text->SetColor(hWnd, theme.GetColor(950));
        step2Text->SetColor(hWnd, theme.GetColor(950));
        step3Text->SetColor(hWnd, theme.GetColor(50));

		delete step3IconDark;
		delete step2IconLight;

        step3IconDark = nullptr;
		step2IconLight = nullptr;

        step2IconDark = new ImageComponent(ICON_SET_DARK, 936, 342, 48, 48);
        step3IconLight = new ImageComponent(ICON_OUTPUT_LIGHT, 1560, 342, 48, 48);

        break;
    }

    counterText->SetText(hWnd, std::to_wstring(state) + L" of 3");

    // Refresh UI
    InvalidateRect(hWnd, NULL, TRUE);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    srand(time(0));
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

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
			// Crée les polices
            fontManager.LoadFont(hWnd);

            // Autorise le drag-and-drop dans la fenêtre
            DragAcceptFiles(hWnd, TRUE);

            // TEST ONLY
            btnTest = new ButtonComponent(1776, 48, 96, 36, 1, true);
            
            // Counter
            counterText = new TextComponent(fontManager.GetFontMuted(), std::to_wstring(state) + L" of 3", 48, 64, 37, theme.GetColor(950));
            counter1Box = new BoxComponent(48, 48, theme.GetColor(900));
            counter2Box = new BoxComponent(100, 48, theme.GetColor(300));
            counter3Box = new BoxComponent(152, 48, theme.GetColor(300));

            // Title
            titleText = new TextComponent(fontManager.GetFontTitle(), L"The best steganography tool to hide or\nextract a message in an image.", 48, 129, 916, theme.GetColor(950));

            // Step1
            step1Box = new BoxComponent(48, 293, theme.GetColor(800));
            step1Text = new TextComponent(fontManager.GetFontLarge(), L"1. Upload the image to start editing it.", 169, 422, 334, theme.GetColor(50));
            step1IconLight = new ImageComponent(ICON_INPUT_LIGHT, 312, 342, 48, 48);

			// Step2
            step2Box = new BoxComponent(672, 293, theme.GetColor(200));
            step2Text = new TextComponent(fontManager.GetFontLarge(), L"2. Hide or extract the message.", 821, 422, 278, theme.GetColor(950));
            step2IconDark = new ImageComponent(ICON_SET_DARK, 936, 342, 48, 48);

            // Step3
            step3Box = new BoxComponent(1296, 293, theme.GetColor(200));
            step3Text = new TextComponent(fontManager.GetFontLarge(), L"3. Check the result.", 1496, 422, 176, theme.GetColor(950));
            step3IconDark = new ImageComponent(ICON_OUTPUT_DARK, 1560, 342, 48, 48);

			// Drag and Drop Area
            CreateDragAndDropArea();
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
                // Si le fichier déposé n'est pas un fichier accepté, affiche un message d'erreur
                if (!uploadedImage->IsValidFile(filePath))
                {
                    MessageBox(hWnd, L"Erreur : Format de fichier non valide.", L"Erreur de format", MB_OK | MB_ICONERROR);
                    DragFinish(hDrop);
                    break;
                }

                // Libérer la ressource précédente si une image était déjà chargée 
                // (pour éviter des problèmes d'affichage non voulus)
                if (uploadedImage->hBitmap)
                {
                    DeleteObject(uploadedImage->hBitmap);
                    uploadedImage->hBitmap = NULL;
                }

                // Récupère la fenêtre actuelle
                HDC hdc = GetDC(hWnd);

                // Charge et affiche l'image dans la fenêtre
                uploadedImage->PaintImage(hdc, hWnd, filePath);
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

			int newState = 0;
			state >= 3 ? newState = 1 : newState = state + 1;
			UpdateState(hWnd, newState);
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
			if(counter1Box) counter1Box->Draw(hdc, 48, 8);
			if(counter2Box) counter2Box->Draw(hdc, 48, 8);
			if(counter3Box) counter3Box->Draw(hdc, 48, 8);
			if(counterText) counterText->Draw(hdc);

            //Text
            if (titleText) titleText->Draw(hdc);

            // Step 1
			if (step1Box) step1Box->Draw(hdc, 576, 200);
			if (step1Text) step1Text->Draw(hdc);
			if (step1IconLight) step1IconLight->Draw(hdc);
			if (step1IconDark) step1IconDark->Draw(hdc);
			
			// Step 2
			if (step2Box) step2Box->Draw(hdc, 576, 200);
			if (step2Text) step2Text->Draw(hdc);
			if (step2IconLight) step2IconLight->Draw(hdc);
			if (step2IconDark) step2IconDark->Draw(hdc);
            
            // Step 3
			if (step3Box) step3Box->Draw(hdc, 576, 200);
			if (step3Text) step3Text->Draw(hdc);
			if (step3IconLight) step3IconLight->Draw(hdc);
			if (step3IconDark) step3IconDark->Draw(hdc);

            DrawDragAndDropArea(hdc);
    
            // Buttons
            if(btnTest) btnTest->Draw(hdc, L"Debug State");

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