#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char, __fastcall, C_0082f9f8_TryAcquire, 0x0048E5D0, (void* self, void* edx, int owner, int unused, int acquire));
CSF_FUNC(char, __fastcall, AiAttackTasks_AddToActionFlag, 0x0048C9D0, (uint8_t* self, void* , char delta));

static int __stdcall AiOrder_AcquireTaskAndSetActionFlag(int order)
{
    void* orderObject = reinterpret_cast<void*>(order);

    int linked = GetLinkedObjectValue_4DB5C0(orderObject, nullptr);
    uint32_t owner = linked ? static_cast<uint32_t>(linked) - 4 : 0;

    linked = GetLinkedObjectValue_4DB5C0(orderObject, nullptr);
    void* linkedBase = reinterpret_cast<void*>(
        linked ? static_cast<uint32_t>(linked) - 4 : 0);

    C_0082f9f8_TryAcquire(
        CSF_AT(void*, linkedBase, 0x2C),
        nullptr, static_cast<int>(owner), 0, 1);

    linked = GetLinkedObjectValue_4DB5C0(orderObject, nullptr);
    linkedBase = reinterpret_cast<void*>(
        linked ? static_cast<uint32_t>(linked) - 4 : 0);

    void* task = CSF_AT(void*, linkedBase, 0x2C);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, task, 0)[0x8C / 4])(
            task, nullptr, 17, 0, -1, 0);

    linked = GetLinkedObjectValue_4DB5C0(orderObject, nullptr);
    void* taskOwner;
    if (linked)
        taskOwner = CSF_AT(void*, reinterpret_cast<void*>(linked), 0x28);
    else
        taskOwner = CSF_AT(void*, static_cast<void*>(nullptr), 0x2C);

    return AiAttackTasks_AddToActionFlag(
        CSF_AT(uint8_t*, taskOwner, 0x204), nullptr, 1);
}

HOOK(0x004ACA40, AiOrder_AcquireTaskAndSetActionFlag, "auto");
