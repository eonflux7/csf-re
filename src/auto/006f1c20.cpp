#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, sub_6F1A00, 0x006F1A00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_6CA030, 0x006CA030, (void* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall C_00862490_AllocateAndBindChild(C_00862490* self, void* /*edx*/)
{
    void* rawMemory = Mem_Alloc(0x90);
    if (rawMemory)
    {
        uint32_t* createdObject = sub_6F1A00(rawMemory, nullptr);
        sub_6CA030(self, nullptr, createdObject);
        return createdObject;
    }

    sub_6CA030(self, nullptr, nullptr);
    return nullptr;
}

HOOK(0x6F1C20, C_00862490_AllocateAndBindChild, "auto");
