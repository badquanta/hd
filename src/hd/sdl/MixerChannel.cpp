#include "hd/sdl/MixerChannel.hpp"
namespace hd::sdl {

    /** **/
    MixerChannel::MixerChannel(int aChannel):channel(aChannel){
      hdDebugCall ("%d", channel);
    }
    /** **/
    int MixerChannel::FadeOut(int aMs){
      hdDebugCall ("%d, %d", channel, aMs);
      assert (channel > -2);
      return Mix_FadeOutChannel (channel, aMs);
    }
    /** **/
    Mix_Fading MixerChannel::FadingStatus(){
      assert (channel > -2);
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
      assert (channel > -2);
      return (Mix_Playing (channel) > 0);
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