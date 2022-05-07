/**
 * @file MixerChunk.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-07
 *
 * @copyright GNU-GPL 3.0
 *
 */
#pragma once
#include "hd/EngineComponent.hpp"
#include "hd/sdl/MixerChannel.hpp"

namespace hd::sdl {
    class MixerChunk : public EngineComponent {
        public:
          typedef std::shared_ptr<MixerChunk> s_ptr;
          /** **/
          static s_ptr Load (char *);
        public: // instance methods
            int SetVolume(int);
            int GetVolume ();
            /** @see Mix_PlayChannel **/
            MixerChannel Play (int aLoops = 0, int aChannel = -1);
            MixerChannel FadeIn (int aTicks, int aLoops = 0, int aChannel = -1);

            ~MixerChunk ();

          private:
            MixerChunk (Mix_Chunk *);
            Mix_Chunk *m_Chunk;
    };
}
