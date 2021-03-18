/*
 * MIT License
 * Copyright (c) 2021 Juan Altmayer Pizzorno
 */

#pragma once
#ifndef JUSTUNIT_HXX
#define JUSTUNIT_HXX

#include <string>
#include <iostream>
#include <sstream>

namespace JustUnit {

/**
 * Implements a singly linked list, intrusively, but malloc free.
 */
template<class C>
class Linkage {
 public:
   C* first{0};
   C* last{0};

   void link(C* c, C* C::*next) {
    if (last != 0) {
      last->*next = c; // ooh, fancy c++
    }
    if (first == 0) {
      first = c;
    }
    last = c;
   }
};


/**
 * Registers a test method.
 * Instantiated through JU_TEST.
 */
class TestMethodRegistry {
 public:
  const char* name;
  void (*func)();
  TestMethodRegistry* next;
   
  TestMethodRegistry(Linkage<TestMethodRegistry>& linkage, const char* name, void(*func)())
    : name(name), func(func), next(0) {
    linkage.link(this, &TestMethodRegistry::next);
  }
};

#define JU_TEST(name)\
  JustUnit::TestMethodRegistry _ju_r_##name{_ju_m_linkage, #name, name};\
  static void name()


/**
 * Base for unit test classes.
 */
class TestClass {
 public:
  TestClass();

  virtual ~TestClass() {}

  void run();

  static void runAll();

 protected:
  Linkage<TestMethodRegistry> _ju_m_linkage;

 private:
  static Linkage<TestClass>& _ju_c_linkage();
  TestClass* _ju_c_next;
};


/**
 * Base for assertion failures.
 */
class AssertionFailure : public std::runtime_error {
  using super = std::runtime_error;
 public:
   AssertionFailure(const std::string& s) : super(s) {}
};

#define ju_assert(pred, msg)\
  if (!(pred)) {\
    stringstream ss;\
    ss << __FILE__ << ":" <<  __LINE__ << ": " << msg;\
    throw JustUnit::AssertionFailure(ss.str());\
  }

#define ju_assert_eq(check, value, msg)\
  ju_assert(check == value, #check << " != " << #value << " " << msg)

#define ju_assert_ne(check, value, msg)\
  ju_assert(check != value, #check << " == " << #value << " " << msg)

#define ju_assert_gt(check, value, msg)\
  ju_assert(check < value, #check << " >= " << #value << " " << msg)

#define ju_assert_ge(check, value, msg)\
  ju_assert(check <= value, #check << " > " << #value << " " << msg)

#define ju_assert_lt(check, value, msg)\
  ju_assert(check > value, #check << " <= " << #value << " " << msg)

#define ju_assert_le(check, value, msg)\
  ju_assert(check >= value, #check << " < " << #value << " " << msg)

} // namespace
#endif
