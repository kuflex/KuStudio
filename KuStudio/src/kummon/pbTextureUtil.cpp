#include "pbTextureUtil.h"


//--------------------------------------------------------------------------
void pbTextureUtil::drawQuads( vector<float> &pointP, vector<float> &textureP, ofTexture &tex, bool _3d )
{
	int k = pointP.size();
	int step = ( _3d ) ? 12 : 8;
	int nQuads = k / step;
	if ( nQuads * step != k || k <= 0 || textureP.size() != k ) {
		cout << "ERROR in pbTextureUtil::drawQuads, have " <<  pointP.size() << " " << textureP.size() << endl;
	}
	else {
		drawQuads( &pointP[0], &textureP[0], nQuads, tex, _3d );
	}
}

//--------------------------------------------------------------------------
void pbTextureUtil::drawQuads( float *pointP, float *textureP, int nQuads, ofTexture &tex, bool _3d )
{
	if ( nQuads > 0 ) {
		int vertexCoords = ( _3d ) ? 3 : 2;

		tex.bind();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(vertexCoords, GL_FLOAT, 0, pointP);
		glTexCoordPointer( 2, GL_FLOAT, 0, textureP );
		glDrawArrays(GL_QUADS, 0, nQuads * 4);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		tex.unbind();
	}
}

//--------------------------------------------------------------------------
