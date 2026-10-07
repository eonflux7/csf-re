#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00864508_InvokeEntryTargetCallbacks(
    C_00864508* self, void* /*edx*/)
{
    const uint32_t beginAddress = self->field_0014;
    uint32_t remaining = 0;

    if (beginAddress != 0) {
        remaining = static_cast<uint32_t>(
            static_cast<int32_t>(self->field_0018 - beginAddress) >> 6);
    }

    uint32_t recordOffset = 0;
    while (remaining != 0) {
        void* record = reinterpret_cast<void*>(
            self->field_0014 + recordOffset);
        void* object = CSF_AT(void*, record, 0x38);

        if (object != nullptr) {
            void* initialTarget =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[8])(object, nullptr);

            if (initialTarget != nullptr) {
                object = CSF_AT(void*, record, 0x38);
                void* target =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, object, 0)[8])(object, nullptr);

                ((void (__fastcall*)(void*, void*))
                    CSF_AT(void**, target, 0)[23])(target, nullptr);
            }
        }

        recordOffset += 0x40;
        --remaining;
    }

    return static_cast<int32_t>(remaining);
}

HOOK(0x0071A580, C_00864508_InvokeEntryTargetCallbacks, "auto");
