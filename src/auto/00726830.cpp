#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, InitializeResourceWrapper, 0x00737A70,
         (void* self, void* edx, C_008655e8* owner));

static int __fastcall C_008655e8_CreateResourceWrapper(
    C_008655e8* self, void* /*edx*/, int resourceKey)
{
    const int resource =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0x80 / 4])(
                self, nullptr, resourceKey);

    if (!resource)
        return 0;

    void* memory = Mem_Alloc(0x90);
    void* wrapper = nullptr;
    if (memory)
        wrapper = InitializeResourceWrapper(memory, nullptr, self);

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, wrapper, 0)[0x20 / 4])(
            wrapper, nullptr, resource);

    return reinterpret_cast<int>(wrapper);
}

HOOK(0x00726830, C_008655e8_CreateResourceWrapper, "auto");
