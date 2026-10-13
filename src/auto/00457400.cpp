#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, C_0082d410_AssignResourceString, 0x00429C60, (void* self, void* , const char* text));
CSF_FUNC(int, __fastcall, HudPortraits_FindDefinition, 0x00456EC0, (void* self, void* edx, char* a2));
CSF_FUNC(C_00831304*, __fastcall, C_00831304_InitializeDefaults, 0x00456FC0, (C_00831304* self, void*));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

static int __fastcall HudPortraits_CreatePortrait(
    void* self, void* /*edx*/, char* name, int option, int context)
{
    // These two packet-listed identities belong to the bound callees.
    (void)CSF_VTBL(0x00456EC0);
    (void)CSF_VTBL(0x0041E5F0);

    if (!name || !*name)
        return -1;

    int portraitId = -1;
    auto* definition = reinterpret_cast<std::uint8_t*>(
        HudPortraits_FindDefinition(self, nullptr, name));

    if (!definition)
        return portraitId;

    const std::uint32_t references = CSF_AT(std::uint32_t, definition, 0x48);
    if (!references || !CSF_AT(std::uint8_t, definition, 0x4C))
    {
        CSF_AT(std::uint32_t, definition, 0x48) = references + 1;

        auto* allocation = static_cast<C_00831304*>(Mem_Alloc(0x3C));
        if (allocation)
        {
            C_00831304* portrait =
                C_00831304_InitializeDefaults(allocation, nullptr);
            if (portrait)
            {
                void* accessor = reinterpret_cast<void*>(
                    GlobalInt_ResolveAccessor(
                        reinterpret_cast<void*>(GetGlobalInt()), nullptr));

                void* textureService =
                    reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, accessor, 0)[0x10 / 4])(
                            accessor, nullptr);

                reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                    CSF_AT(void**, textureService, 0)[0x10 / 4])(
                        textureService, nullptr,
                        CSF_STR(0x0083132C, "menus\\textures\\retratos\\"));

                void* controls = MenuControls_ResolveObject(
                    reinterpret_cast<void*>(GetGlobalInt()), nullptr);
                const char* textureName =
                    CSF_AT(std::uint32_t, definition, 0x3C) < 0x10u
                    ? reinterpret_cast<const char*>(definition + 0x28)
                    : CSF_AT(const char*, definition, 0x28);

                portrait->field_0024 =
                    reinterpret_cast<std::uint32_t(__fastcall*)(
                        void*, void*, const char*)>(
                            CSF_AT(void**, controls, 0)[0x38 / 4])(
                                controls, nullptr, textureName);

                controls = MenuControls_ResolveObject(
                    reinterpret_cast<void*>(GetGlobalInt()), nullptr);
                portrait->field_0028 =
                    reinterpret_cast<std::uint32_t(__fastcall*)(
                        void*, void*, const char*)>(
                            CSF_AT(void**, controls, 0)[0x38 / 4])(
                                controls, nullptr,
                                CSF_STR(0x00831320, "reTach.png"));

                portrait->field_002c =
                    reinterpret_cast<std::uint32_t>(definition);
                C_0082d410_AssignResourceString(
                    reinterpret_cast<std::uint8_t*>(portrait) + 8,
                    nullptr, name);

                portraitId = CSF_AT(int, self, 0x0C);
                CSF_AT(std::uint32_t, self, 0x0C) =
                    static_cast<std::uint32_t>(portraitId) + 1;
                portrait->field_0034 = context;
                void* overlay = reinterpret_cast<void*>(portrait->field_0028);
                portrait->field_0004 = portraitId;
                portrait->field_0038 = option;
                portrait->field_0030 = 0;

                if (overlay)
                {
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, overlay, 0)[0x70 / 4])(
                            overlay, nullptr, 1);
                }

                if (CSF_AT(std::uint32_t, self, 0x28) >=
                    CSF_AT(std::uint32_t, self, 0x24))
                {
                    S_0041e5f0_GrowArrayCapacity(
                        reinterpret_cast<S_0041e5f0*>(
                            static_cast<std::uint8_t*>(self) + 0x20),
                        nullptr, CSF_AT(int, self, 0x2C));
                }

                const std::uint32_t count =
                    CSF_AT(std::uint32_t, self, 0x28);
                void** destination = CSF_AT(void**, self, 0x20) + count;
                CSF_AT(std::uint32_t, self, 0x28) = count + 1;
                *destination = portrait;
            }
        }
    }

    if (CSF_AT(std::uint8_t, definition, 0x4C))
    {
        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x28);
        for (std::uint32_t index = 0; index < count; ++index)
        {
            auto* portrait = static_cast<C_00831304*>(
                CSF_AT(void**, self, 0x20)[index]);
            void* existingDefinition =
                reinterpret_cast<void*>(portrait->field_002c);
            if (CSF_AT(std::uint32_t, existingDefinition, 4) ==
                CSF_AT(std::uint32_t, definition, 4))
            {
                portraitId = static_cast<int>(portrait->field_0004);
                break;
            }
        }
    }

    return portraitId;
}

HOOK(0x00457400, HudPortraits_CreatePortrait, "auto");
