#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(unsigned int, __fastcall, ReserveObjectPointers, 0x0050CD00, (void* self, void* edx, unsigned int capacity));
CSF_FUNC(int, __cdecl, MenuMpStats_ResolveVideoPath, 0x00541580, (C_0082d410* self));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, CoreFiles_CheckScenarioFile, 0x00697720, (char* path));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall S_0050d0a0_ConstructFadeController(
    S_0050d0a0* self, void* /*edx*/)
{
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    auto* objectPointers = reinterpret_cast<std::uint8_t*>(self) + 8;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0021 = 0;
    self->field_0024 = 0;
    self->field_0028 = 1;
    self->field_0029 = 0;
    self->field_002a = 0;

    self->field_0004 = CSF_AT(
        std::uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x21D8);

    auto* settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
    alignas(8) std::uint8_t videoNameBytes[0x20];
    auto& videoName = *reinterpret_cast<C_0082d410*>(videoNameBytes);

    CSF_AT(std::uint32_t, videoNameBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, videoNameBytes, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&videoName.field_0004) = 0;
    AssignStringSubstring(&videoName, nullptr, settings + 0x21B8, 0,
                          static_cast<std::uint32_t>(-1));

    auto* resolvedPath = reinterpret_cast<std::uint8_t*>(
        MenuMpStats_ResolveVideoPath(&videoName));
    char* path;
    if (CSF_AT(std::uint32_t, resolvedPath, 0x18) < 0x10u)
        path = reinterpret_cast<char*>(resolvedPath + 4);
    else
        path = reinterpret_cast<char*>(
            CSF_AT(std::uint32_t, resolvedPath, 4));

    const char scenarioExists = CoreFiles_CheckScenarioFile(path);
    auto* currentSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    self->field_0001 =
        !CSF_AT(std::uint8_t, currentSettings, 0xA6C) && scenarioExists;

    if (self->field_000c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_000c));

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    ReserveObjectPointers(objectPointers, nullptr, 2);

    if (CSF_AT(std::uint32_t, videoNameBytes, 0x18) >= 0x10 &&
        videoName.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(videoName.field_0004));
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x0050D0A0, S_0050d0a0_ConstructFadeController, "auto");
