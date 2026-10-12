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

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ProcessSubObjectRecord, 0x00443000, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_UnitScaleFactor, 0x0082E25C);

static int __fastcall S_00536940_SubmitScaledRecord(S_00536940* self, void* /*edx*/, float seconds, char flag)
{
    // 0x18-byte record on the stack, passed to ProcessSubObjectRecord.
    alignas(8) uint8_t recordBytes[0x18];
    std::memset(recordBytes, 0, sizeof(recordBytes));

    CSF_AT(uint32_t, recordBytes, 0x00) = 0;
    CSF_AT(float, recordBytes, 0x04) = seconds;
    CSF_AT(uint32_t, recordBytes, 0x08) = 0;
    CSF_AT(char, recordBytes, 0x0C) = flag;
    CSF_AT(int, recordBytes, 0x10) = static_cast<int>(seconds * g_UnitScaleFactor);
    CSF_AT(int, recordBytes, 0x14) = 0;

    C_00867260* globalValue = reinterpret_cast<C_00867260*>(GetGlobalValue_4FBDE0());
    void* globalSubObject = GetGlobalSubObject_4FC0A0(globalValue, nullptr);
    int result = ProcessSubObjectRecord(globalSubObject, nullptr,
        static_cast<int>(reinterpret_cast<intptr_t>(recordBytes)));

    self->field_0144 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x536940, S_00536940_SubmitScaledRecord, "auto");
