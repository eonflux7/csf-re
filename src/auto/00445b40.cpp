#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static std::uint64_t __fastcall S_00445b40_UpdateHandlesAndInterval(
    S_00445b40* self, void* /*edx*/, float interval)
{
    if (self->field_00d0 != 0)
    {
        std::int32_t index =
            static_cast<std::int32_t>(self->field_00d0 - 1);
        if (index >= 0)
        {
            std::uint32_t byteOffset =
                12 * static_cast<std::uint32_t>(index);
            do
            {
                if (!CHandleTable_Resolve(
                        g_pCurrentScene, nullptr,
                        static_cast<int>(self->field_00c8 + byteOffset)))
                {
                    const std::uint32_t remaining = self->field_00d0 - 1;
                    self->field_00d0 = remaining;
                    if (static_cast<std::uint32_t>(index) != remaining)
                    {
                        const std::uint32_t destination =
                            self->field_00c8 + byteOffset;
                        _memcpy(
                            reinterpret_cast<void*>(destination),
                            reinterpret_cast<const void*>(destination + 12),
                            12 * (remaining - static_cast<std::uint32_t>(index)));
                    }
                }
                --index;
                byteOffset -= 12;
            } while (index >= 0);
        }
    }

    if (self->field_00c4 == 0)
    {
        if (self->field_00d0 != 0)
        {
            const std::uint32_t remaining = self->field_00d0 - 1;
            if (remaining != 0)
            {
                const std::uint32_t destination = self->field_00c8;
                self->field_00d0 = remaining;
                _memcpy(
                    reinterpret_cast<void*>(destination),
                    reinterpret_cast<const void*>(destination + 12),
                    12 * remaining);
            }
            else
            {
                self->field_00d0 = 0;
            }
        }
    }

    self->field_00c4 = 0;
    const std::uint64_t result = static_cast<std::uint64_t>(
        static_cast<std::int64_t>(interval * g_MaxPercent));
    self->field_00c0 = static_cast<std::uint32_t>(result);
    if (static_cast<std::uint32_t>(result) == 0)
        self->field_00c0 = 1;
    return result;
}

HOOK(0x00445B40, S_00445b40_UpdateHandlesAndInterval, "auto");
