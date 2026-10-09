#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_FUNC(S_00768e80*, __fastcall, sub_7693C0, 0x007693C0,
         (S_00768e80* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7,
          void* v10a, void* v10b, void* v10c, void* v10d, void* v10e));

CSF_FUNC(char, __fastcall, sub_76A140, 0x0076A140, (void* self, void* edx, uint32_t* a3));

static char __fastcall InputBindings_AllocAndStoreBinding(S_0076a140* self, void* /*edx*/,
                                                          int a2, int a3, int a4, int a5, int a6,
                                                          int* a7)
{
    int32_t v10[2];
    v10[0] = 0;
    v10[1] = static_cast<int32_t>(CSF_ADDR_DATA(0x0086F164));

    S_00768e80* binding = static_cast<S_00768e80*>(Mem_Alloc(32));
    if (binding)
    {
        binding = sub_7693C0(binding, nullptr, a2, a3, a4, a5, a6,
                             static_cast<int>(reinterpret_cast<intptr_t>(a7)),
                             v10, v10, v10, v10, v10);
        a7 = reinterpret_cast<int*>(binding);
        return sub_76A140(self, nullptr, reinterpret_cast<uint32_t*>(&a7));
    }

    a7 = nullptr;
    return sub_76A140(self, nullptr, reinterpret_cast<uint32_t*>(&a7));
}

HOOK(0x0076A1B0, InputBindings_AllocAndStoreBinding, "auto");
