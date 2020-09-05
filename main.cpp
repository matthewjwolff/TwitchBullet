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

  // disable gravity. we will be manually controlling gravity
  dynamicsWorld->setGravity(btVector3(0.0f,0.0f,0.0f));

  // Initialize a debug drawer
  GLDebugRenderer* renderer = new GLDebugRenderer();
  dynamicsWorld->setDebugDrawer(renderer);

  // Add a box to the world

  // create the box's model. It's a cube of half-side-length 0.5
  btBoxShape* shape = new btBoxShape(btVector3(btScalar(5), btScalar(5), btScalar(5)));
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
  
  // i'm pretty sure orbits aren't stable because of rounding issues
  // because we are dealing with finite precision floats
  // if we tweak the numbers enough, the error should be minimal
  float M = 100000000;
  float m = 1;
  float r = 30;
  // really tiny
  float g = .00006;

  glEnable(GL_DEPTH_TEST);
  S* a = new S(-r, 0, 0, m);
  S* b = new S(0.0, 0, 0, M);
  dynamicsWorld->addRigidBody(a->body);
  dynamicsWorld->addRigidBody(b->body);
  dynamicsWorld->debugDrawWorld();
  renderer->renderLines();

  // i do not know if G=1 is an appropriate simplification
  //or maybe we should just use calculus and figure out the correct relationship between initial velocity and force of gravity
  float force_of_gravity = g*((M*m)/(r*r)); // G=1, it is a constant
  // Apply initial velocity, vector will be in y direction
  // initial velocity must be sqrt(G(M+m)/r)
  double velocity = sqrt(g*((M)/r));
  a->body->setLinearVelocity(btVector3(0, 1.0, 0) * velocity);
  btVector3 initV = a->body->getLinearVelocity();
  printf("initial velocity: (%f %f %f) m=%f a=%f\n", initV.getX(), initV.getY(), initV.getZ(), initV.length(), btDegrees(tan(initV.getY()/initV.getX())));
  
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
    
    btVector3 between = central.getOrigin() - orbiter.getOrigin();

    btVector3 toCenter = (between).normalized();

    // force is G(M*m / r)
    // force = mass times acceleration
    // acceleration = velocity^2 / r
    // we cannot disregard little m; it is acceleration that is constant, not force
    // a larger object needs a larger force to have the same acceleration as a smaller object
    double accel = velocity * velocity / r;
    double force = m * accel;
    a->body->applyCentralForce(toCenter * force);
    
    btVector3 vel = a->body->getLinearVelocity();

    // Apply tidal force to rotate the body

    // a->body->applyTorque(btVector3(0, 0, 10.0f));

    dynamicsWorld->stepSimulation(btScalar(1./120.));
    btTransform newPos;
    a->body->getMotionState()->getWorldTransform(newPos);
    printf("p=(%f %f %f) r=%f d=%f; v=(%f %f %f) m=%f a=%f\n", newPos.getOrigin().getX(), newPos.getOrigin().getY(), newPos.getOrigin().getZ(), between.length(), between.length()-r, vel.getX(), vel.getY(), vel.getZ(), vel.length(), btDegrees(tan(vel.getY() / vel.getX())));

    dynamicsWorld->debugDrawWorld();
    renderer->renderLines();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
