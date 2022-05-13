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
#include "hd/IntDispatch.hpp"
namespace hd {
  class EngineComponent {
      public:
        Engine::s_ptr engine = Engine::Get();
  };
}