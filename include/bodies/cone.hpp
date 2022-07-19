#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "math/vec2.hpp"


class Cone : public sf::Drawable
{
  public:
    Cone() = default;
    ~Cone();
    void create(unsigned int triangles, float radius, float aperture);
    void set_position(Vec2f pos);
    void set_rotation(float rotation);
    void set_color(sf::Color from, sf::Color to);

  private:
    float m_radius, m_aperture;
    unsigned int m_point_count;
    Vec2f *m_points, m_pos;
    sf::VertexArray m_vertex_array; 
    
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

