#include <cstdint>
#ifdef CSF_STANDALONE
void csf_script_payload_failed(void*, int);
#endif
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, AllocateScratchEntries, 0x004723D0, (int a1));

static uint8_t* __fastcall ReserveScratchEntries(void* self, void* /*edx*/, int count)
{
    auto* entries = reinterpret_cast<uint8_t*>(AllocateScratchEntries(count));
    CSF_AT(uint8_t*, self, 0x04) = entries;
#ifdef CSF_STANDALONE
    if (!entries) {
        csf_script_payload_failed(self, count);
    }
#endif
    if (entries)
        *entries = static_cast<uint8_t>(count);
    return entries;
}

HOOK(0x004725C0, ReserveScratchEntries, "auto");
