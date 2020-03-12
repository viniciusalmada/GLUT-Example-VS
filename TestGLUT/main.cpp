#include <cstdio>
#include <GL/glew.h>
#include <GL/glut.h>

static void InitScene();

static void Init();

static void Display();

static void DrawScene();

static void CreateShaderProgram();

static void CompileShader(GLuint id);

static void LinkProgram(GLuint id);

static void Reshape(int w, int h);

GLuint prog;
GLuint vao;

static const char* vsource =
  "#version 430 core\n"
  "layout(location = 0) in vec4 inVertex;\n"
  "layout(location = 1) in vec4 inColor;\n"
  "out vec4 color;\n"
  "void main(void)\n"
  "{\n"
  "color = inColor;\n"
  "gl_Position = inVertex;\n"
  "}";

static const char* fsource =
  "#version 430 core\n"
  "in vec4 color;\n"
  "out vec4 FragColor;\n"
  "void main(void)\n"
  "{\n"
  "FragColor = color;\n"
  "}";

int main(int argc, char** argv)
{
  printf("%s", "main()\n");

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(800, 600);

  glutCreateWindow("OpenGL Sample Drawing with freeGLUT");
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);

  Init();

  glutMainLoop();
  return 0;
}

static void Reshape(int w, int h)
{
  printf("%s", "Reshape()\n");
  glViewport(0, 0, w, h);
}

void Error(const char* message)
{
  fprintf(stderr, "%s", message);
}

static void LinkProgram(GLuint id)
{
  printf("%s", "LinkProgram()\n");
  GLint status;
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if (!status)
  {
    GLint len;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
    char *message = new char();
    glGetProgramInfoLog(id, len, 0, message);
    Error(message);
    delete message;
  }
}

static void CompileShader(GLuint id)
{
  printf("%s", "CompileShader()\n");
  GLint status;
  glCompileShader(id);
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (!status)
  {
    GLint len;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
    char *message = new char();
    glGetShaderInfoLog(id, len, 0, message);
    Error(message);
    delete message;
  }
}


static void CreateShaderProgram()
{
  printf("%s", "CreateShaderProgram()\n");
  GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
  if (v_id == 0)
    Error("Could not create vertex shader object");
  glShaderSource(v_id, 1, &vsource, 0);
  CompileShader(v_id);

  GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);
  if (f_id == 0)
    Error("Could not create fragment shader object");
  glShaderSource(f_id, 1, &fsource, 0);
  CompileShader(f_id);

  prog = glCreateProgram();
  if (prog == 0)
    Error("Could not create program object");
  glAttachShader(prog, v_id);
  glAttachShader(prog, f_id);
  LinkProgram(prog);
}

static void DrawScene()
{
  printf("%s", "DrawScene()\n");
  glUseProgram(prog);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void Display()
{
  printf("%s", "Display()\n");
  static bool first = true;
  if (first)
  {
    InitScene();
    first = false;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  DrawScene();

  glutSwapBuffers();
}

static void Init()
{
  printf("%s", "Init()\n");

  // open GLEW
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    fprintf(stderr, "GLEW Error: %s\n", (const char*)glewGetErrorString(err));
    exit(-1);
  }

  // init OpenGL state
  printf("OpenGL version = %s\n", (const char*)glGetString(GL_VERSION));
  glClearColor(1.0f, 1.0f, 1.0f, 10.f);
  CreateShaderProgram();
}

static void InitScene()
{
  printf("%s", "InitScene()\n");
  static float coord[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
  };

  static float color[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint id[2];
  glGenBuffers(2, id);

  glBindBuffer(GL_ARRAY_BUFFER, id[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coord), (void*)coord, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, id[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), (void*)color, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);
}
