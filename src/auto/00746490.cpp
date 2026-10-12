#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, NetPlayerDdl_ApplyProperty, 0x0066CD80, (void* self, void* edx, int arg0, int a3, int a4, int a5, void* a6));
CSF_FUNC(void, __fastcall, NetPlayerDdl_ApplyBaseProperties, 0x007A38E0, (void* self, void* edx, void* a2, int a3));

static void C_008622e8_ApplyMemberProperty(
    C_008622e8* self, C_008622e8* other, int mode,
    uint8_t propertyId, std::size_t memberOffset)
{
    alignas(8) uint8_t propertyBytes[4];
    alignas(8) uint8_t propertyIdBytes[4];

    const int objectIndex = static_cast<int>(
        CSF_AT(uint32_t, self, 0x2C) >> 22);
    propertyIdBytes[0] = propertyId;

    const int object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0x0C,
        nullptr,
        reinterpret_cast<void**>(propertyBytes),
        propertyIdBytes);

    void* property = *reinterpret_cast<void**>(propertyBytes);
    void* handler = CSF_AT(void*, property, 0x10);
    NetPlayerDdl_ApplyProperty(
        handler, nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + memberOffset),
        propertyId, mode, other);
}

static void __fastcall C_008622e8_ApplyProperties(
    C_008622e8* self, void* /*edx*/, C_008622e8* other, int mode)
{
    C_008622e8_ApplyMemberProperty(self, other, mode, 4, 1088);
    C_008622e8_ApplyMemberProperty(self, other, mode, 5, 1091);
    C_008622e8_ApplyMemberProperty(self, other, mode, 6, 1094);
    C_008622e8_ApplyMemberProperty(self, other, mode, 7, 1100);
    C_008622e8_ApplyMemberProperty(self, other, mode, 8, 1103);
    C_008622e8_ApplyMemberProperty(self, other, mode, 9, 1106);
    C_008622e8_ApplyMemberProperty(self, other, mode, 10, 1112);
    C_008622e8_ApplyMemberProperty(self, other, mode, 11, 1640);
    C_008622e8_ApplyMemberProperty(self, other, mode, 12, 1652);
    C_008622e8_ApplyMemberProperty(self, other, mode, 13, 1664);
    C_008622e8_ApplyMemberProperty(self, other, mode, 14, 1670);
    C_008622e8_ApplyMemberProperty(self, other, mode, 15, 1676);
    C_008622e8_ApplyMemberProperty(self, other, mode, 16, 1682);
    C_008622e8_ApplyMemberProperty(self, other, mode, 17, 1688);
    C_008622e8_ApplyMemberProperty(self, other, mode, 18, 1694);
    C_008622e8_ApplyMemberProperty(self, other, mode, 19, 1700);
    C_008622e8_ApplyMemberProperty(self, other, mode, 20, 1704);

    NetPlayerDdl_ApplyBaseProperties(self, nullptr, other, mode);
}

HOOK(0x00746490, C_008622e8_ApplyProperties, "auto");
