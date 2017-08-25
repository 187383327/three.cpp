//
// Created by byter on 20.08.17.
//

#ifndef THREE_QT_PLANE_H
#define THREE_QT_PLANE_H

#include <cstdint>
#include "core/StaticGeometry.h"
#include "core/BufferGeometry.h"

namespace three {
namespace geometry {

struct PlaneParameters
{
  float width;
  float height;
  float widthSegments;
  float heightSegments;

  PlaneParameters(float width, float height, float widthSegments, float heightSegments)
     : width(width), height(height), widthSegments(widthSegments),
       heightSegments(heightSegments)
  {}
};

namespace buffer {

class Plane : public BufferGeometry, public PlaneParameters
{
public:
  Plane(float width, float height, float widthSegments, float heightSegments)
     : PlaneParameters(width, height, widthSegments, heightSegments)
  {
    float width_half = width / 2;
    float height_half = height / 2;

    unsigned gridX = (unsigned)std::floor(widthSegments > 0 ? widthSegments : 1);
    unsigned gridY = (unsigned)std::floor(heightSegments > 0 ? heightSegments : 1);

    unsigned gridX1 = gridX + 1;
    unsigned gridY1 = gridY + 1;

    float segment_width = width / gridX;
    float segment_height = height / gridY;

    // buffers
    std::vector<uint32_t> indices(gridX1 * gridY1 * 6);
    std::vector<float> vertices(gridX1 * gridY1 * 3);
    std::vector<float> normals(gridX1 * gridY1 * 3);
    std::vector<float> uvs(gridX1 * gridY1 * 2);

    // generate vertices, normals and uvs
    for (unsigned iy = 0; iy < gridY1; iy ++) {
      float y = iy * segment_height - height_half;

      for (unsigned ix = 0; ix < gridX1; ix ++ ) {
        float x = ix * segment_width - width_half;

        vertices.insert(vertices.end(), {x, - y, 0});

        normals.insert(normals.end(), {0, 0, 1});

        uvs.push_back( ix / gridX );
        uvs.push_back( 1 - ( iy / gridY ) );
      }
    }

    // indices
    for (unsigned iy = 0; iy < gridY; iy ++ ) {
      for (unsigned ix = 0; ix < gridX; ix ++ ) {

        unsigned a = ix + gridX1 * iy;
        unsigned b = ix + gridX1 * ( iy + 1 );
        unsigned c = ( ix + 1 ) + gridX1 * ( iy + 1 );
        unsigned d = ( ix + 1 ) + gridX1 * iy;

        // faces
        indices.insert(indices.end(), {a, b, d, b, c, d});
      }
    }

    // build geometry
    setIndex( indices );
    addAttribute("position", BufferAttribute<float>::make(vertices, 3));
    addAttribute("normal", BufferAttribute<float>::make(normals, 3));
    addAttribute("uv", BufferAttribute<float>::make(uvs, 2));
  }
};

}

class Plane : public StaticGeometry, public PlaneParameters
{
public:
  Plane(float width, float height, float widthSegments, float heightSegments)
     : PlaneParameters(width, height, widthSegments, heightSegments)
  {
    //setBufferGeometry(new buffer::Plane(width, height, widthSegments, heightSegments));
  }
};

}
}

#endif //THREE_QT_PLANE_H
