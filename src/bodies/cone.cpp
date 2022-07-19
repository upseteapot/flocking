#include "bodies/cone.hpp"


Cone::~Cone()
{
  if (m_point_count != 0)
    delete[] m_points;
}
  
void Cone::create(unsigned int triangles, float radius, float aperture)
{
  m_radius = radius;
  m_aperture = aperture;

  m_point_count = 1 + triangles * 2;
  m_points = new Vec2f[m_point_count];
  m_vertex_array = sf::VertexArray(sf::TriangleFan, m_point_count);

  m_vertex_array[0].position = sf::Vector2f(0.0f, 0.0f);
  m_points[0] = Vec2f(0.0f, 0.0f);

  for (unsigned int i=1; i < m_point_count; i++) {
    float angle = ((2.f * i / m_point_count) - 1.f) * aperture / 2.f;
    m_points[i] = Vec2f::polar(radius, angle);
    m_vertex_array[i].position = vec2f_sfml(m_points[i]);
  }
}

void Cone::set_position(Vec2f pos) 
{
  m_pos = pos;
  for (unsigned int i=0; i < m_point_count; i++)
    m_vertex_array[i].position = vec2f_sfml(m_points[i] + pos);
}

void Cone::set_rotation(float rotation)
{
  for (unsigned int i=1; i < m_point_count; i++) {
    float angle = ((2.f * i / m_point_count) - 1.f) * m_aperture / 2.f + rotation;
    m_points[i] = Vec2f::polar(m_radius, angle);
    m_vertex_array[i].position = vec2f_sfml(m_points[i] + m_pos);
  }
}

void Cone::set_color(sf::Color from, sf::Color to)
{
  m_vertex_array[0].color = from;
  for (unsigned int i=1; i < m_point_count; i++)
    m_vertex_array[i].color = to;
}


void Cone::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  target.draw(m_vertex_array, states);
}

