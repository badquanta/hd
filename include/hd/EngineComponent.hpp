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
  class EngineComponent {
    private:
      static int m_ActiveEngineComponents;
      evt::IntDispatch::Handle processHandle;

    public:
      static int ActiveCount ();
      EngineComponent ();
      ~EngineComponent ();
      const Engine::s_ptr engine;
      evt::IntDispatch process;

  };
  class EngineOutputComponent: public EngineComponent {
    public:


  };
}