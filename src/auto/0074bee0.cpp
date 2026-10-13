#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, ProcessDDLChunk, 0x0066CB20, (void* self, void* edx, int arg0, int a3, int a4, void* a5));

static void __fastcall NetGhostDdl_ProcessProperties(
    char* self, void* /*edx*/, C_008622e8* stream)
{
    alignas(8) uint8_t propertyBytes[0x4];
    alignas(8) uint8_t firstKeyBytes[0x4];
    void** propertyOut = reinterpret_cast<void**>(propertyBytes);
    uint8_t propertyKey;

    int objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    firstKeyBytes[0] = 1;
    int object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, propertyOut, firstKeyBytes);
    ProcessDDLChunk(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x44),
        1, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    propertyKey = 2;
    object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, propertyOut, &propertyKey);
    ProcessDDLChunk(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x47),
        2, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    propertyKey = 3;
    object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, propertyOut, &propertyKey);
    ProcessDDLChunk(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x4A),
        3, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    propertyKey = 4;
    object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, propertyOut, &propertyKey);
    ProcessDDLChunk(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x50),
        4, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    propertyKey = 5;
    object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, propertyOut, &propertyKey);
    ProcessDDLChunk(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x5C),
        5, stream);

    objectIndex = static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    propertyKey = 6;
    object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, propertyOut, &propertyKey);
    ProcessDDLChunk(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x68),
        6, stream);

    C_0085ec18_NullSub(reinterpret_cast<int>(stream));
}

HOOK(0x0074BEE0, NetGhostDdl_ProcessProperties, "auto");
