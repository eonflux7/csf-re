#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, DestroyElementRange, 0x006DEC40,
         (int* begin, int* end, ...));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int* __fastcall CGuiTextureListBox_ClearBaseStorage(
    CGuiTextureListBox* self, void* /*edx*/)
{
    int* result = reinterpret_cast<int*>(self->field_018c);
    auto* storage = reinterpret_cast<std::uint8_t*>(self) + 0x188;

    if (result != nullptr) {
        DestroyElementRange(
            result,
            reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x190)),
            storage, self);

        result = reinterpret_cast<int*>(self->field_018c);
        if (result != nullptr) {
            result = reinterpret_cast<int*>(
                static_cast<std::uintptr_t>(Mem_FreeDebug(result)));
        }
    }

    self->field_018c = 0;
    CSF_AT(std::uint32_t, self, 0x190) = 0;
    CSF_AT(std::uint32_t, self, 0x194) = 0;
    return result;
}

HOOK(0x007242D0, CGuiTextureListBox_ClearBaseStorage, "auto");
