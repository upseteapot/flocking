#pragma once
#include <SFML/Graphics.hpp>
#include "shapes/section.hpp"


class BoidViews : public sf::Drawable
{
  public:
    BoidViews() = default;
    void setup(float range, float field_view, Vec2f *pos, Vec2f *vel);
    void update();

  private:
    Vec2f *m_pos, *m_vel;
    SectionShape m_shape[4];

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

