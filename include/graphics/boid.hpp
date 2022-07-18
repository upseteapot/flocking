#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "math/vec2.hpp"


struct BoidStates
{
    float max_vel    = 70.0f;
    float range      = 200.0f;
    float field_view = 3.14159265f;
    float radius     = 7.0f;
    float tune       = 0.05f;
    float tune_c     = 0.31f;
    float tune_s     = 0.29f;
    float tune_a     = 0.28;
};


class Boid : public sf::Drawable
{
  public:
    Boid() = default;
    void setup(Vec2f pos, float heading, sf::Color color, BoidStates states=BoidStates());
    void apply_boid_rules(std::vector<Boid> &boids);
    void update(float dt);
    void bounds(int width, int height);
    void outline(int thickness, sf::Color outline_color);
    void select();
    void deselect();
    Vec2f get_position() const;
    Vec2f get_velocity() const;
      
  private:
    BoidStates m_states;
    bool m_selected = false;
    Vec2f m_pos[4], m_vel;
    sf::ConvexShape m_shape;
    sf::CircleShape m_collision_bubble[4];

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

