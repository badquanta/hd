/**
 * @file MixerChunk.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-07
 *
 * @copyright GNU-GPL 3.0
 * @example playAudio.cpp
 */
#pragma once
#include "hd/EngineComponent.hpp"
#include "hd/Wrap.hpp"
#include "hd/sdl/MixerChannel.hpp"

namespace hd::sdl {
  /**
   * @brief Represents a chunk of audio to play.
   */
  class MixerChunk : public EngineComponent,
  public WRAP_PTR<Mix_Chunk> {
  public: // class static methods
    using WRAP_PTR::WRAP_PTR;
    /** **/
    static MixerChunk Load (char *);

  public: // instance methods
    int SetVolume (int);
    int GetVolume ();
    /** @see Mix_PlayChannel **/
    MixerChannel Play (int aLoops = 0, int aChannel = -1);
    MixerChannel FadeIn (int aTicks, int aLoops = 0, int aChannel = -1);

  protected:

  };
}
