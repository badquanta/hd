/**
 * @file Wrap.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief Template that wraps a simple class around a identity signature.
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright GNU-GPL 3.0
 *
 */
#pragma once
#include <functional>
#include <memory>
namespace hd {
  /** Wrap up the identity of an object around the value of a type. **/
  template <typename WRAP_TYPE> class WRAP_IDENTITY {
  public:
    WRAP_TYPE m_IDENTITY;
    /** Object instance is value of identity type. **/
    WRAP_IDENTITY (WRAP_TYPE anIdentity) : m_IDENTITY (anIdentity){};
    WRAP_IDENTITY (const WRAP_IDENTITY &other)
    {
      m_IDENTITY = other.m_IDENTITY;
    };
    WRAP_IDENTITY &
    operator= (const WRAP_IDENTITY &other)
    {
      m_IDENTITY = other.m_IDENTITY;
      return *this;
    };
    WRAP_TYPE
    operator= (WRAP_TYPE aIdentity) { return m_IDENTITY = aIdentity; };
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
    /** so that it can be reference succinctly **/
    typedef std::shared_ptr<WRAP_PTR_TYPE> s_ptr;
    using WRAP_IDENTITY<s_ptr>::operator=;
    WRAP_PTR (s_ptr aPointer = s_ptr (nullptr))
        : WRAP_IDENTITY<s_ptr> (aPointer){};
    /** In context `ptr` is a reference to `m_IDENTITY` so that it can be
     * referenced succinctly. **/
    s_ptr &ptr = WRAP_IDENTITY<s_ptr>::m_IDENTITY;
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