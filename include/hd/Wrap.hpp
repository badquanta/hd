/**
 * @file Wrap.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief Wrap the identity around a type, also provide a [shared pointer][std::shared_ptr] variant.
 * [std::shared_ptr]: https://en.cppreference.com/w/cpp/memory/shared_ptr
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright GNU-GPL 3.0 Copyright (C) 2022 Jón Davíð Sawyer (badquanta@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */
#pragma once
#include <memory>
namespace hd {
  /** Wrap up the identity of an object around the value of a type. **/
  template <typename WRAP_TYPE> class WRAP_IDENTITY {
  public:
    /** this identity **/
    WRAP_TYPE m_IDENTITY;
    /** Object instance is value of identity type. **/
    WRAP_IDENTITY (WRAP_TYPE anIdentity) : m_IDENTITY (anIdentity){};
    /** Assume the identity of another instance **/
    WRAP_IDENTITY (const WRAP_IDENTITY &other)
    {
      m_IDENTITY = other.m_IDENTITY;
    };
    /** Take the identity of another instance **/
    WRAP_IDENTITY &
    operator= (const WRAP_IDENTITY &other)
    {
      m_IDENTITY = other.m_IDENTITY;
      return *this;
    };
    /** Simply be given a new identity **/
    WRAP_TYPE
    operator= (WRAP_TYPE aIdentity) { return m_IDENTITY = aIdentity; };
    /** When asked for this identity **/
    operator WRAP_TYPE () { return m_IDENTITY; };
  };
  /**
   * @brief Simple example of an object identity wrap as an integer;
   *
   *
   */
  class WrapInt : public WRAP_IDENTITY<int> {
    using WRAP_IDENTITY::WRAP_IDENTITY;
    using WRAP_IDENTITY::operator=;
  };
  /**
   * @brief A template to make a SMART pointer wrapper.
   *
   * @tparam WRAP_PTR_TYPE
   */
  template <typename WRAP_PTR_TYPE>
  class WRAP_PTR : public WRAP_IDENTITY<std::shared_ptr<WRAP_PTR_TYPE> > {
  public:
    /**
     * @name __pad0__
     * @brief
     * so that it can be reference succinctly
     * **/
    typedef std::shared_ptr<WRAP_PTR_TYPE> s_ptr;
    /**
     * @brief
     *
     */
    using WRAP_IDENTITY<s_ptr>::operator=;
    WRAP_PTR (s_ptr aPointer = s_ptr (nullptr))
        : WRAP_IDENTITY<s_ptr> (aPointer){};
    /** In context `ptr` is a reference to `m_IDENTITY` so that it can be
     * referenced succinctly. **/
    s_ptr &ptr = WRAP_IDENTITY<s_ptr>::m_IDENTITY;
    /** take the identity pointer of another **/
    WRAP_PTR &
    operator= (const WRAP_PTR &anOther)
    {
      WRAP_IDENTITY<s_ptr>::m_IDENTITY = anOther.m_IDENTITY;
      return *this;
    };
    /**
     * @brief convert into instances of `WRAP_PTR_TYPE`
     * @note simply calls s_ptr instance `get()` and returns value.
     * @return WRAP_PTR_TYPE *
     */
    operator WRAP_PTR_TYPE * () const
    {
      return WRAP_IDENTITY<s_ptr>::m_IDENTITY.get ();
    };
    /**
     * @brief
     *
     * @return true
     * @return false
     */
    operator bool () const
    {
      return (WRAP_IDENTITY<s_ptr>::m_IDENTITY.get () != NULL);
    };
  };
}