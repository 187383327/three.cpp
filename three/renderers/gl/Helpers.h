//
// Created by byter on 20.09.17.
//

#ifndef THREE_QT_HELPERS_H
#define THREE_QT_HELPERS_H

#include <string>
#include <Constants.h>

namespace three {
namespace gl {

struct MemoryInfo {
  unsigned geomtries = 0;
  unsigned textures = 0;
};

struct RenderInfo
{
  unsigned frame = 0;
  unsigned  calls = 0;
  unsigned  vertices = 0;
  unsigned  faces = 0;
  unsigned  points = 0;
};

struct Buffer
{
  GLuint buf;
  GLenum type;
  unsigned bytesPerElement;
  unsigned version;
};

}
}
#endif //THREE_QT_HELPERS_H
