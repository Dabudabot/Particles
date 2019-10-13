#pragma once

#include "Common.h"
#include "Mocks.h"

class GameTest : public ::testing::Test
{
protected:

  std::shared_ptr<ScreenMock> screen_;
  std::shared_ptr<WallHostMock> wall_host_;
  std::shared_ptr<SwarmHostMock> swarm_host_;
  std::shared_ptr<GameMock> game_;
  std::shared_ptr<SDL_Event> event_;

  void SetUp() override
  {
    screen_ = std::make_shared<ScreenMock>();
    swarm_host_ = std::make_shared<SwarmHostMock>();
    wall_host_ = std::make_shared<WallHostMock>();

    game_ = std::make_shared<GameMock>(
      screen_,
      swarm_host_,
      wall_host_
    );

    event_ = std::make_shared<SDL_Event>();

    EXPECT_CALL(*screen_, set_text).Times(0);
    EXPECT_CALL(*screen_, print_help).Times(0);
    EXPECT_CALL(*screen_, present).Times(0);
    EXPECT_CALL(*screen_, update).Times(0);
    EXPECT_CALL(*screen_, clear).Times(0);
    EXPECT_CALL(*screen_, motion_blur).Times(0);

    EXPECT_CALL(*wall_host_, start_wall).Times(0);
    EXPECT_CALL(*wall_host_, move_wall).Times(0);
    EXPECT_CALL(*wall_host_, end_wall).Times(0);
    EXPECT_CALL(*wall_host_, is_collide).Times(0);
    EXPECT_CALL(*wall_host_, draw_walls).Times(0);
    EXPECT_CALL(*wall_host_, is_overflow).Times(0);
    EXPECT_CALL(*wall_host_, serialize_walls).Times(0);
    EXPECT_CALL(*wall_host_, deserialize_walls).Times(0);

    EXPECT_CALL(*swarm_host_, generate_swarm).Times(0);
    EXPECT_CALL(*swarm_host_, update).Times(0);
    EXPECT_CALL(*swarm_host_, get_particles).Times(0);

    EXPECT_CALL(*game_, run()).Times(0);
    EXPECT_CALL(*game_, draw_help()).Times(0);
    EXPECT_CALL(*game_, process_event).Times(0);
    EXPECT_CALL(*game_, restore_defaults()).Times(0);
    EXPECT_CALL(*game_, save).Times(0);
    EXPECT_CALL(*game_, load).Times(0);
  }

  void TearDown() override
  {
    ASSERT_NE(game_->get_wall_host().use_count(), 0);
    ASSERT_NE(game_->get_screen().use_count(), 0);
    ASSERT_NE(game_->get_swarm_host().use_count(), 0);
  }
};

TEST_F(GameTest, ProcessEventFalse)
{
  event_->type = SDL_QUIT;

  EXPECT_FALSE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventRestoreDefaults)
{
  event_->type = SDL_KEYDOWN;
  event_->key.keysym.sym = SDLK_ESCAPE;

  EXPECT_CALL(*game_, restore_defaults());

  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventShowHelp)
{
  event_->type = SDL_KEYDOWN;
  event_->key.keysym.sym = SDLK_TAB;

  game_->set_help(false);

  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_TRUE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventSave)
{
  event_->type = SDL_KEYDOWN;
  event_->key.keysym.sym = SDLK_F9;

  EXPECT_CALL(*game_, save("save.save"));

  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventLoad)
{
  event_->type = SDL_KEYDOWN;
  event_->key.keysym.sym = SDLK_F10;

  EXPECT_CALL(*game_, load("save.save"));

  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventHideHelp)
{
  event_->type = SDL_KEYUP;
  event_->key.keysym.sym = SDLK_TAB;

  game_->set_help(true);

  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventEndWall)
{
  game_->set_wall_building(true);

  event_->type = SDL_MOUSEBUTTONDOWN;
  event_->button.button = SDL_BUTTON_LEFT;
  event_->button.x = 42;
  event_->button.y = 43;

  EXPECT_CALL(*wall_host_, end_wall(42, 43));

  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventBreakWall)
{
  game_->set_wall_building(true);

  event_->type = SDL_MOUSEBUTTONDOWN;
  event_->button.button = SDL_BUTTON_RIGHT;

  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventMoveWall)
{
  game_->set_wall_building(true);

  event_->type = SDL_MOUSEMOTION;
  event_->button.x = 42;
  event_->button.y = 43;

  EXPECT_CALL(*wall_host_, move_wall(static_cast<std::shared_ptr<particle::Screen>>(screen_), 42, 43));

  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_TRUE(game_->is_wall_building());
  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventStartWallNoOverflow)
{
  game_->set_wall_building(false);

  event_->type = SDL_MOUSEBUTTONDOWN;
  event_->button.button = SDL_BUTTON_LEFT;
  event_->button.x = 42;
  event_->button.y = 43;

  EXPECT_CALL(*wall_host_, is_overflow()).WillOnce(::testing::Return(false));;
  EXPECT_CALL(*wall_host_, start_wall(42, 43));
  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_TRUE(game_->is_wall_building());
  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventStartWallOverflow)
{
  game_->set_wall_building(false);

  event_->type = SDL_MOUSEBUTTONDOWN;
  event_->button.button = SDL_BUTTON_LEFT;

  EXPECT_CALL(*wall_host_, is_overflow()).WillOnce(::testing::Return(true));
  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, ProcessEventGenerateSwarm)
{
  game_->set_wall_building(false);

  event_->type = SDL_MOUSEBUTTONDOWN;
  event_->button.button = SDL_BUTTON_RIGHT;
  event_->button.x = 42;
  event_->button.y = 43;

  EXPECT_CALL(*swarm_host_, generate_swarm(42, 43));
  EXPECT_TRUE(game_->game_process_event(*event_));

  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_TRUE(game_->is_running());
  ASSERT_EQ(game_->get_help_fade(), 0xff);
}

TEST_F(GameTest, DrawHelpNoHelp)
{
  Uint8 zero = 0;

  game_->set_help(false);
  game_->set_help_fade(zero);

  game_->game_draw_help();

  ASSERT_EQ(game_->get_help_fade(), zero);
  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_TRUE(game_->is_running());
}

TEST_F(GameTest, DrawHelpShowHelp)
{
  const Uint8 zero = 0xff;

  game_->set_help(true);

  EXPECT_CALL(*screen_, print_help(zero));

  game_->game_draw_help();

  ASSERT_EQ(game_->get_help_fade(), zero);
  EXPECT_TRUE(game_->is_help_shown());
  EXPECT_TRUE(game_->is_running());
}

TEST_F(GameTest, DrawHelpFadeHelp)
{
  const Uint8 zero = 0x55;
  const Uint8 res = zero - 0x3;

  game_->set_help(false);

  EXPECT_CALL(*screen_, print_help(res));

  game_->game_draw_help();

  ASSERT_EQ(game_->get_help_fade(), res);
  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_TRUE(game_->is_running());
}

TEST_F(GameTest, RestoreDefaults)
{
  Uint8 zero = 0x55;
  const Uint8 res = 0x0;

  game_->set_wall_building(true);
  game_->set_help(true);
  game_->set_help_fade(zero);

  game_->game_restore_defaults();

  ASSERT_EQ(game_->get_help_fade(), res);
  EXPECT_FALSE(game_->is_help_shown());
  EXPECT_FALSE(game_->is_wall_building());
  EXPECT_TRUE(game_->is_running());
}