#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee: RenderWare frame accessor, returns the frame's LTM matrix pointer.
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));

// Writes the result into the caller-supplied out-struct at a2 and returns a2.
// If the particle atomic has an attached child object (field_0028), that object's
// virtual slot 35 (+0x8C) is invoked with a2 instead.
static int __fastcall C_00862748_GetLtmOrForward(C_00862748* self, void* /*edx*/, int a2)
{
    std::uint32_t childObject = self->field_0028;
    void* outStruct = reinterpret_cast<void*>(static_cast<std::uintptr_t>(a2));

    if (childObject != 0)
    {
        void* child = reinterpret_cast<void*>(static_cast<std::uintptr_t>(childObject));
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[140 / 4])(child, nullptr, a2);
    }
    else
    {
        void* frameHolder = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0024));
        std::uint32_t frame = CSF_AT(std::uint32_t, frameHolder, 4);
        std::uint32_t* ltm = RwFrameGetLTM(frame);
        CSF_AT(std::uint32_t, outStruct, 0) = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(ltm));
        CSF_AT(std::uint8_t, outStruct, 4) = 0;
    }
    return a2;
}

HOOK(0x006F3F40, C_00862748_GetLtmOrForward, "auto");
