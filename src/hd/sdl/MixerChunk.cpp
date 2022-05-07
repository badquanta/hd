#include "hd/sdl/MixerChunk.hpp"

namespace hd::sdl {
  MixerChannel MixerChunk::FadeIn(int aTicks, int aLoops, int aChannel){
    hdDebugCall ("%d, %d, %d", aChannel, aLoops, aTicks);
    return MixerChannel (
        Mix_FadeInChannel (aChannel, m_Chunk, aLoops, aTicks));
  }
  MixerChannel MixerChunk::Play(int aLoops,int aChannel){
    hdDebugCall ("%d, %d", aChannel, aLoops);
    int channelPlaying = Mix_PlayChannel (aChannel, m_Chunk, aLoops);
    if(channelPlaying < 0){
      hdError ("Unable to play chunk on channel %d because: %s",
               aChannel,
               Mix_GetError ());
    }
    return MixerChannel(channelPlaying);
  }
  int
  MixerChunk::SetVolume (int aLevel)
  {
    assert (m_Chunk);
    return Mix_VolumeChunk (m_Chunk, aLevel);
  }
  int
  MixerChunk::GetVolume ()
  {
    return SetVolume (-1);
  }
  MixerChunk::s_ptr
  MixerChunk::Load (char *aFile)
  {
    std::filesystem::path realPath = Engine::FindPath (aFile);
    Mix_Chunk *chunk = Mix_LoadWAV (realPath.generic_string ().c_str ());
    if (chunk == NULL) {
      hdError ("Unable to load MixerChunk from path '%s' because: %s",
               aFile,
               Mix_GetError ());
      return NULL;
    }
    return MixerChunk::s_ptr (new MixerChunk (chunk));
  }

  MixerChunk::MixerChunk (Mix_Chunk *aChunk) : m_Chunk (aChunk) {}

  MixerChunk::~MixerChunk () { Mix_FreeChunk (m_Chunk); }
}