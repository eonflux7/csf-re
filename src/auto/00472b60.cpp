#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082d410_AssignWholeString, 0x00433FA0, (C_0082d410* self, void* , int sourceValue));
CSF_FUNC(int, __cdecl, AllocateEntryPayload, 0x004723D0, (int a1));
CSF_FUNC(char*, __fastcall, ResetEntryPayload, 0x00472AB0, (void* self, void* edx));

static C_008313f8* __fastcall C_008313f8_AssignEntryValue(
    C_008313f8* self, void* /*edx*/, int sourceEntry)
{
    void* entry = reinterpret_cast<void*>(sourceEntry);
    uint8_t* source = CSF_AT(uint8_t*, entry, 0x04);
    const uint8_t type = CSF_AT(uint8_t, source, 0x00);

    if (type == 1)
        return self;

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x00) != type)
    {
        ResetEntryPayload(self, nullptr);
        uint8_t* payload = reinterpret_cast<uint8_t*>(AllocateEntryPayload(type));
        self->field_0004 = reinterpret_cast<uint32_t>(payload);
        if (payload)
            CSF_AT(uint8_t, payload, 0x00) = type;
    }

    source = CSF_AT(uint8_t*, entry, 0x04);
    switch (CSF_AT(uint8_t, source, 0x00))
    {
    case 4:
    {
        uint8_t* destination = reinterpret_cast<uint8_t*>(self->field_0004) + 4;
        CSF_AT(uint32_t, destination, 0x04) = CSF_AT(uint32_t, source, 0x08);
        CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, source, 0x0C);
        break;
    }

    case 5:
    {
        uint8_t* destination = reinterpret_cast<uint8_t*>(self->field_0004) + 4;
        uint8_t* values = source + 4;
        CSF_AT(uint32_t, destination, 0x00) = CSF_AT(uint32_t, values, 0x00);
        CSF_AT(uint32_t, destination, 0x04) = CSF_AT(uint32_t, values, 0x04);
        CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, values, 0x08);

        source = CSF_AT(uint8_t*, entry, 0x04);
        destination = reinterpret_cast<uint8_t*>(self->field_0004);
        CSF_AT(uint32_t, destination, 0x10) = CSF_AT(uint32_t, source, 0x10);

        source = CSF_AT(uint8_t*, entry, 0x04);
        destination = reinterpret_cast<uint8_t*>(self->field_0004);
        CSF_AT(uint32_t, destination, 0x14) = CSF_AT(uint32_t, source, 0x14);
        break;
    }

    case 6:
    case 7:
    case 8:
    case 9:
    case 0xA:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0x11:
    case 0x16:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
    case 0x20:
    case 0x24:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:
    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x2F:
    case 0x30:
    case 0x31:
    case 0x32:
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x04) =
            CSF_AT(uint32_t, source, 0x04);
        break;

    case 0xB:
        C_0082d410_AssignWholeString(
            reinterpret_cast<C_0082d410*>(
                reinterpret_cast<uint8_t*>(self->field_0004) + 4),
            nullptr, reinterpret_cast<int>(source + 4));
        source = CSF_AT(uint8_t*, entry, 0x04);
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x20) =
            CSF_AT(uint32_t, source, 0x20);
        break;

    case 0x10:
    case 0x12:
    case 0x17:
    case 0x25:
        C_0082d410_AssignWholeString(
            reinterpret_cast<C_0082d410*>(
                reinterpret_cast<uint8_t*>(self->field_0004) + 4),
            nullptr, reinterpret_cast<int>(source + 4));
        break;

    case 0x14:
    {
        uint8_t* destination = reinterpret_cast<uint8_t*>(self->field_0004) + 4;
        uint8_t* values = source + 4;
        CSF_AT(uint32_t, destination, 0x00) = CSF_AT(uint32_t, values, 0x00);
        CSF_AT(uint32_t, destination, 0x04) = CSF_AT(uint32_t, values, 0x04);
        CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, values, 0x08);
        break;
    }

    case 0x15:
    case 0x21:
    case 0x22:
    case 0x23:
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x01) =
            CSF_AT(uint8_t, source, 0x01);
        break;

    default:
        break;
    }

    return self;
}

HOOK(0x00472B60, C_008313f8_AssignEntryValue, "auto");
