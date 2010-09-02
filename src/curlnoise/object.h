#ifndef CURLNOISE_OBJECT_H
#define CURLNOISE_OBJECT_H

#include "line.h"

#include <vector>

namespace curlnoise
{

typedef perlin::Point2< float > Float2;

class Object
{
public:

	Object( std::vector< Line* >& lines )
	 : m_lines( lines ) {}


	float distanceSq( const Float2& point ) const
	{
		std::vector< Line* >::const_iterator it = m_lines.begin();
		std::vector< Line* >::const_iterator end = m_lines.end();

		// TODO: fix
		float minDistanceSq = 10000.0f;
		for ( ; it != end; ++it )
		{
			float distanceSq = (*it)->distanceSq( point );
			if ( distanceSq < minDistanceSq )
				minDistanceSq = distanceSq;
		}

		return minDistanceSq;
	}


	void draw() const
	{
		std::vector< Line* >::const_iterator it = m_lines.begin();
		std::vector< Line* >::const_iterator end = m_lines.end();

		glBegin( GL_LINES );

		// TODO: fix
		for ( ; it != end; ++it )
		{
			(*it)->draw();
		}

		glEnd();
	}


private:

	const std::vector< Line* > m_lines;
};


class StaticObject : public Object
{
public:

	StaticObject( std::vector< Line* >& lines) : Object( lines ) {}
	
};

}; // namespace curlnoise

#endif // CURLNOISE_OBJECT_H


