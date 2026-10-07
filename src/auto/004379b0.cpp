
#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// m_id (.ID in .fbs), a VC7.1 std::string, as a C string.
static char* __fastcall CGuiControl_GetId(CGuiControl* self, void* /*edx*/)
{
    return const_cast<char*>(self->m_id.c_str());
}

HOOK(0x4379B0, CGuiControl_GetId, "auto");
