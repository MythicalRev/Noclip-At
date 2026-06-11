#pragma once

#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Event.hpp>

class NoclipData {
public:
    void createNewKeyframe(GJGameLevel* level, float percent, bool toggle);
    void removeKeyframeByIndex(GJGameLevel* level, int index);
    void updateValues(GJGameLevel* level, int index, float newPercent, bool newToggle);
    std::vector<std::tuple<float, bool>> getNoclipKeyframes(GJGameLevel* level);
};

