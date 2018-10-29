//
// Created by byter on 10/21/18.
//

#ifndef THREE_PP_MODEL_H
#define THREE_PP_MODEL_H

#include <threepp/core/Object3D.h>
#include <threepp/scene/Scene.h>
#include <threepp/objects/Mesh.h>
#include <reactphysics3d/reactphysics3d.h>
#include <unordered_map>
#include <set>
#include <mutex>
#include "timer.h"

namespace reactphysics3d {
  class RigidBody;
}

namespace three {
namespace react3d {

class PhysicsObject
{
  friend class PhysicsScene;

  rp3d::RigidBody * const _body;
  std::unordered_map<std::string, rp3d::RigidBody *> _extraBodies;
  rp3d::Transform _previousTransform;

  math::Matrix4 _scalingMatrix = math::Matrix4::identity();

public:
  void updateTransform(Object3D *object, float interpolation);

  PhysicsObject(rp3d::RigidBody * body, const rp3d::Transform &transform)
     : _body(body), _previousTransform(transform)
  {}

  rp3d::RigidBody * body() const {return _body;}
};

class PhysicsScene
{
  Timer _timer;

  rp3d::DynamicsWorld *_dynamicsWorld;

  float _interpolationFactor = 0.0f;

  std::mutex _createBodyMutex;

  std::unordered_map<Object3D *, PhysicsObject> _objects;
  std::set<rp3d::Joint*> _joints;

public:
  PhysicsScene(Scene::Ptr object, rp3d::DynamicsWorld *dynamicsWorld) : _dynamicsWorld(dynamicsWorld) {}
  ~PhysicsScene();

  using Ptr = std::shared_ptr<PhysicsScene>;
  static Ptr make(Scene::Ptr scene, rp3d::DynamicsWorld *dynamicsWorld) {
    return Ptr(new PhysicsScene(scene, dynamicsWorld));
  }

  Timer &timer() {return _timer;}

  void reset();

  void update();

  rp3d::DynamicsWorld *dynamicsWorld() {return _dynamicsWorld;}

  rp3d::HingeJoint* createHingeJoint(const rp3d::HingeJointInfo& jointInfo);

  PhysicsObject *createPhysics(Object3D::Ptr object, bool addShapes=false);

  PhysicsObject *getPhysics(Object3D::Ptr object, bool create=false, bool addShapes=false);

  rp3d::RigidBody *getPhysics(Object3D::Ptr object,
                              const std::string &name,
                              const rp3d::Vector3 &hingePosition,
                              rp3d::CollisionShape *shape,
                              const rp3d::Transform &shapeTransform,
                              rp3d::decimal shapeMass);

  void remove(Object3D::Ptr object);
  void remove(rp3d::Joint *joint);
};

}
}

#endif //THREE_PP_MODEL_H