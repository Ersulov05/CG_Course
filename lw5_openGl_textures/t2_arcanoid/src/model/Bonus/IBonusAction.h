#pragma once

enum class BonusActionType {
    AddSizeBalls,
    SubSizeBalls,
    AddSizeRacket,
    SubSizeRacket,
    AddSpeedBalls,
    SubSpeedBalls,
    SplitBalls,
};

class IBonusAction {
public:
    virtual void Apply() = 0;
    virtual void ReApply() {};
    virtual void Update(float deltatime) {};
    virtual BonusActionType GetType() const = 0;
    virtual bool IsActionEnd() const 
    {
        return true;
    };
};