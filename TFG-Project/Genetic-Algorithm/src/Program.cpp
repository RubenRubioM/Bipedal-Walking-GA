#include "Program.h"

#include <iostream>

/// <summary>
/// Creates or returns a Program instance.
/// </summary>
/// <returns> Static Program instance pointer. </returns>
Program* Program::GetInstance() {
	if (!instance) {
		instance = new Program();
	}
	return instance;
}

/// <summary>
/// Private constructor.
/// </summary>
Program::Program(){
	
}
