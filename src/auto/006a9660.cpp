#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_006a8940_UpdateGathering, 0x006A9270, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0085ff6c_ConstructGatheringBase, 0x00798DF0, (C_0085ff4c* self, void* edx));
CSF_FUNC(int, __fastcall, Gathering_SetType, 0x00798E60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void**, __fastcall, Gathering_SetName, 0x00798E80, (void* self, void* edx, void** a2));

static void __fastcall S_006a8940_CreateGameGathering(
    S_006a8940* self, void* /*edx*/,
    std::int32_t nameSource,
    std::uint32_t options,
    std::uint32_t reserved,
    char firstSetting,
    char secondSetting,
    const char* firstText,
    const char* secondText,
    int gatheringValue)
{
    reinterpret_cast<void (__fastcall*)(S_006a8940*, void*)>(
        reinterpret_cast<void**>(self->field_0000)[9])(self, nullptr);

    self->field_0008 = 3;
    self->field_000c = 2;
    self->field_0010 = 0;

    void* gathering = NetZ_OperatorNew(600);
    if (gathering)
    {
        C_0085ff6c_ConstructGatheringBase(
            reinterpret_cast<C_0085ff4c*>(gathering), nullptr);
        CSF_AT(void**, gathering, 0) = CSF_VTBL(0x0085FF6C);
    }

    self->field_0080 = reinterpret_cast<std::uint32_t>(gathering);

    // The original reuses its twelve-byte by-value argument as string storage.
    alignas(8) std::uint8_t temporaryBytes[12];
    std::memcpy(temporaryBytes, &nameSource, 4);
    std::memcpy(temporaryBytes + 4, &options, 4);
    std::memcpy(temporaryBytes + 8, &reserved, 4);
    C_008310b8& temporary =
        *reinterpret_cast<C_008310b8*>(temporaryBytes);

    InitializeTemporaryString(temporaryBytes, nullptr, nameSource);
    Gathering_SetName(reinterpret_cast<void*>(self->field_0080), nullptr,
                      reinterpret_cast<void**>(temporaryBytes));
    DestroyTemporaryString(temporaryBytes, nullptr);

    CSF_AT(std::uint16_t, reinterpret_cast<void*>(self->field_0080), 0x10) = 0;
    const std::uint8_t optionByte =
        static_cast<std::uint8_t>(temporary.field_0004);
    CSF_AT(std::uint16_t, reinterpret_cast<void*>(self->field_0080), 0x12) = 0;
    CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0080), 0x30) = optionByte;
    CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0080), 0x31) = 0;
    CSF_AT(char, reinterpret_cast<void*>(self->field_0080), 0x32) = firstSetting;
    CSF_AT(char, reinterpret_cast<void*>(self->field_0080), 0x33) = secondSetting;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0080), 0x254) = gatheringValue;

    char* destination = reinterpret_cast<char*>(self->field_0080) + 0x34;
    char character;
    do
    {
        character = *firstText++;
        *destination++ = character;
    } while (character != 0);

    destination = reinterpret_cast<char*>(self->field_0080) + 0x54;
    do
    {
        character = *secondText++;
        *destination++ = character;
    } while (character != 0);

    CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0080), 0x1C) |= 0x10u;
    Gathering_SetType(reinterpret_cast<void*>(self->field_0080), nullptr, 7, 0);
    S_006a8940_UpdateGathering(self, nullptr);
}

HOOK(0x006A9660, S_006a8940_CreateGameGathering, "auto");
