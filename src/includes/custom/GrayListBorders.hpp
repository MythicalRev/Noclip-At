#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GrayListBorders : public ListBorders {
protected:
    bool init() {
        if (!ListBorders::init()) return false;

        auto top = this->getTop();
        auto bottom = this->getBottom();
        auto left = this->getLeft();
        auto right = this->getRight();

        auto NST = NineSlice::create("borderTB.png"_spr);
        NST->setInsets(top->getInsets());
        NST->setContentSize(top->getContentSize());
        NST->setScale(top->getScale());
        NST->setScaleMultiplier(top->getScaleMultiplier());

        auto NSB = NineSlice::create("borderTB.png"_spr);
        NSB->setInsets(bottom->getInsets());
        NSB->setContentSize(bottom->getContentSize());
        NSB->setScale(-1.f);

        auto L = CCSprite::create("borderSide.png"_spr);
        L->setContentSize(left->getContentSize());
        L->setScale(left->getScale());
        L->setFlipX(left->m_bFlipX);
        L->setFlipY(left->m_bFlipY);

        auto R = CCSprite::create("borderSide.png"_spr);
        R->setContentSize(right->getContentSize());
        R->setScale(right->getScale());
        R->setFlipX(right->m_bFlipX);
        R->setFlipY(right->m_bFlipY);

        this->setSprites(NST, NSB, L, R);

        return true;
    }
public:
    static GrayListBorders* create() {
        auto pRet = new GrayListBorders();
        if (pRet->init()) {
            pRet->autorelease();
            return pRet;
        }

        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
};