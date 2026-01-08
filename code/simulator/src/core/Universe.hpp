#pragma once
#include <vector>
#include "Body.hpp"

class Universe {
public:
    Universe() = default;

    void setBodies(const SoABodies& bodies) { bodies_ = bodies; }

    SoABodies& getBodies() { return bodies_; }
    const SoABodies& getBodies() const { return bodies_; }

    size_t getNumBodies() const { return bodies_.size(); }

private:
    SoABodies bodies_;
};
