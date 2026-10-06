#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Constant string "TIrAPuesto" living in the game's data segment.
CSF_GLOBAL(char, g_szIrAPuesto, 0x00834A40);

// Returns the address of the constant string "TIrAPuesto" (0x00834A40).
static const char* __cdecl GetIrAPuestoString()
{
    return &g_szIrAPuesto;
}

HOOK(0x004A6E90, GetIrAPuestoString, "auto");
