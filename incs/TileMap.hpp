#ifndef TILEMAP_HPP
# define TILEMAP_HPP

# include <SFML/Graphics.hpp>
# include <map>
# include <vector>
# include <iostream>

class TileMap : public sf::Drawable, public sf::Transformable
{

	public:

		TileMap(unsigned int width, unsigned int height);
		TileMap(TileMap const &src);
		~TileMap();

		TileMap&		operator=(TileMap const &rhs);
		bool	load(const std::string &tileset, sf::Vector2u tileSize);
		void	toggletile(unsigned int width, unsigned int height);
		void	texturetile(unsigned int width, unsigned int height);

		unsigned int	gettile(unsigned int width, unsigned int height);
		unsigned int	getref(unsigned int width, unsigned int height);

	private:

		virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const;

		sf::VertexArray					m_vertices;
		sf::Texture						m_tileset;
		std::vector<std::vector<int>>	m_map;
		sf::Vector2u					m_tileSize;
		std::map<int, int>				m_bitmask_ref;
		unsigned int					m_width;
		unsigned int					m_height;

};

#endif /* ********************************************************* TILEMAP_H */