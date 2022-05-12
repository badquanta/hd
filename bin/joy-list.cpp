#include "boost/program_options.hpp"
#include "hd/sdl/Joystick.hpp"
#include "hd/sdl/Window.hpp"
#include <iostream>
#include <vector>
using hd::Engine;
using hd::sdl::Joystick;
using std::cout;
using std::string;
using std::vector;
namespace po = boost::program_options;
Joystick js;
void
watch (int anIndex)
{
  Joystick::EventState (true);
  Engine::s_ptr engine = Engine::Get ();
  hd::sdl::Window::s_ptr window = hd::sdl::Window::Create ();
  SDL_EventState(SDL_JOYAXISMOTION, SDL_ENABLE);
  js = Joystick::Open (anIndex);
  engine->input.On([](const SDL_Event&e){
    printf("SDL EVENT %d\n",e.type);
  });
  engine->input.On ([] (const SDL_Event&e) {
    printf ("Joystick input type %x.\n",e.type);
    });
  engine->step.On([](int){
    //hdDebug("Process Update...");
    Joystick::Update();
  });
  engine->Start ();
}
void
printList ()
{
  printf ("number of joysticks: %d\n", hd::sdl::Joystick::Count ());
  for (int joystickIndex = 0; joystickIndex < Joystick::Count ();
       joystickIndex++) {
    std::string guid
        = Joystick::GetGuidString (Joystick::GetDeviceGUID (joystickIndex));
    std::string name = Joystick::NameForIndex (joystickIndex);
    printf ("Joystick#%d {%s} Name='%s'\n",
            joystickIndex,
            guid.c_str (),
            name.c_str ());
    Joystick joystick = Joystick::Open (joystickIndex);
    int numAxes = joystick.NumAxes ();
    int numBalls = joystick.NumBalls ();
    int numButtons = joystick.NumButtons ();
    int numHats = joystick.NumHats ();
    std::string powerLevel;
    switch (joystick.CurrentPowerLevel ()) {
    SDL_JOYSTICK_POWER_FULL:
      powerLevel = "full";
      break;
    SDL_JOYSTICK_POWER_MEDIUM:
      powerLevel = "medium";
      break;
    SDL_JOYSTICK_POWER_LOW:
      powerLevel = "low";
      break;
    SDL_JOYSTICK_POWER_MAX:
      powerLevel = "max";
      break;
    SDL_JOYSTICK_POWER_EMPTY:
      powerLevel = "empty";
      break;
    SDL_JOYSTICK_POWER_WIRED:
      powerLevel = "wired";
      break;
    SDL_JOYSTICK_POWER_UNKNOWN:
    default:
      powerLevel = "unknown";
    }

    printf ("\t %d buttons, %d axes, %d hats, %d balls, power level `%s`.\n",
            numButtons,
            numAxes,
            numHats,
            numBalls,
            powerLevel.c_str ());
  }
}
int
main (int argc, char **argv)
{
  po::options_description description ("Allowed options");
  description.add_options ()           //
      ("help", "produce help message") //
      ("watch",
       po::value<int> (),
       "open a joystick and watch events ")                             //
      ("include-path,I", po::value<vector<string> > (), "include path") //
      ("input-file", po::value<vector<string> > (), "input file");

  po::variables_map vm;
  po::store (po::parse_command_line (argc, argv, description), vm);
  po::notify (vm);
  Engine::s_ptr engine = Engine::Get (); // @todo Some of the static members
                                         // depend on the engine being running.

  if (vm.count ("help")) {
    cout << description << "\n";
    return 1;
  }

  if (vm.count ("watch")) {
    int index = vm["watch"].as<int> ();
    watch (index);
    return 0;
  }
  printList ();
  if(Joystick::Count()>0){
    watch (0);
  }
  return 0;
}