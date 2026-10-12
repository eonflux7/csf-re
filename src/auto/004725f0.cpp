#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vector3ExactEquals, 0x004069B0, (float* self, void* , float* other));
CSF_FUNC(int, __cdecl, CompareValueData, 0x00472290, (unsigned int* a1, uint32_t* a2));

static bool __fastcall S_004725f0_EqualsValue(
    S_004725f0* self, void* /*edx*/, S_004725f0* other)
{
    auto* otherData = reinterpret_cast<uint8_t*>(other->field_0004);

    switch (CSF_AT(uint8_t, otherData, 0x00))
    {
    case 4:
        return CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x08)
            == CSF_AT(uint32_t, otherData, 0x08);

    case 5:
    {
        auto* data = reinterpret_cast<uint8_t*>(self->field_0004);
        return CSF_AT(uint32_t, data, 0x10) == CSF_AT(uint32_t, otherData, 0x10)
            && CSF_AT(uint32_t, data, 0x14) == CSF_AT(uint32_t, otherData, 0x14);
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
        return CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x04)
            == CSF_AT(uint32_t, otherData, 0x04);

    case 0xB:
        return CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x20)
            == CSF_AT(uint32_t, otherData, 0x20);

    case 0x10:
    case 0x12:
    case 0x17:
    case 0x25:
        return static_cast<uint8_t>(CompareValueData(
            reinterpret_cast<unsigned int*>(
                reinterpret_cast<uint8_t*>(self->field_0004) + 4),
            reinterpret_cast<uint32_t*>(otherData + 4))) != 0;

    case 0x14:
        return static_cast<uint8_t>(Vector3ExactEquals(
            reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(self->field_0004) + 4),
            nullptr, reinterpret_cast<float*>(otherData + 4))) != 0;

    case 0x15:
    case 0x21:
    case 0x22:
    case 0x23:
        return CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x01)
            == CSF_AT(uint8_t, otherData, 0x01);

    case 0x16:
        return CSF_AT(float, reinterpret_cast<void*>(self->field_0004), 0x04)
            == CSF_AT(float, otherData, 0x04);

    default:
        return false;
    }
}

HOOK(0x004725F0, S_004725f0_EqualsValue, "auto");
