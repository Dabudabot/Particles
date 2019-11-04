
#include "Audio.h"
#include <string>
#include <iostream>
#include "Screen.h"

#include <Windows.h>

particle::Audio::Audio()
{
  music_ = nullptr;
  sounds_.resize(SOUNDS_AMOUNT, nullptr);
}

particle::Audio::~Audio()
{
  for (auto sound : sounds_)
  {
    //Free the sound effects
    if (sound)
    {
      Mix_FreeChunk(sound);
    }
  }

  //Free the music
  if (music_)
  {
    Mix_FreeMusic(music_);
  }

  //Quit SDL_mixer
  Mix_CloseAudio();
}

bool particle::Audio::init()
{
  //Initialize SDL_mixer
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
  {
    MessageBox(
      nullptr,
      LPCSTR("Audio stream open error"),
      LPCSTR("Audio init"),
      MB_OK
    );

    return false;
  }

  music_ = Mix_LoadMUS(music_filename_);

  //If there was a problem loading the music
  if (!music_)
  {
    MessageBox(
      nullptr,
      LPCSTR("Ambient not loaded"),
      LPCSTR("Audio init"),
      MB_OK
    );

    return false;
  }

  for (auto i = 0; i < SOUNDS_AMOUNT; i++)
  {
    std::string buf(sounds_folder_);
    buf.append(std::to_string(i));
    buf.append(".wav");

    sounds_[i] = Mix_LoadWAV(buf.c_str());

    if (!sounds_[i])
    {
      MessageBox(
        nullptr,
        LPCSTR("Sound not loaded"),
        LPCSTR("Audio init"),
        MB_OK
      );

      return false;
    }
  }

  const auto result = Mix_AllocateChannels(SOUNDS_AMOUNT*10);

  if (SOUNDS_AMOUNT * 10 != result)
  {
    MessageBox(
      nullptr,
      LPCSTR("Channels not allocated"),
      LPCSTR("Audio init"),
      MB_OK
    );

    return false;
  }

  return true;
}

bool particle::Audio::play_music() const
{
  // play music forever, fading in over 20 seconds
  if (Mix_FadeInMusic(music_, -1, 20000) == -1)
  {

    return false;
    // well, there's no music, but most games don't break without music...
  }

  return true;
}

bool particle::Audio::play_sound(
  const int x, 
  const int y, 
  const int max_x, 
  const int max_y
)
{
  auto channel = 0;
  Sint16 dir;

  // find free channel
  while (Mix_Playing(channel))
  {
    channel++;
  }

  // find sound
  const auto index = (SOUNDS_AMOUNT - 1) - (SOUNDS_AMOUNT - 1) * y / max_y;
  const auto sound = sounds_[index];

  if (Mix_PlayChannel(channel, sound, 0) == -1)
  {
    return false;
  }

  // find dir
  if (x > max_x / 2)
  {
    dir = 90;
  }
  else
  {
    dir = 270;
  }

  // find dist
  const Sint8 dist = 100 * abs(max_x / 2 - x) / (max_x / 2);

  if (Mix_SetPosition(channel, dir, dist) == -1)
  {
    return false;
  }

  return true;
}
