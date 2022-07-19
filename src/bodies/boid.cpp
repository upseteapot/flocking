#include "bodies/boid.hpp"


void Boid::setup(Vec2f pos, float heading, sf::Color color, LayerManager *layer_manager, BoidStates states) 
{
  m_pos[0] = pos;
  m_vel = Vec2f::polar(m_states.max_vel, heading);
  m_states = states;
  m_color = color;
  m_layer_manager = layer_manager;

  float x = m_states.radius * 4.0f / 5.0f;
  float y = sqrt(pow(m_states.radius, 2) - pow(x, 2)); 

  m_boid_shape.setPointCount(3);
  m_boid_shape.setPoint(0, sf::Vector2f( m_states.radius, 0.0f));
  m_boid_shape.setPoint(1, sf::Vector2f(-x, -y));
  m_boid_shape.setPoint(2, sf::Vector2f(-x,  y));
  m_boid_shape.setOutlineThickness(-1);
  m_boid_shape.setOutlineColor(color); 
  m_boid_shape.setFillColor(sf::Color(0, 0, 0, 0));
  m_layer_manager->add(0, &m_boid_shape);
  
  m_collision_bubble.setRadius(m_states.radius);
  m_collision_bubble.setOrigin(sf::Vector2f(m_states.radius, m_states.radius));
  m_collision_bubble.setFillColor(sf::Color(m_color.r, m_color.g, m_color.b, 50));

  for (int i=0; i < 4; i++) {
    m_detection_cones[i].create(30, m_states.range, m_states.field_view);
    m_detection_cones[i].set_color(sf::Color(0, 0, 0, 0), sf::Color(20, 120, 150, 50));
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
        cohesion += m_pos[0] + (other_pos - m_pos[i]);
        separation += (m_pos[i] - other_pos) / pow(distance, 3);
        allignment += boid.get_velocity();
        
        if (m_selected)
          boid.set_outline(sf::Color(255, 190, 0));
      } 
      
      else if (m_selected)
        boid.reset_outline();  
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

  m_boid_shape.setPosition(vec2f_sfml(m_pos[0]));
  
  if (m_selected) {
    m_collision_bubble.setPosition(vec2f_sfml(m_pos[0]));
  
    for (int i=0; i < 4; i++) {
      m_detection_cones[i].set_position(m_pos[i]);
      m_detection_cones[i].set_rotation(m_vel.angle());
    }
  }

  if (m_vel.mag() != 0)
    m_boid_shape.setRotation(rad_degrees(m_vel.angle()));
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

void Boid::set_outline(sf::Color outline_color)
{
  m_boid_shape.setOutlineColor(outline_color);
}

void Boid::reset_outline()
{
  m_boid_shape.setOutlineColor(m_color);
}

void Boid::select()
{
  m_selected = true;
  m_layer_manager->add(1, m_detection_cones);
  m_layer_manager->add(1, &m_collision_bubble);
}
    
void Boid::deselect()
{
  m_selected = false;
  m_layer_manager->clear_layer(1);
}

Vec2f Boid::get_position() const
{
  return m_pos[0];
}

Vec2f Boid::get_velocity() const
{
  return m_vel;
}

float Boid::get_radius() const
{
  return m_states.radius;
}

