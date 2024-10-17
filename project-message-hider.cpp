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
#include "ButtonModeComponent.h"
#include "TextComponent.h"
#include "BoxComponent.h"
#include "ImageComponent.h"
#include "TextFieldComponent.h"

ULONG_PTR gdiplusToken;

#define MAX_LOADSTRING 100

// Ressources
#define ICON_INPUT_LIGHT L"resources\\icons\\input-icon-light.png"
#define ICON_INPUT_DARK L"resources\\icons\\input-icon-dark.png"
#define ICON_OUTPUT_LIGHT L"resources\\icons\\output-icon-light.png"
#define ICON_OUTPUT_DARK L"resources\\icons\\output-icon-dark.png"
#define ICON_SET_LIGHT L"resources\\icons\\set-icon-light.png"
#define ICON_SET_DARK L"resources\\icons\\set-icon-dark.png"
#define PREVIEW_IMAGE L"resources\\images\\Preview.png"

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
bool isLightMode = true;

// Composants

// Counter
static TextComponent* counterText = nullptr;
static BoxComponent* counter1Box = nullptr;
static BoxComponent* counter2Box = nullptr;
static BoxComponent* counter3Box = nullptr;

// encryption
static BoxComponent* encryptionBox;
static TextComponent* encryptionText;
static TextComponent* encryptionLabelText;
static TextFieldComponent* encryptionTextField;

// Title
static TextComponent* titleText = nullptr;

// Image
static ImageComponent* uploadedImage = nullptr;
static ImageComponent* previewImage = nullptr;

// TODO : TO REMOVE, TEST ONLY
static ButtonComponent* btnTest = nullptr;
static ButtonComponent* encryptBtn = nullptr;
static ButtonComponent* decryptBtn = nullptr;

// Buttons
static ButtonComponent* btnSubmit = nullptr;
static ButtonComponent* btnReset= nullptr;
static ButtonComponent* btnDownload= nullptr;
static ButtonModeComponent* btnToggleMode = nullptr;

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
    int up = 541;
    int down = workAreaHeight - (titleBarHeight + 48);
    dragDropArea = { left, up, right, down };

    dNDCenterX = dragDropArea.left + ((dragDropArea.right - dragDropArea.left) / 2);
    dNDCenterY = dragDropArea.top + ((dragDropArea.bottom - dragDropArea.top) / 2);
    
    dragAndDropAreaText = new TextComponent(fontManager.GetFontParagraph(), L"Drag and drop the image here.", (dNDCenterX + offsetX), dNDCenterY, 231, 950);
}

void DrawDragAndDropArea(HDC hdc)
{
    // Crée un stylo en pointillé pour dessiner les bordures
    HPEN hPen = CreatePen(PS_DASH, 1, theme.GetColor(950));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Dessine le cadre autour de la zone sans remplir
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, dragDropArea.left, dragDropArea.top, dragDropArea.right, dragDropArea.bottom);

    // Restaure l'ancien stylo et brosse
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);

    DeleteObject(hPen);

    if (dragAndDropAreaText) {
        dragAndDropAreaText->Draw(hdc, theme.GetColor(dragAndDropAreaText->GetColorID()));
    }
}

bool IsPointInRect(RECT rect, POINT pt)
{
    return (pt.x >= rect.left && pt.x <= rect.right &&
        pt.y >= rect.top && pt.y <= rect.bottom);
}

static void ToggleIcon1Mode()
{
    if (step1IconLight)
    {
        delete step1IconLight;
        step1IconLight = nullptr;
    }
    else
    {
        step1IconLight = new ImageComponent(ICON_INPUT_LIGHT, 312, 342, 48, 48);
    }

    if (step1IconDark)
    {
        delete step1IconDark;
        step1IconDark = nullptr;
    }
    else
    {
        step1IconDark = new ImageComponent(ICON_INPUT_DARK, 312, 342, 48, 48);
    }
}

static void ToggleIcon2Mode()
{
	if (step2IconLight)
	{
		delete step2IconLight;
		step2IconLight = nullptr;
	}
	else
	{
		step2IconLight = new ImageComponent(ICON_SET_LIGHT, 936, 342, 48, 48);
	}

	if (step2IconDark)
	{
		delete step2IconDark;
		step2IconDark = nullptr;
	}
	else
	{
		step2IconDark = new ImageComponent(ICON_SET_DARK, 936, 342, 48, 48);
	}
}

static void ToggleIcon3Mode()
{
	if (step3IconLight)
	{
		delete step3IconLight;
		step3IconLight = nullptr;
	}
	else
	{
		step3IconLight = new ImageComponent(ICON_OUTPUT_LIGHT, 1560, 342, 48, 48);
	}

	if (step3IconDark)
	{
		delete step3IconDark;
		step3IconDark = nullptr;
	}
	else
	{
		step3IconDark = new ImageComponent(ICON_OUTPUT_DARK, 1560, 342, 48, 48);
	}
}


static void UpdateState(HWND hWnd, int newState)
{
    if (newState != state && state == 2)
        encryptionTextField->Hide();
    state = newState;

    switch (state)
    {
    case 1:
        // Counter
        counter1Box->SetColor(hWnd, 800);
        counter2Box->SetColor(hWnd, 300);
        counter3Box->SetColor(hWnd, 300);

        // Steps
		step1Box->SetColor(hWnd, 900);
		step2Box->SetColor(hWnd, 200);
		step3Box->SetColor(hWnd, 200);

        step1Text->SetColor(hWnd, 50);
        step2Text->SetColor(hWnd, 950);
        step3Text->SetColor(hWnd, 950);

        ToggleIcon1Mode();
        ToggleIcon3Mode();

        break;
    case 2:
        // Counter
		counter1Box->SetColor(hWnd, 300);
        counter2Box->SetColor(hWnd, 800);
        counter3Box->SetColor(hWnd, 300);

        // Steps
        step1Box->SetColor(hWnd, 200);
        step2Box->SetColor(hWnd, 900);
        step3Box->SetColor(hWnd, 200);

		step1Text->SetColor(hWnd, 950);
		step2Text->SetColor(hWnd, 50);
		step3Text->SetColor(hWnd, 950);

        ToggleIcon1Mode();
        ToggleIcon2Mode();

        break;
    case 3:
        // Counter
        counter1Box->SetColor(hWnd, 300);
        counter2Box->SetColor(hWnd, 300);
        counter3Box->SetColor(hWnd, 800);

        // Steps
		step1Box->SetColor(hWnd, 200);
		step2Box->SetColor(hWnd, 200);
		step3Box->SetColor(hWnd, 900);

        step1Text->SetColor(hWnd, 950);
        step2Text->SetColor(hWnd, 950);
        step3Text->SetColor(hWnd, 50);

        ToggleIcon2Mode();
        ToggleIcon3Mode();

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
    HDC hdc = GetDC(hWnd);

    switch (message)
    {
    case WM_CREATE:
        {
			// Crée les polices
            fontManager.LoadFont(hWnd);

            // Autorise le drag-and-drop dans la fenêtre
            DragAcceptFiles(hWnd, TRUE);

            // TEST ONLY
            btnTest = new ButtonComponent(1776, 150, 96, 36, 1, true);
            encryptBtn = new ButtonComponent(90, 830, 96, 36, 2, true);

			// Buttons
			btnToggleMode = new ButtonModeComponent(1776, 48, 96, 36, 1, 300, 900);
            
            // Counter
            counterText = new TextComponent(fontManager.GetFontMuted(), std::to_wstring(state) + L" of 3", 48, 64, 37, 600);
            counter1Box = new BoxComponent(48, 48, 800);
            counter2Box = new BoxComponent(100, 48, 300);
            counter3Box = new BoxComponent(152, 48, 300);

            // Title
            titleText = new TextComponent(fontManager.GetFontTitle(), L"The best steganography tool to hide or\nextract a message in an image.", 48, 129, 916, 950);

            // Step1
            step1Box = new BoxComponent(48, 293, 900);
            step1Text = new TextComponent(fontManager.GetFontLarge(), L"1. Upload the image to start editing it.", 169, 422, 334, 50);
            step1IconLight = new ImageComponent(ICON_INPUT_LIGHT, 312, 342, 48, 48);

			// Step2
            step2Box = new BoxComponent(672, 293, 200);
            step2Text = new TextComponent(fontManager.GetFontLarge(), L"2. Hide or extract the message.", 821, 422, 278, 950);
            step2IconDark = new ImageComponent(ICON_SET_DARK, 936, 342, 48, 48);

            // Step3
            step3Box = new BoxComponent(1296, 293, 200);
            step3Text = new TextComponent(fontManager.GetFontLarge(), L"3. Check the result.", 1496, 422, 176, 950);
            step3IconDark = new ImageComponent(ICON_OUTPUT_DARK, 1560, 342, 48, 48);

            // encryption
            encryptionBox = new BoxComponent(48, 550, 200);
            encryptionText = new TextComponent(fontManager.GetFontLarge(), L"Hide a Message", 90, 590, 334, 950);
            encryptionTextField = new TextFieldComponent(hWnd, ((LPCREATESTRUCT)lParam)->hInstance, 90, 700, 350, 40);
            encryptionLabelText = new TextComponent(fontManager.GetFontLarge(), encryptionTextField->UpdateCharCount(), 90, 670, 334, 950);
            encryptionTextField->Hide();

			// Drag and Drop Area
            previewImage = new ImageComponent(PREVIEW_IMAGE, 1381, 541, 463, 463);

            CreateDragAndDropArea();
        }
        break;
    case WM_COMMAND:
        switch (HIWORD(wParam)) {
        case EN_CHANGE:
            if ((HWND)lParam == encryptionTextField->GetHandle())
                InvalidateRect(hWnd, nullptr, TRUE);
            break;
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
				// Crée un nouvel objet ImageComponent
				uploadedImage = new ImageComponent(filePath, 1381, 541, 463, 463);

                // Si le fichier déposé n'est pas un fichier accepté, affiche un message d'erreur
                if (!uploadedImage->IsValidFile(filePath))
                {
                    MessageBox(hWnd, L"Erreur : Format de fichier non valide.", L"Erreur de format", MB_OK | MB_ICONERROR);
                    DragFinish(hDrop);
                    break;
                }

                delete previewImage;
                previewImage = nullptr;

                // Récupère la fenêtre actuelle
                HDC hdc = GetDC(hWnd);

                std::wstring path(filePath);
                std::wstring destPath(path.substr(0, path.find('.')) + L"_encrypted.png");
                if (messManager.HideMessage(filePath, "Super Secret messs", hdc))
                    MessageBox(hWnd, (L"You can find your encrypted file at " + destPath).c_str(), L"Succes", MB_OK | MB_ICONINFORMATION);
                OutputDebugStringA(messManager.GetMessage(destPath, hdc).c_str());

                UpdateState(hWnd, 2);

                InvalidateRect(hWnd, NULL, TRUE);

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
		if (btnToggleMode && btnToggleMode->HitTest(LOWORD(lParam), HIWORD(lParam))) {
            theme.ToggleMode();
            ToggleIcon1Mode();
			ToggleIcon2Mode();
			ToggleIcon3Mode();
			btnToggleMode->MoveInnerRectangle(!btnToggleMode->GetIsOn());
            InvalidateRect(hWnd, NULL, TRUE);
		}

        if (encryptBtn && encryptBtn->HitTest(LOWORD(lParam), HIWORD(lParam)) && encryptBtn->GetId() == 2) {
            encryptBtn->OnClick();
            /*delete btnTest;
            btnTest = nullptr;
            btnTest->DeleteButton(hWnd);*/
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
			if(counter1Box) counter1Box->Draw(hdc, 48, 8, theme.GetColor(counter1Box->GetColorID()));
            if (counter2Box) counter2Box->Draw(hdc, 48, 8, theme.GetColor(counter2Box->GetColorID()));
			if(counter3Box) counter3Box->Draw(hdc, 48, 8, theme.GetColor(counter3Box->GetColorID()));
			if(counterText) counterText->Draw(hdc, theme.GetColor(counterText->GetColorID()));

            //Text
            if (titleText) titleText->Draw(hdc, theme.GetColor(titleText->GetColorID()));

            // Step 1
			if (step1Box) step1Box->Draw(hdc, 576, 200, theme.GetColor(step1Box->GetColorID()));
			if (step1Text) step1Text->Draw(hdc, theme.GetColor(step1Text->GetColorID()));
			if (step1IconLight) step1IconLight->Draw(hdc);
			if (step1IconDark) step1IconDark->Draw(hdc);
			
			// Step 2
			if (step2Box) step2Box->Draw(hdc, 576, 200, theme.GetColor(step2Box->GetColorID()));
			if (step2Text) step2Text->Draw(hdc, theme.GetColor(step2Text->GetColorID()));
			if (step2IconLight) step2IconLight->Draw(hdc);
			if (step2IconDark) step2IconDark->Draw(hdc);
            
            // Step 3
			if (step3Box) step3Box->Draw(hdc, 576, 200, theme.GetColor(step3Box->GetColorID()));
			if (step3Text) step3Text->Draw(hdc, theme.GetColor(step3Text->GetColorID()));
			if (step3IconLight) step3IconLight->Draw(hdc);
			if (step3IconDark) step3IconDark->Draw(hdc);

			// Preview image
            if(previewImage) previewImage->Draw(hdc);
			if (uploadedImage) uploadedImage->Draw(hdc);

            // encryption
            if (state == 2) {
                encryptionBox->Draw(hdc, 576, 400, theme.GetColor(encryptionBox->GetColorID()));
                encryptionText->Draw(hdc, theme.GetColor(encryptionText->GetColorID()));
                if (!IsWindowVisible(encryptionTextField->GetHandle()))
                    encryptionTextField->Draw();
                encryptionLabelText->SetText(hWnd, encryptionTextField->UpdateCharCount());
                encryptionLabelText->Draw(hdc, theme.GetColor(encryptionLabelText->GetColorID()));
                encryptBtn->Draw(hdc, L"Submit");
            }
            if (state == 1)
                DrawDragAndDropArea(hdc);
    
            // Buttons
            // Buttons
            if(btnToggleMode) btnToggleMode->Draw(hdc, theme.GetColor(btnToggleMode->GetOutRectColorID()), theme.GetColor(btnToggleMode->GetInRectColorID()));
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