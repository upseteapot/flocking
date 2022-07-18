#pragma once
#include <ctime>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

#include "graphics/engine.hpp"
#include "graphics/boid.hpp"
#include "math/vec2.hpp"


#define map(x, x1, x2, y1, y2) (((x - x1) / (x2 -  x1)) * (y2 - y1)) + y1  


class Simulation : public Engine
{
  public:
    Simulation() = default;

  private:
    const int m_boids_size = 220;
    std::vector<Boid> m_boids;

    void setup() override;
    void run(float dt) override;
    void event() override;
    void clear() override;
};

