#pragma once

struct Shader {
	unsigned vert;
	unsigned frag;
	unsigned prog;

	/// <summary>
	/// Get a handle to a uniform variable from its name.
	/// </summary>
	/// <param name="name">Name of the uniform variable inside the shader.</param>
	/// <returns>A handle to the uniform variable.</returns>
	unsigned get_uniform(const char* name) const;

	Shader(const char* vert_fname, const char* frag_fname);

	void set_mat4(const char* name, float* ptr00) const;
	void set_vec3(const char* name, float x, float y, float z) const;
	void set_float(const char* name, float x) const;
	void set_int(const char* name, int x) const;
private:
	/// <summary>
	/// Load a single shader from a file.
	/// </summary>
	/// <param name="fname">Path to the shader source code.</param>
	/// <param name="shader_type">Type of shader(G_VERTEX_SHADER or GL_FRAGMENT_SHADER).</param>
	/// <returns>A handle to the shader.</returns>
	unsigned int load_shader(const char* fname, int shader_type);
	/// <summary>
	/// Check if there were shader compile errors and exit the program if there were.
	/// </summary>
	/// <param name="fname">Path to the shader source code. Used for printing to the output.</param>
	/// <param name="gl_shader">Handle to the shader returned by load_shader</param>
	void check_compile_errors(const char* fname, unsigned gl_shader);
};