#ifndef CURLNOISE_LINE_H
#define CURLNOISE_LINE_H

#include <curlnoise/line.h>

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

private:

	const Float2 m_start;
	const Float2 m_dir;
};

}; // namespace curlnoise

#endif // CURLNOISE_H


