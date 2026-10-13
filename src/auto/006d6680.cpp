#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Walks the child array (field_0054, count field_005c). Each child reports a kind via
// vtbl[0x10]. Kind 1 children are counted in field_0104 when their vtbl[0x38] flag is set.
// Every other kind is counted in field_00fc, and field_0100 counts those with the flag set.
// Returns the address of field_00fc.
static uint32_t* __fastcall C_00861120_RecountChildKinds(C_00861120* self, void* /*edx*/)
{
    uint32_t* result = &self->field_00fc;
    self->field_00fc = 0;
    self->field_0104 = 0;
    self->field_0100 = 0;

    uint32_t index = 0;
    if (self->field_005c != 0) {
        do {
            uint32_t childValue = reinterpret_cast<uint32_t*>(self->field_0054)[index];
            void* child = reinterpret_cast<void*>(childValue);

            using KindGetter = int(__fastcall*)(void*, void*);
            using FlagGetter = bool(__fastcall*)(void*, void*);

            int kind = (reinterpret_cast<KindGetter>(CSF_AT(void**, child, 0)[4]))(child, nullptr);
            if (kind == 1) {
                bool flagged = (reinterpret_cast<FlagGetter>(CSF_AT(void**, child, 0)[0x38 / 4]))(child, nullptr);
                if (flagged) {
                    ++self->field_0104;
                }
            } else {
                ++self->field_00fc;
                bool flagged = (reinterpret_cast<FlagGetter>(CSF_AT(void**, child, 0)[0x38 / 4]))(child, nullptr);
                if (flagged) {
                    ++self->field_0100;
                }
            }
            ++index;
        } while (index < self->field_005c);
    }
    return result;
}

HOOK(0x6D6680, C_00861120_RecountChildKinds, "auto");
