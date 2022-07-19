#include "graphics/layer_manager.hpp"


void LayerManager::setup(std::size_t layers)
{
  m_layers.resize(layers);
}

void LayerManager::add(std::size_t index, sf::Drawable *object)
{
  m_layers[index].push_back(object);
}

void LayerManager::clear_layer(std::size_t index)
{
  m_layers[index].clear();
}


void LayerManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  for (auto &layer : m_layers)
    for (auto &object : layer)
      target.draw(*object, states);
}

