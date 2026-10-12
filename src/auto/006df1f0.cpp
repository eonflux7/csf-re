#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int*, __fastcall, GfxTextureLoader_TruncateEntries, 0x006DEED0,
         (void* self, void* edx, int* a2, int a3, int a4));
CSF_FUNC(char, __fastcall, GfxTextureLoader_GrowEntries, 0x006DEF50,
         (void* self, void* edx, void* a1, unsigned int a3, int a2));

static int __fastcall GfxTextureLoader_ResizeEntries(
    S_006deed0* self, void* /*edx*/, unsigned int count,
    int fillValue, void* block, int /*a5*/, int /*a6*/)
{
    // The packet also identifies this helper address as a vtable reference.
    (void)CSF_VTBL(0x006DEED0);

    const uint32_t begin = self->field_0004;
    uint32_t currentCount = 0;
    if (begin)
        currentCount = static_cast<uint32_t>(
            static_cast<int32_t>(self->field_0008 - begin) >> 4);

    if (currentCount >= count) {
        if (begin) {
            const uint32_t end = self->field_0008;
            const uint32_t checkedCount = static_cast<uint32_t>(
                static_cast<int32_t>(end - begin) >> 4);
            if (count < checkedCount) {
                GfxTextureLoader_TruncateEntries(
                    self, nullptr, reinterpret_cast<int*>(&count),
                    static_cast<int>(begin + 16 * count),
                    static_cast<int>(end));
            }
        }
    } else {
        uint32_t checkedCount = 0;
        if (begin)
            checkedCount = static_cast<uint32_t>(
                static_cast<int32_t>(self->field_0008 - begin) >> 4);

        GfxTextureLoader_GrowEntries(
            self, nullptr, reinterpret_cast<void*>(self->field_0008),
            count - checkedCount, reinterpret_cast<int>(&fillValue));
    }

    if (block)
        return Mem_FreeDebug(block);
    return 0;
}

HOOK(0x006DF1F0, GfxTextureLoader_ResizeEntries, "auto");
