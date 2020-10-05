#pragma once

/// <summary>
/// Class to handle the Game.
/// </summary>
class Game{
	public:
		/// <summary>
		/// Creates or returns a Game instance.
		/// </summary>
		/// <returns> Static Game instance pointer. </returns>
		static Game* GetInstance();

		/// <summary>
		/// Destructor.
		/// </summary>
		~Game() = default;
	private:
		/// <summary>
		/// Private constructor.
		/// </summary>
		Game();

		/// <summary>
		/// Static Game instance pointer for the singleton <see href="https://en.wikipedia.org/wiki/Singleton_pattern" />
		/// </summary>
		inline static Game* instance{nullptr};
};
