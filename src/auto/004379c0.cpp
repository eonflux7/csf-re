#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// m_idLeft (.ID_LEFT in .fbs), a VC7.1 std::string, as a C string.
static void* __fastcall CGuiControl_GetIdLeft(CGuiControl* self, void* /*edx*/)
{
    return const_cast<char*>(self->m_idLeft.c_str());
}

HOOK(0x004379C0, CGuiControl_GetIdLeft, "auto");
