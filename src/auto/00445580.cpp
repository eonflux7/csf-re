#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* , int additionalCapacity));

static int __fastcall ColorAnim_ClearForControl(void* self, void* /*edx*/, std::uint32_t* control)
{
    const std::uint32_t type = CSF_AT(std::uint32_t, control, 0x50);
    if (type == 1)
    {
        ++CSF_AT(std::uint32_t, self, 0x0C);
        switch (CSF_AT(std::uint32_t, control, 0x7C))
        {
        case 1: ++CSF_AT(std::uint32_t, self, 0x14); break;
        case 2: ++CSF_AT(std::uint32_t, self, 0x1C); break;
        case 3: ++CSF_AT(std::uint32_t, self, 0x24); break;
        case 4: ++CSF_AT(std::uint32_t, self, 0x2C); break;
        case 5: ++CSF_AT(std::uint32_t, self, 0x34); break;
        default: break;
        }
    }
    else
    {
        if (type != 2 && type != 3)
            return static_cast<int>(type);
        ++CSF_AT(std::uint32_t, self, 0x08);
    }

    auto* records = reinterpret_cast<S_00413570*>(
        static_cast<std::uint8_t*>(self) + 0x54);

    // Address classified as a vtable reference in the supplied packet.
    (void)CSF_VTBL(0x00413570);

    if (CSF_AT(std::uint32_t, self, 0x5C) >=
        CSF_AT(std::uint32_t, self, 0x58))
    {
        S_00413570_GrowEventRecords(
            records, nullptr, CSF_AT(int, self, 0x60));
    }

    const std::uint32_t index = CSF_AT(std::uint32_t, self, 0x5C);
    auto* data = CSF_AT(std::uint8_t*, self, 0x54);
    CSF_AT(std::uint32_t, self, 0x5C) = index + 1;
    auto* record = data + 12 * index;
    CSF_AT(std::uint32_t, record, 0x04) =
        CSF_AT(std::uint32_t, control, 0x20);
    const int result = CSF_AT(int, control, 0x24);
    CSF_AT(int, record, 0x08) = result;
    return result;
}

HOOK(0x00445580, ColorAnim_ClearForControl, "auto");
