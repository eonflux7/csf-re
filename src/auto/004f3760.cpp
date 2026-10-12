#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ConstructElementAtEnd, 0x004F2C00, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, GrowAndEmplaceElement, 0x004F36F0, (void* self, void* edx, uint32_t* a2, void* a3, int a4));

static uint32_t* __fastcall S_004f36f0_EmplaceBack(
    S_004f36f0* self, void* /*edx*/, int value)
{
    uint32_t begin = self->field_0004;
    unsigned int index = 0;
    if (begin != 0)
        index = static_cast<unsigned int>(
            static_cast<int>(self->field_0008 - begin) / 124);

    if (begin == 0 ||
        index >= static_cast<unsigned int>(
            static_cast<int>(self->field_000c - begin) / 124))
    {
        (void)CSF_VTBL(0x004F36F0);
        return GrowAndEmplaceElement(
            self, nullptr, reinterpret_cast<uint32_t*>(&value),
            reinterpret_cast<void*>(self->field_0008), value);
    }

    uint32_t end = self->field_0008;
    uint32_t* result = reinterpret_cast<uint32_t*>(
        ConstructElementAtEnd(
            static_cast<int>(end), 1, value,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), value));
    self->field_0008 = end + 124;
    return result;
}

HOOK(0x004F3760, S_004f36f0_EmplaceBack, "auto");
