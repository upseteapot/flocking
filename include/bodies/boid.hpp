#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "managers/layer_manager.hpp"
#include "math/vec2.hpp"
#include "bodies/cone.hpp"


struct BoidStates
{
    float max_vel    = 70.0f;
    float range      = 200.0f;
    float field_view = 3.14159265f;
    float radius     = 9.0f;
    float tune       = 0.05f;
    float tune_c     = 0.31f;
    float tune_s     = 0.29f;
    float tune_a     = 0.28;
};


class Boid
{
  public:
    Boid() = default;
    void setup(Vec2f pos, float heading, sf::Color color, LayerManager *layer_manager, BoidStates states=BoidStates());
    void apply_boid_rules(std::vector<Boid> &boids);
    void update(float dt);
    void bounds(int width, int height);
    void set_outline(sf::Color outline_color);
    void reset_outline();
    void select();
    void deselect();
    Vec2f get_position() const;
    Vec2f get_velocity() const;
    float get_radius() const;

  private:
    BoidStates m_states;
    LayerManager *m_layer_manager;
    bool m_selected = false;
    Vec2f m_pos[4], m_vel;
    sf::Color m_color;
    sf::ConvexShape m_boid_shape;
    sf::CircleShape m_collision_bubble;
    Cone m_detection_cones[4];
};

