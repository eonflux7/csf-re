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

static int __fastcall Session_InitializeProperties(
    Session* self, void* /*edx*/, int initializationMode)
{
    alignas(8) uint8_t propertyBytes[0x4];
    CSF_AT(void*, propertyBytes, 0) = self;

    NetCogibleDdl_InitializationCallback(initializationMode);
    if (initializationMode != 0)
        return 0;

    NetCogibleDdl_RegisterProperty(self, nullptr, 1);

    void* gatherer = AllocateGatheringStorage(
        4, CSF_STR(0x00868698, ".\\PCGenerados\\NetPlayerListDDL.cpp"), 25);
    if (gatherer)
        CSF_AT(void**, gatherer, 0) = CSF_VTBL(0x008686E4);

    auto* propertyOut = reinterpret_cast<void**>(propertyBytes);
    auto* propertyId = reinterpret_cast<uint8_t*>(&initializationMode) + 3;
    auto* properties = reinterpret_cast<uint8_t*>(self) + 0xC;

    *propertyId = 1;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, CSF_AT(void*, propertyBytes, 0), 0x10),
        nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(gatherer)));

    void* updater = AllocateGatheringStorage(
        4, CSF_STR(0x00868698, ".\\PCGenerados\\NetPlayerListDDL.cpp"), 26);
    if (updater)
        CSF_AT(void**, updater, 0) = CSF_VTBL(0x008686FC);

    *propertyId = 1;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, propertyId);
    return NetCogibleDdl_SetUpdater(
        CSF_AT(void*, CSF_AT(void*, propertyBytes, 0), 0x10),
        nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(updater)));
}

HOOK(0x0074F630, Session_InitializeProperties, "auto");
