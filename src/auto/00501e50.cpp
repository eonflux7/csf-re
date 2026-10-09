#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address.
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __fastcall, GetCurrentMenuStringText, 0x00500350, (void* self, void* edx));

// Data referenced by the original.
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// The original calls GetCurrentMenuStringText, which references S_00500350's vtable.
static void* const s_StringTableVtable [[maybe_unused]] = CSF_VTBL(0x00500350);

static C_0082d410* __fastcall S_00500350_ReleaseMenuStringTable(S_00500350* self, void* /*edx*/)
{
    C_0082d410* result = reinterpret_cast<C_0082d410*>(self->field_0030);
    if (result)
    {
        // Number of 88-byte entries between the two bounds (signed division).
        int32_t entryCount = static_cast<int32_t>(self->field_0034 - self->field_0030) / 88;
        result = reinterpret_cast<C_0082d410*>(static_cast<intptr_t>(entryCount));
        if (result)
        {
            self->field_000c = 0;
            char* text = reinterpret_cast<char*>(static_cast<intptr_t>(GetCurrentMenuStringText(self, nullptr)));
            result = reinterpret_cast<C_0082d410*>(text);
            if (result)
            {
                void* subObject = reinterpret_cast<uint8_t*>(self) + 0x7C;
                AssignStringRange_429B60(subObject, nullptr, text, static_cast<unsigned int>(std::strlen(text)));

                // Re-read the field after the call; the callee may have changed it.
                uint32_t compareCount = CSF_AT(uint32_t, self, 0x90);
                int compareResult = CompareStringSubstring(
                    reinterpret_cast<uint8_t*>(self) + 0x7C, nullptr,
                    0u, compareCount,
                    reinterpret_cast<const char*>(&g_byte_82E65C), 0u);
                result = reinterpret_cast<C_0082d410*>(static_cast<intptr_t>(compareResult));
                if (result)
                {
                    void* secondSubObject = reinterpret_cast<uint8_t*>(self) + 0x98;
                    result = reinterpret_cast<C_0082d410*>(
                        AssignStringRange_429B60(secondSubObject, nullptr,
                            reinterpret_cast<char*>(&g_byte_82E65C), 0u));
                    self->field_005c = 1;
                }
            }
        }
    }
    return result;
}

HOOK(0x00501E50, S_00500350_ReleaseMenuStringTable, "auto");
