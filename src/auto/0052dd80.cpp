#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, AppendStringSubstring, 0x0043EA30, (void* self, void* edx, uint32_t* a2, unsigned int a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __stdcall, TryPlaySoundInManager, 0x00521EB0, (int soundId));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(std::uint32_t, g_BriefingExtensionWord, 0x00830FA0);

static void __fastcall CMenuScenarioSelect_UpdateAnimations(
    CMenuScenarioSelect* self, void* /*edx*/, float elapsedTime)
{
    if (!self->field_0040)
        return;

    self->field_0094 = elapsedTime + self->field_0094;

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    // The original comparison also proceeds when either operand is NaN.
    if (self->field_0094 < CSF_AT(float, settings, 0x2284))
        return;

    self->field_0094 = 0.0f;

    alignas(8) std::uint8_t pathBytes[0x1C];
    CSF_AT(std::uint32_t, pathBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, pathBytes, 0x14) = 0;
    CSF_AT(std::uint8_t, pathBytes, 0x04) = 0;

    AssignStringRange_429B60(
        pathBytes, nullptr,
        const_cast<char*>(CSF_STR(0x00830FA8, "Sounds\\Scenes\\Briefings\\")),
        24);

    auto* briefingString =
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x78);

    AppendStringSubstring(pathBytes, nullptr, briefingString, 0, -1);
    Profiles_AppendStringRange(pathBytes, 0, &g_BriefingExtensionWord, 4);
    AssignStringSubstring(briefingString, nullptr, pathBytes, 0, -1);

    void* soundName;
    if (CSF_AT(std::uint32_t, self, 0x90) < 0x10)
        soundName = reinterpret_cast<std::uint8_t*>(self) + 0x7C;
    else
        soundName = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x7C));

    GetGlobalValue_521BB0();
    TryPlaySoundInManager(reinterpret_cast<int>(soundName));

    const bool inlinePath = CSF_AT(std::uint32_t, pathBytes, 0x18) < 0x10;
    self->field_0040 = 0;

    if (!inlinePath)
    {
        void* allocation = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, pathBytes, 0x04));
        if (allocation)
            Mem_FreeDebug(allocation);
    }
}

HOOK(0x0052DD80, CMenuScenarioSelect_UpdateAnimations, "auto");
