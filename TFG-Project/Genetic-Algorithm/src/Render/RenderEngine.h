#pragma once

#include <CLEngine.h>

class RenderEngine{
	public:
		/// <summary>
		/// RenderEngine constructor.
		/// </summary>
		RenderEngine();

		/// <summary>
		/// RenderEngine destructor.
		/// </summary>
		~RenderEngine();
	
	private:
		/// <summary>
		/// CLEngine pointer.
		/// </summary>
		CLE::CLEngine* device{ nullptr };
};

