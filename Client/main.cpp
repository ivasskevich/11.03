
#pragma comment(lib, "Ws2_32.lib")
#include "WinSock2.h"
#include <ws2tcpip.h>
#include <tchar.h>
#include "resource.h"
#include <string>
#include <iostream>

const int MAXSTRLEN = 256;
WSADATA wsaData;

SOCKET _socket;
sockaddr_in addr;

int CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

int CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	switch (mes)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wp))
		{
		case IDC_BUTTON1:
		{
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			addr.sin_family = AF_INET;
			inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

			addr.sin_port = htons(20000);

			connect(_socket, (SOCKADDR*)&addr, sizeof(addr));
			break;
		}
		case IDC_BUTTON2:
		{
			char bufer1[MAXSTRLEN];

			GetWindowTextA(GetDlgItem(hWnd, IDC_EDIT1), bufer1, MAXSTRLEN);

			send(_socket, bufer1, strlen(bufer1), 0);

			char bufer2[MAXSTRLEN];

			int i = recv(_socket, bufer2, MAXSTRLEN - 1, 0);
			bufer2[i] = '\0';
			SendMessageA(GetDlgItem(hWnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)bufer2);
			closesocket(_socket);
			WSACleanup();
			break;
		}
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}