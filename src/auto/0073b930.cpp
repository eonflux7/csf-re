#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint8_t*, __fastcall, InitializeExtendedObject, 0x0073B7F0, (void* self, void* edx));

static uint8_t* __cdecl CreateTypedObject(int type, int value)
{
    uint8_t* object = nullptr;

    switch (type)
    {
    case 0:
        object = reinterpret_cast<uint8_t*>(CoreMemory_AllocUntagged(36));
        if (!object)
            return nullptr;
        CSF_AT(uint8_t, object, 0x04) = 0;
        CSF_AT(void**, object, 0x00) = CSF_VTBL(0x008674AC);
        CSF_AT(uint32_t, object, 0x18) = 0;
        CSF_AT(uint32_t, object, 0x1C) = 0;
        CSF_AT(uint32_t, object, 0x20) = 0;
        break;

    case 1:
        object = reinterpret_cast<uint8_t*>(CoreMemory_AllocUntagged(40));
        if (!object)
            return nullptr;
        CSF_AT(uint8_t, object, 0x04) = 0;
        CSF_AT(void**, object, 0x00) = CSF_VTBL(0x008674D4);
        CSF_AT(uint32_t, object, 0x18) = 0;
        CSF_AT(uint32_t, object, 0x24) = 0;
        CSF_AT(uint32_t, object, 0x1C) = 0;
        break;

    case 2:
        object = reinterpret_cast<uint8_t*>(CoreMemory_AllocUntagged(24));
        if (!object)
            return nullptr;
        CSF_AT(uint8_t, object, 0x04) = 0;
        CSF_AT(void**, object, 0x00) = CSF_VTBL(0x008674FC);
        break;

    case 3:
        object = reinterpret_cast<uint8_t*>(CoreMemory_AllocUntagged(24));
        if (!object)
            return nullptr;
        CSF_AT(uint8_t, object, 0x04) = 0;
        CSF_AT(void**, object, 0x00) = CSF_VTBL(0x00867524);
        break;

    case 4:
        object = reinterpret_cast<uint8_t*>(CoreMemory_AllocUntagged(52));
        if (!object)
            return nullptr;
        object = InitializeExtendedObject(object, nullptr);
        break;

    default:
        return nullptr;
    }

    if (type != 4)
    {
        CSF_AT(uint32_t, object, 0x10) = 0;
        CSF_AT(uint32_t, object, 0x14) = 0;
    }

    if (object)
    {
        CSF_AT(int, object, 0x0C) = type;
        CSF_AT(int, object, 0x08) = value;
    }

    return object;
}

HOOK(0x0073B930, CreateTypedObject, "auto");
