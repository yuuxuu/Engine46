/**
 * @file CParticleEmitter.h
 * @brief
 * @author 木村優
 * @date 2021/11/24
 */

#pragma once

#include "CActor.h"

namespace Engine46 {

    // 前方宣言
    class CRendererBase;
    class CUnorderedAccessBufferBase;
    class CTextureBase;

    constexpr UINT DEFAULT_MAX_PARTICLE = 20000;

    struct Particle {
        VECTOR3 pos;
        VECTOR4 color;
        VECTOR2	uv;
        VECTOR3	normal;

        VECTOR3 initPos;
        VECTOR3 velocity;
        float lifeTime;
        float gravity;
    };

    class CParticleEmitter : public CActorBase {
    private:
        CTextureBase*                               pParticleTexture;

        UINT                                        m_maxParticle;

        std::unique_ptr<CUnorderedAccessBufferBase> m_pParticleUab;

    public:
        explicit CParticleEmitter(const char* particleEmitterName);
        ~CParticleEmitter();

        void Initialize() override;
        void Update() override;
        void Draw() override;

        void Update(std::vector<Particle>& vecParticle);
    };

} // namespace
