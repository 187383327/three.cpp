//
// Created by byter on 30.09.17.
//

#ifndef THREE_QT_SHADERLIB_H
#define THREE_QT_SHADERLIB_H

#include <QString>
#include <QResource>

#include <helper/Shader.h>

namespace three {
namespace gl {

enum class ShaderID : unsigned
{
  basic=0,
  lambert=1,
  phong=2,
  standard=3,
  normal=4,
  points=5,
  dashed=6,
  depth=7,
  cube=8,
  equirect=9,
  distanceRGBA=10,
  shadow=11,
  physical=12,

  undefined=999
};

struct ShaderInfo
{
  const UniformValues &uniforms;
  const char * const vertexShader;
  const char * const fragmentShader;

  ShaderInfo(const UniformValues &uniforms, const char * vertexShader, const char * fragmentShader)
     : uniforms(uniforms), vertexShader(vertexShader), fragmentShader(fragmentShader)
  {}
};

namespace shaderlib {

ShaderInfo get(ShaderID id);

Shader get(ShaderID id, const char *name);

}

}
}
#endif //THREE_QT_SHADERLIB_H
