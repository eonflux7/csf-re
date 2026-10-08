#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);

CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, CMenuAccountEdit_ResetInnerObject, 0x00518890,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuAccounts_ClearChildTextsAndRefresh,
         0x0055C6A0, (CMenuAccountEdit* self, void* edx));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0,
         (char* destination, const char* source, std::size_t count));

static int __fastcall CMenuAccountEdit_LoadSelectedAccount(
    CMenuAccountEdit* self, void* /*edx*/, int /*unused*/)
{
    auto* bytes = reinterpret_cast<std::uint8_t*>(self);

    if (g_SelectedAccountIndex == -1)
    {
        CMenuAccountEdit_ResetInnerObject(bytes + 0x34, nullptr);
    }
    else
    {
        GetGlobalValue_8B056C();
        void* manager = GetGlobalValue_8B056C();
        void* accountStore = CSF_AT(void*, manager, 0x14);
        const int selectedIndex = g_SelectedAccountIndex;

        auto getAccount = reinterpret_cast<
            const void* (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, accountStore, 0)[0x30 / 4]);
        const void* account = getAccount(accountStore, nullptr, selectedIndex);

        auto* destination = reinterpret_cast<std::uint32_t*>(bytes + 0x34);
        auto* source = static_cast<const std::uint32_t*>(account);
        for (std::size_t i = 0; i < 0x93; ++i)
            destination[i] = source[i];

        _strncpy(reinterpret_cast<char*>(bytes + 0x28C),
                        reinterpret_cast<const char*>(bytes + 0x5D), 0x14);
    }

    const int result = MenuAccounts_ClearChildTextsAndRefresh(self, nullptr);
    self->field_0280 = 1;
    return result;
}

HOOK(0x0055C830, CMenuAccountEdit_LoadSelectedAccount, "auto");
