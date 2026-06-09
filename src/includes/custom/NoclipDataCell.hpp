#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include "../modifies/noclip/NoclipData.hpp"
#include "Geode/utils/general.hpp"

using namespace geode::prelude;

class NoclipDataCell : public CCLayerColor {
public:
    CCMenuItemSpriteExtra* deleteBtn;
    TextInput* percentInput;
    CCMenuItemToggler* toggleInput;

    std::string lvlName;
    int cellIndex;

    std::function<void()> onDelete;

    bool init(std::string levelName, int posIndex, float percent, bool toggle) {
        if (!CCLayerColor::init()) return false;

        if (posIndex == 0 || (posIndex % 2) == 0) {
            this->setOpacity(100);
        } else {
            this->setOpacity(50);    
        }

        this->setContentSize(ccp(315, 35));
        this->setAnchorPoint(ccp(0, 1));
        this->setPositionY(207);

        percentInput = TextInput::create(60, "%");

        percentInput->setTextAlign(TextInputAlign::Left);
        percentInput->setString(geode::utils::numToString<float>(percent));
        percentInput->setAnchorPoint(ccp(0,0.5f));
        percentInput->setScale(0.8f);
        percentInput->setPosition(ccp(10, this->getContentHeight() / 2));
        percentInput->setFilter("1234567890.");
        percentInput->setCallback([this](const std::string& value) {
            onDataChange(value, toggleInput->isOn());
        });

        auto offSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        auto onSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

        toggleInput = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(NoclipDataCell::onToggle), .7f);
        toggleInput->setAnchorPoint(ccp(0, 0.5f));
        toggleInput->toggle(toggle);

        auto toggleMenu = CCMenu::createWithItem(toggleInput);
        toggleMenu->setContentSize(toggleInput->getContentSize());
        toggleMenu->setPosition(ccp(60, this->getContentHeight() / 2));

        auto deleteBtnSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
        deleteBtnSpr->setScale(0.65f);
        deleteBtn = CCMenuItemSpriteExtra::create(deleteBtnSpr, this, menu_selector(NoclipDataCell::onDeleteKeyframe));

        auto menuLayout = RowLayout::create();
        menuLayout->setAxisAlignment(AxisAlignment::End);

        auto leftMenu = CCMenu::createWithItem(deleteBtn);
        leftMenu->setAnchorPoint(ccp(1, 0.5f));
        leftMenu->ignoreAnchorPointForPosition(false);
        leftMenu->setPosition(CCPoint(this->getContentWidth() - 10,this->getContentHeight()/ 2));
        leftMenu->setContentSize(CCSize(100, 30));

        menuLayout->apply(leftMenu);

        leftMenu->updateLayout();
        
        this->addChild(percentInput);
        this->addChild(toggleMenu);
        this->addChild(leftMenu);

        lvlName = levelName;
        cellIndex = posIndex;

        return true;
    }

    static NoclipDataCell* create(std::string levelName, int posIndex, float percent, bool toggle) {
        NoclipDataCell* pRet = new NoclipDataCell();
        if (pRet && pRet->init(levelName, posIndex, percent, toggle)) {
            pRet->autorelease();
            return pRet;
        } else {
            CC_SAFE_DELETE(pRet);
            return nullptr;
        }
    }
protected:
    void onDeleteKeyframe(CCObject* sender) {
        NoclipData noclipData;

        noclipData.removeKeyframeByIndex(lvlName, cellIndex);

        if (onDelete) onDelete();
    }

    void onDataChange(const std::string& percentStr, bool toggle) {
        NoclipData noclipData;

        float percent = geode::utils::numFromString<float>(percentStr).unwrapOr(1.f);

        if (percent) {
            noclipData.updateValues(lvlName, cellIndex, percent, toggle);
        }
    }

    void onToggle(CCObject* sender) {
        NoclipData noclipData;

        float percent = geode::utils::numFromString<float>(percentInput->getString()).unwrapOr(1.f);
        auto toggler = static_cast<CCMenuItemToggler*>(sender);

        if (percent) {
            noclipData.updateValues(lvlName, cellIndex, percent, !toggler->isOn());
        }
    }
};