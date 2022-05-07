/**
 * @file MixerChannel.hpp
 *  * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-07
 *
 * @copyright GNU-GPL 3.0
 *
 */
#pragma once
#include "hd/EngineComponent.hpp"

namespace hd::sdl {
    /**
     * @brief
     *
     */
    class MixerChannel {
        public:
          int channel;
          MixerChannel (int);
          int FadeOut (int ms);
          Mix_Fading FadingStatus ();
          bool IsFadingIn ();
          bool IsFadingOut ();
          bool IsFading ();
          bool IsPlaying ();
          static int CountPlaying ();
          bool IsPaused();
          static int CountPaused ();
          std::function<void ()> Pause = [this] () { Mix_Pause (channel); };
    };
}