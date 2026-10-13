#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(int, __fastcall, sub_69B750, 0x0069B750,
         (void* self, void* edx, unsigned int a2, unsigned int a3));
CSF_FUNC(int, __fastcall, sub_69B7B0, 0x0069B7B0,
         (void* self, void* edx, unsigned int a2, unsigned int arg4, uint8_t* a3));

static int __cdecl CoreParser_ResolveEntryByKey(unsigned int key, int flags)
{
    const unsigned int directCategories[] = {1, 2, 4, 8};
    for (unsigned int category : directCategories)
    {
        if ((flags & category) == 0)
            continue;

        uint8_t found = 0;
        void* table = reinterpret_cast<void*>(GetGlobalValue_69B700());
        int result = sub_69B7B0(table, nullptr, key, category, &found);
        if (found && result)
            return result;
    }

    const unsigned int remainingCategories[] = {16, 32, 64, 128, 256, 512};
    for (unsigned int category : remainingCategories)
    {
        hook::Regs registers{};
        registers.eax = category;
        registers.ecx = static_cast<uint32_t>(flags);
        registers.edx = key;
        registers.ebx = static_cast<uint32_t>(flags);
        registers.esi = key;
        int result = static_cast<int>(
            hook::call_regs(CSF_ADDR_FN(0x00692410), &registers, nullptr, 0));
        if (result)
            return result;
    }

    void* table = reinterpret_cast<void*>(GetGlobalValue_69B700());
    return sub_69B750(table, nullptr, key, 1);
}

HOOK(0x00692440, CoreParser_ResolveEntryByKey, "auto");
