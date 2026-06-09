#pragma once

#include <Geode/loader/Event.hpp>

class NoclipData {
public:
    void createNewKeyframe(std::string levelName, float percent, bool toggle);
    void removeKeyframeByIndex(std::string levelName, int index);
    void updateValues(std::string levelName, int index, float newPercent, bool newToggle);
    std::vector<std::tuple<float, bool>> getNoclipKeyframes(std::string levelName);
};

