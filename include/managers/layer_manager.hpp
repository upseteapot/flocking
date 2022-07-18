#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


class LayerManager : public sf::Drawable
{
  public:
    LayerManager() = default;
    void setup(std::size_t layers);
    void add(std::size_t index, sf::Drawable *object);
    void clear_layer(std::size_t index);

  private:
    std::vector<std::vector<sf::Drawable *>> m_layers;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

