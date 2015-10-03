#include "utils/SpriteBatch.hpp"

namespace sfutils
{
	SpriteBatch::SpriteBatch(const sf::Texture* tex) : _tex(tex), _vertices(sf::Quads)
	{

	}

	SpriteBatch::SpriteBatch(int size) : _tex(0), _vertices(sf::Quads, size * 6)
	{

	}

	SpriteBatch::SpriteBatch(int size, const sf::Texture* tex) : _tex(tex), _vertices(sf::Quads, size * 6)
	{

	}


	void SpriteBatch::Add(const sf::FloatRect& sprrect, const sf::IntRect& texrect)
	{
		auto rect(_transform.transformRect(sprrect));

		_vertices.append(
				sf::Vertex(sf::Vector2f(rect.left, rect.top),
				sf::Vector2f(texrect.left, texrect.top)));
		_vertices.append(sf::Vertex(
				sf::Vector2f(rect.left + rect.width, rect.top),
				sf::Vector2f(texrect.left + texrect.width, texrect.top)));
		_vertices.append(sf::Vertex(
				sf::Vector2f(rect.left + rect.width, rect.top + rect.height),
				sf::Vector2f(texrect.left + texrect.width, texrect.top + texrect.height)));
		_vertices.append(sf::Vertex(
				sf::Vector2f(rect.left, rect.top + rect.height),
				sf::Vector2f(texrect.left, texrect.top + texrect.height)));
	}

	void SpriteBatch::Add(const sf::Sprite& sprite)
	{
		sf::IntRect trect(sprite.getTextureRect());
		sf::FloatRect rect(sprite.getLocalBounds());
		rect.left = sprite.getPosition().x;
		rect.top = sprite.getPosition().y;

		Add(rect, trect);
	}

	void SpriteBatch::Clear()
	{
		_vertices.clear();
	}


	size_t SpriteBatch::Size() const
	{
		return _vertices.getVertexCount();
	}


	void SpriteBatch::SetTexture(const sf::Texture* tex)
	{
		_tex = tex;
	}

	void SpriteBatch::SetTransform(const sf::Transform& transform)
	{
		_transform = transform;
	}


	void SpriteBatch::Render(sf::RenderTarget& surface) const
	{
		sf::RenderStates states(_tex);
		//states.transform = _transform;
		Render(surface, states);
	}

	void SpriteBatch::Render(sf::RenderTarget& surface, const sf::RenderStates& states) const
	{
		surface.draw(_vertices, states);
	}
}
