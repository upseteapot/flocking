#include "graphics/boid.hpp"


void Boid::setup(Vec2f pos, float heading, sf::Color color, BoidStates states) 
{
  m_pos[0] = pos;
  m_vel = Vec2f::polar(m_states.max_vel, heading);
  m_states = states;

  m_shape.setPointCount(3);
  m_shape.setPoint(0, sf::Vector2f( m_states.radius, 0.0f));
  m_shape.setPoint(1, sf::Vector2f(-m_states.radius, -m_states.radius * 2.0f / 3.0f));
  m_shape.setPoint(2, sf::Vector2f(-m_states.radius,  m_states.radius * 2.0f / 3.0f));
  m_shape.setOutlineThickness(-1);
  m_shape.setOutlineColor(color); 
  m_shape.setFillColor(sf::Color(0, 0, 0, 0));

  for (int i=0; i < 4; i++) {
    m_collision_bubble[i].setRadius(m_states.range);
    m_collision_bubble[i].setOrigin(sf::Vector2f(m_states.range, m_states.range));
    m_collision_bubble[i].setFillColor(sf::Color(0, 255, 255, 50));
  }
}

void Boid::apply_boid_rules(std::vector<Boid> &boids)
{  
  int count = 0;
  Vec2f rel_pos, cohesion, separation, allignment;

  for (auto &boid : boids) {
    Vec2f other_pos = boid.get_position();
    
    for (int i=0; i < 4; i++) {
      float distance = Vec2f::dist(m_pos[i], other_pos);
      bool check = m_pos[i] != other_pos &&
                   distance <= m_states.range &&
                   Vec2f::angle_between(m_vel, other_pos - m_pos[i]) <= m_states.field_view / 2.f;

      if (check) {
        count++;
        cohesion   += m_pos[0] + (other_pos - m_pos[i]);
        separation += (m_pos[i] - other_pos) / pow(distance, 3);
        allignment += boid.get_velocity();
        
        if (m_selected)
          boid.outline(1, sf::Color(255, 255, 0));
      } 
      
      else if (m_selected)
        boid.outline(0, sf::Color(0, 255, 255));  
    }
  }

  if (count > 0) {
    cohesion   = Vec2f::set_mag((cohesion / count) - m_pos[0], m_states.max_vel) - m_vel;
    separation = Vec2f::set_mag((separation / count), m_states.max_vel) - m_vel;
    allignment = Vec2f::set_mag(allignment / count, m_states.max_vel) - m_vel;
    m_vel += (cohesion * m_states.tune_c + separation * m_states.tune_s + allignment * m_states.tune_a) * m_states.tune; 
  }

  m_vel.set_mag(m_states.max_vel);
} 

void Boid::update(float dt)
{
  m_pos[0] += m_vel * dt;

  m_shape.setPosition(vec2f_sfml(m_pos[0]));

  for (int i=0; i < 4; i++)
    m_collision_bubble[i].setPosition(vec2f_sfml(m_pos[i]));

  if (m_vel.mag() != 0)
    m_shape.setRotation(rad_degrees(m_vel.angle()));
}

void Boid::bounds(int width, int height)
{
  if (m_pos[0].x < -width / 2.f - 1 - m_states.radius)
    m_pos[0].x = width / 2.f + m_states.radius - 1;
  else if (m_pos[0].x > width / 2.f + 1 + m_states.radius)
    m_pos[0].x = -width / 2.f - m_states.radius + 1;

  if (m_pos[0].y < -height / 2.f - 1 - m_states.radius)
    m_pos[0].y = height / 2.f + m_states.radius - 1;
  else if (m_pos[0].y > height / 2.f + 1 + m_states.radius)
    m_pos[0].y = -height / 2.f - m_states.radius + 1;

  for (int i=1; i < 4; i++)
    m_pos[i] = m_pos[0];

  if (m_pos[0].x < -width / 2.f + m_states.range) {
    m_pos[1].x = width + m_pos[0].x;
    m_pos[2].x = width + m_pos[0].x;
  } else if (m_pos[0].x > width / 2.f - m_states.range) {
    m_pos[1].x = -width + m_pos[0].x;
    m_pos[2].x = -width + m_pos[0].x;
  }
  
  if (m_pos[0].y < -height / 2.f + m_states.range) {
    m_pos[1].y = height + m_pos[0].y;
    m_pos[3].y = height + m_pos[0].y;
  } else if (m_pos[0].y > height / 2.f - m_states.range) {
    m_pos[1].y = -height + m_pos[0].y;
    m_pos[3].y = -height + m_pos[0].y;
  }
}

void Boid::outline(int thickness, sf::Color outline_color)
{
  m_shape.setOutlineThickness(thickness);
  m_shape.setOutlineColor(outline_color);
}

void Boid::select()
{
  m_selected = true;
}
    
void Boid::deselect()
{
  m_selected = false;
}

Vec2f Boid::get_position() const
{
  return m_pos[0];
}

Vec2f Boid::get_velocity() const
{
  return m_vel;
}


void Boid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  target.draw(m_shape, states);

  if (m_selected) {
    target.draw(m_collision_bubble[0]);

    for (int i=1; i < 4; i++)
      if (m_pos[i] != m_pos[0])
        target.draw(m_collision_bubble[i]);
  }  
}

