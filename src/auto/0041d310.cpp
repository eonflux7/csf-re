#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, sub_407F40, 0x00407F40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4084C0, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __cdecl, sub_40A430, 0x0040A430, (int a1));
CSF_FUNC(char, __fastcall, sub_418B40, 0x00418B40, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, sub_419A00, 0x00419A00, (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(int, __fastcall, sub_419A80, 0x00419A80, (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static char __fastcall C_0082e428_InitializeWheels(
    C_0082e428* self, void* /*edx*/, int target, int options)
{
    self->field_0004 = target;
    auto* records = sub_40A430(3);
    self->field_0030 = reinterpret_cast<uint32_t>(records);

    char valid = static_cast<char>(
        reinterpret_cast<C_0082e520*>(records)->vSlot8(target) & 1);

    for (int slot = 0; slot < 10; ++slot)
    {
        if (sub_407F40(reinterpret_cast<void*>(self->field_0030), nullptr, slot) ||
            sub_407F40(reinterpret_cast<void*>(self->field_0030), nullptr, slot + 10))
        {
            sub_419A00(self, nullptr, slot, 1090519040, 25.0f);
            sub_419A80(self, nullptr, slot + 10, 1090519040, 25.0f);
        }
    }

    valid &= static_cast<char>(self->field_003c >= 4);
    char buffer[0x74];
    uint32_t wheelIndex = 0;
    uint32_t recordOffset = 0;

    if (self->field_003c != 0)
    {
        do
        {
            // The original clears the first 100 bytes of this stack buffer.
            std::memset(buffer, 0, 0x64);
            _sprintf(buffer, CSF_STR(0x0082E4A0, "Rueda derecha %d"), wheelIndex);

            bool active = sub_407F40(
                reinterpret_cast<void*>(self->field_0030), nullptr,
                CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0034) + recordOffset, 0));
            if (!active)
                HandleTargetUnavailable(reinterpret_cast<void*>(self->field_0004), nullptr);
            valid &= static_cast<char>(active);

            _sprintf(buffer, CSF_STR(0x0082E48C, "Rueda izquierda %d"), wheelIndex);

            active = sub_407F40(
                reinterpret_cast<void*>(self->field_0030), nullptr,
                CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0044) + recordOffset, 0));
            if (!active)
                HandleTargetUnavailable(reinterpret_cast<void*>(self->field_0004), nullptr);
            valid &= static_cast<char>(active);

            ++wheelIndex;
            recordOffset += 32;
        } while (wheelIndex < self->field_003c);
    }

    if (valid)
    {
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0034), 8) = 0;
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0034), 20) = 0;
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0044), 8) = 0;
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0044), 20) = 0;
        CSF_AT(uint32_t,
            reinterpret_cast<uint8_t*>(self->field_0034) + 32 * self->field_003c, -24) = 0;
        CSF_AT(uint32_t,
            reinterpret_cast<uint8_t*>(self->field_0034) + 32 * self->field_003c, -12) = 0;
        CSF_AT(uint32_t,
            reinterpret_cast<uint8_t*>(self->field_0044) + 32 * self->field_004c, -24) = 0;
        CSF_AT(uint32_t,
            reinterpret_cast<uint8_t*>(self->field_0044) + 32 * self->field_004c, -12) = 0;

        wheelIndex = 0;
        recordOffset = 0;
        if (self->field_004c != 0)
        {
            do
            {
                auto* leftEntry = reinterpret_cast<void*>(sub_4084C0(
                    reinterpret_cast<void*>(self->field_0030), nullptr,
                    CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0044) + recordOffset, 0)));
                auto* rightEntry = reinterpret_cast<void*>(sub_4084C0(
                    reinterpret_cast<void*>(self->field_0030), nullptr,
                    CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0034) + recordOffset, 0)));

                auto* leftValue =
                    reinterpret_cast<uint8_t*>(self->field_0044) + recordOffset + 4;
                auto* leftPosition =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, leftEntry, 0)[0x18 / 4])(leftEntry, nullptr);
                CSF_AT(float, leftValue, 0) =
                    CSF_AT(float, leftPosition, 12) + CSF_AT(float, leftValue, 0);

                auto* rightValue =
                    reinterpret_cast<uint8_t*>(self->field_0034) + recordOffset + 4;
                auto* rightPosition =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, rightEntry, 0)[0x18 / 4])(rightEntry, nullptr);
                CSF_AT(float, rightValue, 0) =
                    CSF_AT(float, rightPosition, 12) + CSF_AT(float, rightValue, 0);

                ++wheelIndex;
                recordOffset += 32;
            } while (wheelIndex < self->field_004c);
        }
    }

    const char initialized = sub_418B40(self, nullptr, target, options);
    return static_cast<char>(valid & initialized);
}

HOOK(0x0041D310, C_0082e428_InitializeWheels, "auto");
