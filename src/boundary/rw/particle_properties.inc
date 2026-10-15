// Property callbacks decoded from the executable. Stream data contains words,
// never host pointers. Access pointer-bearing payloads with memcpy: x86 packs
// pointers at +4 and native fixtures may place properties on 4-byte boundaries.
template<class T> T payloadGet(const void* p,size_t off=0) {
    T v; std::memcpy(&v,static_cast<const char*>(p)+off,sizeof(v)); return v;
}
template<class T> void payloadSet(void* p,size_t off,T v) {
    std::memcpy(static_cast<char*>(p)+off,&v,sizeof(v));
}
constexpr size_t pointerGrowth=sizeof(void*)-4;
CSF_GLOBAL(int32_t,particleStreamVersion,0x008BE6AC);
CSF_FUNC(Stream*,__cdecl,propertyReadWords,0x00629C90,(Stream*,void*,uint32_t));
CSF_FUNC(Stream*,__cdecl,propertyWriteWords,0x00629C70,(Stream*,const void*,uint32_t));
int propertyBytes(ParticleEmitter* e,int id) {
    auto* t=e->emitterClass->properties;
    for(int i=0;i<t->numProperties;i++) if(t->ids[i]==id) return t->sizes[i];
    return 0;
}
bool propertyIO(Stream* s,void* p,uint32_t n,bool read) {
    return read ? propertyReadWords(s,p,n)!=nullptr : propertyWriteWords(s,p,n)!=nullptr;
}
template<int Id,int Bytes,int BoolWord=-1,bool ResetLast=false>
ParticleEmitter* __cdecl fixedClone(Atomic*,ParticleEmitter* s,ParticleEmitter* d) {
    void* a=particleProperty(s,Id);void* b=particleProperty(d,Id);
    if(a && b) {
        std::memcpy(b,a,Bytes);
        if(ResetLast) payloadSet<void*>(b,Bytes-4,nullptr);
    }
    return d;
}
template<int Id,int Bytes,int BoolWord=-1,bool ResetLast=false>
ParticleEmitter* __cdecl fixedSize(Atomic*,ParticleEmitter* e,int* size) {
    *size=particleProperty(e,Id) ? Bytes-(ResetLast ? 4 : 0) : 0; return e;
}
template<int Id,int Bytes,int BoolWord=-1,bool ResetLast=false,bool Read=true>
ParticleEmitter* __cdecl fixedStream(Atomic*,ParticleEmitter* e,Stream* s) {
    auto* p=static_cast<char*>(particleProperty(e,Id));
    if(p) for(int i=0;i<Bytes/4-(ResetLast ? 1 : 0);i++) {
        uint32_t word=payloadGet<uint32_t>(p,4*i);
        if(!Read && i==BoolWord) word=word==1;
        if(!propertyIO(s,&word,4,Read)) return nullptr;
        if(Read) payloadSet(p,4*i,i==BoolWord ? uint32_t(word==1) : word);
    }
    return e;
}

// Multi-color, multi-texcoord and multi-size share a counted inline payload.
// Offsets in their emitter header address the *particle* payload, not emitter.
template<int Id> constexpr int multiHeader() { return Id==0x1000003 || Id==0x1000004 ? 28 : 20; }
template<int Id> constexpr int multiElement() { return Id==0x1000005 ? 24 : 40; }
template<int Id> void multiOffsets(void* p,int n,bool initialize=true) {
    size_t o=8+pointerGrowth;
    payloadSet<int>(p,o,4);payloadSet<int>(p,o+4,4+4*n);
    if constexpr(Id==0x1000002) payloadSet<int>(p,o+8,4+20*n);
    if constexpr(Id==0x1000003) {
        payloadSet<int>(p,o+8,4+12*n);payloadSet<int>(p,o+12,4+20*n);payloadSet<int>(p,o+16,4+28*n);
    }
    if constexpr(Id==0x1000004) {
        if(initialize) { payloadSet<int>(p,o+8,0);payloadSet<int>(p,o+16,0); }
        payloadSet<int>(p,o+12,4+12*n);
    }
    if constexpr(Id==0x1000005) payloadSet<int>(p,o+8,4+12*n);
}
template<int Id> ParticleEmitter* __cdecl multiCreate(Atomic*,ParticleEmitter* e,void*) {
    auto* p=static_cast<char*>(particleProperty(e,Id));
    if(p) {
        int n=(propertyBytes(e,Id)-multiHeader<Id>())/multiElement<Id>();
        if(n<0) return nullptr;
        payloadSet(p,0,n);payloadSet<void*>(p,4,p+multiHeader<Id>()+pointerGrowth);multiOffsets<Id>(p,n);
    }
    return e;
}
template<int Id> ParticleEmitter* __cdecl multiClone(Atomic*,ParticleEmitter* s,ParticleEmitter* d) {
    auto* a=static_cast<char*>(particleProperty(s,Id));auto* b=static_cast<char*>(particleProperty(d,Id));
    if(a && b) {
        int n=payloadGet<int>(a);
        size_t bytes=multiHeader<Id>()+size_t(std::max(n,0))*multiElement<Id>();
        if(n<0 || bytes>size_t(propertyBytes(s,Id)) || bytes>size_t(propertyBytes(d,Id))) return nullptr;
        std::memcpy(b,a,bytes+pointerGrowth);payloadSet<void*>(b,4,b+multiHeader<Id>()+pointerGrowth);
    }
    return d;
}
template<int Id> ParticleEmitter* __cdecl multiSize(Atomic*,ParticleEmitter* e,int* size) {
    auto* p=particleProperty(e,Id);*size=p ? 4+multiElement<Id>()*payloadGet<int>(p) : 0;return e;
}
template<int Id,bool Read> bool multiIO(ParticleEmitter* e,Stream* s) {
    void* p=particleProperty(e,Id);if(!p) return true;
    int n=payloadGet<int>(p);
    if(!propertyIO(s,&n,4,Read)) return false;
    // Unlike the original unchecked read, malformed counts cannot overrun the
    // class allocation. Do not publish a count until its capacity is checked.
    if(n<0 || n>(propertyBytes(e,Id)-multiHeader<Id>())/multiElement<Id>()) return false;
    if(Read) payloadSet(p,0,n);
    if(n && !propertyIO(s,payloadGet<void*>(p,4),n*multiElement<Id>(),Read)) return false;
    if(Read && n && particleStreamVersion<3) multiOffsets<Id>(p,n,false);
    return true;
}
template<int Id,bool Read> ParticleEmitter* __cdecl multiStream(Atomic*,ParticleEmitter* e,Stream* s) {
    return multiIO<Id,Read>(e,s) ? e : nullptr;
}
ParticleEmitter* __cdecl multiPropsCreate(Atomic* a,ParticleEmitter* e,void* d) {
    return multiCreate<0x1000002>(a,e,d) && multiCreate<0x1000003>(a,e,d) && multiCreate<0x1000005>(a,e,d) ? e : nullptr;
}
ParticleEmitter* __cdecl multiPropsClone(Atomic* a,ParticleEmitter* s,ParticleEmitter* d) {
    return multiClone<0x1000002>(a,s,d) && multiClone<0x1000003>(a,s,d) && multiClone<0x1000005>(a,s,d) ? d : nullptr;
}
ParticleEmitter* __cdecl multiPropsSize(Atomic* a,ParticleEmitter* e,int* size) {
    int x,y,z;multiSize<0x1000002>(a,e,&x);multiSize<0x1000003>(a,e,&y);multiSize<0x1000005>(a,e,&z);*size=x+y+z;return e;
}
template<bool Read> ParticleEmitter* __cdecl multiPropsStream(Atomic*,ParticleEmitter* e,Stream* s) {
    return multiIO<0x1000002,Read>(e,s) && multiIO<0x1000003,Read>(e,s) && multiIO<0x1000005,Read>(e,s) ? e : nullptr;
}

constexpr int pointId=0x1000006;
ParticleEmitter* __cdecl pointCreate(Atomic*,ParticleEmitter* e,void*) {
    auto* p=static_cast<char*>(particleProperty(e,pointId));if(!p) return e;
    int n=(propertyBytes(e,pointId)-24)/24;if(n<0) return nullptr;
    payloadSet(p,0,n);payloadSet<int>(p,4,0);payloadSet<int>(p,8,0);payloadSet<int>(p,12,0);
    payloadSet<void*>(p,16,p+24+2*pointerGrowth);
    payloadSet<void*>(p,20+pointerGrowth,p+24+2*pointerGrowth+12*n);return e;
}
ParticleEmitter* __cdecl pointClone(Atomic*,ParticleEmitter* s,ParticleEmitter* d) {
    auto* a=particleProperty(s,pointId);auto* b=particleProperty(d,pointId);
    if(a && b) {
        int n=payloadGet<int>(a);if(n<0 || n>(propertyBytes(d,pointId)-24)/24) return nullptr;
        for(int o:{0,8,12}) payloadSet(b,o,payloadGet<int>(a,o));
        std::memcpy(payloadGet<void*>(b,16),payloadGet<void*>(a,16),12*n);
        if(payloadGet<int>(a,8)==1) std::memcpy(payloadGet<void*>(b,20+pointerGrowth),payloadGet<void*>(a,20+pointerGrowth),12*n);
    }
    return d;
}
ParticleEmitter* __cdecl pointSize(Atomic*,ParticleEmitter* e,int* size) {
    auto* p=particleProperty(e,pointId);*size=0;
    if(p) *size=20+12*payloadGet<int>(p)*(int(payloadGet<void*>(p,16)!=nullptr)+int(payloadGet<void*>(p,20+pointerGrowth)!=nullptr));
    return e;
}
template<bool Read> ParticleEmitter* __cdecl pointStream(Atomic*,ParticleEmitter* e,Stream* s) {
    auto* p=particleProperty(e,pointId);if(!p) return e;
    int n=payloadGet<int>(p);if(!propertyIO(s,&n,4,Read)) return nullptr;
    if(n<0 || n>(propertyBytes(e,pointId)-24)/24) return nullptr;
    if(Read) payloadSet(p,0,n);
    for(int o:{8,12}) {
        int b=payloadGet<int>(p,o)==1;if(!propertyIO(s,&b,4,Read)) return nullptr;
        if(Read) payloadSet<int>(p,o,b==1);
    }
    for(size_t o:{size_t(16),20+pointerGrowth}) {
        void* data=payloadGet<void*>(p,o);int present=data!=nullptr;
        if(!propertyIO(s,&present,4,Read)) return nullptr;
        if(present==1) { if(!data || !propertyIO(s,data,12*n,Read)) return nullptr; }
        else if(Read) payloadSet<void*>(p,o,nullptr);
    }
    return e;
}

// These callbacks switch at most one interpolation segment per invocation,
// even if a large dt crosses several keys. They run before standard compaction.
template<int Id,int Target> void multiAdvance(ParticleEmitter* e,unsigned char* particle,double age) {
    void* emitter=particleProperty(e,Id);if(!emitter) return;
    int source=propOffset(e->particleClass->properties,Id),target=propOffset(e->particleClass->properties,Target);
    if(source<0 || target<0) return;
    auto* p=particle+source;uint32_t key=payloadGet<uint32_t>(p);
    if(key>=uint32_t(payloadGet<int>(emitter)) || !(age>=payloadGet<float>(p,4+4*key))) return;
    const size_t meta=8+pointerGrowth;
    auto copy=[&](int word,int dest,int bytes,int element) {
        int offset=payloadGet<int>(emitter,meta+4*word);
        std::memcpy(particle+target+dest,p+offset+key*element,bytes);
    };
    if constexpr(Id==0x1000002) { copy(1,0,16,16);copy(2,16,16,16); }
    if constexpr(Id==0x1000003) { copy(1,0,8,8);copy(2,16,8,8);copy(3,8,8,8);copy(4,24,8,8); }
    if constexpr(Id==0x1000004) { copy(1,0,8,8);copy(3,8,8,8); }
    if constexpr(Id==0x1000005) { copy(1,0,8,8);copy(2,8,8,8); }
    payloadSet(p,0,key+1);
}
template<bool Stepped> ParticleBatch* __cdecl multiParticleUpdate(ParticleEmitter* e,ParticleBatch* b,const float* dt) {
    if(!e || !e->particleClass || !b || !dt) return nullptr;
    auto* p=reinterpret_cast<unsigned char*>(b)+b->dataOffset;
    for(int i=0;i<b->numParticles;i++,p+=e->particleClass->objectSize) {
        float age=payloadGet<float>(p,4),death=payloadGet<float>(p,8);
        if(!(age<death)) continue;
        double next=double(age)+*dt;
        if constexpr(Stepped) multiAdvance<0x1000004,3>(e,p,next);
        else { multiAdvance<0x1000002,2>(e,p,next);multiAdvance<0x1000003,3>(e,p,next);multiAdvance<0x1000005,5>(e,p,next); }
    }
    return b;
}
