/*
  Simple client with GUI interface
 */
#include "helper.h"
#include "netsetup.h"
#include "request.h"
#include "resource.h"

#include <windows.h>
#include <tchar.h>

namespace
{
    static const int BUF_SIZE = 256;    
    static const int TRY_COUNT = 5;
    void AddEntryToLog(HWND hWnd, const char* message)
    {
        HWND hList;        
        hList = GetDlgItem(hWnd, IDLOG);        
        LRESULT lResult = (int)SendMessage(hList, LB_ADDSTRING, BUF_SIZE, (LPARAM)message);
        if (lResult == LB_ERR || lResult == LB_ERRSPACE)
            MessageBox(hWnd, "Can't add message to log", "Error", MB_OK);
    }

    void SendMessageToServer(HWND hWnd)
    {
        using namespace std;        
        char buf[BUF_SIZE];
            
        GetDlgItemText(hWnd, IDHOST, buf, BUF_SIZE);
        string host(buf);
        GetDlgItemText(hWnd, IDPORT, buf, BUF_SIZE);

        int port = CharToInt(buf);
        if(!IsValidPortNumber(port))
        {
            MessageBox(hWnd, "Icorrect port number", "Error", MB_OK);        
            return;
        }
        GetDlgItemText(hWnd, IDMESSAGE, buf, BUF_SIZE);
        string message(buf);

        string result;
        for(int i = 0; i < TRY_COUNT; ++i)
        {
            try
            {
                result = Client::SendRequest(host, port, SOCK_STREAM, message);
                AddEntryToLog(hWnd, Client::DescribeReply(result).c_str());
                return;
            }
            catch (const char* s)
            {
                Sleep(1000);
                continue;
            } 
            catch (...)
            {
                AddEntryToLog(hWnd, "Fail: unknown error");                
            }
        }
        AddEntryToLog(hWnd, "Fail: server not respond");
    }
}


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
    NetSetup network;    
    DialogBoxParam(hInstance, DlgName, NULL, DialogProc, (LPARAM) NULL);
    return TRUE;
}

INT_PTR CALLBACK DialogProc(HWND hWnd,
                            UINT uMsg,
                            WPARAM wParam,
                            LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
         SetDlgItemText(hWnd, IDHOST, "localhost");
         SetDlgItemText(hWnd, IDPORT, "4242");
         SetDlgItemText(hWnd, IDMESSAGE, "Hello, server!");                  
        break;
        
    case WM_COMMAND:        
        if (LOWORD(wParam) == IDSEND)
        {
            SendMessageToServer(hWnd);
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
