#pragma once
#include <ctime>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>
#include "graphics/engine.hpp"
#include "graphics/layer_manager.hpp"
#include "entities/boid.hpp"
#include "math/vec2.hpp"

#define map(x, x1, x2, y1, y2) (((x - x1) / (x2 -  x1)) * (y2 - y1)) + y1  


class Simulation : public Engine
{
  public:
    Simulation() = default;

  private:
    bool m_paused = true;
    bool m_has_selected = false;
    const int m_boids_size = 220;
    std::size_t m_selected_index = -1;
    std::vector<Boid> m_boids;
    
    LayerManager m_layer_manager;

    void setup() override;
    void run(float dt) override;
    void event() override;
    void clear() override;
};

