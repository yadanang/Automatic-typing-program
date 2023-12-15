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
            // ��������ո񣬷��Ϳո��
            input.ki.wVk = VK_SPACE;
            input.ki.dwFlags = 0; // ���¼�
            SendInput(1, &input, sizeof(INPUT));

            input.ki.dwFlags = KEYEVENTF_KEYUP; // �����
            SendInput(1, &input, sizeof(INPUT));

            // ���ͻس���
            input.ki.wVk = VK_RETURN;
            input.ki.dwFlags = 0; // ���¼�
            SendInput(1, &input, sizeof(INPUT));

            input.ki.dwFlags = KEYEVENTF_KEYUP; // �����
            SendInput(1, &input, sizeof(INPUT));
        }
        else {
            input.ki.wVk = 0; // 0 ��ʾ Unicode
            input.ki.dwFlags = KEYEVENTF_UNICODE; // ���¼�
            input.ki.wScan = *c;
            SendInput(1, &input, sizeof(INPUT));

            input.ki.dwFlags = KEYEVENTF_KEYUP; // �����
            SendInput(1, &input, sizeof(INPUT));
        }

        // ��ѡ�İ���֮���ӳ٣�������Ҫ������
        Sleep(1000 / speed);
    }
}

void executeTexts(const std::vector<std::wstring>& texts, int speed, HWND targetWindow) {
    for (const auto& text : texts) {
        typeText(text.c_str(), speed, targetWindow);
        Sleep(1000); // ��ѡ���ı�֮���ӳ٣�������Ҫ������
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

    // ��������û�������ı���ֱ������ ESC ��Ϊֹ
    while (!isEscapePressed()) {
        // ��ȡ��ǰ����ڵľ��
        HWND activeWindow = GetForegroundWindow();
        executeTexts({ userText }, speed, activeWindow);
        Sleep(500); // ��ѡ���ظ�֮���ӳ٣�������Ҫ������
    }

    return 0;
}
