#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Check_75BE70, 0x0075BE70, (void* self, void* edx, int a2));
CSF_FUNC(uint8_t*, __fastcall, SetRangeCursor_51B330, 0x0051B330, (void* self, void* edx, unsigned int a2, int a3));

static char __fastcall C_00840e38_RefreshRangeCursorIfNonEmpty(C_00840e38* self, void* /*edx*/, int a2) {
    char isValid = Check_75BE70(self, nullptr, a2);
    if (isValid) {
        uint8_t* range = reinterpret_cast<uint8_t*>(self->field_0004);
        uint32_t begin = CSF_AT(uint32_t, range, 0x4);
        if (begin != 0) {
            int count = static_cast<int>(CSF_AT(uint32_t, range, 0x8) - begin) / 68;
            if (count != 0) {
                uint32_t head = CSF_AT(uint32_t, range, 0x4);
                if (head == 0) {
                    SetRangeCursor_51B330((uint8_t*)self + 8, nullptr, 0, 0);
                    return isValid;
                }
                int newCount = static_cast<int>(CSF_AT(uint32_t, range, 0x8) - head) / 68;
                SetRangeCursor_51B330((uint8_t*)self + 8, nullptr, newCount, 0);
            }
        }
    }
    return isValid;
}

HOOK(0x0051BC10, C_00840e38_RefreshRangeCursorIfNonEmpty, "auto");
