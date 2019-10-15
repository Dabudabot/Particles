
#include "Audio.h"
#include <string>
#include <iostream>
#include "Screen.h"

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
    return false;
  }

  music_ = Mix_LoadMUS(music_filename_);

  //If there was a problem loading the music
  if (!music_)
  {
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
      return false;
    }
  }

  Mix_AllocateChannels(SOUNDS_AMOUNT*10);

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

bool particle::Audio::play_sound(const int x, const int y)
{
  const auto sound = sounds_[sequence_[cur_seq_]];

  auto channel = sequence_[cur_seq_];

  while (Mix_Playing(channel))
  {
    channel++;
  }

  if (Mix_PlayChannel(channel, sound, 0) == -1)
  {
    return false;
  }

  const auto new_x = x - Screen::screen_width / 2;
  const auto new_y = -1 * (y - Screen::screen_height / 2);

  const auto r = sqrt(new_x*new_x + new_y*new_y);
  auto a = atan2(new_y, new_x) * 180 / PI;

  if (a < 0) a += 360;

  a -= 360;
  a = -a;
  a += 90;

  if (a > 360) a -= 360;

  if (Mix_SetPosition(channel, a, r) == -1)
  {
    return false;
  }

  cur_seq_++;

  if (cur_seq_ > SEQ_AMOUNT)
  {
    cur_seq_ = 0;
  }
  
  return true;
}
