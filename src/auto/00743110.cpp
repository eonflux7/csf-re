#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(char, __stdcall, NetDdl_FallbackDispatch, 0x0065E720, (int a1, int* a2));
CSF_FUNC(void**, __fastcall, NetDdl_ResolveChannel, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(void, __fastcall, NetDdl_SendSubField, 0x0066CF50, (void* self, void* edx, void* a2, int a3, int a4, int a5, int a6));

static void SendSubField(C_00858180* self, int* args, int offset, uint8_t index)
{
    int obj = LookupObjectByIndex(self->field_002c >> 22);
    void* outPtr;
    uint8_t idx = index;
    NetDdl_ResolveChannel(reinterpret_cast<uint8_t*>(obj) + 0xC, nullptr, &outPtr, &idx);
    void* target = CSF_AT(void*, outPtr, 0x10);
    NetDdl_SendSubField(target, nullptr, self,
                        static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + offset)),
                        index, args[0], args[1]);
}

static char __fastcall C_00858180_SendSubFields(C_00858180* self, void* /*edx*/, C_00858180* target, int* args)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    uint8_t* t = reinterpret_cast<uint8_t*>(target);

    if (!t || t == base + 0x44) {
        SendSubField(self, args, 0x44, 1);
        if (t == base + 0x44)
            return 1;
    }
    if (!t || t == base + 0x6C) {
        SendSubField(self, args, 0x6C, 2);
        if (t == base + 0x6C)
            return 1;
    }
    if (!t || t == base + 0x6F) {
        SendSubField(self, args, 0x6F, 3);
        if (t == base + 0x6F)
            return 1;
    }
    if (!t || t == base + 0x72) {
        SendSubField(self, args, 0x72, 4);
        if (t == base + 0x72)
            return 1;
    }
    return NetDdl_FallbackDispatch(static_cast<int>(reinterpret_cast<uintptr_t>(target)), args);
}

HOOK(0x00743110, C_00858180_SendSubFields, "auto");
