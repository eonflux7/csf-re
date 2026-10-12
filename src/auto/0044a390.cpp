#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, S_0044a1e0_ConstructElement20, 0x004495B0,
         (uint32_t* a1, int a2, uint32_t* a3, int a4, int a5));

CSF_FUNC(uint32_t*, __fastcall, S_0044a1e0_InsertElement20, 0x0044A1E0,
         (void* self, void* edx, uint32_t* a2, char* a3, int* a4));

static uint32_t* __fastcall S_0044a1e0_AppendElement20(
    S_0044a1e0* self, void* /*edx*/, int* element)
{
    (void)CSF_VTBL(0x0044A1E0);

    const uint32_t begin = self->field_0004;
    uint32_t count = 0;
    if (begin != 0)
    {
        count = static_cast<uint32_t>(
            static_cast<int32_t>(self->field_0008 - begin) / 20);
    }

    if (begin == 0 ||
        count >= static_cast<uint32_t>(
                     static_cast<int32_t>(self->field_000c - begin) / 20))
    {
        return S_0044a1e0_InsertElement20(
            self, nullptr, reinterpret_cast<uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<uintptr_t>(self->field_0008)),
            element);
    }

    const uint32_t end = self->field_0008;
    uint32_t* result = S_0044a1e0_ConstructElement20(
        reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(end)),
        1, reinterpret_cast<uint32_t*>(element),
        static_cast<int>(reinterpret_cast<intptr_t>(self)),
        static_cast<int>(reinterpret_cast<intptr_t>(element)));
    self->field_0008 = end + 20;
    return result;
}

HOOK(0x0044A390, S_0044a1e0_AppendElement20, "auto");
