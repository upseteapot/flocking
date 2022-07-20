#include "entities/boid_views.hpp"
#include <iostream>


void BoidViews::setup(float range, float field_view, Vec2f *pos, Vec2f *vel)
{
  m_pos = pos;
  m_vel = vel;

  for (int i=0; i < 4; i++) {
    m_shape[i].create(30, range, field_view);
    m_shape[i].set_color(sf::Color(0, 0, 0, 0), sf::Color(20, 120, 150, 50));
  }
}

void BoidViews::update()
{
  for (int i=0; i < 4; i++) {
    m_shape[i].set_position(m_pos[i]);
    m_shape[i].set_rotation(m_vel->angle());
  }
}


void BoidViews::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  int j;
  for (int i=0; i < 4; i++) {
    for (j=0; j < i; j++)
      if (m_pos[i] == m_pos[j])
        break;
    if (j == i)
      target.draw(m_shape[i], states);
  }
}

