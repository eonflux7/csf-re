#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(void, __stdcall, NetCogibleDdl_InitializationCallback, 0x0065E6E0, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_RegisterProperty, 0x00662890, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, NetCogibleDdl_SetGatherer, 0x0066CBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetCogibleDdl_SetUpdater, 0x0066D1C0, (void* self, void* edx, int a2));

static void __fastcall NetCogible_InitializePropertyHandlers(
    NetCogible* self, void* /*edx*/, int mode)
{
    NetCogibleDdl_InitializationCallback(mode);
    if (mode != 0)
        return;

    alignas(8) uint8_t propertyBytes[0x4];
    auto** propertyOut = reinterpret_cast<void**>(propertyBytes);
    auto* propertyId = reinterpret_cast<uint8_t*>(&mode) + 3;
    void* properties = reinterpret_cast<uint8_t*>(self) + 0xC;

    NetCogibleDdl_RegisterProperty(self, nullptr, 1);
    void* handler = AllocateGatheringStorage(
        4, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 32);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x00867A0C);
    *propertyId = 1;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 33);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x00867A24);
    *propertyId = 1;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 2);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 34);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x00867A30);
    *propertyId = 2;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 35);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x00867A48);
    *propertyId = 2;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 3);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 36);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x00867A30);
    *propertyId = 3;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 37);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x00867A48);
    *propertyId = 3;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 4);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 38);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x00867A30);
    *propertyId = 4;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 39);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x00867A48);
    *propertyId = 4;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));
}

HOOK(0x00742CC0, NetCogible_InitializePropertyHandlers, "auto");
