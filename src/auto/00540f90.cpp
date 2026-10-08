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

// Callee: int __cdecl sub_692440(int code, int flag)
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x692440, (int code, int flag));

// Maps the stat/panel index to its four-character layout code and forwards
// it to the renderer with flag = 1. Index 0 -> "CAFD", 2 -> "FIFD",
// 3 -> "MOCD", anything else -> "ROND".
static int __cdecl MenuMpStats_RenderPanelByIndex(int panelIndex)
{
    switch (panelIndex)
    {
    case 0:
        return MenuMessageBox_ResolveEntryByKey(0x44464143, 1);
    case 2:
        return MenuMessageBox_ResolveEntryByKey(0x44444946, 1);
    case 3:
        return MenuMessageBox_ResolveEntryByKey(0x44434F4D, 1);
    }
    return MenuMessageBox_ResolveEntryByKey(0x444E4F52, 1);
}

HOOK(0x540F90, MenuMpStats_RenderPanelByIndex, "auto");
