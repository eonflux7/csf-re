#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_RegisterProperty, 0x00662890, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, NetCogibleDdl_SetGatherer, 0x0066CBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetCogibleDdl_SetUpdater, 0x0066D1C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Sub_7A36E0, 0x007A36E0, (void* self, void* edx, int a1));

static int NetPlayer_RegisterPropertyHandlers(
    NetPlayer* self,
    uint8_t propertyId,
    int gathererLine,
    int updaterLine,
    void** gathererVtable,
    void** updaterVtable,
    void** propertyOut)
{
    NetCogibleDdl_RegisterProperty(self, nullptr, static_cast<char>(propertyId));

    void* gatherer = AllocateGatheringStorage(
        4, CSF_STR(0x00867B48, ".\\PCGenerados\\NetPlayerDDL.cpp"), gathererLine);
    if (gatherer)
        CSF_AT(void**, gatherer, 0) = gathererVtable;

    uint8_t lookupId = propertyId;
    void* properties = reinterpret_cast<uint8_t*>(self) + 0xC;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, &lookupId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(gatherer)));

    void* updater = AllocateGatheringStorage(
        4, CSF_STR(0x00867B48, ".\\PCGenerados\\NetPlayerDDL.cpp"), updaterLine);
    if (updater)
        CSF_AT(void**, updater, 0) = updaterVtable;

    lookupId = propertyId;
    NetCogibleDdl_FindProperty(properties, nullptr, propertyOut, &lookupId);
    return NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(updater)));
}

static int __fastcall NetPlayer_RegisterProperties(
    NetPlayer* self, void* /*edx*/, int mode)
{
    int result = Sub_7A36E0(self, nullptr, mode);
    if (mode)
        return result;

    alignas(8) uint8_t propertyBytes[0x4] = {};
    void** propertyOut = reinterpret_cast<void**>(propertyBytes);

    NetPlayer_RegisterPropertyHandlers(
        self, 4, 38, 39, CSF_VTBL(0x00867A30), CSF_VTBL(0x00867A48), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 5, 40, 41, CSF_VTBL(0x00867A30), CSF_VTBL(0x00867A48), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 6, 42, 43, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 7, 44, 45, CSF_VTBL(0x00867A30), CSF_VTBL(0x00867A48), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 8, 46, 47, CSF_VTBL(0x00867A30), CSF_VTBL(0x00867A48), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 9, 48, 49, CSF_VTBL(0x00867DE0), CSF_VTBL(0x00867DF8), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 10, 50, 51, CSF_VTBL(0x00867E04), CSF_VTBL(0x00867E1C), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 11, 52, 53, CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 12, 54, 55, CSF_VTBL(0x00867E28), CSF_VTBL(0x00867E40), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 13, 56, 57, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 14, 58, 59, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 15, 60, 61, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 16, 62, 63, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 17, 64, 65, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 18, 66, 67, CSF_VTBL(0x00867DBC), CSF_VTBL(0x00867DD4), propertyOut);
    NetPlayer_RegisterPropertyHandlers(
        self, 19, 68, 69, CSF_VTBL(0x00867A30), CSF_VTBL(0x00867A48), propertyOut);
    return NetPlayer_RegisterPropertyHandlers(
        self, 20, 70, 71, CSF_VTBL(0x00867E4C), CSF_VTBL(0x00867E64), propertyOut);
}

HOOK(0x00745B40, NetPlayer_RegisterProperties, "auto");
