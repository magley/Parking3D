#include "shader.h"
#include "GL/glew.h"
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const char* vert_fname, const char* frag_fname) {
    vert = load_shader(vert_fname, GL_VERTEX_SHADER);
    check_compile_errors(vert_fname, vert);

    frag = load_shader(frag_fname, GL_FRAGMENT_SHADER);
    check_compile_errors(frag_fname, frag);

    prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);
}

unsigned Shader::get_uniform(const char* name) {
    return glGetUniformLocation(prog, name);
}

unsigned int Shader::load_shader(const char* fname, int shader_type) {
    std::ifstream f(fname);
    if (!f) {
        printf("Could not open file %s\n", fname);
        exit(1);
    }
    std::stringstream ss;
    ss << f.rdbuf();
    std::string shader_code = ss.str();
    const char* shader_code_c = shader_code.c_str();

    unsigned int shd = glCreateShader(shader_type);
    glShaderSource(shd, 1, &shader_code_c, 0);
    glCompileShader(shd);
    check_compile_errors(fname, shd);
    return shd;
}

void Shader::check_compile_errors(const char* fname, unsigned gl_shader) {
    int success = 0;
    glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        const int len = 256;
        char log[len];
        glGetShaderInfoLog(gl_shader, len, (int*)&len, &log[0]);

        printf("Could not compile shader %s\n%s\n", fname, log);
        exit(1);
    }
}