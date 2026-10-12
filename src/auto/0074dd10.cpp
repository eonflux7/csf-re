#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, ProcessDDLChunk, 0x0066CB20, (void* self, void* edx, int arg0, int a3, int a4, void* a5));

static char NetGameDdl_ProcessProperty(
    char* self,
    C_008622e8* context,
    uint8_t propertyId,
    int memberOffset,
    void** property,
    uint8_t* propertyKey)
{
    const int objectIndex = CSF_AT(uint32_t, self, 0x2C) >> 22;
    *propertyKey = propertyId;

    auto* object = reinterpret_cast<uint8_t*>(LookupObjectByIndex(objectIndex));
    NetCogibleDdl_FindProperty(object + 0x0C, nullptr, property, propertyKey);

    return ProcessDDLChunk(
        CSF_AT(void*, *property, 0x10),
        nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(self + memberOffset),
        propertyId,
        context);
}

static char __fastcall NetGameDdl_ProcessGameProperties(
    char* self, void* /*edx*/, C_008622e8* context)
{
    alignas(8) uint8_t propertyBytes[0x4];
    alignas(8) uint8_t firstKeyBytes[0x4];
    alignas(8) uint8_t remainingKeyBytes[0x4];
    auto** property = reinterpret_cast<void**>(propertyBytes);

    NetGameDdl_ProcessProperty(self, context, 1, 68, property, firstKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 2, 80, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 3, 92, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 4, 95, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 5, 100, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 6, 696, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 7, 708, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 8, 714, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 9, 720, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 10, 726, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 11, 732, property, remainingKeyBytes);
    NetGameDdl_ProcessProperty(self, context, 12, 744, property, remainingKeyBytes);
    const char result =
        NetGameDdl_ProcessProperty(self, context, 13, 768, property, remainingKeyBytes);

    C_0085ec18_NullSub(reinterpret_cast<int>(context));
    return result;
}

HOOK(0x0074DD10, NetGameDdl_ProcessGameProperties, "auto");
