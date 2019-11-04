#pragma once

#include <SDL_mixer.h>
#include <vector>

constexpr auto SEQ_AMOUNT = 11110;
constexpr auto SOUNDS_AMOUNT = 10;
constexpr auto PI = 3.14159265;

namespace particle
{
  class Audio
  {
    Mix_Music* music_;
    std::vector<Mix_Chunk*> sounds_;

    const char* music_filename_ = R"(..\audio\Ambient.mp3)";
    const char* sounds_folder_ = R"(..\audio\A)";

  public:

    // delete unused ctors
    Audio(const Audio&) = delete;
    Audio(Audio&&) = delete;
    Audio& operator= (const Audio&) = delete;
    Audio& operator= (Audio&&) = delete;

    Audio();
    ~Audio();

    bool init();
    bool play_music() const;  // NOLINT(modernize-use-nodiscard)
    bool play_sound(
      int x, 
      int y, 
      int max_x,
      int max_y
    );

  };
}
