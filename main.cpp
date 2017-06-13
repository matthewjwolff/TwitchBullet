#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include "GLDebugRenderer.cpp"
#include "bulletutils.cpp"

void glfw_error_callback(int error, const char* description) {
  printf("%d: %s", error, description);
}

int main(int argc, char** argv) {
  glfwSetErrorCallback(glfw_error_callback);

  glfwInit();
  GLFWwindow* window;
  window = glfwCreateWindow(640, 640, "Galaxy Sim", NULL, NULL);
  glfwMakeContextCurrent(window);

  if(glewInit() != GLEW_OK) {
    printf("failed to init glew");
    return -1;
  }

  // Initialize a bullet world
  btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
  btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

  btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

  dynamicsWorld->setGravity(btVector3(0.0f,0.0f,0.0f));

  // Initialize a debug drawer
  GLDebugRenderer* renderer = new GLDebugRenderer();
  dynamicsWorld->setDebugDrawer(renderer);

  // Add a box to the world

  // create the box's model. It's a cube of half-side-length 0.5
  btBoxShape* shape = new btBoxShape(btVector3(btScalar(0.5), btScalar(0.5), btScalar(0.5)));
  btScalar mass = btScalar(1000);
  btTransform trans;
  trans.setIdentity();
  trans.setOrigin(btVector3(btScalar(0.0f), btScalar(0.0f), btScalar(0.0f)));
  btDefaultMotionState* state = new btDefaultMotionState(trans);
  //btDefaultMotionState* defaultState = new btDefaultMotionState();

  // create a rigidbody. using backwards compatible version because who has time to use a factory
  btRigidBody* body = new btRigidBody(mass, state, shape);
  // Temporarily disabled for speed testing
  //dynamicsWorld->addRigidBody(body);

  glEnable(GL_DEPTH_TEST);
  S* a = new S(-4.0, 0, 0);
  S* b = new S(0.0, 0, 0);
  dynamicsWorld->addRigidBody(a->body);
  dynamicsWorld->addRigidBody(b->body);
  dynamicsWorld->debugDrawWorld();
  renderer->renderLines();

  float gravity = 4;
  // Apply initial velocity, vector will be in y direction
  a->body->setLinearVelocity(btVector3(0, 1.0, 0) * gravity);
  
  while(!glfwWindowShouldClose(window)) {
    // Windows are double-buffered. Tell GLFW we're done rendering to the
    // back buffer, so switch the buffers

    // but first actually use the buffer. Computer shits itself when you don't
    // cornflower blue lol
    glClearColor(0.3922, 0.5843, 0.9294, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //a->body->applyCentralForce(btVector3(btScalar(0.4f), btScalar(0.0f), btScalar(0.0f)));
    //Draw the world

    // Every timestep, apply a force toward the central body
    btTransform central, orbiter;
    b->body->getMotionState()->getWorldTransform(central);
    a->body->getMotionState()->getWorldTransform(orbiter);

    btVector3 toCenter = (central.getOrigin() - orbiter.getOrigin()).normalized();

    a->body->applyCentralForce(toCenter * (gravity+4));

    // Apply tidal force to rotate the body

    // a->body->applyTorque(btVector3(0, 0, 10.0f));

    dynamicsWorld->stepSimulation(btScalar(1./60.));
    btTransform newPos;
    a->body->getMotionState()->getWorldTransform(newPos);
    printf("(%f %f %f)\n", newPos.getOrigin().getX(), newPos.getOrigin().getY(), newPos.getOrigin().getZ());

    dynamicsWorld->debugDrawWorld();
    renderer->renderLines();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
