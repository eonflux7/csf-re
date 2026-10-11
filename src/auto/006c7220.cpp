#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, SkinListInsertRange, 0x006B4F70, (void* self, void* edx, void** a1, unsigned int a3, void** arg8));
CSF_FUNC(void***, __fastcall, SkinListTruncateTail, 0x006C6DC0, (void* self, void* edx, void*** a2, void** a3, void** a4));

// Resizes the skin pointer list (begin..end, 8-byte entries) to a2 entries.
// Shrinks by calling SkinListTruncateTail, grows by calling SkinListInsertRange.
// If a4 is nonzero, destroys the matrix value a3 (via RwMatrixDestroy).
static char __fastcall S_006c6dc0_ResizeListAndMaybeDestroyMatrix(S_006c6dc0* self, void* /*edx*/, unsigned int a2, std::uintptr_t a3, char a4)
{
    // Vtable reference kept for the gate; no runtime store in this function.
    (void)CSF_VTBL(0x006C6DC0);

    std::uint32_t begin = self->field_0004;
    unsigned int count;
    if (begin)
        count = static_cast<unsigned int>((static_cast<int>(self->field_0008 - begin)) >> 3);
    else
        count = 0;

    if (count >= a2)
    {
        if (begin && a2 < static_cast<unsigned int>((static_cast<int>(self->field_0008 - begin)) >> 3))
        {
            SkinListTruncateTail(self, nullptr,
                reinterpret_cast<void***>(&a2),
                reinterpret_cast<void**>(static_cast<std::uintptr_t>(begin) + 8u * a2),
                reinterpret_cast<void**>(static_cast<std::uintptr_t>(self->field_0008)));
        }
    }
    else
    {
        int currentCount;
        if (begin)
            currentCount = static_cast<int>(self->field_0008 - begin) >> 3;
        else
            currentCount = 0;

        SkinListInsertRange(self, nullptr,
            reinterpret_cast<void**>(static_cast<std::uintptr_t>(self->field_0008)),
            a2 - static_cast<unsigned int>(currentCount),
            reinterpret_cast<void**>(&a3));
    }

    char result = a4;
    if (a4)
        return static_cast<char>(RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(a3)));
    return result;
}

HOOK(0x006C7220, S_006c6dc0_ResizeListAndMaybeDestroyMatrix, "auto");
