//
// Created by byter on 11/17/17.
//

#ifndef THREEPP_REFRESH_UNIFORMS_H
#define THREEPP_REFRESH_UNIFORMS_H

#include <threepp/material/Material.h>

namespace three {
namespace gl {

template<typename ... Maps>
struct Refresh;

template<typename Map, typename ... Maps>
struct Refresh<Map, Maps...>
{
  template<typename ... Ms>
  static void mixin(UniformValues &uniforms, MaterialT<Ms...> &material)
  {
    Refresh<Map>::mixin(uniforms, material);
    Refresh<Maps...>::mixin(uniforms, material);
  }

  static void handle(UniformValues &uniforms, MaterialT<Map, Maps...> &material)
  {
    Refresh<Map>::mixin(uniforms, material);
    Refresh<Maps...>::mixin(uniforms, material);
  }
};

template<>
struct Refresh<material::Diffuse>
{
  static void handle(UniformValues &uniforms, MaterialT <material::Diffuse> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::Diffuse &material)
  {
    uniforms[UniformName::map] = material.map;
    uniforms[UniformName::diffuse] = material.color;
    uniforms[UniformName::opacity] = material.opacity;
  }
};

template<>
struct Refresh<material::LightMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::LightMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::LightMap &material)
  {
    uniforms[UniformName::lightMap] = material.lightMap;
    uniforms[UniformName::lightMapIntensity] = material.lightMapIntensity;
  }
};

template<>
struct Refresh<material::Emissive>
{
  static void handle(UniformValues &uniforms, MaterialT <material::Emissive> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::Emissive &material)
  {
    uniforms[UniformName::emissiveMap] = material.emissiveMap;
    uniforms[UniformName::emissive] = material.emissive * material.emissiveIntensity;
  }
};

template<>
struct Refresh<material::AoMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::AoMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::AoMap &material)
  {
    uniforms[UniformName::aoMap] = material.aoMap;
    uniforms[UniformName::aoMapIntensity] = material.aoMapIntensity;
  }
};

template<>
struct Refresh<material::EnvMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::EnvMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::EnvMap &material)
  {
    uniforms[UniformName::envMap] = material.envMap;

    // don't flip CubeTexture envMaps, flip everything else:
    //  WebGLRenderTargetCube will be flipped for backwards compatibility
    //  WebGLRenderTargetCube.texture will be flipped because it's a Texture and NOT a CubeTexture
    // this check must be handled differently, or removed entirely, if WebGLRenderTargetCube uses a CubeTexture in the future
    uniforms[UniformName::flipEnvMap] = (!(material.envMap && material.envMap->dontFlip())) ? 1.0f : -1.0f;

    uniforms[UniformName::reflectivity] = material.reflectivity;
    uniforms[UniformName::refractionRatio] = material.refractionRatio;
    uniforms[UniformName::envMapIntensity] = material.envMapIntensity;
  }
};

template<>
struct Refresh<material::AlphaMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::AlphaMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::AlphaMap &material)
  {
    uniforms[UniformName::alphaMap] = material.alphaMap;
  }
};

template<>
struct Refresh<material::SpecularMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::SpecularMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::SpecularMap &material)
  {
    uniforms[UniformName::specularMap] = material.specularMap;
  }
};

template<>
struct Refresh<material::Specular>
{
  static void handle(UniformValues &uniforms, MaterialT <material::Specular> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::Specular &material)
  {
    uniforms[UniformName::shininess] = std::max(material.shininess, 1e-4f); // to prevent pow( 0.0, 0.0 )
    uniforms[UniformName::specular] = material.specular;
    uniforms[UniformName::specularMap] = material.specularMap;
  }
};

template<>
struct Refresh<material::DisplacementMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::DisplacementMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::DisplacementMap &material)
  {
    if (material.displacementMap) {
      uniforms[UniformName::displacementMap] = material.displacementMap;
      uniforms[UniformName::displacementBias] = material.displacementBias;
      uniforms[UniformName::displacementScale] = material.displacementScale;
    }
  }
};

template<>
struct Refresh<material::BumpMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::BumpMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::BumpMap &material)
  {
    uniforms[UniformName::bumpMap] = material.bumpMap;
    uniforms[UniformName::bumpScale] = material.bumpScale;
  }
};

template<>
struct Refresh<material::NormalMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::NormalMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::NormalMap &material)
  {
    uniforms[UniformName::normalMap] = material.normalMap;
    uniforms[UniformName::normalScale] = material.normalScale;
  }
};

template<>
struct Refresh<material::RoughnessMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::RoughnessMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::RoughnessMap &material)
  {
    uniforms[UniformName::roughness] = material.roughness;
    uniforms[UniformName::roughnessMap] = material.roughnessMap;
  }
};

template<>
struct Refresh<material::MetalnessMap>
{
  static void handle(UniformValues &uniforms, MaterialT <material::MetalnessMap> &material)
  {
    mixin(uniforms, material);
  }

  static void mixin(UniformValues &uniforms, material::MetalnessMap &material)
  {
    uniforms[UniformName::metalness] = material.metalness;
    uniforms[UniformName::metalnessMap] = material.metalnessMap;
  }
};

template<typename ... Maps>
void refresh(UniformValues &uniforms, MaterialT<Maps...> &material)
{
  Refresh<Maps...>::handle(uniforms, material);

  // uv repeat and offset setting priorities
  // 1. color map
  // 2. specular map
  // 3. normal map
  // 4. bump map
  // 5. alpha map
  // 6. emissive map

  Texture::Ptr uvScaleMap;

  if ((uniforms.contains(UniformName::map) && (uvScaleMap = uniforms[UniformName::map].get<Texture::Ptr>()))
      || (uniforms.contains(UniformName::specularMap) && (uvScaleMap = uniforms[UniformName::specularMap].get<Texture::Ptr>()))
      || (uniforms.contains(UniformName::displacementMap) && (uvScaleMap = uniforms[UniformName::displacementMap].get<Texture::Ptr>()))
      || (uniforms.contains(UniformName::normalMap) && (uvScaleMap = uniforms[UniformName::normalMap].get<Texture::Ptr>()))
      || (uniforms.contains(UniformName::bumpMap) && (uvScaleMap = uniforms[UniformName::bumpMap].get<Texture::Ptr>()))
      || (uniforms.contains(UniformName::roughnessMap) && (uvScaleMap = uniforms[UniformName::roughnessMap].get<Texture::Ptr>()))
      || (uniforms.contains(UniformName::metalnessMap) && (uvScaleMap = uniforms[UniformName::metalnessMap].get<Texture::Ptr>()))
      || (uniforms.contains(UniformName::alphaMap) && (uvScaleMap = uniforms[UniformName::alphaMap].get<Texture::Ptr>()))
      || (uniforms.contains(UniformName::emissiveMap) && (uvScaleMap = uniforms[UniformName::emissiveMap].get<Texture::Ptr>())))
  {
    if (uvScaleMap->matrixAutoUpdate()) {

      uvScaleMap->updateMatrix();
    }
    uniforms[UniformName::uvTransform] = uvScaleMap->matrix();
  }
}

}
}

#endif //THREEPP_REFRESH_UNIFORMS_H
