#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, GetToggleTimerRotation, 0x004A1200, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_004a1210_IsToggleTimerActive, 0x004A1210, (S_004a1210* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_MinimumRotation, 0x008305EC);
CSF_GLOBAL(float, g_MaximumRotation, 0x008305F0);

// Separate fixed-slot calls ensure each dispatch reloads the current vtable.
static __declspec(noinline) void __fastcall CharacterModel_SetFirstRotation(
    void* character, void* /*edx*/, int rotation)
{
    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, character, 0)[0x40 / 4])(
            character, nullptr, rotation);
}

static __declspec(noinline) int __fastcall CharacterModel_SetPitch(
    void* character, void* /*edx*/, float pitch)
{
    return ((int (__fastcall*)(void*, void*, float))
        CSF_AT(void**, character, 0)[0x3C / 4])(
            character, nullptr, pitch);
}

static int __fastcall CharacterModel_UpdateSpecialCharacter(
    void* self, void* /*edx*/, int /*unused*/)
{
    (void)CSF_VTBL(0x004A1210);

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(firstElement), nullptr);
    void* referencedObject = GetReferencedObject(provider, nullptr);

    ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
            referencedObject, nullptr);

    int result = CSF_AT(int, self, 0x2E0);
    if ((result & 0x2000) == 0)
        return result;

    auto* timer = reinterpret_cast<S_004a1210*>(
        static_cast<std::uint8_t*>(self) + 0x344);
    result = S_004a1210_IsToggleTimerActive(timer, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    char* rotation = GetToggleTimerRotation(timer, nullptr);
    int firstRotationWord = CSF_AT(int, rotation, 0);
    float pitch = CSF_AT(float, rotation, 4);

    globalValue = GetGlobalValue_4FBDE0();
    firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(firstElement), nullptr);
    referencedObject = GetReferencedObject(provider, nullptr);

    void* character = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
            referencedObject, nullptr);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, character, 0)[0x58 / 4])(
            character, nullptr, static_cast<std::uint8_t*>(self) + 0x3A8);

    if (pitch > g_MaximumRotation)
        pitch = 89.0f;
    else if (pitch < g_MinimumRotation)
        pitch = -89.0f;

    CharacterModel_SetFirstRotation(character, nullptr, firstRotationWord);
    return CharacterModel_SetPitch(character, nullptr, pitch);
}

HOOK(0x004958C0, CharacterModel_UpdateSpecialCharacter, "auto");
