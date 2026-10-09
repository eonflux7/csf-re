#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static unsigned int __fastcall CPlayerCharacter_CreateSceneObject(
    CPlayerCharacter* self, void* /*edx*/, int configuration)
{
    auto* source = reinterpret_cast<std::uint8_t*>(configuration);

    const void* thirdName;
    if (CSF_AT(std::uint32_t, source, 0xA8) < 0x10u)
        thirdName = source + 0x94;
    else
        thirdName = CSF_AT(void*, source, 0x94);

    const void* secondName;
    if (CSF_AT(std::uint32_t, source, 0x8C) < 0x10u)
        secondName = source + 0x78;
    else
        secondName = CSF_AT(void*, source, 0x78);

    const void* firstName;
    if (CSF_AT(std::uint32_t, source, 0x54) < 0x10u)
        firstName = source + 0x40;
    else
        firstName = CSF_AT(void*, source, 0x40);

    alignas(4) std::uint8_t request[0x18] = {};
    CSF_AT(const void*, request, 0x00) = firstName;
    const int firstParameter = CSF_AT(int, source, 0xAC);
    CSF_AT(const void*, request, 0x08) = secondName;
    const int secondParameter = CSF_AT(int, source, 0xB0);
    CSF_AT(std::uint8_t, request, 0x04) = 1;
    CSF_AT(const void*, request, 0x0C) = thirdName;
    CSF_AT(int, request, 0x10) = firstParameter;
    CSF_AT(int, request, 0x14) = secondParameter;

    void* globals = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(globals, nullptr));
    unsigned int result =
        reinterpret_cast<unsigned int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, controller, 0)[20 / 4])(
                controller, nullptr, request);
    self->field_039c = result;
    if (!result)
        return result;

    // Contiguous options object, including its embedded storage and string.
    alignas(4) std::uint8_t options[0xB8];
    InitializeSceneObjectOptions(options, nullptr);

    std::uint32_t flags = CSF_AT(std::uint32_t, options, 0);
    if (CSF_AT(std::uint8_t, source, 0x159))
        flags |= 0x405;
    else
        flags = (flags & 0xFFFFFBFA) | 5;

    if (CSF_AT(std::uint8_t, source, 0x15A))
        flags &= 0xFFFFFFDF;
    else
        flags |= 0x20;

    if (CSF_AT(std::uint8_t, source, 0x15B))
        flags |= 0x4000;
    else
        flags &= 0xFFFFBFFF;

    const int parameter = CSF_AT(int, source, 0xCC);
    CSF_AT(std::uint32_t, options, 0) = flags;
    CSF_AT(int, options, 0x6C) = CSF_AT(int, source, 0x160);
    CSF_AT(int, options, 0x68) = parameter;
    const int lastParameter = CSF_AT(int, source, 0x168);
    CSF_AT(int, options, 0x70) = CSF_AT(int, source, 0x164);
    CSF_AT(int, options, 0x74) = lastParameter;

    auto* optionString = reinterpret_cast<C_0082d410*>(options + 0x78);
    AssignStringSubstring(optionString, nullptr, source + 0x16C, 0, -1);

    const int nextParameter = CSF_AT(int, source, 0x18C);
    CSF_AT(std::uint32_t, options, 0x94) =
        CSF_AT(std::uint32_t, source, 0x188) - 1;
    const int followingParameter = CSF_AT(int, source, 0x190);
    const int finalParameter = CSF_AT(int, source, 0x194);
    CSF_AT(int, options, 0x98) = nextParameter;
    const int additionalParameter = CSF_AT(int, source, 0xD0);
    CSF_AT(int, options, 0xA0) = finalParameter;
    CSF_AT(int, options, 0xA4) = additionalParameter;

    void* sceneObject = reinterpret_cast<void*>(self->field_039c);
    CSF_AT(int, options, 0x9C) = followingParameter;
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, sceneObject, 0)[16 / 4])(
            sceneObject, nullptr, options);

    const bool inlineString = CSF_AT(std::uint32_t, options, 0x90) < 0x10;
    self->field_03a0 = CSF_AT(std::uint32_t, source, 0);
    if (!inlineString && optionString->field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(optionString->field_0004));

    CSF_AT(std::uint32_t, options, 0x90) = 15;
    CSF_AT(std::uint32_t, options, 0x8C) = 0;
    optionString->field_0004 &= 0xFFFFFF00;

    return DestroySceneObjectOptionsStorage(options + 0x04, nullptr);
}

HOOK(0x0049B030, CPlayerCharacter_CreateSceneObject, "auto");
