#include "graphics/simulation.hpp"


void Simulation::setup()
{
  srand(time(0));
  
  m_layer_manager.setup(2);
  
  m_boids.resize(m_boids_size);
  for (std::size_t i=0; i < m_boids.size(); i++) {
    Vec2f pos = Vec2f((m_size.x * rand() / RAND_MAX) - (m_size.x / 2.f), (m_size.y * rand() / RAND_MAX) - (m_size.y / 2.f));
    float heading = 2.f * 3.14159265f * rand() / RAND_MAX;
    
    float color_value = (float)rand() / RAND_MAX;
    int r = (int)map(color_value, 0.0f, 1.0f, 220.0f,   0.0f);
    int g = (int)map(color_value, 0.0f, 1.0f,  10.0f,  50.0f);
    int b = (int)map(color_value, 0.0f, 1.0f,   0.0f, 220.0f);
    
    BoidStates default_states;
    default_states.max_vel    += map((float)rand() / RAND_MAX, 0.0f, 1.0f, -30.0f, 30.0f);
    default_states.range      += map((float)rand() / RAND_MAX, 0.0f, 1.0f, -50.f, 50.0f);
    default_states.field_view += map((float)rand() / RAND_MAX, 0.0f, 1.0f, -3.14159265f / 6.f, 3.14159265 / 6.f);
    default_states.radius     += map((float)rand() / RAND_MAX, 0.0f, 1.0f, -2.f, 2.5f);
    default_states.tune       += map((float)rand() / RAND_MAX, 0.0f, 1.0f, -0.01f, 0.01f);
    default_states.tune_c     += map((float)rand() / RAND_MAX, 0.0f, 1.0f, -0.03f, 0.03f);
    default_states.tune_s     += map((float)rand() / RAND_MAX, 0.0f, 1.0f, -0.03f, 0.03f);
    default_states.tune_a     += map((float)rand() / RAND_MAX, 0.0f, 1.0f, -0.03f, 0.03f);

    m_boids[i].setup(pos, heading, sf::Color(r, g, b), &m_layer_manager, default_states);
    m_boids[i].bounds(m_size.x, m_size.y);
  }
}

void Simulation::run(float dt)
{
  if (!m_paused) {
    for (auto &boid : m_boids) {
      boid.apply_boid_rules(m_boids);
      boid.update(dt);
      boid.bounds(m_size.x, m_size.y);
    }
  }

  m_renderer.clear(sf::Color::Black);
  m_renderer.draw(m_layer_manager);
  m_renderer.display();
}

void Simulation::event()
{
  switch (m_event.type) {

    case sf::Event::MouseButtonPressed:
      if (m_event.mouseButton.button == sf::Mouse::Left && !m_paused) {
        if (m_has_selected) {
          m_boids[m_selected_index].deselect();
          for (auto &boid : m_boids)
            boid.reset_outline();
          m_has_selected = false;  
        }
        sf::Vector2f mouse_pos = m_renderer.mapPixelToCoords(sf::Mouse::getPosition(m_renderer));
        for (std::size_t i=0; i < m_boids.size(); i++)
          if (Vec2f::dist(sfml_vec2f(mouse_pos), m_boids[i].get_position()) <= 10) {
            m_boids[i].select();
            m_selected_index = i;
            m_has_selected = true;
            break;
          }
      }
      break;

    case sf::Event::KeyPressed:
      if (m_event.key.code == sf::Keyboard::Space)
        m_paused = !m_paused;
      break;

    default:
      break;
  }
}

void Simulation::clear()
{
}

