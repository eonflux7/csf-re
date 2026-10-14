// Included inside particles.cpp's private namespace. Layouts and event order
// derived from 57CFE0/57DBA0/57BBD0; librw has no particle toolkit counterpart.
CSF_FUNC(double,__cdecl,particleNormalize,0x00623250,(rw::V3d*,const rw::V3d*));
CSF_FUNC(double,__cdecl,particleSqrt,0x00622EE0,(float));
CSF_FUNC(rw::V3d*,__cdecl,particlePoint,0x00623010,(rw::V3d*,const rw::V3d*,const rw::Matrix*));
CSF_FUNC(rw::V3d*,__cdecl,particleVector,0x00623070,(rw::V3d*,const rw::V3d*,const rw::Matrix*));
CSF_FUNC(rw::Matrix*,__cdecl,particleMultiply,0x00624630,(rw::Matrix*,const rw::Matrix*,const rw::Matrix*));
float* particleData(unsigned char* p,ParticleClass* c,int id) {
    int off=propOffset(c->properties,id);
    return off<0 ? nullptr : reinterpret_cast<float*>(p+off);
}
float* emitterData(ParticleEmitter* e,int id) { return static_cast<float*>(particleProperty(e,id)); }
double particleRandom(uint32_t& seed) {
    seed=seed*0x0bb38435u+0x3619636bu;
    return double(seed)*0x1p-32*2-1;
}
float vary(uint32_t& seed,float base,float bias) { return float(particleRandom(seed)*bias+base); }
rw::V3d particleCross(const rw::V3d& a,const rw::V3d& b) {
    return {float(double(a.y)*b.z-double(a.z)*b.y),float(double(a.z)*b.x-double(a.x)*b.z),float(double(a.x)*b.y-double(a.y)*b.x)};
}
struct ParticleClusters {
    EmitterTank* tank;
    unsigned char *in[6]{},*out[6]{};
    int stride[6]{};
    static constexpr uint32_t flags[6]={1,2,4,8,32,128};
    explicit ParticleClusters(EmitterTank* t):tank(t) {
        if(!t) return;
        int k=0;
        for(int i=0;i<6;i++) if(t->dataFlags&flags[i]) {
            in[i]=t->input ? t->input[k] : nullptr;
            out[i]=t->output[k];
            stride[i]=t->strides[k++];
        }
    }
    bool enabled(int i,bool emission=false) const {
        return tank && (tank->dataFlags & (emission ? uint32_t(tank->platformFlags) : uint32_t(tank->flags)) & flags[i]);
    }
    void finish(int count,bool emission=false) {
        if(!tank) return;
        tank->numParticles+=count;
        int k=0;
        for(int i=0;i<6;i++) if(tank->dataFlags&flags[i]) {
            if(!emission) tank->input[k]=in[i];
            tank->output[k++]=out[i];
        }
    }
};
ParticleBatch* __cdecl standardParticleUpdate(ParticleEmitter* e,ParticleBatch* b,const float* delta) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    if(!s || !b || !delta || !e->particleClass) return nullptr;
    auto* t=static_cast<EmitterTank*>(particleProperty(e,6));
    ParticleClusters clusters(t);
    int positionCluster=(t && (t->dataFlags&8)) ? 3 : 0;
    int positionStride=clusters.enabled(3) ? clusters.stride[3] : clusters.enabled(0) ? clusters.stride[0] : 0;
    auto* matrix=emitterData(e,8);
    if(matrix && matrix[28]==0) matrix=nullptr;
    float dt=*delta, radius=0;
    rw::V3d origin=s->ltm ? s->ltm->pos : rw::V3d{0,0,0};
    int live=0, bytes=e->particleClass->objectSize;
    auto* src=reinterpret_cast<unsigned char*>(b)+b->dataOffset;
    auto* dst=src;
    for(int n=0;n<b->numParticles;n++,src+=bytes) {
        auto* life=reinterpret_cast<float*>(src);
        if(life[1]<life[2]) {
            double age=double(dt)+life[1];life[1]=float(age);
            float alpha=float(age*life[3]);
            if(alpha>1) alpha=1;
            auto* velocity=particleData(src,e->particleClass,6);
            auto* pos=particleData(src,e->particleClass,1);
            auto* size=particleData(src,e->particleClass,5);
            auto* color=particleData(src,e->particleClass,2);
            auto* uv=particleData(src,e->particleClass,3);
            auto* rot=particleData(src,e->particleClass,4);
            auto* transform=particleData(src,e->particleClass,7);
            if(velocity) for(int j=0;j<3;j++) velocity[j]=float(double(dt)*(&s->force.x)[j]+velocity[j]);
            if(pos && velocity) for(int j=0;j<3;j++) pos[j]=float(double(dt)*velocity[j]+pos[j]);
            float ratio[2]={1,1};
            if(size) for(int j=0;j<2;j++) {
                float old=size[4+j]; size[4+j]=float(double(alpha)*size[2+j]+size[j]);
                if(old!=0) ratio[j]=size[4+j]/old;
            }
            if(src!=dst) std::memmove(dst,src,size_t(bytes));
            if(clusters.enabled(0) && velocity) {
                auto* in=reinterpret_cast<float*>(clusters.in[positionCluster]);
                auto* out=reinterpret_cast<float*>(clusters.out[positionCluster]);
                for(int j=0;j<3;j++) out[j]=float(double(dt)*velocity[j]+in[j]);
                double r=0;for(int j=0;j<3;j++) { double d=double(out[j])-(&origin.x)[j];r+=d*d; }
                if(radius<=r) radius=float(r);
            }
            if(clusters.enabled(0)) clusters.out[positionCluster]+=positionStride;
            if(clusters.enabled(3)) {
                auto* in=reinterpret_cast<rw::Matrix*>(clusters.in[3]);
                auto* out=reinterpret_cast<rw::Matrix*>(clusters.out[3]);
                rw::V3d previous=in->pos;
                if(transform || matrix) {
                    in->pos={0,0,0};
                    if(size) { in->right=rw::scale(in->right,ratio[0]); in->up=rw::scale(in->up,ratio[1]); }
                    particleMultiply(out,in,reinterpret_cast<rw::Matrix*>(transform ? transform : matrix));
                } else {
                    if(in!=out) std::memcpy(out,in,48);
                    if(size) { in->right=rw::scale(in->right,ratio[0]); in->up=rw::scale(in->up,ratio[1]); }
                }
                for(int j=0;j<3;j++) (&out->pos.x)[j]=float(double(dt)*(velocity ? velocity[j] : 0)+(&out->pos.x)[j]+(&previous.x)[j]);
                double r=0;for(int j=0;j<3;j++) { double d=double((&out->pos.x)[j])-(&origin.x)[j];r+=d*d; }
                if(radius<=r) radius=float(r);
                clusters.out[positionCluster]+=positionStride;
            }
            for(int i : {1,2,4,5}) if(clusters.enabled(i)) {
                if(i==1 && color) for(int j=0;j<4;j++) clusters.out[i][j]=static_cast<unsigned char>(int(float(double(alpha)*color[4+j]+color[j])));
                else if(i==2 && size) for(int j=0;j<2;j++) reinterpret_cast<float*>(clusters.out[i])[j]=float(double(alpha)*size[2+j]+size[j]);
                else if(i==4 && rot) *reinterpret_cast<float*>(clusters.out[i])=float(double(alpha)*rot[1]+rot[0]);
                else if(i==5 && uv) for(int j=0;j<4;j++) reinterpret_cast<float*>(clusters.out[i])[j]=float(double(alpha)*uv[4+j]+uv[j]);
                else if(clusters.out[i]!=clusters.in[i]) std::memcpy(clusters.out[i],clusters.in[i],i==1 || i==4 ? 4 : i==2 ? 8 : 16);
            }
            for(int i : {1,2,4,5}) if(clusters.enabled(i)) clusters.out[i]+=clusters.stride[i];
            dst+=bytes;live++;
        }
        for(int i : {1,2,4,5}) if(clusters.enabled(i)) clusters.in[i]+=clusters.stride[i];
        if(positionStride) clusters.in[positionCluster]+=positionStride;
    }
    if(t && (t->dataFlags&8)) { clusters.in[0]=clusters.in[3];clusters.out[0]=clusters.out[3]; }
    b->numParticles=live;clusters.finish(live);
    if(radius>e->boundingRadiusSquared) e->boundingRadiusSquared=radius;
    return b;
}
ParticleEmitter* __cdecl standardEmit(Atomic*,ParticleEmitter* e,void*) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    if(!s || !e->particleClass) return nullptr;
    if(s->currentTime<s->nextEmit) return e;
    uint32_t seed=s->seed;
    int count=int(float(particleRandom(seed)*s->emitCountBias))+s->emitCount;
    auto* t=static_cast<EmitterTank*>(particleProperty(e,6));
    if(t && count+e->numParticles>t->maxParticles) count=t->maxParticles-e->numParticles;
    if(count<=0) return e; // count RNG is deliberately not committed here.
    auto* b=e->activeBatches;
    if(!b || count+b->numParticles>b->maxParticles) {
        b=newBatch(e);
        if(b) { b->newParticle=0;b->numParticles=0;count=std::min(count,b->maxParticles); }
    }
    if(count<=0) return e;
    if(!b) return nullptr;
    ParticleClusters clusters(t);
    int positionCluster=(t && (t->dataFlags&8)) ? 3 : 0;
    int positionStride=clusters.stride[positionCluster];
    auto* ec=emitterData(e,2);auto* eu=emitterData(e,3);
    auto* es=emitterData(e,5);auto* er=emitterData(e,4);auto* em=emitterData(e,8);
    auto* p=reinterpret_cast<unsigned char*>(b)+b->dataOffset+e->particleClass->objectSize*b->numParticles;
    for(int n=0;n<count;n++,p+=e->particleClass->objectSize) {
        auto* life=reinterpret_cast<float*>(p);
        life[1]=0;double lifetime=particleRandom(seed)*s->lifeBias+s->life;
        life[2]=float(lifetime);life[3]=float(1/lifetime);
        auto* vel=particleData(p,e->particleClass,6);
        rw::V3d position{0,0,0};
        if(vel) {
            rw::V3d dir;for(int j=0;j<3;j++) (&dir.x)[j]=vary(seed,(&s->direction.x)[j],(&s->directionBias.x)[j]);
            particleNormalize(&dir,&dir);if(s->ltm) particleVector(&dir,&dir,s->ltm);
            double speed=particleRandom(seed)*s->speedBias+s->speed;
            for(int j=0;j<3;j++) vel[j]=float(speed*(&dir.x)[j]);
            rw::V3d local;for(int j=0;j<3;j++) (&local.x)[j]=vary(seed,(&s->position.x)[j],(&s->positionBias.x)[j]);
            // Original leaves position undefined without an LTM. Define the
            // local value on that unsupported path; real game emitters have one.
            position=local;if(s->ltm) particlePoint(&position,&local,s->ltm);
        }
        if(auto* pos=particleData(p,e->particleClass,1)) std::memcpy(pos,&position,12);
        if(auto* m=particleData(p,e->particleClass,7)) {
            m[0]=m[5]=m[10]=1;m[1]=m[2]=m[4]=m[6]=m[8]=m[9]=m[12]=m[13]=m[14]=0;
            uint32_t flags;std::memcpy(&flags,m+3,4);flags|=0x20003;std::memcpy(m+3,&flags,4);
        }
        float size[2]={s->size.x,s->size.y};
        if(es) {
            for(int j=0;j<2;j++) size[j]=vary(seed,es[j],es[2+j]);
            if(auto* ps=particleData(p,e->particleClass,5)) {
                ps[0]=ps[4]=size[0];ps[1]=ps[5]=size[1];
                for(int j=0;j<2;j++) ps[2+j]=float(particleRandom(seed)*es[6+j]+es[4+j]-size[j]);
            }
        }
        float color[4]{};
        if(ec) {
            for(int j=0;j<4;j++) color[j]=vary(seed,ec[j],ec[4+j]);
            if(auto* pc=particleData(p,e->particleClass,2)) {
                std::memcpy(pc,color,16);
                for(int j=0;j<4;j++) pc[4+j]=float(particleRandom(seed)*ec[12+j]+ec[8+j]-color[j]);
            }
        }
        float rotation=0;
        if(er) {
            rotation=vary(seed,er[0],er[1]);
            if(rotation>3.1415927410125732f) rotation-=6.2831854820251465f;
            if(rotation< -3.1415927410125732f) rotation+=6.2831854820251465f;
            if(auto* pr=particleData(p,e->particleClass,4)) { pr[0]=rotation;pr[1]=float(particleRandom(seed)*er[3]+er[2]-rotation); }
        }
        float uv[4]{};
        if(eu) {
            for(int j=0;j<4;j++) { int k=j<2 ? j : j+6;uv[j]=vary(seed,eu[k],eu[k+2]); }
            if(auto* pu=particleData(p,e->particleClass,3)) {
                std::memcpy(pu,uv,16);
                for(int j=0;j<4;j++) { int k=j<2 ? j+4 : j+10;pu[4+j]=float(particleRandom(seed)*eu[k+2]+eu[k]-uv[j]); }
            }
        }
        if(clusters.enabled(0,true)) { std::memcpy(clusters.out[positionCluster],&position,12);clusters.out[positionCluster]+=positionStride; }
        if(clusters.enabled(3,true)) {
            if(!em) return nullptr;
            rw::V3d at,up;for(int j=0;j<3;j++) (&at.x)[j]=vary(seed,em[16+j],em[19+j]);
            particleNormalize(&at,&at);
            for(int j=0;j<3;j++) (&up.x)[j]=vary(seed,em[22+j],em[25+j]);
            particleNormalize(&up,&up);
            auto* m=reinterpret_cast<rw::Matrix*>(clusters.out[3]);
            m->at=at;m->right=particleCross(at,up);m->up=particleCross(m->right,at);
            m->right=rw::scale(m->right,size[0]);m->up=rw::scale(m->up,size[1]);m->pos=position;
            clusters.out[positionCluster]+=positionStride;
        }
        if(clusters.enabled(1,true)) for(int j=0;j<4;j++) clusters.out[1][j]=static_cast<unsigned char>(int(color[j]));
        if(clusters.enabled(2,true)) std::memcpy(clusters.out[2],size,8);
        if(clusters.enabled(4,true)) std::memcpy(clusters.out[4],&rotation,4);
        if(clusters.enabled(5,true)) std::memcpy(clusters.out[5],uv,16);
        for(int i : {1,2,4,5}) if(clusters.enabled(i,true)) clusters.out[i]+=clusters.stride[i];
    }
    if(t && (t->dataFlags&8)) clusters.out[0]=clusters.out[3];
    clusters.finish(count,true);b->newParticle=b->numParticles;b->numParticles+=count;
    e->numParticles+=count;s->seed=seed;return e;
}
ParticleEmitter* __cdecl sphereEmit(Atomic*,ParticleEmitter* e,void*) {
    auto* b=e->activeBatches;
    if(!b || b->newParticle==-1) return e;
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    auto* sphere=emitterData(e,0x1000008);
    if(!s || !sphere || !e->particleClass) return nullptr;
    auto* t=static_cast<EmitterTank*>(particleProperty(e,6));
    int count=b->numParticles-b->newParticle;
    unsigned char* tankPos=nullptr;int stride=0;
    if(t && (t->dataFlags&9)) {
        int k=(t->dataFlags&1) && (t->dataFlags&8) ? 1 : 0;
        stride=t->strides[k];tankPos=t->output[k]-stride*count;
    }
    auto* p=reinterpret_cast<unsigned char*>(b)+b->dataOffset+e->particleClass->objectSize*b->newParticle;
    uint32_t seed=s->seed;
    for(int i=0;i<count;i++,p+=e->particleClass->objectSize) {
        auto* vel=particleData(p,e->particleClass,6);
        rw::V3d position{0,0,0};
        if(vel) {
            float angle=float(particleRandom(seed)*3.141592502593994140625);
            double z=particleRandom(seed);float roundedZ=float(z);
            double root=particleSqrt(float(1-z*roundedZ));
            seed=seed*0x0bb38435u+0x3619636bu;
            double radius=(double(int32_t(seed))*0x1p-32+0.5)*sphere[1]+sphere[0];
            rw::V3d local{float(std::cos(double(angle))*radius*root),float(std::sin(double(angle))*radius*root),float(radius*roundedZ)};
            position=local;if(s->ltm) particlePoint(&position,&local,s->ltm);
            int useDirection;std::memcpy(&useDirection,sphere+2,4);
            if(useDirection==1) {
                particleNormalize(&local,&local);if(s->ltm) particleVector(&local,&local,s->ltm);
                double speed=particleRandom(seed)*s->speedBias+s->speed;
                for(int j=0;j<3;j++) vel[j]=float(speed*(&local.x)[j]);
            }
        }
        if(auto* pos=particleData(p,e->particleClass,1)) std::memcpy(pos,&position,12);
        if(t && (t->dataFlags & uint32_t(t->flags) & 1)) { std::memcpy(tankPos,&position,12);tankPos+=stride; }
    }
    s->seed=seed;return e;
}
