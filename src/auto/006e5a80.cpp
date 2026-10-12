#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameTranslate, 0x00623830, (int a1, int a2, int a3));

// Translates the RW frame attached to this object's sub-record by a2 (RW_TRANSFORM mode 2).
static int __fastcall C_00861ce0_TranslateRwFrame(C_00861ce0* self, void* /*edx*/, int translation) {
    uint8_t* subRecord = reinterpret_cast<uint8_t*>(self->field_000c);
    uint32_t frameHandle = CSF_AT(uint32_t, subRecord, 4);
    return RwFrameTranslate(static_cast<int>(frameHandle), translation, 2);
}

HOOK(0x006E5A80, C_00861ce0_TranslateRwFrame, "auto");
