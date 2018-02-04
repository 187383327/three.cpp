//
// Created by byter on 29.07.17.
//

#ifndef THREEQT_CAMERA
#define THREEQT_CAMERA

#include <three/math/Matrix4.h>
#include <three/math/Vector3.h>
#include <three/math/Quaternion.h>
#include <three/math/Ray.h>
#include <three/core/Object3D.h>
#include <three/helper/Resolver.h>

namespace three {

struct Viewport
{
  unsigned fullWidth=0;
  unsigned fullHeight=0;
  unsigned offsetX=0;
  unsigned offsetY=0;
  unsigned width=0;
  unsigned height=0;

  bool isNull() {
    return fullWidth == 0 && fullHeight == 0;
  }
};

class Raycaster;

class Camera : public Object3D
{
protected:
  math::Matrix4 _matrixWorldInverse;
  math::Matrix4 _projectionMatrix;
  float _zoom   = 1;
  float _near   = 0.1;
  float _far    = 2000;

  Camera(camera::Resolver::Ptr resolver)
    : Object3D(object::Resolver::makeNull()), cameraResolver(resolver), _projectionMatrix(math::Matrix4::identity()),
      _matrixWorldInverse(_matrixWorld.inverse()) {}

  Camera()
    : Object3D(object::Resolver::makeNull()), cameraResolver(camera::Resolver::makeNull()),
      _projectionMatrix(math::Matrix4::identity()), _matrixWorldInverse(_matrixWorld.inverse()) {}

  virtual ~Camera() {}

public:
  using Ptr = std::shared_ptr<Camera>;

  camera::Resolver::Ptr cameraResolver;

  const math::Matrix4 &projectionMatrix() const {return _projectionMatrix;}

  const math::Matrix4 &matrixWorldInverse() const {return _matrixWorldInverse;}

  math::Vector3 getWorldDirection() const override
  {
    math::Quaternion quaternion = getWorldQuaternion();

    return math::Vector3(0, 0, - 1).apply(quaternion);
  }

  void updateMatrixWorld(bool force) override {

    Object3D::updateMatrixWorld(force);

    _matrixWorldInverse = _matrixWorld.inverse();
  }

  void lookAt(const math::Vector3 &vector) override
  {
    math::Matrix4 m1( _position, vector, _up );

    _quaternion.set(m1);
  }

  float near() const {return _near;}

  void setNear(float near) {
    _near = near;
  }

  float far() const {return _far;}

  void setFar(float far) {
    _far = far;
  }

  float zoom() const {return _zoom;}

  void setZoom(float zoom) {
    _zoom = zoom;
  }

  void setNearFar(float near, float far) {
    _near = near;
    _far = far;
  }

  virtual void setAspect(float aspect) {}

  virtual void setup(math::Ray &ray, float x, float y) = 0;

  virtual void updateProjectionMatrix() = 0;
};

}


#endif //THREEQT_CAMERA
