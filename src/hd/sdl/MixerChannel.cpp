#include "hd/sdl/MixerChannel.hpp"

namespace hd::sdl {
    /** **/
    MixerChannel::MixerChannel(int aChannel):channel(aChannel){}
    /** **/
    int MixerChannel::FadeOut(int aMs){
      return Mix_FadeOutChannel (channel, aMs);
    }
    /** **/
    Mix_Fading MixerChannel::FadingStatus(){
      return Mix_FadingChannel (channel);
    }
    /** **/
    bool MixerChannel::IsFading(){
      return (FadingStatus () != MIX_NO_FADING);
    }
    /** **/
    bool MixerChannel::IsFadingOut(){
      return (FadingStatus () == MIX_FADING_OUT);
    }
    /** **/
    bool MixerChannel::IsFadingIn(){
      return (FadingStatus () == MIX_FADING_IN);
    }
    /** **/
    int MixerChannel::CountPlaying(){
      return Mix_Playing (-1);
    }
    /** **/
    bool MixerChannel::IsPlaying(){
      return (Mix_Playing (channel) == 1);
    }
    /** **/
    int MixerChannel::CountPaused(){
      return Mix_Paused (-1);
    }
    /** **/
    bool MixerChannel::IsPaused(){
      return (Mix_Paused (channel)==1);
    }

}