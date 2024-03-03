#include "includes.h"
#include "clipboard.h"

std::wstring replace_letters(std::wstring& text_for_processing)
{
	std::map<wchar_t, wchar_t> replacements = {
		{ L'A', L'А' }, { L'B', L'В' }, { L'C', L'С' }, { L'E', L'Е' }, { L'H', L'Н' }, { L'N', 0x039D },
		{ L'S', L'Ѕ' }, { L'J', 0x0408 }, { L'Y', 0x03A5 },
		{ L'K', L'К' }, { L'M', L'М' }, { L'O', L'О' }, { L'P', L'Р' }, { L'T', L'Т' }, { L'X', L'Х' }, { L'Z', 0x0396 }
	};

	std::wstring output;
	output.reserve(text_for_processing.length());

	std::transform(text_for_processing.begin(), text_for_processing.end(), std::back_inserter(output),
		[&replacements](wchar_t c) {
			auto it = replacements.find(c);
			return (it != replacements.end()) ? it->second : c;
		});

	return output;
}

int main()
{
	setlocale(LC_ALL, "rus");
	_setmode(_fileno(stdout), _O_U16TEXT);

	std::wstring last_copy_text = L"";
	bool first_iter = true;

	while (true) 
	{
		if (last_copy_text != ClipboardHJ::get_clipboard_text() && !first_iter)
		{
			if (ClipboardHJ::catch_change_text(last_copy_text))
			{
				std::wstring text_for_processing = ClipboardHJ::get_clipboard_text();
				std::wcout << "\n[DETECTED TEXT CHANGE] text = " << text_for_processing << std::endl;

				std::wstring ready_text = replace_letters(text_for_processing);
				std::wcout << "[CHANGED TEXT] text = " << ready_text << std::endl;

				if (ClipboardHJ::set_clipboard_text(ready_text))
				{
					std::wcout << "successful copy" << std::endl;
				}

				last_copy_text = ClipboardHJ::get_clipboard_text();
			}
		} 

		last_copy_text = ClipboardHJ::get_clipboard_text();
		first_iter = false;
		Sleep(10);
	}

	return 0;
}