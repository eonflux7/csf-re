#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0,
         (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, CScene_ProcessEntryName, 0x00452800,
         (void* self, void* edx, int a2, int a3));

CSF_GLOBAL(uint8_t, g_SceneLoaderMainScenePresent, 0x008AFC8C);

[[maybe_unused]] static const auto g_ButtonArrayVtable =
    CSF_VTBL(0x0041E5F0);

static int __fastcall CScene_AddSceneEntry(
    CScene* self, void* /*edx*/, int entryAddress)
{
    auto* entry = reinterpret_cast<uint8_t*>(entryAddress);
    const int previousIndex = CSF_AT(int, entry, 0x24);

    if (previousIndex == -1 || g_SceneLoaderMainScenePresent)
    {
        uint32_t index = 0;
        bool inserted = false;

        if (CSF_AT(uint32_t, self, 0x18) != 0)
        {
            do
            {
                if (inserted)
                    break;

                auto* entries = reinterpret_cast<uint32_t*>(
                    CSF_AT(uint32_t, self, 0x10));

                if (entries[index] != 0)
                {
                    ++index;
                }
                else
                {
                    entries[index] = static_cast<uint32_t>(entryAddress);
                    inserted = true;
                    CSF_AT(uint32_t, entry, 0x24) = index;
                }
            }
            while (index < CSF_AT(uint32_t, self, 0x18));
        }

        if (!inserted)
        {
            if (CSF_AT(uint32_t, self, 0x18) >=
                CSF_AT(uint32_t, self, 0x14))
            {
                GrowButtonArray(
                    reinterpret_cast<uint8_t*>(self) + 0x10,
                    nullptr, CSF_AT(int, self, 0x1C));
            }

            const uint32_t count = CSF_AT(uint32_t, self, 0x18);
            auto* slot = reinterpret_cast<uint32_t*>(
                CSF_AT(uint32_t, self, 0x10)) + count;

            CSF_AT(uint32_t, self, 0x18) = count + 1;
            *slot = static_cast<uint32_t>(entryAddress);
            CSF_AT(uint32_t, entry, 0x24) =
                CSF_AT(uint32_t, self, 0x18) - 1;
        }
    }
    else
    {
        auto* entries = reinterpret_cast<uint32_t*>(
            CSF_AT(uint32_t, self, 0x10));
        entries[previousIndex] = static_cast<uint32_t>(entryAddress);
    }

    CSF_AT(int, self, 0x70) = CSF_AT(int, entry, 0x24);
    return CScene_ProcessEntryName(
        self, nullptr, reinterpret_cast<int>(entry + 0x1C), 5);
}

HOOK(0x00453150, CScene_AddSceneEntry, "auto");
