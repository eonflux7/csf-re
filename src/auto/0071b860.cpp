#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, C_00864508_ProcessEntry, 0x0071B6D0,
         (C_00864508* self, void* edx, int index));
CSF_FUNC(void*, __cdecl, CopyEntryBytes, 0x007AEE60,
         (void* destination, const void* source, std::size_t size));

static int __fastcall C_00864508_UpdatePendingEntries(
    C_00864508* self, void* /*edx*/)
{
    const std::uint32_t secondaryBegin = self->field_0034;
    if (secondaryBegin != 0) {
        const std::int32_t secondaryCount =
            static_cast<std::int32_t>(self->field_0038 - secondaryBegin) >> 3;
        if (secondaryCount != 0)
            return secondaryCount;
    }

    const std::uint32_t pendingBegin = self->field_0024;
    const std::uint32_t pendingCount = pendingBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0028 - pendingBegin) >> 2)
        : 0;

    for (std::uint32_t position = 0; position < pendingCount; ++position) {
        const std::uint32_t entryAddress =
            self->field_0024 + (position << 2);
        const std::uint32_t recordsAddress = self->field_0014;
        const std::uint32_t index =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(entryAddress), 0);
        void* object = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(recordsAddress + (index << 6)),
                   0x38));

        const std::uint8_t active =
            ((std::uint8_t(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x24 / 4])(object, nullptr);

        bool removeEntry = false;
        // The original tests x87 C0, which also treats unordered as true.
        if (!active || !(self->field_0078 >= g_fZero)) {
            C_00864508_ProcessEntry(self, nullptr, static_cast<int>(index));
            removeEntry = true;
        } else {
            void* attachedObject =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x20 / 4])(object, nullptr);

            if (attachedObject == nullptr) {
                C_00864508_ProcessEntry(self, nullptr, static_cast<int>(index));
                attachedObject =
                    ((void*(__fastcall*)(void*, void*))
                        CSF_AT(void**, object, 0)[0x20 / 4])(object, nullptr);

                const std::uint32_t parameter =
                    __builtin_bit_cast(std::uint32_t, self->field_0078);
                ((void(__fastcall*)(void*, void*, std::uint32_t, int))
                    CSF_AT(void**, attachedObject, 0)[0x198 / 4])(
                        attachedObject, nullptr, parameter, 1);
                removeEntry = true;
            } else {
                const int retainEntry =
                    ((int(__fastcall*)(void*, void*))
                        CSF_AT(void**, attachedObject, 0)[0x194 / 4])(
                            attachedObject, nullptr);

                if (!retainEntry) {
                    C_00864508_ProcessEntry(
                        self, nullptr, static_cast<int>(index));
                    const std::uint32_t parameter =
                        __builtin_bit_cast(std::uint32_t, self->field_0078);
                    ((void(__fastcall*)(void*, void*, std::uint32_t, int))
                        CSF_AT(void**, attachedObject, 0)[0x198 / 4])(
                            attachedObject, nullptr, parameter, 1);
                    removeEntry = true;
                }
            }
        }

        if (removeEntry) {
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->field_0024 + (position << 2)),
                   0) = UINT32_MAX;
        }
    }

    const std::uint32_t compactBegin = self->field_0024;
    std::uint32_t result = compactBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0028 - compactBegin) >> 2)
        : 0;

    for (std::int32_t position = static_cast<std::int32_t>(result - 1);
         position >= 0; --position) {
        result = self->field_0024 +
                 (static_cast<std::uint32_t>(position) << 2);
        if (CSF_AT(std::uint32_t, reinterpret_cast<void*>(result), 0)
            == UINT32_MAX) {
            const std::uint32_t source = result + 4;
            const std::uint32_t byteCount =
                static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_0028 - source) >> 2)
                << 2;
            CopyEntryBytes(reinterpret_cast<void*>(result),
                           reinterpret_cast<const void*>(source), byteCount);
            result = self->field_0028 - 4;
            self->field_0028 = result;
        }
    }

    return static_cast<int>(result);
}

HOOK(0x0071B860, C_00864508_UpdatePendingEntries, "auto");
