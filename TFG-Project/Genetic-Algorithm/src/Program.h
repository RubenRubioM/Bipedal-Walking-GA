#pragma once

/// <summary>
/// Class to handle the Program.
/// </summary>
class Program{
	public:
		/// <summary>
		/// Creates or returns a Program instance.
		/// </summary>
		/// <returns> Static Program instance pointer. </returns>
		static Program* GetInstance();

		/// <summary>
		/// Destructor.
		/// </summary>
		~Program() = default;

		/// <summary>
		/// Start the program.
		/// </summary>
		void Start();

	private:
		/// <summary>
		/// Private constructor.
		/// </summary>
		Program();

		/// <summary>
		/// Static Program instance pointer for the singleton <see href="https://en.wikipedia.org/wiki/Singleton_pattern" />
		/// </summary>
		inline static Program* instance{nullptr};
};
