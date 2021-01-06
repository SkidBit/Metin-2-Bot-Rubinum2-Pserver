#pragma once
#include "stdafx.h"

void console(LPVOID hModule) { //console assignment

	if (AllocConsole()) {
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout); //have to use freopen_s instead of freopen to get ride of the _CRT_SECURE_NO_WARNINGS message
		freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
		freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);

		AttachConsole(GetProcessId((hModule)));

		SetConsoleTitle(L"SteinBoxer v1.8");

		// disable close button
		EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_DISABLED);
	}
}
