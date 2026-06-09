#include <Geode/Geode.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include "includes/modifies/NCPlayLayer.hpp"
#include "includes/custom/NoclipDataPopup.hpp"

using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
class $modify(BPBPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto spr = CircleButtonSprite::createWithSprite("buttonicon.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::Tiny);

        spr->setScale(1.2f);

        auto openBars = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(BPBPauseLayer::onButton)
        );

        auto menu = this->getChildByID("right-button-menu");
        menu->addChild(openBars);
        openBars->setID("openBars"_spr);
        menu->updateLayout();
    }

    void onButton(CCObject* sender) {
        auto playLayer = PlayLayer::get();
        NoclipDataPopup::create(playLayer->m_level)->show();
    }
};