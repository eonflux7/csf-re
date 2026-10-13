#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bindings (one name and signature per address)
CSF_FUNC(int, __fastcall, FindDestinationSlotIndex, 0x0044BC20, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int text, int flag));
CSF_FUNC(int, __cdecl, MenuMessageBox_TestEntryAgainstFlag, 0x00695500, (uint16_t* entry, char* flagByte));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Reference to the vtable address the packet lists for this site.
[[maybe_unused]] static const void* s_stateVtableRef = CSF_VTBL(0x0044BC20);

// Looks up the destination slot for a2, then, if the entry at a3 passes the
// flag test, copies the string a3 into that slot's 100-byte record (+0x40).
static C_0082d410* __fastcall DestinationDispatchWithArgs(S_0044bc20* self, void* /*edx*/, int a2, uint32_t* a3)
{
    int slotIndex = FindDestinationSlotIndex(self, nullptr, a2);
    if (slotIndex == -1) {
        return reinterpret_cast<C_0082d410*>(static_cast<intptr_t>(slotIndex));
    }

    int encodedText = Text_ConvertToDisplayEncoding(
        static_cast<int>(reinterpret_cast<uintptr_t>(a3)), 12);
    int flagResult = MenuMessageBox_TestEntryAgainstFlag(
        reinterpret_cast<uint16_t*>(encodedText),
        reinterpret_cast<char*>(&g_byte_82E65C));
    if (flagResult == 0) {
        return nullptr;
    }

    // Length of the NUL-terminated string (inline loop in the original).
    char* text = reinterpret_cast<char*>(a3);
    unsigned int textLength = 0;
    while (text[textLength] != '\0') {
        ++textLength;
    }

    // Destination = base + slot * 100 + 0x40; field read after the flag test, as in the original.
    uint32_t destAddr = self->field_0010 + static_cast<uint32_t>(slotIndex * 100) + 64u;
    void* destSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(destAddr));

    return reinterpret_cast<C_0082d410*>(
        AssignStringRange_429B60(destSelf, nullptr, text, textLength));
}

HOOK(0x0044C440, DestinationDispatchWithArgs, "auto");
