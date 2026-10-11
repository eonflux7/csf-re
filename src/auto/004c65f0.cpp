#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, sub_4BC610, 0x004BC610, (void* self, void* edx, float a2, int a3));
CSF_FUNC(int, __fastcall, sub_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

// Resolves the owner object of the task (linked object minus the 4-byte secondary-base offset).
static uint8_t* CoverAttack_GetOwner(int task)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(task), nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

// Appends one dword to the dynamic array {data, capacity, count, growth}.
static void CoverAttack_PushValue(uint8_t* arr, int value)
{
    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4))
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));
    int index = CSF_AT(int, arr, 8);
    int* data = CSF_AT(int*, arr, 0);
    int* slot = data + index;
    CSF_AT(int, arr, 8) = index + 1;
    *slot = value;
}

static int __stdcall CoverAttack_SetupParapetCommands(int a1)
{
    uint8_t* owner = CoverAttack_GetOwner(a1);
    sub_4DC180(reinterpret_cast<void*>(a1), nullptr, reinterpret_cast<int>(owner + 0x440));

    uint8_t* obj = CoverAttack_GetOwner(a1);
    uint8_t* obj2 = CoverAttack_GetOwner(a1);

    uint8_t* src;
    if (CSF_AT(int, obj2, 0x630) < 0)
        src = obj2 + 0x610;
    else
        src = obj2 + 0x628;

    uint8_t* arr = obj + 0x50C;
    sub_4C5150(arr, nullptr, reinterpret_cast<int>(src));

    CoverAttack_PushValue(arr, 5);
    CoverAttack_PushValue(arr, 10);

    int state = GetStateValue(reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 3);
    float dir;
    if (state == 0) {
        dir = 1.0f;
    } else if (state == 1) {
        dir = -1.0f;
    } else {
        return state;
    }

    CoverAttack_PushValue(arr, 13);
    sub_4BC610(arr, nullptr, dir, 1145569280);
    sub_4BC3A0(arr, nullptr, 2.0f, 5.0f);
    CoverAttack_PushValue(arr, 14);
    return CSF_AT(int, arr, 8);
}

HOOK(0x004C65F0, CoverAttack_SetupParapetCommands, "auto");
