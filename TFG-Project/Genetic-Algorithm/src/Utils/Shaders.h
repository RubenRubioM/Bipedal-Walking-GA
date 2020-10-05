#pragma once

#include <string>
#include <ostream>

/// <summary>
/// Shader class template.
/// </summary>
struct Shader {
	/// <summary>
	/// Assign operator overload.
	/// </summary>
	/// <param name="in"> Shader to assign. </param>
	void operator=(const Shader &in) {
		vertex = in.vertex;
		fragment = in.fragment;
	}

	/// <summary>
	/// Output operator overload.
	/// </summary>
	/// <param name="out"> Output. </param>
	/// <param name="in"> Shader to output. </param>
	/// <returns> Output. </returns>
	friend std::ostream& operator<<(std::ostream& out, const Shader& in){
		out << in.vertex << std::endl << in.fragment << std::endl;
		return out;
	}

	/// <summary>
	/// Vertex shader path.
	/// </summary>
	std::string vertex;

	/// <summary>
	/// Fragment shader path.
	/// </summary>
	std::string fragment;

};

/// <summary>
/// Static class to storage shaders.
/// </summary>
class Shaders {
	public:
		/// <summary>
		/// Basic shader without lights.
		/// </summary>
		inline static const Shader BasicShader{ "src/CLEngine/src/Shaders/basicShader.vert","src/CLEngine/src/Shaders/basicShader.frag" };

		/// <summary>
		/// Cartoon shader with lights.
		/// </summary>
		inline static const Shader CartoonShader{ "src/CLEngine/src/Shaders/cartoonShader.vert","src/CLEngine/src/Shaders/cartoonShader.frag" };
};
