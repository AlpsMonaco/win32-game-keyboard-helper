#include <stdio.h>
#include <windows.h>

static LRESULT PrintKeyboardInputProc(int code, WPARAM wParam, LPARAM lParam) {
  KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
  printf("scan_code:%lu vk_code:%lu press:%d\n", ks->scanCode, ks->vkCode,
         wParam);
  return CallNextHookEx(NULL, code, wParam, lParam);
}

static int flag = 0;

static LRESULT KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
  KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
  if (ks->vkCode == VK_F1) {
    if (wParam == WM_KEYDOWN) {
      if (flag == 0) {
        keybd_event(0, 0x11, KEYEVENTF_SCANCODE, 0);
        flag = 1;
      } else {
        flag = 0;
        keybd_event(0, 0x11, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
      }
    }
    return 1;
  }
  if (ks->vkCode == 0x57 && wParam == WM_KEYUP) {
    flag = 0;
  }
  return CallNextHookEx(NULL, code, wParam, lParam);
}

int main() {
  SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
  }
}