#include "hd/sdl/MixerChunk.hpp"
#include "hd/Debug.hpp"
#include "hd/Error.hpp"
namespace hd::sdl {
  MixerChannel
  MixerChunk::FadeIn (int aTicks, int aLoops, int aChannel)
  {
    hdDebugCall ("%d, %d, %d", aChannel, aLoops, aTicks);
    return MixerChannel (Mix_FadeInChannel (aChannel, *this, aLoops, aTicks));
  }
  MixerChannel
  MixerChunk::Play (int aLoops, int aChannel)
  {
    hdDebugCall ("%d, %d", aChannel, aLoops);
    int channelPlaying = Mix_PlayChannel (aChannel, *this, aLoops);
    if (channelPlaying < 0) {
      hdError ("Unable to play chunk on channel %d because: %s",
               aChannel,
               Mix_GetError ());
    }
    return MixerChannel (channelPlaying);
  }
  int
  MixerChunk::SetVolume (int aLevel)
  {
    assert (*this);
    return Mix_VolumeChunk (*this, aLevel);
  }
  int
  MixerChunk::GetVolume ()
  {
    return SetVolume (-1);
  }
  MixerChunk
  MixerChunk::Load (std::string aFile)
  {
    std::filesystem::path realPath = Engine::FindPath (aFile);
    Mix_Chunk *chunk = Mix_LoadWAV (realPath.generic_string ().c_str ());
    if (chunk == NULL) {
      hdError ("Unable to load MixerChunk from path '%s' because: %s",
               aFile,
               Mix_GetError ());
    }
    return MixerChunk (s_ptr (chunk, Mix_FreeChunk));
  }
}