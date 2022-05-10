/**
 * @file EngineComponent.hpp
 * @author BadQuanta (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-05-04
 *
 * @copyright GNU GPL 3.0
 */
#pragma once
#include "hd/Engine.hpp"
#include "hd/evt/IntDispatch.hpp"
namespace hd {
  template <typename WRAPPER, typename TARGET > class EngineComponent {
      protected:
        bool m_Free;
        TARGET *m_Component;
        evt::IntDispatch::Handle processHandle;
      public:
        TARGET *GetComponent (){
          return m_Component;
        };
        typedef std::shared_ptr<WRAPPER> s_ptr;
        evt::IntDispatch step;
        EngineComponent (TARGET *p, bool aFree)
            : m_Component (p), m_Free (aFree), engine (Engine::Get ()){
              processHandle = engine->step.On (step.pipe);
            };
        ~EngineComponent(){

          if(m_Free){this->Free();}
          engine->step.Delete (processHandle);
        };
        const Engine::s_ptr engine;
        virtual void Free () {
          hdDebugCall (NULL);
        };
  };
}