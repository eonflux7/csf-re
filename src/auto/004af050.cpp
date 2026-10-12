#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __cdecl, IsSubObjectFieldFlagSet, 0x00760FF0, (int a1));

static C_00862490* __fastcall AiFncStatic_PrepareLinkedObject(
    void* self, void* /*edx*/)
{
    void* linkedObject = CSF_AT(void*, CSF_AT(void*, self, 0x2C), 0x44);
    C_008319c0* currentObject =
        ((C_008319c0* (__fastcall*)(void*, void*))
            CSF_AT(void**, linkedObject, 0)[0xD8 / 4])(
                linkedObject, nullptr);
    std::uintptr_t result = reinterpret_cast<std::uintptr_t>(currentObject);

    if (CSF_AT(int, self, 0x68C) == -1)
    {
        void* refreshedLinkedObject =
            CSF_AT(void*, CSF_AT(void*, self, 0x2C), 0x44);
        void* handle =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, refreshedLinkedObject, 0)[0xD8 / 4])(
                    refreshedLinkedObject, nullptr);
        result = reinterpret_cast<std::uintptr_t>(handle);
        if (handle)
        {
            int valueAddress = Handle_GetValuePointer(handle, nullptr);
            result = CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(
                    static_cast<std::uint32_t>(valueAddress))), 0);
            CSF_AT(std::uint32_t, self, 0x68C) =
                static_cast<std::uint32_t>(result);
        }
    }

    if (currentObject)
    {
        int fieldValue = C_008319c0_GetSubObjectField74(currentObject, nullptr);
        result = static_cast<std::uint32_t>(IsSubObjectFieldFlagSet(fieldValue));
        if (static_cast<std::uint8_t>(result) == 0)
        {
            std::uint32_t count =
                ((std::uint32_t (__fastcall*)(void*, void*))
                    CSF_AT(void**, linkedObject, 0)[0xA0 / 4])(
                        linkedObject, nullptr);
            result = count;

            for (std::uint32_t index = 0; index < count; ++index)
            {
                C_008319c0* candidate =
                    ((C_008319c0* (__fastcall*)(void*, void*, std::uint32_t))
                        CSF_AT(void**, linkedObject, 0)[0xC8 / 4])(
                            linkedObject, nullptr, index);
                fieldValue = C_008319c0_GetSubObjectField74(candidate, nullptr);
                result = static_cast<std::uint32_t>(
                    IsSubObjectFieldFlagSet(fieldValue));
                if (static_cast<std::uint8_t>(result) != 0)
                {
                    if (index != static_cast<std::uint32_t>(-1))
                    {
                        return ((C_00862490* (__fastcall*)(
                                    void*, void*, std::uint32_t))
                            CSF_AT(void**, linkedObject, 0)[0xF8 / 4])(
                                linkedObject, nullptr, index);
                    }
                    break;
                }
            }
        }
    }

    return reinterpret_cast<C_00862490*>(result);
}

HOOK(0x004AF050, AiFncStatic_PrepareLinkedObject, "auto");
