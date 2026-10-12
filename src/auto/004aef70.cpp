#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __cdecl, IsSubObjectFieldFlagSet, 0x00760FF0, (int a1));
CSF_FUNC(int, __cdecl, AiStatic_IsSubObjectUsable, 0x00761190, (unsigned int a1));

static C_008319c0* __fastcall S_004aef70_SelectUsableTarget(
    S_004aef70* self, void* /*edx*/)
{
    if (self->field_068c == UINT32_MAX)
    {
        void* source = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_002c), 0x44);
        C_008319c0* current =
            ((C_008319c0* (__fastcall*)(void*, void*))
                CSF_AT(void**, source, 0)[0xD8 / 4])(source, nullptr);

        if (current)
        {
            int valuePointer = Handle_GetValuePointer(current, nullptr);
            self->field_068c =
                CSF_AT(uint32_t, reinterpret_cast<void*>(valuePointer), 0);
        }
    }

    void* source = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_002c), 0x44);
    C_008319c0* result =
        ((C_008319c0* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0xD8 / 4])(source, nullptr);

    if (result)
    {
        int subObject = C_008319c0_GetSubObjectField74(result, nullptr);
        int usable = AiStatic_IsSubObjectUsable(
            static_cast<unsigned int>(subObject));

        if (static_cast<uint8_t>(usable) != 0)
        {
            int flagged = IsSubObjectFieldFlagSet(subObject);
            result = reinterpret_cast<C_008319c0*>(
                static_cast<uintptr_t>(flagged));

            if (static_cast<uint8_t>(flagged) == 0)
                return result;
        }
    }

    uint32_t count =
        ((uint32_t (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0xA0 / 4])(source, nullptr);
    result = reinterpret_cast<C_008319c0*>(static_cast<uintptr_t>(count));

    for (uint32_t index = 0; index < count; ++index)
    {
        result =
            ((C_008319c0* (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, source, 0)[0xC8 / 4])(
                    source, nullptr, index);

        if (result)
        {
            int subObject = C_008319c0_GetSubObjectField74(result, nullptr);
            int usable = AiStatic_IsSubObjectUsable(
                static_cast<unsigned int>(subObject));
            result = reinterpret_cast<C_008319c0*>(
                static_cast<uintptr_t>(usable));

            if (static_cast<uint8_t>(usable) != 0)
            {
                int flagged = IsSubObjectFieldFlagSet(subObject);
                result = reinterpret_cast<C_008319c0*>(
                    static_cast<uintptr_t>(flagged));

                if (static_cast<uint8_t>(flagged) == 0)
                {
                    if (index != UINT32_MAX)
                    {
                        return
                            ((C_008319c0* (__fastcall*)(
                                void*, void*, uint32_t))
                                CSF_AT(void**, source, 0)[0xF8 / 4])(
                                    source, nullptr, index);
                    }
                    return result;
                }
            }
        }
    }

    return result;
}

HOOK(0x004AEF70, S_004aef70_SelectUsableTarget, "auto");
