#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static char __fastcall C_008316d8_CopyEntryMatrix(
    C_008316d8* self, void* /*edx*/, int entryId, void** output)
{
    struct MatrixReference {
        std::uint32_t* matrix;
        unsigned char owned;
        unsigned char padding[3];
    };
    struct SavedMatrix {
        std::uint32_t* matrix;
        unsigned char owned;
        unsigned char padding[3];
        void* entry;
    };

    MatrixReference temporary{};
    SavedMatrix saved{};

    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* entries =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])
        (owner, nullptr);
    int index =
        ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, entries, 0)[0x98 / 4])
        (entries, nullptr, entryId);

    void* selected = nullptr;
    if (index != -1) {
        selected =
            ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, entries, 0)[0x8C / 4])
            (entries, nullptr, index);
        saved.entry = selected;
    }

    if (index == -1 || selected == nullptr) {
        entries = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
        index =
            ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, entries, 0)[0x98 / 4])
            (entries, nullptr, entryId);
        if (index == -1)
            return 0;

        selected =
            ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, entries, 0)[0x8C / 4])
            (entries, nullptr, index);
        saved.entry = selected;
        if (!selected)
            return 0;
    }

    bool replacedMatrix = false;
    saved.matrix = RwMatrixCreate();
    owner = reinterpret_cast<void*>(self->field_0004);
    saved.owned = 1;

    if (!CSF_AT(std::uint32_t, owner, 0x50) &&
        !CSF_AT(unsigned char, owner, 0x228)) {
        void* root =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])
            (owner, nullptr);
        void* transform =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, root, 0)[0x18 / 4])
            (root, nullptr);
        void* reference =
            ((void* (__fastcall*)(void*, void*, void*))CSF_AT(void**, transform, 0)[0x8C / 4])
            (transform, nullptr, &temporary);
        std::memcpy(saved.matrix, CSF_AT(const void*, reference, 0), 0x40);
        if (temporary.owned)
            RwMatrixDestroy(temporary.matrix);

        int globalValue = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
        void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
        void* referenced = GetReferencedObject(provider, nullptr);
        void* replacementTransform =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, referenced, 0)[0x10 / 4])
            (referenced, nullptr);

        owner = reinterpret_cast<void*>(self->field_0004);
        root =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])
            (owner, nullptr);
        transform =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, root, 0)[0x18 / 4])
            (root, nullptr);
        void** transformVtable = CSF_AT(void**, transform, 0);

        reference =
            ((void* (__fastcall*)(void*, void*, void*))CSF_AT(void**, replacementTransform, 0)[0x8C / 4])
            (replacementTransform, nullptr, &temporary);
        ((void (__fastcall*)(void*, void*, void*))transformVtable[0x94 / 4])
            (transform, nullptr, reference);
        if (temporary.owned)
            RwMatrixDestroy(temporary.matrix);

        selected = saved.entry;
        replacedMatrix = true;
    }

    void* entryTransform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, selected, 0)[0x18 / 4])
        (selected, nullptr);
    void* entryReference =
        ((void* (__fastcall*)(void*, void*, void*))CSF_AT(void**, entryTransform, 0)[0x8C / 4])
        (entryTransform, nullptr, &temporary);
    std::memcpy(*output, CSF_AT(const void*, entryReference, 0), 0x40);
    if (temporary.owned)
        RwMatrixDestroy(temporary.matrix);

    if (replacedMatrix) {
        owner = reinterpret_cast<void*>(self->field_0004);
        void* root =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])
            (owner, nullptr);
        void* transform =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, root, 0)[0x18 / 4])
            (root, nullptr);
        ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, transform, 0)[0x94 / 4])
            (transform, nullptr, &saved);
    }

    if (saved.owned)
        RwMatrixDestroy(saved.matrix);
    return 1;
}

HOOK(0x00475DD0, C_008316d8_CopyEntryMatrix, "auto");
