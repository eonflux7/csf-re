#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original 0x6B1380: int __cdecl GetGlobalInt(void)
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, (void));

// Original 0x6B1660: int __thiscall (this)
CSF_FUNC(int, __fastcall, MenuRuntime_ReinitForGlobal, 0x006B1660, (void* self, void* edx));

// Original 0x4109E0: std::_String_base* __thiscall (this, source, offset, count), ret 12
CSF_FUNC(void*, __fastcall, StringBase_AssignSubstr, 0x004109E0,
         (void* self, void* edx, const void* source, uint32_t offset, uint32_t count));

// Original 0x541580: __cdecl (string, 2, 0) as pushed by the caller; caller pops 4 and reuses
// the two extra pushed words as arguments of MenuRuntime_SetPageByIndex.
CSF_FUNC(void*, __cdecl, MenuPage_LookupByName, 0x00541580,
         (void* stringObject, int unusedArg2, int unusedArg3));

// Original 0x6B15D0: char __thiscall (this, a2, a3, a4), ret 12
CSF_FUNC(char, __fastcall, MenuRuntime_SetPageByIndex, 0x006B15D0,
         (void* self, void* edx, int a2, int a3, int a4));

// Original 0x690E60: int __cdecl Mem_FreeDebug(void* Block)
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Switches to the menu screen named by the string stored at this+4, then marks the fader state as 3.
static void* __fastcall CMenuScreenFader_SwitchScreenByName(CMenuScreenFader* self, void* /*edx*/,
                                                           int /*unusedArgument*/)
{
    // Runtime object lookup (result discarded, as in the original).
    MenuRuntime_ReinitForGlobal(reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt())), nullptr);

    // Local std::string-like object at [ebp-0x1c]:
    //   +0x04 = buffer pointer (Block), +0x14 = size, +0x18 = capacity.
    uint8_t localString[0x1C];
    *reinterpret_cast<uint32_t*>(localString + 0x18) = 0x0F;
    *reinterpret_cast<uint32_t*>(localString + 0x14) = 0;
    localString[0x04] = 0;

    StringBase_AssignSubstr(localString, nullptr,
                            reinterpret_cast<uint8_t*>(self) + 4, 0u, 0xFFFFFFFFu);

    void* lookedUpPage = MenuPage_LookupByName(localString, 2, 0);
    int pageHandle = static_cast<int>(reinterpret_cast<uintptr_t>(lookedUpPage));

    char pageSetByte = MenuRuntime_SetPageByIndex(
        reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt())), nullptr, pageHandle, 2, 0);
    void* result = reinterpret_cast<void*>(static_cast<intptr_t>(pageSetByte));

    uint32_t bufferCapacity = *reinterpret_cast<uint32_t*>(localString + 0x18);
    self->m_state = 3;

    if (bufferCapacity < 0x10u)
    {
        return result;
    }

    void* block = *reinterpret_cast<void**>(localString + 0x04);
    result = block;
    if (block)
    {
        return reinterpret_cast<void*>(static_cast<intptr_t>(Mem_FreeDebug(block)));
    }
    return result;
}

HOOK(0x0050C1B0, CMenuScreenFader_SwitchScreenByName, "auto");
