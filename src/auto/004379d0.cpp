#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// m_idRight (.ID_RIGHT in .fbs), a VC7.1 std::string, as a C string.
static char* __fastcall CGuiControl_GetIdRight(CGuiControl* self, void* /*edx*/)
{
    return const_cast<char*>(self->m_idRight.c_str());
}

HOOK(0x4379D0, CGuiControl_GetIdRight, "auto");
