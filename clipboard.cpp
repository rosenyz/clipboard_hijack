#include "clipboard.h"

std::wstring ClipboardHJ::get_clipboard_text() 
{
	OpenClipboard(NULL);
	HANDLE h_data = GetClipboardData(CF_UNICODETEXT);

	if (h_data == NULL)
	{
		CloseClipboard();
		return L"[ERROR] h_data == NULL";
	}

	wchar_t* psz_text = static_cast<wchar_t*>(GlobalLock(h_data));
	if (psz_text == NULL)
	{
		GlobalUnlock(h_data);
		CloseClipboard();
		return L"[ERROR] psz_text == NULL";
	}

	std::wstring wide_text(psz_text);
	GlobalUnlock(h_data);
	CloseClipboard();

	return wide_text;
}

bool ClipboardHJ::catch_change_text(std::wstring wtext) 
{
	std::wstring new_text = ClipboardHJ::get_clipboard_text();

	if (wtext == new_text) {
		return false;
	}

	return true;
}

bool ClipboardHJ::set_clipboard_text(std::wstring text)
{
	if (!OpenClipboard(NULL)) {
		return false;
	}

	EmptyClipboard();

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1) * sizeof(wchar_t));
	if (!hMem) {
		CloseClipboard();
		return false;
	}

	wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
	if (!pMem) {
		GlobalFree(hMem);
		CloseClipboard();
		return false;
	}

	wcscpy_s(pMem, text.length() + 1, text.c_str());
	GlobalUnlock(hMem);

	SetClipboardData(CF_UNICODETEXT, hMem);
	CloseClipboard();

	return true;
}