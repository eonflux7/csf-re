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

CSF_FUNC(char, __fastcall, sub_44F8C0, 0x0044F8C0, (void* self, void* edx, char a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads a byte from the record reached through the argument's double pointer chain
// (*(*(arg) + 4) + 1) and forwards it, together with the current scene, to sub_44F8C0.
static char __stdcall ForwardSceneRecordByteToControl(int /*a1*/, int /*a2*/, int a3)
{
    void* argPtr = reinterpret_cast<void*>(a3);
    void* recordPtr = *CSF_AT(void**, argPtr, 0);
    void* subRecord = *CSF_AT(void**, recordPtr, 4);
    uint8_t flagByte = *CSF_AT(uint8_t*, subRecord, 1);
    return sub_44F8C0(g_pCurrentScene, nullptr, static_cast<char>(flagByte));
}

HOOK(0x45BA40, ForwardSceneRecordByteToControl, "auto");
