#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

// Returns the vtable-slot-4 result of the embedded object at +0x30 (a pointer to 3 dwords).
// Copies those three dwords into self+0x50, then calls slot 27 (+0x6C) on the global's +0xF0 object
// with (self+0x50, self+0xCC, 2). Otherwise returns 1.
static char __fastcall GfxObject_RefreshPositionAndNotify(void* self, void* /*edx*/)
{
    char result = 1;
    if ((CSF_AT(uint8_t, self, 0x64) & 0x20) != 0)
    {
        // Inner object at self+0x30: its pointer is loaded from +0x30, and the call passes self+0x30 as `this`.
        void* innerObject = CSF_AT(void*, self, 0x30);
        uint32_t* sourceTriplet = reinterpret_cast<uint32_t*>(
            reinterpret_cast<char* (__fastcall*)(void*, void*)>(CSF_AT(void**, innerObject, 0)[4])(
                innerObject, nullptr));

        CSF_AT(uint32_t, self, 0x50) = sourceTriplet[0];
        CSF_AT(uint32_t, self, 0x54) = sourceTriplet[1];
        CSF_AT(uint32_t, self, 0x58) = sourceTriplet[2];

        int globalValue = GetGlobalValue_6C5760();
        uint8_t* globalBase = reinterpret_cast<uint8_t*>(globalValue);
        void* target = CSF_AT(void*, globalBase, 0xF0);

        void* positionArg = reinterpret_cast<uint8_t*>(self) + 0x50;
        void* notifyArg = reinterpret_cast<uint8_t*>(self) + 0xCC;

        return reinterpret_cast<char (__fastcall*)(void*, void*, void*, void*, int)>(
            CSF_AT(void**, target, 0)[27])(target, nullptr, positionArg, notifyArg, 2);
    }
    return result;
}

HOOK(0x6B31E0, GfxObject_RefreshPositionAndNotify, "auto");
