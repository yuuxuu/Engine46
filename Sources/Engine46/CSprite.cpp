/**
 * @file CSprite.cpp
 * @brief
 * @author 木村優
 * @date 2001/05/06
 */

#include "CSprite.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CConstantBuffer.h"

namespace Engine46 {

    // コンストラクタ
    CSprite::CSprite(const std::string& actorName) :
        CActorBase((int)ActorType::Sprite, actorName, Transform(VECTOR3(), VECTOR3(), VECTOR3(10.0f, 10.0f, 10.0f)))
    {}

    // デストラクタ
    CSprite::~CSprite()
    {}

    // 更新
    void CSprite::Update() {

    }

} // namespace