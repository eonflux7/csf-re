#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, ProcessDDLChunk, 0x0066CB20, (void* self, void* edx, int arg0, int a3, int a4, void* a5));

static int __fastcall NetProyectilDDL_ProcessProperty(
    void* self, void* /*edx*/, C_008622e8* context)
{
    alignas(8) uint8_t lookupBytes[8];
    void** propertyOut = reinterpret_cast<void**>(lookupBytes);
    uint8_t* propertyKey = lookupBytes + 4;

    const int objectIndex =
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    propertyKey[0] = 1;

    const int object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(object)) + 0xC,
        nullptr, propertyOut, propertyKey);

    void* chunk = CSF_AT(void*, *propertyOut, 0x10);
    ProcessDDLChunk(
        chunk, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<uintptr_t>(
            reinterpret_cast<uint8_t*>(self) + 0x44)),
        1, context);

    // Preserve the original final call's ECX and full EAX result.
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        C_0085ec18_NullSub)(
            self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(context)));
}

HOOK(0x747CE0, NetProyectilDDL_ProcessProperty, "auto");
