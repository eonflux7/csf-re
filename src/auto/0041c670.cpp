#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, IsRecordSlotActive, 0x00407F40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __cdecl, AllocateSubobjectHandle, 0x0040A430, (int a1));
CSF_FUNC(char, __fastcall, InitializeBase, 0x00418B40, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));

static bool C_0082e3b8_ValidateRecordSlot(C_0082e3b8* self, int slot)
{
    if (IsRecordSlotActive(reinterpret_cast<void*>(self->field_0030), nullptr, slot))
        return true;

    HandleTargetUnavailable(reinterpret_cast<void*>(self->field_0004), nullptr);
    return false;
}

static char __fastcall C_0082e3b8_InitializeWheelRecords(
    C_0082e3b8* self, void* /*edx*/, int target, int options)
{
    self->field_0004 = target;
    uint32_t* records = AllocateSubobjectHandle(2);
    self->field_0030 = reinterpret_cast<uint32_t>(records);

    char success = static_cast<char>(
        ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, records, 0)[8 / 4])(
            records, nullptr, target) & 1);

    success = C_0082e3b8_ValidateRecordSlot(self, 0) & success;
    success = C_0082e3b8_ValidateRecordSlot(self, 9) & success;
    success = C_0082e3b8_ValidateRecordSlot(self, 1) & success;
    success = C_0082e3b8_ValidateRecordSlot(self, 10) & success;

    int pairedSlot = 11;
    int remaining = 6;
    do
    {
        success = C_0082e3b8_ValidateRecordSlot(self, pairedSlot - 9) & success;
        success = C_0082e3b8_ValidateRecordSlot(self, pairedSlot) & success;
        ++pairedSlot;
        --remaining;
    } while (remaining);

    success = C_0082e3b8_ValidateRecordSlot(self, 8) & success;
    success = C_0082e3b8_ValidateRecordSlot(self, 17) & success;

    if (success)
    {
        uint32_t index = 0;
        uint32_t byteOffset = 0;
        if (self->field_004c)
        {
            do
            {
                auto* firstEntry =
                    reinterpret_cast<uint8_t*>(self->field_0044) + byteOffset;
                void* firstRecord = reinterpret_cast<void*>(GetTableEntryDwordByIndex(
                    reinterpret_cast<void*>(self->field_0030), nullptr,
                    CSF_AT(int, firstEntry, 0)));

                auto* secondEntry =
                    reinterpret_cast<uint8_t*>(self->field_0034) + byteOffset;
                void* secondRecord = reinterpret_cast<void*>(GetTableEntryDwordByIndex(
                    reinterpret_cast<void*>(self->field_0030), nullptr,
                    CSF_AT(int, secondEntry, 0)));

                auto* firstAdjustment =
                    reinterpret_cast<uint8_t*>(self->field_0044) + byteOffset + 4;
                void* firstTransform =
                    ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, firstRecord, 0)[24 / 4])(
                            firstRecord, nullptr);
                CSF_AT(float, firstAdjustment, 0) =
                    CSF_AT(float, firstTransform, 12) +
                    CSF_AT(float, firstAdjustment, 0);

                auto* secondAdjustment =
                    reinterpret_cast<uint8_t*>(self->field_0034) + byteOffset + 4;
                void* secondTransform =
                    ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, secondRecord, 0)[24 / 4])(
                            secondRecord, nullptr);
                byteOffset += 32;
                CSF_AT(float, secondAdjustment, 0) =
                    CSF_AT(float, secondTransform, 12) +
                    CSF_AT(float, secondAdjustment, 0);
                ++index;
            } while (index < self->field_004c);
        }
    }

    char baseResult = InitializeBase(self, nullptr, target, options);
    return static_cast<char>(success & baseResult);
}

HOOK(0x0041C670, C_0082e3b8_InitializeWheelRecords, "auto");
