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

// Helpers the original calls, bound by address.
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(int, __fastcall, sub_66B590, 0x0066B590, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_64AD60, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, sub_64CEB0, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));

// Writes three config dwords through the object returned by sub_64AD60, then calls sub_64CEB0(this, 1, obj).
// __thiscall (ret 0xC): ecx = this, three stack arguments.
static char __fastcall C_00858180_WriteConfigTriplet(C_00858180* self, void* /*edx*/, char a2, char a3, char a4)
{
    // Temporary string object on the stack; its first dword holds `this`.
    alignas(8) uint8_t tempStringBytes[0xC];
    *reinterpret_cast<void**>(tempStringBytes) = self;

    InitializeTemporaryString(tempStringBytes, nullptr, CSF_ADDR_DATA(0x8679EC));
    int keyValue = sub_66B590(tempStringBytes, nullptr);

    void* configObject = sub_64AD60(self, nullptr, (int)&keyValue);

    GameConfig_WriteDwordField(configObject, nullptr, a2);
    GameConfig_WriteDwordField(configObject, nullptr, a3);
    GameConfig_WriteDwordField(configObject, nullptr, a4);

    return sub_64CEB0(self, nullptr, 1, configObject);
}

HOOK(0x742670, C_00858180_WriteConfigTriplet, "auto");
