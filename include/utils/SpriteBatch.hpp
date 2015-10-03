#ifndef SFMLUTILS_SPRITEBATCH_HPP
#define SFMLUTILS_SPRITEBATCH_HPP

#include <SFML/Graphics.hpp>

namespace sfutils
{
	class SpriteBatch
	{
		public:
			SpriteBatch(const sf::Texture* tex = 0);
			SpriteBatch(int size);
			SpriteBatch(int size, const sf::Texture* tex);

			void Add(const sf::FloatRect& sprrect, const sf::IntRect& texrect);
			void Add(const sf::Sprite& sprite);
			void Clear();

			size_t Size() const;

			void SetTexture(const sf::Texture* tex);
			void SetTransform(const sf::Transform& transform);

			void Render(sf::RenderTarget& surface) const;
			void Render(sf::RenderTarget& surface, const sf::RenderStates& states) const;

		private:
			const sf::Texture* _tex;
			sf::VertexArray _vertices;
			sf::Transform _transform;
	};
}

#endif
