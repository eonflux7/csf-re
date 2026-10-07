#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBoxBase_DispatchEvent13ForNonemptyList(
    CGuiListBoxBase* self, void* /*edx*/)
{
    int result =
        (reinterpret_cast<int (__fastcall*)(CGuiListBoxBase*, void*)>(
            CSF_AT(void**, self, 0)[0x140 / 4]))(self, nullptr);

    if ((result & 1) != 0)
    {
        const uint32_t itemsBegin = self->field_01ac;
        if (itemsBegin != 0)
        {
            result = static_cast<int32_t>(self->field_01b0 - itemsBegin) >> 2;
            if (result > 0)
            {
                (reinterpret_cast<void (__fastcall*)(CGuiListBoxBase*, void*, int)>(
                    CSF_AT(void**, self, 0)[0x138 / 4]))(self, nullptr, 13);

                result = static_cast<int32_t>(self->field_0010);
                void* link = CSF_AT(
                    void*, reinterpret_cast<void*>(static_cast<uint32_t>(result)), 0);
                void* recipient = CSF_AT(void*, link, 0);
                if (recipient != nullptr)
                {
                    return
                        (reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, recipient, 0)[0x24 / 4]))(
                                recipient, nullptr, 0);
                }
            }
        }
    }

    return result;
}

HOOK(0x007149F0, CGuiListBoxBase_DispatchEvent13ForNonemptyList, "auto");
