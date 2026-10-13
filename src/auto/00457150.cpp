#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudPortraits_FindDefinition, 0x00456EC0, (void* self, void* edx, char* a2));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeNameList, 0x004E8060, (S_0041e5f0* a1, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

// The packet also labels the lookup function's address as a vtable reference.
static void** const g_PortraitLookupVtableAnnotation = CSF_VTBL(0x00456EC0);

static int __fastcall HudPortraits_Serialize(void* self, void* /*edx*/, void* state)
{
    auto* bytes = static_cast<std::uint8_t*>(self);

    bool failed = false;
    if (!CSF_AT(std::uint8_t, state, 0))
    {
        void* stream = CSF_AT(void*, state, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, state);
    }

    const std::uint32_t savedContext = CSF_AT(std::uint32_t, state, 0x1C);
    CSF_AT(std::uint8_t, state, 0) = failed;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;

    if (!failed)
    {
        void* stream = CSF_AT(void*, state, 4);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, bytes + 8, state) == -1;
    }

    bool active = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (active)
    {
        void* stream = CSF_AT(void*, state, 4);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, bytes + 4, state) == -1;
    }

    if (!CSF_AT(std::uint8_t, state, 0x14))
    {
        for (std::uint32_t index = 0;
             index < CSF_AT(std::uint32_t, self, 0x14); ++index)
        {
            void* definition = CSF_AT(void**, self, 0xC)[index];
            CSF_AT(std::uint32_t, definition, 0x48) = 0;
        }
    }

    alignas(8) std::uint8_t countBytes[0x4];
    auto& serializedCount = *reinterpret_cast<std::uint32_t*>(countBytes);
    serializedCount = CSF_AT(std::uint32_t, self, 0x24);

    active = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (active)
    {
        void* stream = CSF_AT(void*, state, 4);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, countBytes, state) == -1;
    }

    if (!CSF_AT(std::uint8_t, state, 0x14))
    {
        for (std::uint32_t index = 0; index < serializedCount; ++index)
        {
            auto* portrait = static_cast<C_00831304*>(Mem_Alloc(60));
            if (portrait)
            {
                portrait->vtbl = CSF_VTBL(0x00831304);
                portrait->field_0004 = -1;
                portrait->field_0020 = 15;
                portrait->field_001c = 0;
                portrait->field_000c = 0;
                portrait->field_0024 = 0;
                portrait->field_0028 = 0;
                portrait->field_002c = 0;
                portrait->field_0030 = 0;
                portrait->field_0034 = 0;
                portrait->field_0038 = 0;
            }

            const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x20);
            if (CSF_AT(std::uint32_t, self, 0x24) >= capacity)
            {
                const std::uint32_t growth = CSF_AT(std::uint32_t, self, 0x28);
                void* replacement = Mem_Alloc(4 * (growth + capacity));
                const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x24);
                if (count)
                    std::memcpy(replacement, CSF_AT(void*, self, 0x1C), 4 * count);

                CSF_AT(std::uint32_t, self, 0x20) += growth;
                if (CSF_AT(void*, self, 0x1C))
                {
                    Mem_Free(CSF_AT(void*, self, 0x1C));
                    CSF_AT(void*, self, 0x1C) = nullptr;
                }
                CSF_AT(void*, self, 0x1C) = replacement;
            }

            const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x24);
            C_00831304** destination =
                CSF_AT(C_00831304**, self, 0x1C) + count;
            CSF_AT(std::uint32_t, self, 0x24) = count + 1;
            *destination = portrait;
        }
    }

    active = CSF_AT(std::uint8_t, state, 0) == 0;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (active)
    {
        CSF_AT(std::uint8_t, state, 0x15) = 1;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        Scene_SerializeNameList(
            reinterpret_cast<S_0041e5f0*>(bytes + 0x1C),
            reinterpret_cast<int>(state));
    }

    if (!CSF_AT(std::uint8_t, state, 0x14))
    {
        for (std::uint32_t index = 0;
             index < CSF_AT(std::uint32_t, self, 0x24); ++index)
        {
            C_00831304* portrait = CSF_AT(C_00831304**, self, 0x1C)[index];
            char* name = portrait->field_0020 < 0x10u
                ? reinterpret_cast<char*>(&portrait->field_000c)
                : CSF_AT(char*, portrait, 0xC);

            void* definition = reinterpret_cast<void*>(
                HudPortraits_FindDefinition(bytes - 4, nullptr, name));
            ++CSF_AT(std::uint32_t, definition, 0x48);
            portrait->field_002c = reinterpret_cast<std::uint32_t>(definition);

            void* globals = reinterpret_cast<void*>(GetGlobalInt());
            void* accessor = reinterpret_cast<void*>(
                GlobalInt_ResolveAccessor(globals, nullptr));
            void* texturePaths =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, texturePaths, 0)[0x10 / 4])(
                    texturePaths, nullptr,
                    CSF_STR(0x0083132C, "menus\\textures\\retratos\\"));

            globals = reinterpret_cast<void*>(GetGlobalInt());
            void* textures = MenuControls_ResolveObject(globals, nullptr);
            const char* textureName =
                CSF_AT(std::uint32_t, definition, 0x3C) < 0x10u
                ? reinterpret_cast<const char*>(definition) + 0x28
                : CSF_AT(const char*, definition, 0x28);
            portrait->field_0024 =
                reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
                    CSF_AT(void**, textures, 0)[0x38 / 4])(
                        textures, nullptr, textureName);

            globals = reinterpret_cast<void*>(GetGlobalInt());
            textures = MenuControls_ResolveObject(globals, nullptr);
            portrait->field_0028 =
                reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
                    CSF_AT(void**, textures, 0)[0x38 / 4])(
                        textures, nullptr, CSF_STR(0x00831320, "reTach.png"));
        }
    }

    CSF_AT(std::uint32_t, state, 0x1C) = savedContext;
    if (!CSF_AT(std::uint8_t, state, 0))
    {
        void* stream = CSF_AT(void*, state, 4);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, state))
        {
            CSF_AT(std::uint8_t, state, 0) = 0;
            return 0;
        }
    }

    CSF_AT(std::uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x00457150, HudPortraits_Serialize, "auto");
