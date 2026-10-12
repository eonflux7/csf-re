#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, EraseTailRange, 0x006959B0,
    (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4));
static CSF_FUNC(char, __fastcall, AppendFillElements, 0x00696410,
    (void* self, void* edx, uint32_t* a1, unsigned int a3, uint32_t* arg8));

static char __fastcall S_006959b0_Resize(
    S_006959b0* self, void* /*edx*/, unsigned int a2, int a3)
{
    // The packet also labels the direct callee address as a vtable reference.
    (void)CSF_VTBL(0x006959B0);

    const uint32_t begin = self->field_0004;
    unsigned int size = begin
        ? static_cast<int32_t>(self->field_0008 - begin) >> 2
        : 0;

    if (size >= a2)
    {
        if (begin)
        {
            size = static_cast<int32_t>(self->field_0008 - begin) >> 2;
            if (a2 < size)
            {
                uint32_t* result = EraseTailRange(
                    self, nullptr, &a2,
                    reinterpret_cast<uint32_t*>(begin + 4 * a2),
                    reinterpret_cast<uint32_t*>(self->field_0008));
                return static_cast<char>(reinterpret_cast<uint32_t>(result));
            }
        }
        return static_cast<char>(size);
    }

    const unsigned int currentSize = begin
        ? static_cast<int32_t>(self->field_0008 - begin) >> 2
        : 0;
    return AppendFillElements(
        self, nullptr,
        reinterpret_cast<uint32_t*>(self->field_0008),
        a2 - currentSize,
        reinterpret_cast<uint32_t*>(&a3));
}

HOOK(0x00696670, S_006959b0_Resize, "auto");
