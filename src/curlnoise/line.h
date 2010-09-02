#ifndef CURLNOISE_LINE_H
#define CURLNOISE_LINE_H

#include <GL/gl.h>

namespace curlnoise
{

typedef perlin::Point2< float > Float2;

class Line
{
public:

	Line( Float2 start, Float2 dir )
     : m_start( start ),
	   m_dir( dir ) {}

	float distanceSq( const Float2& point ) const
	{
		float parameter = ( point - m_start ).dot( m_dir ) / m_dir.lengthSq();

		if ( parameter > 1.0f )
			return ( ( m_start + m_dir ) - point ).lengthSq();

		else if ( parameter < 0.0f )
			return ( m_start - point ).lengthSq();

		return ( ( m_start + ( m_dir * parameter ) ) - point ).lengthSq();
	}

	void draw() const
	{
		glVertex3f( m_start.x, m_start.y, -20.0f );

		glVertex3f( m_start.x + m_dir.x, m_start.y + m_dir.y, -20.0f );
	}

private:

	const Float2 m_start;
	const Float2 m_dir;
};


class LineFactory 
{
public:

	LineFactory() {}
	
	Line* between( const Float2& start, const Float2& end ) const
	{
		return new Line( start, end - start );
	}

	Line* create( const Float2& start, const Float2& dir ) const
	{
		return new Line( start, dir );
	}
};

}; // namespace curlnoise

#endif // CURLNOISE_H


