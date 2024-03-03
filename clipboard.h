#pragma once
#include "includes.h"

class ClipboardHJ {
public:
	static std::wstring get_clipboard_text();
	static bool catch_change_text(std::wstring wtext);
	static bool set_clipboard_text(std::wstring text);
};