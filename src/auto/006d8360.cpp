#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(std::uint32_t, __fastcall, GfxPtank_UpdateParticlePlacement, 0x006D7DA0, (void* self, void* /*edx*/, void* source));

// Updates the additional-object list of a PTank skin once per frame.
// Returns the frame counter value (or the current counter when the update is skipped).
static int __fastcall GfxSkin_UpdateAdditionalObject(void* self, void* /*edx*/)
{
    uint8_t* obj = static_cast<uint8_t*>(self);
    int result = CSF_AT(int, obj, 0xA4);
    if (result != 0)
    {
        result = CSF_AT(int, obj, 0xF0);
        if (result == 0)
        {
            int globalValue = GetGlobalValue_6C5760();
            uint8_t* globalBase = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(globalValue));
            GfxPtank_UpdateParticlePlacement(obj, nullptr, CSF_AT(void*, globalBase, 0xF0));

            CSF_AT(uint8_t, obj, 0x118) = 1;

            uint32_t index = 0;
            if (CSF_AT(uint32_t, obj, 0xD0) != 0)
            {
                do
                {
                    uint8_t* listBase = CSF_AT(uint8_t*, obj, 0xC8);
                    void* item = reinterpret_cast<void**>(listBase)[index];
                    uint8_t* member = static_cast<uint8_t*>(item) + 0x20;

                    void** memberVtbl = CSF_AT(void**, member, 0);
                    auto findFn = reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(memberVtbl[6]);
                    void* found = findFn(member, nullptr, CSF_AT(uint32_t, obj, 0xF0));

                    if (found != nullptr)
                    {
                        void** foundVtbl = CSF_AT(void**, found, 0);
                        reinterpret_cast<void (__fastcall*)(void*, void*)>(foundVtbl[31])(found, nullptr);
                    }
                    ++index;
                } while (index < CSF_AT(uint32_t, obj, 0xD0));
            }

            result = static_cast<int>(CSF_AT(uint32_t, obj, 0xF0) + 1);
            CSF_AT(uint8_t, obj, 0x118) = 0;
            CSF_AT(int, obj, 0xF0) = result;
        }
    }
    return result;
}

HOOK(0x006D8360, GfxSkin_UpdateAdditionalObject, "auto");
