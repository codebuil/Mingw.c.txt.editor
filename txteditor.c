#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
 #define MAX_FIELDS 100
 #define IDC_TEXTBOX 100
 static HWND hStartEdit, hEndEdit, hButton, hButton2;
char *fields[MAX_FIELDS];
int fildscount;
int obter_numero_antigo(char *line) {
    int i=0;
    i = 0;
        fields[i] = strtok(line, ":");
        while (fields[i] != NULL && i < MAX_FIELDS) {
            i++;
            fields[i] = strtok(NULL, ":");
        }
        fildscount=i;
        if(i>0){
            return atoi( fields[0]);
        }
        return 0;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
     WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    // Definir as propriedades da janela
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1); // definir pincel para azul
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "MinhaJanela";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    // Registrar a classe da janela
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Falha no registro da classe da janela!", "Erro", MB_ICONERROR | MB_OK);
        return 0;
    }

    // Criar a janela
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "MinhaJanela", "editor",
                          WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                          600, 400, NULL, NULL, hInstance, NULL);
                                  hStartEdit =CreateWindow(TEXT("EDIT"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, 10, 100, 30,  hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
        hEndEdit = CreateWindow(TEXT("EDIT"), NULL,
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        10, 40, 550, 310,  hwnd, (HMENU)IDC_TEXTBOX, hInstance, NULL);
        hButton = CreateWindow("BUTTON", "load", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            120, 10, 100, 30,  hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);
        hButton2 = CreateWindow("BUTTON", "save", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            220, 10, 100, 30,  hwnd, (HMENU)4, GetModuleHandle(NULL), NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    static char startStr[300], endStr[67000];
     static WCHAR wstartStr[30], wendStr[30];
     char *result1,*result2;
     int count=0;
     FILE *f1;
    switch (message)
    {
    case WM_CREATE:

        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 3: // Botão "load"
            memset(endStr, 0, 66000);
            strcpy(endStr,"file error");
           count=0;
            GetWindowText(hStartEdit, startStr, 250);
             SetWindowText(hEndEdit,endStr);
            f1=fopen(startStr,"rb");
            
            if(f1!=NULL){
                count=fread(endStr,1, 66000, f1);
                endStr[66000-1]=0;
                
                fclose(f1);
                SendMessage(hEndEdit, WM_SETTEXT,66000-1,(LPARAM) endStr);
                //SetWindowText(hEndEdit,endStr);


            }
            break;
            case 4: // Botão "save"
            
            
           count=0;
            GetWindowText(hStartEdit, startStr, 250);
            GetWindowText(hEndEdit, endStr, 65999);
            f1=fopen(startStr,"wb");
            
            if(f1!=NULL){
                count=fwrite(endStr,1, strlen(endStr), f1);
                
                
                fclose(f1);

            }
            break;
        }
    break;   
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // Define a cor de fundo da janela para azul
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);
            
            // Define a cor das linhas para branco
            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
            SelectObject(hdc, hPen);
            
            // Desenha as linhas com base nas coordenadas fornecidas
            
            
            // Libera os recursos utilizados
            DeleteObject(hPen);
            EndPaint(hWnd, &ps);
            break;
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
