#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include "../modifies/noclip/NoclipData.hpp"
#include "GrayListBorders.hpp"
#include "NoclipDataCell.hpp"

using namespace geode::prelude;

class NoclipDataPopup : public Popup {
protected:
    bool init(GJGameLevel* level) {
        if (!Popup::init(350, 260, "GJ_square05.png"))
            return false;

        this->setTitle("Noclip Keyframes", "bigFont.fnt");

        auto scrollLayerBG = CCLayerColor::create({0, 0, 0, 60}, 313, 190);
        scrollLayerBG->ignoreAnchorPointForPosition(false);
        scrollLayerBG->setAnchorPoint(ccp(0.5, 0.5));

        scrollLayer = ScrollLayer::create(ccp(313, 190));
        scrollLayer->ignoreAnchorPointForPosition(false);
        scrollLayer->setAnchorPoint(ccp(0.5, 0.5));

        auto scrollBar = Scrollbar::create(scrollLayer);

        auto border = GrayListBorders::create();
        
        border->setContentSize(ccp(313, 190));
        border->ignoreAnchorPointForPosition(false);
        border->setAnchorPoint(ccp(0.5, 0.5));

        m_mainLayer->addChildAtPosition(scrollLayerBG, Anchor::Center);
        m_mainLayer->addChildAtPosition(scrollLayer, Anchor::Center);
        m_mainLayer->addChildAtPosition(border, Anchor::Center);
        m_mainLayer->addChildAtPosition(scrollBar, Anchor::Center, ccp(162.5, 0));

        auto layout = ColumnLayout::create();
        layout->setAxisReverse(true);
        layout->setGap(0);
        layout->setAxisAlignment(AxisAlignment::Start);

        scrollLayer->m_contentLayer->setLayout(layout);

        reloadList(level);

        return true;
    }

    void reloadList(GJGameLevel* level) {       
        scrollLayer->m_contentLayer->removeAllChildren();

        NoclipData noclipData;
        auto keyframes = noclipData.getNoclipKeyframes(level->m_levelName);

        scrollLayer->m_contentLayer->setContentSize(CCSize(313, 35 * keyframes.size()));

        int index = 0;
        for (auto& [percent, toggle] : keyframes) {
            auto newCell = NoclipDataCell::create(level->m_levelName, index, percent, toggle);
            newCell->setContentSize(ccp(313, 35));
            newCell->ignoreAnchorPointForPosition(false);
            newCell->setAnchorPoint(ccp(0, 1));
            newCell->onDelete = [this, level = Ref<GJGameLevel>(level)]() {
                Loader::get()->queueInMainThread([this, level = level.data()]() {
                    reloadList(level);
                });
            };

            scrollLayer->m_contentLayer->addChild(newCell);
            ++index;
        }

        scrollLayer->m_contentLayer->updateLayout();
        scrollLayer->moveToTop();
    }
public:
    ScrollLayer* scrollLayer;

    static NoclipDataPopup* create(GJGameLevel* level) {
        auto pRet = new NoclipDataPopup();
        if (pRet->init(level)) {
            pRet->autorelease();
            return pRet;
        }

        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
};