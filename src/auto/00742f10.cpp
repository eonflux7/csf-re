#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void, __stdcall, sub_64AF90, 0x0064AF90, (int a1, int a2));
CSF_FUNC(void**, __fastcall, sub_6622F0, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, sub_66CD80, 0x0066CD80, (void* self, void* edx, int arg0, int a3, int a4, int a5, void* a6));

static int __fastcall NetCogibleDDL_RegisterChannels(char* self, void* /*edx*/, C_008622e8* a2, int a3)
{
    void* outObj = nullptr;
    uint8_t key = 1;

    uint8_t* obj = reinterpret_cast<uint8_t*>(LookupObjectByIndex((int)(CSF_AT(uint32_t, self, 0x2c) >> 22)));
    sub_6622F0(obj + 0xC, nullptr, &outObj, &key);
    sub_66CD80(CSF_AT(void*, outObj, 0x10), nullptr, (int)self, (int)(self + 0x44), 1, a3, a2);

    key = 2;
    obj = reinterpret_cast<uint8_t*>(LookupObjectByIndex((int)(CSF_AT(uint32_t, self, 0x2c) >> 22)));
    sub_6622F0(obj + 0xC, nullptr, &outObj, &key);
    sub_66CD80(CSF_AT(void*, outObj, 0x10), nullptr, (int)self, (int)(self + 0x6C), 2, a3, a2);

    key = 3;
    obj = reinterpret_cast<uint8_t*>(LookupObjectByIndex((int)(CSF_AT(uint32_t, self, 0x2c) >> 22)));
    sub_6622F0(obj + 0xC, nullptr, &outObj, &key);
    sub_66CD80(CSF_AT(void*, outObj, 0x10), nullptr, (int)self, (int)(self + 0x6F), 3, a3, a2);

    key = 4;
    obj = reinterpret_cast<uint8_t*>(LookupObjectByIndex((int)(CSF_AT(uint32_t, self, 0x2c) >> 22)));
    sub_6622F0(obj + 0xC, nullptr, &outObj, &key);
    sub_66CD80(CSF_AT(void*, outObj, 0x10), nullptr, (int)self, (int)(self + 0x72), 4, a3, a2);

    sub_64AF90((int)a2, a3);
    return 0;
}

HOOK(0x00742F10, NetCogibleDDL_RegisterChannels, "auto");
