#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x71D300. Its first argument arrives in ecx (the sub-object from this+0x14).
CSF_FUNC(std::uint32_t, __fastcall, AppendNamedEntry, 0x0071D300, (void* self, void* , int value, char* name, char* alternateText));

// Original: ecx = this, two stack args (a1, a2), ret 8.
// Forwards the menu manager's field_0014 object as the callee's self, passes a1 as value,
// a2 as name, and a null alternate text.
static std::uint32_t __fastcall C_008407a8_AppendNamedEntryNoAlternate(C_008407a8* self, void* /*edx*/, int a1, int a2)
{
    void* entryOwner = reinterpret_cast<void*>(self->field_0014);
    return AppendNamedEntry(entryOwner, nullptr, a1, reinterpret_cast<char*>(a2), nullptr);
}

HOOK(0x0050DE30, C_008407a8_AppendNamedEntryNoAlternate, "auto");
