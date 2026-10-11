#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(unsigned int, __fastcall, AppMain_GrowAliasList, 0x00508030, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

// The packet also identifies the direct getter address as a vtable binding.
[[maybe_unused]] static const auto kMenuExtrasVtable = CSF_VTBL(0x00692B80);

static int __fastcall S_005080e0_LoadAliases(
    S_005080e0* self, void* /*edx*/, S_00692b80* node)
{
    alignas(8) std::uint8_t fileBytes[0x64];
    alignas(8) std::uint8_t aliasBytes[0x28];
    alignas(8) std::uint8_t entryBytes[0x30];
    char* file = reinterpret_cast<char*>(fileBytes);
    char* alias = reinterpret_cast<char*>(aliasBytes);
    C_0082d410& temporary = *reinterpret_cast<C_0082d410*>(entryBytes);

    bool allResolved = true;
    const std::uint32_t childCount = MenuExtras_GetChildCount(node, nullptr);

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, index));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(CSF_STR(0x0083F794, ".ALIAS")), alias, 40);
        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(CSF_STR(0x0083F79C, ".FILE")), file, 100);

        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* resolver = MenuControls_ResolveObject(global, nullptr);
        void* object =
            reinterpret_cast<void* (__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, resolver, 0)[0x4C / 4])(
                    resolver, nullptr, file);

        allResolved = object != nullptr;
        if (!allResolved)
            break;

        CSF_AT(std::uint32_t, &temporary, 0x18) = 15;
        CSF_AT(std::uint32_t, &temporary, 0x14) = 0;
        reinterpret_cast<std::uint8_t*>(&temporary.field_0004)[0] = 0;
        AssignStringRange_429B60(
            &temporary, nullptr, alias, static_cast<unsigned int>(std::strlen(alias)));

        CSF_AT(void*, &temporary, 0x1C) = object;
        const std::uint8_t state =
            reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr);
        CSF_AT(std::uint8_t, &temporary, 0x20) = state;

        if (CSF_AT(std::uint32_t, self, 0x4C) >=
            CSF_AT(std::uint32_t, self, 0x48))
        {
            AppMain_GrowAliasList(
                reinterpret_cast<std::uint8_t*>(self) + 0x44,
                nullptr, CSF_AT(int, self, 0x50));
        }

        const std::uint32_t entryIndex = CSF_AT(std::uint32_t, self, 0x4C);
        auto* entries = CSF_AT(std::uint8_t*, self, 0x44);
        CSF_AT(std::uint32_t, self, 0x4C) = entryIndex + 1;
        auto* entry = entries + 36 * entryIndex;

        AssignStringSubstring(entry, nullptr, &temporary, 0, -1);
        CSF_AT(void*, entry, 0x1C) = CSF_AT(void*, &temporary, 0x1C);
        CSF_AT(std::uint8_t, entry, 0x20) =
            CSF_AT(std::uint8_t, &temporary, 0x20);

        if (CSF_AT(std::uint32_t, &temporary, 0x18) >= 0x10 &&
            temporary.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(temporary.field_0004));
        }

        CSF_AT(std::uint32_t, &temporary, 0x18) = 15;
        CSF_AT(std::uint32_t, &temporary, 0x14) = 0;
        reinterpret_cast<std::uint8_t*>(&temporary.field_0004)[0] = 0;
    }

    if (allResolved)
    {
        std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x4C);
        std::uint32_t offset = 0;
        while (remaining)
        {
            auto* entries = CSF_AT(std::uint8_t*, self, 0x44);
            void* object = CSF_AT(void*, entries + offset, 0x1C);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr, 1);
            offset += 36;
            --remaining;
        }
    }

    if (CSF_AT(std::uint32_t, self, 0x4C))
    {
        auto* entries = CSF_AT(std::uint8_t*, self, 0x44);
        CSF_AT(void*, self, 0x1C) = CSF_AT(void*, entries, 0x1C);
    }

    return childCount && allResolved;
}

HOOK(0x005085F0, S_005080e0_LoadAliases, "auto");
