#include <bullet/LinearMath/btIDebugDraw.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BUFSIZE 1024

glm::vec4 btToGlm(const btVector3 &o) {
  return glm::vec4(o.getX(), o.getY(), o.getZ(), 1.0);
}

class GtkDebugRenderer : public btIDebugDraw {
public:

  glm::mat4 proj;
  
  GtkDebugRenderer() {
    // This one is in radians
    this->proj = glm::perspective ( 45.0f, 1.0f, 0.1f, 100.0f ) * glm::lookAt( glm::vec3(0, 0, 2), glm::vec3(0,0,0), glm::vec3(0,1,0));
  }
  
  void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    // Coords are given in world coordinates. Use OpenGL for perspective.
    // Have to use perspective to get depth and Z axis
    // load color and vertex data to graphics card (bind buffers)
    // glDrawArrays(GL_LINES) probably

    // either the above or collect and copy (OpenGL >2)
    // using old opengl

    glm::vec4 gFrom = proj * btToGlm(from);
    glm::vec4 gTo = proj * btToGlm(to);

    printf("(%f, %f, %f) to (%f, %f, %f)\n\n", gFrom.x, gFrom.y, gFrom.z, gTo.x, gTo.y, gTo.z);
    glColor3f(color.getX(), color.getY(), color.getZ());
    glVertex3f(gFrom.x, gFrom.y, gFrom.z);
    glVertex3f(gTo.x, gTo.y, gTo.z);
  }

  // unused virtual functions
  void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {}

  void reportErrorWarning(const char* warningString) {}

  void draw3dText(const btVector3& location,const char* textString) {}

  void setDebugMode(int debugMode) {}

  int  getDebugMode() const { return 1; }
};
