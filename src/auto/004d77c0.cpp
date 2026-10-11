#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

// Sub-object (this) at +4 of the S_0041f0a0 record: its vtable slot 32 (+0x80) is a
// boolean query that receives (0, record) as stack args; slot 31 (+0x7C) takes only the record.
static int __fastcall Autonomo_UpdateAndReportDone(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* a1)
{
    // Saved before serialization; restored after it.
    uint32_t savedField1C = CSF_AT(uint32_t, a1, 0x1C);

    // Flag byte at +0: when clear, ask the sub-object (slot 32) whether to continue.
    bool v4 = false;
    if (CSF_AT(uint8_t, a1, 0) == 0) {
        void* subObject = CSF_AT(void*, a1, 4);
        bool queryResult = ((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))
            CSF_AT(void**, subObject, 0)[0x80 / 4])(subObject, nullptr, 0, a1);
        v4 = !queryResult;
    }
    CSF_AT(uint8_t, a1, 0) = v4 ? 1 : 0;

    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(a1));

    CSF_AT(uint32_t, a1, 0x1C) = savedField1C;

    if (CSF_AT(uint8_t, a1, 0) == 0) {
        void* subObject = CSF_AT(void*, a1, 4);
        bool finished = ((bool(__fastcall*)(void*, void*, S_0041f0a0*))
            CSF_AT(void**, subObject, 0)[0x7C / 4])(subObject, nullptr, a1);
        if (finished) {
            CSF_AT(uint8_t, a1, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a1, 0) = 1;
    return 1;
}

HOOK(0x4D77C0, Autonomo_UpdateAndReportDone, "auto");
