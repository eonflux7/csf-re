#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, ProcessDDLChunk, 0x0066CB20, (void* self, void* edx, int arg0, int a3, int a4, void* a5));

static void __fastcall NetPostmanDdl_ProcessProperties(
    void* self, void* /*edx*/, C_008622e8* stream)
{
    alignas(8) uint8_t propertyBytes[0x4];
    alignas(8) uint8_t firstKeyBytes[0x4];
    uint8_t subsequentKey;
    auto** propertyOut = reinterpret_cast<void**>(propertyBytes);

    int objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    firstKeyBytes[0] = 1;
    auto* object = reinterpret_cast<uint8_t*>(LookupObjectByIndex(objectIndex));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, propertyOut, firstKeyBytes);
    void* property = CSF_AT(void*, propertyBytes, 0);
    ProcessDDLChunk(CSF_AT(void*, property, 0x10), nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x44), 1, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    subsequentKey = 2;
    object = reinterpret_cast<uint8_t*>(LookupObjectByIndex(objectIndex));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, propertyOut, &subsequentKey);
    property = CSF_AT(void*, propertyBytes, 0);
    ProcessDDLChunk(CSF_AT(void*, property, 0x10), nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x50), 2, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    subsequentKey = 3;
    object = reinterpret_cast<uint8_t*>(LookupObjectByIndex(objectIndex));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, propertyOut, &subsequentKey);
    property = CSF_AT(void*, propertyBytes, 0);
    ProcessDDLChunk(CSF_AT(void*, property, 0x10), nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x5C), 3, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    subsequentKey = 4;
    object = reinterpret_cast<uint8_t*>(LookupObjectByIndex(objectIndex));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, propertyOut, &subsequentKey);
    property = CSF_AT(void*, propertyBytes, 0);
    ProcessDDLChunk(CSF_AT(void*, property, 0x10), nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x6C), 4, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    subsequentKey = 5;
    object = reinterpret_cast<uint8_t*>(LookupObjectByIndex(objectIndex));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, propertyOut, &subsequentKey);
    property = CSF_AT(void*, propertyBytes, 0);
    ProcessDDLChunk(CSF_AT(void*, property, 0x10), nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x78), 5, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    subsequentKey = 6;
    object = reinterpret_cast<uint8_t*>(LookupObjectByIndex(objectIndex));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, propertyOut, &subsequentKey);
    property = CSF_AT(void*, propertyBytes, 0);
    ProcessDDLChunk(CSF_AT(void*, property, 0x10), nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x88), 6, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    subsequentKey = 7;
    object = reinterpret_cast<uint8_t*>(LookupObjectByIndex(objectIndex));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, propertyOut, &subsequentKey);
    property = CSF_AT(void*, propertyBytes, 0);
    ProcessDDLChunk(CSF_AT(void*, property, 0x10), nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0xA0), 7, stream);

    C_0085ec18_NullSub(reinterpret_cast<int>(stream));
}

HOOK(0x750500, NetPostmanDdl_ProcessProperties, "auto");
