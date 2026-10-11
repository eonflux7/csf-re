#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, LinkedObject_EvaluateCondition, 0x004E55C0, (void* self, void* edx, int a2, int a3));

static bool __fastcall C_0083e9d0_IsLinkedObjectActive(C_0083e9d0* self, void* /*edx*/)
{
    bool active = false;

    if (self->field_0030 != 1) {
        return self->field_0014 != 0;
    }

    self->vSlotA4();
    ++self->field_001c;

    uint32_t count = self->field_0028;
    uint32_t byteOffset = 0;
    uint32_t index = 0;

    if (count != 0) {
        do {
            uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0020) + byteOffset;
            int state = *reinterpret_cast<int*>(entry);
            bool entryActive;

            if (state == -1) {
                if (!self->field_004d) {
                    active = true;
                    break;
                }
                entryActive = LinkedObject_EvaluateCondition(
                    self, nullptr,
                    static_cast<int>(reinterpret_cast<uintptr_t>(entry + 8)),
                    *reinterpret_cast<int*>(entry + 4));
                *reinterpret_cast<int*>(entry) = entryActive ? 1 : 0;
            } else {
                entryActive = (state == 1);
            }

            if (entryActive) {
                active = true;
                break;
            }

            ++index;
            byteOffset += 20;
        } while (index < count);
    }

    uint8_t depth = self->field_001c;
    if (depth) {
        self->field_001c = depth - 1;
    }
    return active;
}

HOOK(0x004E5980, C_0083e9d0_IsLinkedObjectActive, "auto");
