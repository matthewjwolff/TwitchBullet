#include <LinearMath/btIDebugDraw.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shaderutils.cpp"

#define BUFSIZE 1024

glm::vec4 btToGlm(const btVector3 &o) {
  return glm::vec4(o.getX(), o.getY(), o.getZ(), 1.0);
}

class GLDebugRenderer : public btIDebugDraw {

private:

  std::vector<btVector3> points;
  std::vector<btVector3> colors;

  GLuint vertarrid;
  GLuint pointBuffer;
  GLuint colorBuffer;

  GLuint program;

  bool printed;
  
public:
  glm::mat4 proj;
  
  GLDebugRenderer() {
    // Set up GL code
    // Bind a Vertex Array Object. This will contain all the state of our scene. Not really useful, but it's required.
    glGenVertexArrays(1, &vertarrid);
    glBindVertexArray(vertarrid);
    glGenBuffers(1, &pointBuffer);
    glGenBuffers(1, &colorBuffer);

    this->program = LoadShaders("vertex.glsl", "fragment.glsl");
    glUseProgram(this->program);

    // This one is in radians
    this->proj = glm::perspective ( 45.0f, 1.0f, 0.1f, 100.0f ) * glm::lookAt( glm::vec3(0, 0, 12), glm::vec3(0,0,0), glm::vec3(0,1,0));

    // load matrix
    glUniformMatrix4fv(glGetUniformLocation(this->program, "MVP"),1,GL_FALSE,&this->proj[0][0]);
    this->printed = false;
    
  }
  
  void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    // Coords are given in world coordinates. Use OpenGL for perspective.
    // Have to use perspective to get depth and Z axis
    // load color and vertex data to graphics card (bind buffers)
    // glDrawArrays(GL_LINES) probably


    // Store points in GL format
    points.push_back(from);
    points.push_back(to);
    colors.push_back(color);
    // either the above or collect and copy (OpenGL >2)
    // using old opengl

    // glm::vec4 gFrom = proj * btToGlm(from);
    // glm::vec4 gTo = proj * btToGlm(to);

    // printf("(%f, %f, %f) to (%f, %f, %f)\n\n", gFrom.x, gFrom.y, gFrom.z, gTo.x, gTo.y, gTo.z);
    // glColor3f(color.getX(), color.getY(), color.getZ());
    // glVertex3f(gFrom.x, gFrom.y, gFrom.z);
    // glVertex3f(gTo.x, gTo.y, gTo.z);
  }

  void renderLines() {
    // create an array of the points
    GLfloat fpoints[points.size() * 3];
    for(int i=0; i<points.size(); i++) {
      fpoints[i*3] = points[i].getX();
      fpoints[(i*3)+1] = points[i].getY();
      fpoints[(i*3)+2] = points[i].getZ();
      if (!this->printed) {
        if ((i%2)==0)
          printf("(%f %f %f) - ", points[i].getX(), points[i].getY(), points[i].getZ());
        else
          printf("(%f %f %f)\n", points[i].getX(), points[i].getY(), points[i].getZ());
      }
    }
    // move the points to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float) * 3, fpoints, GL_STATIC_DRAW);
    // enable the first array attribute in the vertex shader (the points above)
    glEnableVertexAttribArray(0);
    // describe the attribute above. it's an attribute with 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // same for colors
    GLfloat fcolors[colors.size() *3];
    for(int i=0; i<colors.size(); i++) {
        fcolors[i*3] = colors[i].getX();
        fcolors[(i*3)+1] = colors[i].getY();
        fcolors[(i*3)+1] = colors[i].getZ();
    }
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float) * 3, fcolors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(glGetAttribLocation(this->program, "color"));
    glVertexAttribPointer(glGetAttribLocation(this->program, "color"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    // draw
    glDrawArrays(GL_LINES, 0, points.size() * 3);
    // cleanup
    glDisableVertexAttribArray(0);
    points.clear();
    colors.clear();
    this->printed=true;
  }

  // unused virtual functions
  void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {}

  void reportErrorWarning(const char* warningString) {}

  void draw3dText(const btVector3& location,const char* textString) {}

  void setDebugMode(int debugMode) {}

  int  getDebugMode() const { return 1; }
};

