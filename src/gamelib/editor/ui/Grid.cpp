#include "gamelib/editor/ui/Grid.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Grid:: Grid(int w, int h) :
        _gridsize(w, h),
        _lastsize(-1, -1),
        _cache(sf::Lines)
    { }

    void Grid::increase()
    {
        _gridsize /= 2;
        _gridsize.x = std::max(_gridsize.x, 1);
        _gridsize.y = std::max(_gridsize.y, 1);
        _recalculate(_lastsize);
    }

    void Grid::decrease()
    {
        _gridsize *= 2;
        _recalculate(_lastsize);
    }

    math::Point2f Grid::snap(const math::Point2f& p) const
    {
        return math::smartSnap(p, _gridsize.asPoint());
    }

    void Grid::_recalculate(const sf::Vector2f& size)
    {
        _lastsize = size;
        _cache.clear();

        const sf::Color color = sf::Color(150, 150, 150, 100);

        for (int i = 0; i < 1 + (size.x / _gridsize.x); ++i)
        {
            _cache.append(sf::Vertex(sf::Vector2f(i * _gridsize.x, 0), color));
            _cache.append(sf::Vertex(sf::Vector2f(i * _gridsize.x, size.y + _gridsize.y), color));
        }
        for (int i = 0; i < 1 + (size.y / _gridsize.y); ++i)
        {
            _cache.append(sf::Vertex(sf::Vector2f(0, i * _gridsize.y), color));
            _cache.append(sf::Vertex(sf::Vector2f(size.x + _gridsize.x, i * _gridsize.y), color));
        }
    }

    void Grid::render(sf::RenderTarget& target)
    {
        auto pos = target.getView().getCenter() - target.getView().getSize();
        auto size = target.getView().getSize();

        if (size != _lastsize || _lastsize.x < 0)
            _recalculate(size);

        // Don't ask me why it needs to be translated by pos + size / 2
        // TODO: Maybe find out why
        sf::Transform offset;
        offset.translate(math::snap(pos.x + size.x / 2, _gridsize.x),
                         math::snap(pos.y + size.y / 2, _gridsize.y));
        target.draw(_cache, offset);
    }
}
