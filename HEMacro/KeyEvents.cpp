#include "pch.h"
#include "KeyEvents.h"

using namespace KeyHook;
// 1 ������ ������ ����, 0 ������ �������� �Է�

LRESULT EventDownWithCtrl(PKBDLLHOOKSTRUCT p) {
    InputQueue::Instance.push(VK_CONTROL, DOWN);
    InputQueue::Instance.push(p->vkCode, DOWN);
    return 1;
}
LRESULT EventUpWithCtrl(PKBDLLHOOKSTRUCT p) {
    InputQueue::Instance.push(VK_CONTROL, UP);
    InputQueue::Instance.push(p->vkCode, UP);
    return 1;
}