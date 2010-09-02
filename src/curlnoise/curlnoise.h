#ifndef CURLNOISE_H
#define CURLNOISE_H

#include <perlin/perlin.h>
#include "object.h"

#include <vector>

namespace curlnoise
{

typedef perlin::Point2< float > Float2;

class Noise2D
{
public:

	Noise2D( perlin::Noise2D* noise, std::vector< Object* > objects )
	 : m_noise( noise ),
	   m_objects( objects ) {}

	~Noise2D()
	{
		delete m_noise;
	}

	Float2 generate( float posX, float posY ) const
	{
		return generate( Float2( posX, posY ) );
	}

	Float2 generate( const Float2& p ) const
	{
		float eps = 0.01;

		float dy = scale( p.x, p.y + eps ) * m_noise->generate( p.x, p.y + eps ) - scale( p.x, p.y - eps ) * m_noise->generate( p.x, p.y - eps );
		float dx = scale( p.x + eps, p.y ) * m_noise->generate( p.x + eps, p.y ) - scale( p.x - eps, p.y ) * m_noise->generate( p.x - eps, p.y );

		return Float2( dy, -dx );
	}

	float scale( float x, float y ) const 
	{
		Float2 p( x, y );

		std::vector< Object* >::const_iterator it = m_objects.begin();
		std::vector< Object* >::const_iterator end = m_objects.end();

		// TODO: Fix
		float minDistanceSq = 10000.0f;

		for ( ; it != end; ++it )
		{
			float distanceSq = (*it)->distanceSq( p );
			if ( distanceSq < minDistanceSq ) 
				minDistanceSq = distanceSq;
		}

		float scale = 1.0f;
		float r = sqrt( minDistanceSq );
		r /= 0.5f;
		if ( r < 1.0f )
		{
			scale = (15.0f/8.0f) * r - ( 10.0f/8.0f ) * r * r * r + ( 3.0f/8.0f ) * r * r * r * r * r;
		}

		return scale;
	}

private:

	perlin::Noise2D* m_noise;
	std::vector< Object* > m_objects;
};


class CurlFactory
{
public:

	CurlFactory( perlin::NoiseFactory factory )
	 : m_noiseFactory( factory ) {}

	Noise2D* create2D( std::vector< Object* > boundaries )
	{
		perlin::Noise2D* perlinNoise = m_noiseFactory.create2D();

		return new Noise2D( perlinNoise, boundaries );
	}

private:

	perlin::NoiseFactory m_noiseFactory;

};

}; // namespace curlnoise

#endif // CURLNOISE_H

