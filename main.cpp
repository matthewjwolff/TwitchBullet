#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include "GLDebugRenderer.cpp"

// TODO: use real header file

void error_callback(int error, const char* description) {
  printf("%d: %s", error, description);
}

int main(int argc, char** argv) {
  // standard main method

  // Strategy: use OpenGL (and possibly GLFW) to render
  // Make a simple vertex and fragment shader with one color
  // Initialize everything, possibly set a glClearColor
  // register the debug drawer with the physics system

  // PHYSICS LOOP
  // glClear the screen
  // step through the world, which will call the debug object's drawLine

  glfwSetErrorCallback(error_callback);

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

  //dynamicsWorld->setGravity(btVector3(0,-10,0));

  // Initialize a debug drawer
  GLDebugRenderer* renderer = new GLDebugRenderer();
  dynamicsWorld->setDebugDrawer(renderer);

  // Add a box to the world

  // create the box's model. It's a cube of half-side-length 0.5
  btBoxShape* shape = new btBoxShape(btVector3(btScalar(0.5), btScalar(0.5), btScalar(0.5)));
  btScalar mass = btScalar(1);
  btTransform trans;
  trans.setIdentity();
  trans.setOrigin(btVector3(btScalar(4.0f), btScalar(0.0f), btScalar(0.0f)));
  btDefaultMotionState* state = new btDefaultMotionState(trans);
  //btDefaultMotionState* defaultState = new btDefaultMotionState();

  // create a rigidbody. using backwards compatible version because who has time to use a factory
  btRigidBody* body = new btRigidBody(mass, state, shape);
  dynamicsWorld->addRigidBody(body);

  btTransform trans2;
  trans2.setIdentity();
  trans2.setOrigin(btVector3(btScalar(1.0f), btScalar(0.0f), btScalar(0.0f)));
  btBoxShape* shape2 = new btBoxShape(*shape);
  btDefaultMotionState* state2 = new btDefaultMotionState(trans2);
  btRigidBody* body2 = new btRigidBody(btScalar(1), state2, shape2);

  dynamicsWorld->addRigidBody(body2);
  //dynamicsWorld->stepSimulation(btScalar(1./60.));

  glEnable(GL_DEPTH_TEST);
  
  while(!glfwWindowShouldClose(window)) {
    // Windows are double-buffered. Tell GLFW we're done rendering to the
    // back buffer, so switch the buffers

    // but first actually use the buffer. Computer shits itself when you don't
    // cornflower blue lol
    glClearColor(0.3922, 0.5843, 0.9294, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw the world
    //dynamicsWorld->stepSimulation(btScalar(1./60.));
    dynamicsWorld->debugDrawWorld();
    renderer->renderLines();

    // Render
    glfwSwapBuffers(window);

    // Handle window events (dragging, resizing, closing)
    glfwPollEvents();
  }
  glfwTerminate();
  delete body2;
  delete body;
  delete shape;
  delete shape2;
  delete state;
  delete state2;
  delete renderer;
  delete dynamicsWorld;
  delete solver;
  delete overlappingPairCache;
  delete dispatcher;
  delete collisionConfiguration;
  return 0;
}
