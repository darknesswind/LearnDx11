// stdafx.cpp : source file that includes just the standard includes
// LearnDx11.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

HRESULT ReportErrorResult(HRESULT hr, LPCWSTR code)
{
	if (FAILED(hr))
	{
		LPWSTR output;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&output,
			0,
			NULL);
		int res = MessageBoxW(NULL, code, output, MB_ABORTRETRYIGNORE);
		if (IDRETRY == res)
			DebugBreak();
		else if (IDABORT == res)
			exit(0);
	}
	return hr;
}
