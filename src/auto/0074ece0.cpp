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

static int __fastcall Session_InitializeProperties(Session* self, void* /*edx*/, int phase)
{
    alignas(8) uint8_t propertyBytes[0x4];
    void*& property = *reinterpret_cast<void**>(propertyBytes);
    property = self;

    NetCogibleDdl_InitializationCallback(phase);
    if (phase != 0)
        return 0;

    uint8_t propertyId;

    NetCogibleDdl_RegisterProperty(self, nullptr, 1);
    void* gatherer = AllocateGatheringStorage(
        4, CSF_STR(0x00868524, ".\\PCGenerados\\NetControlDDL.cpp"), 25);
    if (gatherer)
        CSF_AT(void**, gatherer, 0) = CSF_VTBL(0x00868588);

    void* properties = reinterpret_cast<uint8_t*>(self) + 0xC;
    propertyId = 1;
    NetCogibleDdl_FindProperty(properties, nullptr, &property, &propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, property, 0x10), nullptr, reinterpret_cast<int>(gatherer));

    void* updater = AllocateGatheringStorage(
        4, CSF_STR(0x00868524, ".\\PCGenerados\\NetControlDDL.cpp"), 26);
    if (updater)
        CSF_AT(void**, updater, 0) = CSF_VTBL(0x008685A0);

    propertyId = 1;
    NetCogibleDdl_FindProperty(properties, nullptr, &property, &propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, property, 0x10), nullptr, reinterpret_cast<int>(updater));

    NetCogibleDdl_RegisterProperty(self, nullptr, 2);
    gatherer = AllocateGatheringStorage(
        4, CSF_STR(0x00868524, ".\\PCGenerados\\NetControlDDL.cpp"), 27);
    if (gatherer)
        CSF_AT(void**, gatherer, 0) = CSF_VTBL(0x008685AC);

    propertyId = 2;
    NetCogibleDdl_FindProperty(properties, nullptr, &property, &propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, property, 0x10), nullptr, reinterpret_cast<int>(gatherer));

    updater = AllocateGatheringStorage(
        4, CSF_STR(0x00868524, ".\\PCGenerados\\NetControlDDL.cpp"), 28);
    if (updater)
        CSF_AT(void**, updater, 0) = CSF_VTBL(0x008685C4);

    propertyId = 2;
    NetCogibleDdl_FindProperty(properties, nullptr, &property, &propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, property, 0x10), nullptr, reinterpret_cast<int>(updater));

    NetCogibleDdl_RegisterProperty(self, nullptr, 3);
    gatherer = AllocateGatheringStorage(
        4, CSF_STR(0x00868524, ".\\PCGenerados\\NetControlDDL.cpp"), 29);
    if (gatherer)
        CSF_AT(void**, gatherer, 0) = CSF_VTBL(0x008685D0);

    propertyId = 3;
    NetCogibleDdl_FindProperty(properties, nullptr, &property, &propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, property, 0x10), nullptr, reinterpret_cast<int>(gatherer));

    updater = AllocateGatheringStorage(
        4, CSF_STR(0x00868524, ".\\PCGenerados\\NetControlDDL.cpp"), 30);
    if (updater)
        CSF_AT(void**, updater, 0) = CSF_VTBL(0x008685E8);

    propertyId = 3;
    NetCogibleDdl_FindProperty(properties, nullptr, &property, &propertyId);
    return NetCogibleDdl_SetUpdater(
        CSF_AT(void*, property, 0x10), nullptr, reinterpret_cast<int>(updater));
}

HOOK(0x0074ECE0, Session_InitializeProperties, "auto");
