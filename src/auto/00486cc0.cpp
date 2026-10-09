#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CCharActionFsm_PrepareCurrentActionRemoval, 0x004846D0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static std::uint32_t __fastcall CCharActionFsm_RemoveActionAtIndex(
    CCharActionFsm* self, void* /*edx*/, int index)
{
    const std::int32_t currentIndex =
        static_cast<std::int32_t>(self->field_0090);

    if (currentIndex > index)
    {
        self->field_0090 = currentIndex - 1;
    }
    else if (currentIndex == index)
    {
        CCharActionFsm_PrepareCurrentActionRemoval(self, nullptr);

        const std::uint32_t begin = self->field_0078;
        if (begin &&
            static_cast<std::uint32_t>(
                static_cast<std::int32_t>(self->field_007c - begin) >> 2) > 1)
        {
            const std::uint32_t count = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(self->field_007c - begin) >> 2);
            self->field_0090 %= count - 1;
        }
        else
        {
            self->field_0090 = static_cast<std::uint32_t>(-1);
        }
    }

    const std::uint32_t byteOffset = 4 * static_cast<std::uint32_t>(index);
    void* action = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0078 + byteOffset), 0);

    if (action)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, action, 0)[0])(action, nullptr, 1);

        CSF_AT(
            std::uint32_t,
            reinterpret_cast<void*>(self->field_0078 + byteOffset), 0) = 0;
    }

    const std::uint32_t destination = self->field_0078 + byteOffset;
    const std::uint32_t source = destination + 4;
    const std::uint32_t byteCount =
        4 * static_cast<std::uint32_t>(
                static_cast<std::int32_t>(self->field_007c - source) >> 2);

    _memcpy(reinterpret_cast<void*>(destination),
            reinterpret_cast<const void*>(source), byteCount);

    const std::uint32_t result = self->field_007c - 4;
    self->field_007c = result;
    return result;
}

HOOK(0x00486CC0, CCharActionFsm_RemoveActionAtIndex, "auto");
