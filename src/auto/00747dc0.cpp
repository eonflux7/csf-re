#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(char, __stdcall, NetGameDdl_ProcessBaseField, 0x0065E750, (int a1, int a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, NetPlayerDdl_ProcessField, 0x0066CB70, (void* self, void* edx, int a2, int a3, int a4, int a5));

static char __fastcall NetProyectilDdl_ProcessTypedField(
    void* self, void* /*edx*/, int fieldData, int fieldType)
{
    if (static_cast<uint8_t>(fieldType) != 1)
        return NetGameDdl_ProcessBaseField(fieldData, fieldType);

    // The original allocates this slot with push ecx, leaving self in it.
    alignas(8) uint8_t propertyBytes[0x4];
    CSF_AT(void*, propertyBytes, 0) = self;

    const int objectIndex =
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    reinterpret_cast<uint8_t*>(&fieldType)[3] = 1;

    const int objectAddress = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<void*>(static_cast<uintptr_t>(objectAddress) + 0xC),
        nullptr,
        reinterpret_cast<void**>(propertyBytes),
        reinterpret_cast<uint8_t*>(&fieldType) + 3);

    void* property = CSF_AT(void*, propertyBytes, 0);
    NetPlayerDdl_ProcessField(
        CSF_AT(void*, property, 0x10),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<uintptr_t>(
            static_cast<uint8_t*>(self) + 0x44)),
        1,
        fieldData);

    return 1;
}

HOOK(0x00747DC0, NetProyectilDdl_ProcessTypedField, "auto");
