#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, sub_6C7B20, 0x006C7B20, (int a1));
CSF_FUNC(int, __stdcall, Hud_SetWidgetParam, 0x006C7B40, (int a1, int a2));

// Clears the pending flag at +0x10, then walks the (id, value) entry table at +0x08 and
// dispatches each entry: non-negative ids go to sub_6C7B20, negative ids (masked) go to Hud_SetWidgetParam.
static int __fastcall GfxSkin_FlushWidgetParamTable(void* self, void* /*edx*/) {
    int result = CSF_AT(int, self, 0x0C);
    CSF_AT(uint8_t, self, 0x10) = 0;

    if (result != 0) {
        result = CSF_AT(int, self, 0x08);
        if (result != 0) {
            // Original keeps the returned global object's +4 word as the callee context.
            uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
            uint32_t globalContext = CSF_AT(uint32_t, globalValue, 0x04);
            (void)globalContext;

            uint8_t* table = CSF_AT(uint8_t*, self, 0x08);
            result = CSF_AT(int, table, 0x08);
            if (result != 0) {
                int remaining = result;
                int offset = 0;
                do {
                    // Entry array is re-read every iteration, as in the original.
                    uint8_t* entries = CSF_AT(uint8_t*, table, 0x00);
                    int32_t entryId = CSF_AT(int32_t, entries, offset);
                    if (entryId >= 0) {
                        alignas(8) int32_t pairBuf[3];
                        pairBuf[0] = entryId;
                        pairBuf[1] = CSF_AT(int32_t, entries, offset + 0x08);
                        sub_6C7B20(reinterpret_cast<int>(pairBuf));
                    } else {
                        int32_t paramValue = CSF_AT(int32_t, entries, offset + 0x08);
                        Hud_SetWidgetParam(entryId & 0x7FFFFFFF, paramValue);
                    }
                    offset += 12;
                    result = --remaining;
                } while (remaining != 0);
            }
        }
    }
    return result;
}

HOOK(0x006C47F0, GfxSkin_FlushWidgetParamTable, "auto");
