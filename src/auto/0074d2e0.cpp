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

static void __cdecl NetGame_RegisterPropertyHandlers(
    NetGame* self,
    uint8_t propertyId,
    int gathererLine,
    int updaterLine,
    void* gathererVtable,
    void* updaterVtable,
    void** propertyOut,
    uint8_t* lookupId)
{
    NetCogibleDdl_RegisterProperty(self, nullptr, static_cast<char>(propertyId));

    void* gatherer = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), gathererLine);
    if (gatherer)
        CSF_AT(void*, gatherer, 0) = gathererVtable;

    *lookupId = propertyId;
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(self) + 0xC, nullptr, propertyOut, lookupId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        reinterpret_cast<int>(gatherer));

    void* updater = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), updaterLine);
    if (updater)
        CSF_AT(void*, updater, 0) = updaterVtable;

    *lookupId = propertyId;
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(self) + 0xC, nullptr, propertyOut, lookupId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        reinterpret_cast<int>(updater));
}

static void __fastcall NetGame_InitializeProperties(
    NetGame* self, void* /*edx*/, int mode)
{
    alignas(8) uint8_t propertyBytes[0x4];
    void** propertyOut = reinterpret_cast<void**>(propertyBytes);
    // The original reserves this local with push ecx, initializing it to this.
    *propertyOut = self;

    NetCogibleDdl_InitializationCallback(mode);
    if (mode != 0)
        return;

    uint8_t* lookupId = reinterpret_cast<uint8_t*>(&mode) + 3;

    NetGame_RegisterPropertyHandlers(
        self, 1, 32, 33, CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 2, 34, 35, CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 3, 36, 37, CSF_VTBL(0x00867A30), CSF_VTBL(0x00867A48),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 4, 38, 39, CSF_VTBL(0x00867A30), CSF_VTBL(0x00867A48),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 5, 40, 41, CSF_VTBL(0x008684A4), CSF_VTBL(0x008684BC),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 6, 42, 43, CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 7, 44, 45, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 8, 46, 47, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 9, 48, 49, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 10, 50, 51, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 11, 52, 53, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 12, 54, 55, CSF_VTBL(0x00868210), CSF_VTBL(0x00868228),
        propertyOut, lookupId);
    NetGame_RegisterPropertyHandlers(
        self, 13, 56, 57, CSF_VTBL(0x00867A30), CSF_VTBL(0x00867A48),
        propertyOut, lookupId);
}

HOOK(0x0074D2E0, NetGame_InitializeProperties, "auto");
