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

static void __cdecl NetGhost_RegisterPropertyHandlers(
    NetGhost* self,
    uint8_t id,
    void** gathererVtable,
    int gathererSourceLine,
    void** updaterVtable,
    int updaterSourceLine,
    void** propertyStorage,
    uint8_t* propertyId)
{
    NetCogibleDdl_RegisterProperty(self, nullptr, static_cast<char>(id));

    void* gatherer = AllocateGatheringStorage(
        4, CSF_STR(0x008680F8, ".\\PCGenerados\\NetGhostDDL.cpp"),
        gathererSourceLine);
    if (gatherer)
        CSF_AT(void**, gatherer, 0) = gathererVtable;

    *propertyId = id;
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(self) + 0xC,
        nullptr, propertyStorage, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyStorage, 0x10), nullptr,
        reinterpret_cast<int>(gatherer));

    void* updater = AllocateGatheringStorage(
        4, CSF_STR(0x008680F8, ".\\PCGenerados\\NetGhostDDL.cpp"),
        updaterSourceLine);
    if (updater)
        CSF_AT(void**, updater, 0) = updaterVtable;

    *propertyId = id;
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(self) + 0xC,
        nullptr, propertyStorage, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyStorage, 0x10), nullptr,
        reinterpret_cast<int>(updater));
}

static void __fastcall NetGhost_InitializePropertyHandlers(
    NetGhost* self, void* /*edx*/, int initializationMode)
{
    // The original's initial push ecx seeds the property out-storage.
    alignas(8) uint8_t propertyBytes[0x4];
    CSF_AT(void*, propertyBytes, 0) = self;

    NetCogibleDdl_InitializationCallback(initializationMode);
    if (initializationMode != 0)
        return;

    void** propertyStorage = reinterpret_cast<void**>(propertyBytes);
    uint8_t propertyId;

    NetGhost_RegisterPropertyHandlers(
        self, 1, CSF_VTBL(0x00867A30), 28,
        CSF_VTBL(0x00867A48), 29, propertyStorage, &propertyId);
    NetGhost_RegisterPropertyHandlers(
        self, 2, CSF_VTBL(0x00867A30), 30,
        CSF_VTBL(0x00867A48), 31, propertyStorage, &propertyId);
    NetGhost_RegisterPropertyHandlers(
        self, 3, CSF_VTBL(0x00867A30), 32,
        CSF_VTBL(0x00867A48), 33, propertyStorage, &propertyId);
    NetGhost_RegisterPropertyHandlers(
        self, 4, CSF_VTBL(0x008681EC), 34,
        CSF_VTBL(0x00868204), 35, propertyStorage, &propertyId);
    NetGhost_RegisterPropertyHandlers(
        self, 5, CSF_VTBL(0x00867E28), 36,
        CSF_VTBL(0x00867E40), 37, propertyStorage, &propertyId);
    NetGhost_RegisterPropertyHandlers(
        self, 6, CSF_VTBL(0x00868210), 38,
        CSF_VTBL(0x00868228), 39, propertyStorage, &propertyId);
}

HOOK(0x0074BA10, NetGhost_InitializePropertyHandlers, "auto");
