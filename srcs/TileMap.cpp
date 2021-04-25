#include "../incs/TileMap.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

TileMap::TileMap(unsigned int width, unsigned int height) : m_width(width), m_height(height)
{
	m_map.resize(m_width);
	for (int i = 0; i < m_height; ++i)
		m_map[i].resize(m_height);
	m_bitmask_ref = {{0, 47}, {1, 0}, {2, 1}, {8, 2}, {10, 3}, {11, 4}, {16, 5}, {18, 6}, {22, 7}, {24, 8},
					{26, 9}, {27, 10}, {30, 11}, {31, 12}, {64, 13}, {66, 14}, {72, 15}, {74, 16}, {75, 17},
					{80, 18}, {82, 19}, {86, 20}, {88, 21}, {90, 22}, {91, 23}, {94, 24}, {95, 25},
					{104, 26}, {106, 27}, {107, 28}, {120, 29}, {122, 30}, {123, 31}, {126, 32}, {127, 33},
					{208, 34}, {210, 35}, {214, 36}, {216, 37}, {218, 38}, {219, 39}, {222, 40}, {223, 41},
					{248, 42}, {250, 43}, {251, 44}, {254, 45}, {255, 46}};
}

TileMap::TileMap( const TileMap & src )
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

TileMap::~TileMap()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

TileMap &				TileMap::operator=( TileMap const & rhs )
{
	if ( this != &rhs )
	{
		this->m_vertices = rhs.m_vertices;
		this->m_tileset = rhs.m_tileset;
		this->m_map = rhs.m_map;
		this->m_tileSize = rhs.m_tileSize;
		this->m_bitmask_ref = rhs.m_bitmask_ref;
		this->m_width = rhs.m_width;
		this->m_height = rhs.m_height;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	TileMap::load(const std::string& tileset, sf::Vector2u tileSize)
{
	if (!m_tileset.loadFromFile(tileset))
		return false;
	m_tileSize = tileSize;
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(m_width * m_height * 4);
	for (unsigned int i = 0; i < m_width; ++i)
		for (unsigned int j = 0; j < m_height; ++j)
			texturetile(i, j);
	return true;
}

void	TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}

void	TileMap::toggletile(unsigned int width, unsigned int height)
{
	m_map[width][height] = !m_map[width][height];
	if (height && m_map[width][height-1] )
	{
		texturetile(width, height-1);//			N
		if (width && m_map[width][height-1] && m_map[width-1][height] && m_map[width-1][height-1])
			texturetile(width-1, height-1);// 	NW
		if (width < (m_width - 1) && m_map[width][height-1] && m_map[width+1][height] && m_map[width+1][height-1])
			texturetile(width+1, height-1);// 	SE
	}
	if (width && m_map[width-1][height])
		texturetile(width-1, height);//			W
	if (width < (m_width - 1) && m_map[width+1][height])
		texturetile(width+1, height);//			E
	if (height < (m_height - 1) && m_map[width][height+1])
	{
		texturetile(width, height+1);//			S
		if (width && m_map[width][height+1] && m_map[width-1][height] && m_map[width-1][height+1])
			texturetile(width-1, height+1);// 	SW
		if (width < (m_width - 1) && m_map[width][height+1] && m_map[width+1][height] && m_map[width+1][height+1])
			texturetile(width+1, height+1);// 	SE
	}
	texturetile(width, height);
}

void	TileMap::texturetile(unsigned int width, unsigned int height)
{
	int				tileNumber = getref(width, height);

	int tu = tileNumber % (m_tileset.getSize().x / m_tileSize.x);
	int tv = tileNumber / (m_tileset.getSize().x / m_tileSize.x);
	sf::Vertex* quad = &m_vertices[(width + height * m_width) * 4];
	quad[0].position = sf::Vector2f(width * m_tileSize.x, height * m_tileSize.y);
	quad[1].position = sf::Vector2f((width + 1) * m_tileSize.x, height * m_tileSize.y);
	quad[2].position = sf::Vector2f((width + 1) * m_tileSize.x, (height + 1) * m_tileSize.y);
	quad[3].position = sf::Vector2f(width * m_tileSize.x, (height + 1) * m_tileSize.y);
	quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, tv * m_tileSize.y);
	quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, tv * m_tileSize.y);
	quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, (tv + 1) * m_tileSize.y);
	quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, (tv + 1) * m_tileSize.y);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

unsigned int	TileMap::gettile(unsigned int width, unsigned int height)
{
	if (width >= m_width || height >= m_height)
		return(42);
	return (m_map[width][height]);
}

unsigned int	TileMap::getref(unsigned int width, unsigned int height)
{
	unsigned int	north_tile = 0,south_tile = 0,west_tile = 0,east_tile = 0,north_east_tile = 0,north_west_tile = 0,south_east_tile = 0,south_west_tile = 0;
	int				tileNumber = 0;

	if (m_map[width][height])
	{
		if (height >= 1 && m_map[width][height-1])
		{
			if (width && m_map[width-1][height])
				north_west_tile = m_map[width-1][height-1];
			north_tile = m_map[width][height-1] << 1;
			if (width < (m_width - 1) && m_map[width+1][height])
				north_east_tile = m_map[width+1][height-1] << 2;
		}
		if (width >= 1)
			west_tile = m_map[width-1][height] << 3;
		if (width < (m_width - 1))
			east_tile = m_map[width+1][height] << 4;
		if (height < (m_height - 1) && m_map[width][height+1])
		{
			if (width > 0 && m_map[width-1][height])
				south_west_tile = m_map[width-1][height+1] << 5;
			south_tile = m_map[width][height+1] << 6;
			if (width < (m_width - 1) && m_map[width+1][height])
				south_east_tile = m_map[width+1][height+1] << 7;
		}
		tileNumber = north_west_tile | north_tile | north_east_tile | west_tile | east_tile | south_west_tile | south_tile | south_east_tile;
		if (!tileNumber)
			tileNumber = 1;
	}
	return (m_bitmask_ref[tileNumber]);
}

/* ************************************************************************** */