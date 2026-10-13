#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0048e570_ActivateFocusEntry, 0x0048E570, (S_0048e570* self, void* /*edx*/, int* entry));

// Activates the focus entry for this object, then dispatches vtable slot 35 (offset 0x8C)
// on the object stored 12 bytes before `this`, passing (0, 0, -1, 0).
static int __fastcall S_0048e570_ActivateFocusThenDispatch(S_0048e570* self, void* /*edx*/, int* a2)
{
    S_0048e570_ActivateFocusEntry(self, nullptr, a2);

    void* obj = reinterpret_cast<uint8_t*>(self) - 12;
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, obj, 0)[0x8C / 4])(obj, nullptr, 0, 0, -1, 0);
}

HOOK(0x00492AB0, S_0048e570_ActivateFocusThenDispatch, "auto");
