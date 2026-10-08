#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const std::uint32_t kGuiBuilderVtableAddress = CSF_ADDR_VT(0x00864B30);
static const std::uint32_t kBaseBuilderVtableAddress = CSF_ADDR_VT(0x00864A58);

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110,
         (void* storage));

static int __fastcall CGuiBuilder_Destroy(
    CGuiBuilder* self, void* /*edx*/)
{
    const std::uint32_t count = self->field_0014;
    CSF_AT(std::uint32_t, self, 0) = kGuiBuilderVtableAddress;

    for (std::uint32_t index = 0; index < count; ++index)
    {
        void* object = reinterpret_cast<void**>(self->field_000c)[index];
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);

            // Destruction can change the array pointer.
            reinterpret_cast<void**>(self->field_000c)[index] = nullptr;
        }
    }

    void* object = reinterpret_cast<void*>(self->field_001c);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_001c = 0;
    }

    void* referencedObject = reinterpret_cast<void*>(self->field_0084);
    if (referencedObject)
    {
        ReleaseObjectReference(
            static_cast<std::uint8_t*>(referencedObject) + 4, nullptr);
        self->field_0084 = 0;
    }

    void* storage = reinterpret_cast<void*>(self->field_000c);
    int result = 0;
    if (storage)
    {
        result = Mem_Free(storage);
        self->field_000c = 0;
    }

    CSF_AT(std::uint32_t, self, 0) = kBaseBuilderVtableAddress;
    return result;
}

HOOK(0x0071E090, CGuiBuilder_Destroy, "auto");
