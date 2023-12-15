#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

bool isEscapePressed() {
    return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
}

void typeText(const wchar_t* text, int speed, HWND targetWindow) {
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    for (const wchar_t* c = text; *c != L'\0'; c++) {
        if (isEscapePressed()) {
            std::wcout << L"The user presses the ESC key to stop the output.!!!\n";
            return;
        }

        if (*c == L' ') {
            // 如果遇到空格，发送空格键
            input.ki.wVk = VK_SPACE;
            input.ki.dwFlags = 0; // 按下键
            SendInput(1, &input, sizeof(INPUT));

            input.ki.dwFlags = KEYEVENTF_KEYUP; // 弹起键
            SendInput(1, &input, sizeof(INPUT));

            // 发送回车键
            input.ki.wVk = VK_RETURN;
            input.ki.dwFlags = 0; // 按下键
            SendInput(1, &input, sizeof(INPUT));

            input.ki.dwFlags = KEYEVENTF_KEYUP; // 弹起键
            SendInput(1, &input, sizeof(INPUT));
        }
        else {
            input.ki.wVk = 0; // 0 表示 Unicode
            input.ki.dwFlags = KEYEVENTF_UNICODE; // 按下键
            input.ki.wScan = *c;
            SendInput(1, &input, sizeof(INPUT));

            input.ki.dwFlags = KEYEVENTF_KEYUP; // 弹起键
            SendInput(1, &input, sizeof(INPUT));
        }

        // 可选的按键之间延迟（根据需要调整）
        Sleep(1000 / speed);
    }
}

void executeTexts(const std::vector<std::wstring>& texts, int speed, HWND targetWindow) {
    for (const auto& text : texts) {
        typeText(text.c_str(), speed, targetWindow);
        Sleep(1000); // 可选的文本之间延迟（根据需要调整）
    }
}

int main() {
    SetConsoleOutputCP(65001);
    std::wcout.imbue(std::locale(""));
    std::wcin.imbue(std::locale(""));

    std::wstring userText;
    std::wcout << L"Please enter the text you want to type:> ";
    std::getline(std::wcin, userText);

    int speed;
    std::wcout << L"Please enter typing speed (characters / second): >";
    std::wcin >> speed;

    // 持续输出用户定义的文本，直到按下 ESC 键为止
    while (!isEscapePressed()) {
        // 获取当前活动窗口的句柄
        HWND activeWindow = GetForegroundWindow();
        executeTexts({ userText }, speed, activeWindow);
        Sleep(500); // 可选的重复之间延迟（根据需要调整）
    }

    return 0;
}
