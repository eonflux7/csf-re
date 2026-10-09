#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Bound original callees (addresses from the packet).
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, DispatchTargetAction, 0x004EFCA0,
         (void* self, void* edx, int a2, void* a3, int a4, int a5));

// Reads the 32-bit word at (base + offset).
static uint32_t ReadWordAt(uint32_t base, uint32_t offset) {
    return CSF_AT(uint32_t, reinterpret_cast<void*>(base), offset);
}

// Dispatches an action for a record. a3 is an array of three pointers:
//   a3[0] -> object whose field +4 chain yields the record at +0x20,
//   a3[1] -> object whose +4 chain yields the target,
//   a3[2] -> object whose +4 field points to a flags byte at +1.
static int __stdcall DispatchRecordAction(int a1, int a2, uint32_t* a3) {
    (void)a1;
    (void)a2;

    uint32_t resultPtr = ReadWordAt(a3[2], 4);
    int result = static_cast<int>(resultPtr);

    int v4 = static_cast<int>(ReadWordAt(ReadWordAt(a3[0], 4), 0x20));

    uint8_t flagByte = CSF_AT(uint8_t, reinterpret_cast<void*>(resultPtr), 1);
    int v5 = 13 - (flagByte != 0 ? 1 : 0);

    if (v4 != -1) {
        int managerValue = GetValue8B0554();
        uint32_t targetPtr = ReadWordAt(ReadWordAt(a3[1], 4), 4);
        void* manager = reinterpret_cast<void*>(
            ReadWordAt(static_cast<uint32_t>(managerValue), 0x1FDC));
        return DispatchTargetAction(manager, nullptr, v5,
                                    reinterpret_cast<void*>(targetPtr), v4, 0);
    }
    return result;
}

HOOK(0x0045D570, DispatchRecordAction, "auto");
