#ifndef CURLNOISE_H
#define CURLNOISE_H

#include <perlin/perlin.h>

#include <vector>

namespace curlnoise
{

typedef perlin::Point2< float > Float2;

class Noise2D
{
public:

	Noise2D( perlin::Noise2D* noise )
	 : m_noise( noise ) {}

	~Noise2D()
	{
		delete m_noise;
	}

	Float2 generate( float posX, float posY ) const
	{
		float dy = m_noise->generate( posX, posY + 0.01 ) - m_noise->generate( posX, posY - 0.01 );
		float dx = m_noise->generate( posX + 0.01, posY ) - m_noise->generate( posX - 0.01, posY );

		return Float2( dy, -dx );
	}

private:

	perlin::Noise2D* m_noise;
};


class CurlFactory
{
public:

	CurlFactory( perlin::NoiseFactory factory )
	 : m_noiseFactory( factory ) {}

	Noise2D* create2D()
	{
		perlin::Noise2D* perlinNoise = m_noiseFactory.create2D();

		return new Noise2D( perlinNoise );
	}

private:

	perlin::NoiseFactory m_noiseFactory;

};

}; // namespace curlnoise

#endif // CURLNOISE_H

