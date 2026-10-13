#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, ProcessDDLChunk, 0x0066CB20, (void* self, void* edx, int arg0, int a3, int a4, void* a5));

static void __fastcall NetPlayerListDdl_ProcessProperties(
    void* self, void* /*edx*/, C_008622e8* context)
{
    alignas(8) uint8_t propertyBytes[0x4];
    alignas(8) uint8_t firstKeyBytes[0x4];
    void** property = reinterpret_cast<void**>(propertyBytes);

    firstKeyBytes[0] = 1;
    int objectAddress = LookupObjectByIndex(
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(objectAddress) + 0xC,
        nullptr, property, firstKeyBytes);
    ProcessDDLChunk(
        CSF_AT(void*, *property, 0x10), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<uintptr_t>(
            static_cast<uint8_t*>(self) + 0x44)),
        1, context);

    uint8_t subsequentKey = 2;
    objectAddress = LookupObjectByIndex(
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(objectAddress) + 0xC,
        nullptr, property, &subsequentKey);
    ProcessDDLChunk(
        CSF_AT(void*, *property, 0x10), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<uintptr_t>(
            static_cast<uint8_t*>(self) + 0x58)),
        2, context);

    subsequentKey = 3;
    objectAddress = LookupObjectByIndex(
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(objectAddress) + 0xC,
        nullptr, property, &subsequentKey);
    ProcessDDLChunk(
        CSF_AT(void*, *property, 0x10), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<uintptr_t>(
            static_cast<uint8_t*>(self) + 0xA8)),
        3, context);

    C_0085ec18_NullSub(
        static_cast<int>(reinterpret_cast<uintptr_t>(context)));
}

HOOK(0x74EF80, NetPlayerListDdl_ProcessProperties, "auto");
