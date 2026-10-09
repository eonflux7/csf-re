#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_4EF610, 0x004EF610, (void* self, void* edx));

// Scene manager field offsets (dword index 2019 = 0x1F8C, index 2039 = 0x1FDC).
static int __fastcall Scene_ResetGlobalManager(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x1F8C);
    if (result) {
        void* object = reinterpret_cast<void*>(result);
        // Virtual slot 0x20 / 4 = 8
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, object, 0)[8])(object, nullptr);
    }

    int pendingObject = CSF_AT(int, self, 0x1FDC);
    if (pendingObject) {
        // Original tail-jumps here with ecx = pendingObject; its eax result is the function result.
        sub_4EF610(reinterpret_cast<void*>(pendingObject), nullptr);
    }
    return result;
}

HOOK(0x004E8CB0, Scene_ResetGlobalManager, "auto");
