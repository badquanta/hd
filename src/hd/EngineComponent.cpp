#include "hd/EngineComponent.hpp"
namespace hd {
  int EngineComponent::m_ActiveEngineComponents = 0;
  int EngineComponent::ActiveCount(){
    return m_ActiveEngineComponents;
  }
  EngineComponent::EngineComponent () : engine (Engine::Get ())
  {
    m_ActiveEngineComponents++;
    processHandle = engine->step.On (step.pipe);
    hdDebug ("New component, total %d", m_ActiveEngineComponents);
  };
  EngineComponent::~EngineComponent () {
    hdDebugCall (NULL);
    engine->step.Delete (processHandle);
    m_ActiveEngineComponents--;
    hdDebug ("Delete component, total %d", m_ActiveEngineComponents);
  };
}