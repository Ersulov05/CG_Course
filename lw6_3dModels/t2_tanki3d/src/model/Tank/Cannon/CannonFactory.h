#pragma once

class Cannon;
class CannonFactory {
public:
    static Cannon CreateCannonByLevel(unsigned int level);
};