#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <stdio.h>

// In reality, G = 6.67408e-11, but bullet physics work better when not using astronomically (literally) large and small numbers. So let's boost the gravitational constant.
// #define G 6.67408e-1f
#define G 5.0f

btVector3 gravForceD(btRigidBody* a, btRigidBody* b, btVector3 d) {
  float m1 = 1.0f / a->getInvMass();
  float m2 = 1.0f / b->getInvMass();

  btTransform t1, t2;
  a->getMotionState()->getWorldTransform(t1);
  b->getMotionState()->getWorldTransform(t2);
  float r = t1.getOrigin().distance(t2.getOrigin());
  float f = G * ( (m1 * m2) / (r*r) );

  return d * f;
}

btVector3 gravForce(btRigidBody* a, btRigidBody* b) {
  float m1 = 1.0f / a->getInvMass();
  float m2 = 1.0f / b->getInvMass();

  btTransform t1, t2;
  a->getMotionState()->getWorldTransform(t1);
  btVector3 direction = (t2.getOrigin()-t1.getOrigin()).normalized();
  return gravForceD(a, b, direction);
}

class S {
public:
  btRigidBody* body;
  S(float x, float y, float z);
private:
  btDefaultMotionState* state;
  btBoxShape* shape;
};

S::S(float x, float y, float z) {
  btTransform trans;
  trans.setIdentity();
  trans.setOrigin(btVector3(btScalar(x), btScalar(y), btScalar(z)));
  this->state = new btDefaultMotionState(trans);
  this->shape = new btBoxShape(btVector3(btScalar(0.5f), btScalar(0.5f), btScalar(0.5f)));
  this->body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(btScalar(1.0f), state, shape));
}

struct C : btCollisionWorld::ContactResultCallback {
  btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1) {
    printf("A collision happened\n");
  }
};
