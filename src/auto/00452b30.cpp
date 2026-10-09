#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, sub_4A0F50, 0x004A0F50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_492BA0, 0x00492BA0, (void* self, void* edx, int a2));

// Looks up the child at index request->+8 in this control's child table. If the child's id
// (+0x20) matches request->+4, the child has flag 0x2000 (+0x170), a2 is zero and the child's
// slot (+0x1f4) is valid, runs the slot's handler on the request and then calls the child with -1.
static int __fastcall CGuiControl_ActivateChildIfFlagged(CGuiControl* self, void* /*edx*/, int a2, int a3)
{
    int* request = reinterpret_cast<int*>(a3);
    int result = request[2];  // +8: child index

    if (result >= 0 && result < static_cast<int>(self->m_id.alloc)) {
        uint32_t* childTable = reinterpret_cast<uint32_t*>(self->field_0010);
        uint8_t* child = reinterpret_cast<uint8_t*>(childTable[result]);
        if (child) {
            result = CSF_AT(int, child, 0x20);
            if (result == request[1]) {  // +4: expected id
                result = CSF_AT(int, child, 0x170);
                if ((result & 0x2000) != 0) {
                    result = a2;
                    if (!a2) {
                        result = CSF_AT(int, child, 0x1f4);
                        if (result != -1) {
                            uint8_t* slotBase = reinterpret_cast<uint8_t*>(self->field_00cc);
                            uint8_t* slotEntry = slotBase + result * 3 * 16;
                            sub_4A0F50(slotEntry, nullptr, a3);
                            return sub_492BA0(child, nullptr, -1);
                        }
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x00452B30, CGuiControl_ActivateChildIfFlagged, "auto");
