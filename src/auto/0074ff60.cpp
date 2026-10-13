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

static void NetPostman_RegisterPropertyHandlers(
    NetPostman* self, uint8_t propertyId, uint8_t* lookupId,
    void** propertyStorage, int gathererLine, int updaterLine,
    void** gathererVtable, void** updaterVtable)
{
    NetCogibleDdl_RegisterProperty(self, nullptr, static_cast<char>(propertyId));

    void* gatherer = AllocateGatheringStorage(
        4, CSF_STR(0x008687A8, ".\\PCGenerados\\NetPostmanDDL.cpp"),
        gathererLine);
    if (gatherer)
        CSF_AT(void**, gatherer, 0) = gathererVtable;

    *lookupId = propertyId;
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(self) + 0xC, nullptr,
        propertyStorage, lookupId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyStorage, 0x10), nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(gatherer)));

    void* updater = AllocateGatheringStorage(
        4, CSF_STR(0x008687A8, ".\\PCGenerados\\NetPostmanDDL.cpp"),
        updaterLine);
    if (updater)
        CSF_AT(void**, updater, 0) = updaterVtable;

    *lookupId = propertyId;
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(self) + 0xC, nullptr,
        propertyStorage, lookupId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyStorage, 0x10), nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(updater)));
}

static void __fastcall NetPostman_InitializeProperties(
    NetPostman* self, void* /*edx*/, int initializationMode)
{
    alignas(8) uint8_t propertyBytes[0x4];
    auto propertyStorage = reinterpret_cast<void**>(propertyBytes);
    *propertyStorage = self;

    NetCogibleDdl_InitializationCallback(initializationMode);
    if (initializationMode != 0)
        return;

    auto lookupId = reinterpret_cast<uint8_t*>(&initializationMode) + 3;

    NetPostman_RegisterPropertyHandlers(
        self, 1, lookupId, propertyStorage, 28, 29,
        CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40));
    NetPostman_RegisterPropertyHandlers(
        self, 2, lookupId, propertyStorage, 30, 31,
        CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40));
    NetPostman_RegisterPropertyHandlers(
        self, 3, lookupId, propertyStorage, 32, 33,
        CSF_VTBL(0x0086886C), CSF_VTBL(0x00868884));
    NetPostman_RegisterPropertyHandlers(
        self, 4, lookupId, propertyStorage, 34, 35,
        CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40));
    NetPostman_RegisterPropertyHandlers(
        self, 5, lookupId, propertyStorage, 36, 37,
        CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40));
    NetPostman_RegisterPropertyHandlers(
        self, 6, lookupId, propertyStorage, 38, 39,
        CSF_VTBL(0x00868210), CSF_VTBL(0x00868228));
    NetPostman_RegisterPropertyHandlers(
        self, 7, lookupId, propertyStorage, 40, 41,
        CSF_VTBL(0x00868210), CSF_VTBL(0x00868228));
}

HOOK(0x0074FF60, NetPostman_InitializeProperties, "auto");
