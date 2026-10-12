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

CSF_FUNC(int, __fastcall, GfxSkin_ComputeRenderFlags, 0x006B30B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_IsValid, 0x006E5670, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Handle_ApplyRenderFlags, 0x006E68E0, (C_00861c08* self, void* edx, uint8_t a2, char a3));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

// The skin object's vtable (referenced by the original's call target at 0x6B30B0).
[[maybe_unused]] static const auto kGfxSkinVtable = CSF_VTBL(0x006B30B0);

static unsigned int __fastcall GfxSkin_RenderObject(S_006b30b0* self, void* /*edx*/)
{
    unsigned int result = self->field_000c;
    if (result)
    {
        result = self->field_0064 & 0x140;
        if (result == 320)
        {
            const int renderFlags = GfxSkin_ComputeRenderFlags(self, nullptr);

            result = self->field_0028;
            if (result)
            {
                void* skinObject = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
                result = static_cast<unsigned int>(
                    ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, skinObject, 0)[9]))(skinObject, nullptr, 1));
            }

            uint32_t index = 0;
            if (self->field_001c != 0)
            {
                do
                {
                    const uint32_t handleArray = self->field_0018;
                    C_00861c08* handle = reinterpret_cast<C_00861c08**>(static_cast<uintptr_t>(handleArray))[index];

                    if (handle != nullptr &&
                        static_cast<uint8_t>(Handle_IsValid(handle, nullptr)) != 0)
                    {
                        Handle_ApplyRenderFlags(handle, nullptr, static_cast<uint8_t>(renderFlags), 1);

                        const int valuePointerRaw = Handle_GetValuePointer(handle, nullptr);
                        if (valuePointerRaw != 0)
                        {
                            void* valuePointer = reinterpret_cast<void*>(static_cast<uintptr_t>(valuePointerRaw));
                            ((void(__fastcall*)(void*, void*))(CSF_AT(void**, valuePointer, 0)[5]))(valuePointer, nullptr);
                        }
                    }

                    result = self->field_001c;
                    ++index;
                } while (index < result);
            }
        }
    }
    return result;
}

HOOK(0x6B3900, GfxSkin_RenderObject, "auto");
