#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CheckCursorBufferReady, 0x00698EA0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* edx, uint8_t* a2));
CSF_FUNC(void*, __fastcall, AppendNarrowToWideString, 0x00697150, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (S_004f21d0* self, void* edx, int16_t a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(uint8_t, byte_85E71C, 0x0085E71C);

// Reads UTF-16 code units from the buffer between readCursor and bufferEnd, forwarding each
// unit to SaveSlots_ApplyIndex until a CR LF pair is found. Returns 1 when a line was read,
// or the readiness result of CheckCursorBufferReady when the buffer is not ready.
static char __fastcall SortedTable_ReadCrLfLine(S_00698c10* self, void* /*edx*/, S_004f21d0* a2)
{
    char result = CheckCursorBufferReady(self, nullptr);
    if (!result)
        return result;

    TextInputSubobject_Init(a2, nullptr, &g_byte_82E65C);

    while (true) {
        uint32_t cursorAddr = self->field_0018;
        uint32_t endAddr = self->field_001c;
        uint32_t nextAddr = cursorAddr + 2;
        if (nextAddr > endAddr)
            break;

        uint16_t ch = *reinterpret_cast<const uint16_t*>(cursorAddr);
        self->field_0018 = nextAddr;

        if (ch == 13) {
            uint32_t afterCrAddr = nextAddr + 2;
            if (afterCrAddr <= endAddr) {
                uint16_t lf = *reinterpret_cast<const uint16_t*>(nextAddr);
                self->field_0018 = afterCrAddr;
                if (lf == 10)
                    break;
                self->field_0018 = nextAddr;
            }
        } else if (ch != 10) {
            SaveSlots_ApplyIndex(a2, nullptr, static_cast<int16_t>(ch));
        }
    }

    AppendNarrowToWideString(a2, nullptr, &byte_85E71C);
    return 1;
}

HOOK(0x00698F00, SortedTable_ReadCrLfLine, "auto");
