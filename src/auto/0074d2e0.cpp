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

static void __fastcall NetGame_InitializeProperties(
    NetGame* self, void* /*edx*/, int initializationMode)
{
    NetCogibleDdl_InitializationCallback(initializationMode);
    if (initializationMode != 0)
        return;

    alignas(8) uint8_t propertyBytes[0x4];
    void** propertyOut = reinterpret_cast<void**>(propertyBytes);
    uint8_t* propertyId = reinterpret_cast<uint8_t*>(&initializationMode) + 3;
    void* propertyCollection = reinterpret_cast<uint8_t*>(self) + 0xC;
    void* handler;

    NetCogibleDdl_RegisterProperty(self, nullptr, 1);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 32);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867E28);
    *propertyId = 1;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 33);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867E40);
    *propertyId = 1;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 2);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 34);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867E28);
    *propertyId = 2;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 35);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867E40);
    *propertyId = 2;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 3);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 36);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867A30);
    *propertyId = 3;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 37);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867A48);
    *propertyId = 3;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 4);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 38);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867A30);
    *propertyId = 4;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 39);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867A48);
    *propertyId = 4;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 5);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 40);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x8684A4);
    *propertyId = 5;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 41);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x8684BC);
    *propertyId = 5;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 6);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 42);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867E28);
    *propertyId = 6;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 43);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867E40);
    *propertyId = 6;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 7);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 44);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DBC);
    *propertyId = 7;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 45);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DD4);
    *propertyId = 7;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 8);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 46);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DBC);
    *propertyId = 8;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 47);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DD4);
    *propertyId = 8;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 9);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 48);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DBC);
    *propertyId = 9;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 49);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DD4);
    *propertyId = 9;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 10);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 50);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DBC);
    *propertyId = 10;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 51);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DD4);
    *propertyId = 10;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 11);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 52);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DBC);
    *propertyId = 11;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 53);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867DD4);
    *propertyId = 11;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 12);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 54);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x868210);
    *propertyId = 12;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 55);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x868228);
    *propertyId = 12;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    NetCogibleDdl_RegisterProperty(self, nullptr, 13);
    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 56);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867A30);
    *propertyId = 13;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetGatherer(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));

    handler = AllocateGatheringStorage(
        4, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 57);
    if (handler)
        CSF_AT(void**, handler, 0) = CSF_VTBL(0x867A48);
    *propertyId = 13;
    NetCogibleDdl_FindProperty(propertyCollection, nullptr, propertyOut, propertyId);
    NetCogibleDdl_SetUpdater(
        CSF_AT(void*, *propertyOut, 0x10), nullptr, reinterpret_cast<int>(handler));
}

HOOK(0x0074D2E0, NetGame_InitializeProperties, "auto");
