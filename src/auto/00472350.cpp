#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionScript_ReleaseBlock, 0x004714E0, (S_0069b1c0* Block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static unsigned int __fastcall S_00472350_RemoveSlot(
    S_00472350* self, void* /*edx*/, unsigned int index)
{
    const uint32_t begin = self->field_0010;
    if (!begin)
        return 0;

    unsigned int result =
        static_cast<uint32_t>(static_cast<int32_t>(self->field_0014 - begin) >> 2);
    if (index < result)
    {
        const uint32_t offset = 4 * index;
        auto* block = CSF_AT(S_0069b1c0*,
                            reinterpret_cast<void*>(self->field_0010 + offset), 0);
        if (block)
        {
            reinterpret_cast<void (__fastcall*)(S_0069b1c0*, void*, uint32_t)>(
                CSF_AT(void**, block, 0)[0])(block, nullptr, 0);
            MissionScript_ReleaseBlock(block);
        }

        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010 + offset), 0) = 0;

        const uint32_t destination = self->field_0010 + offset;
        const uint32_t source = destination + 4;
        const uint32_t byteCount = 4 * static_cast<uint32_t>(
            static_cast<int32_t>(self->field_0014 - source) >> 2);
        _memcpy(reinterpret_cast<void*>(destination),
                reinterpret_cast<const void*>(source), byteCount);

        result = self->field_0014 - 4;
        self->field_0014 = result;
    }
    return result;
}

HOOK(0x00472350, S_00472350_RemoveSlot, "auto");
