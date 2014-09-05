#include <windows.h>
#include <tchar.h>
#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hwndDlg,
                            UINT uMsg,
                            WPARAM wParam,
                            LPARAM lParam);

LPCTSTR DlgName = MAKEINTRESOURCE(IDD_DIALOG1);

int WINAPI _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     int nCmdShow)
{
    DialogBoxParam(hInstance, DlgName, NULL, DialogProc, (LPARAM) NULL);
    return TRUE;
}

INT_PTR CALLBACK DialogProc(HWND hWnd,
                            UINT uMsg,
                            WPARAM wParam,
                            LPARAM lParam)
{

    static HWND hHost;
    static HWND hPort;
    static HWND hProto;
    static HWND hMessage;
    static HWND hList;
    
    
    switch(uMsg)
    {
    case WM_INITDIALOG:
//        hEdit = GetDlgItem(hWnd, IDC_EDIT1);
        hList = GetDlgItem(hWnd, IDLOG);
        SendMessage(GetDlgItem(hWnd, IDTCP), BM_SETCHECK, BST_CHECKED, 0);        
        break;
        
    case WM_COMMAND:        
        if (LOWORD(wParam) == IDSEND)
        {
            //           MessageBox(NULL,
            //"Message send","Message send",MB_OK);

//            SetDlgItemText(hWnd, IDC_EDIT1, TestString);
//            char buf[256];
            LRESULT lResult = (int)SendMessage(hList, LB_ADDSTRING, 256, (LPARAM)"buf");
            if (lResult == LB_ERR || lResult == LB_ERRSPACE)
                MessageBox(hWnd, "Ошибка при добавлении строки в список", "Ошибка", MB_OK);
            if(IsDlgButtonChecked(hWnd, IDTCP))
                MessageBox(hWnd, "TCP", "TCP", MB_OK);                
        }
        break;        
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        break;
    default:
        return FALSE;
    }

    return TRUE;
}
