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
      last->*next = c; // ooh, fancy c++ :)
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
  TestClass() {
    _ju_c_linkage().link(this, &TestClass::_ju_c_next);
  }

  virtual ~TestClass() {}

  virtual void startUp() {}
  virtual void tearDown() {}

  /**
   * Runs all test methods in this test class.
   */
  int run() {
    int count{0};

    for (TestMethodRegistry* m = _ju_m_linkage.first; m; m = m->next) {
      try {
        ++count;
        startUp();
        std::cerr << "\r" << m->name << "...    ";
        m->func();
        tearDown();
      }
      catch (std::exception& e) { // XXX are we catching too much?
        std::cerr << m->name << " " << e.what() << "\n";
        tearDown();
      }
    }

    return count;
  }

  /**
   * Runs all test methods in all test classes.
   */
  static void runAll() {
    int methodCount{0};
    int classCount{0};

    for (TestClass* t = _ju_c_linkage().first; t; t = t->_ju_c_next) {
      ++classCount;
      methodCount += t->run();
    }

    std::cerr << methodCount << " test" << (methodCount == 1 ? "" : "s")
              << " in " << classCount << " class" << (classCount == 1 ? "" : "es") << ".\n";
  }

 protected:
  Linkage<TestMethodRegistry> _ju_m_linkage;

 private:
  static Linkage<TestClass>& _ju_c_linkage() {
    // this is safer than a static member, as it is initialized on demand on 1st usage,
    // whereas a static member's constructor may only run after a TestClass' constructor
    static Linkage<TestClass> linkage;
    return linkage;
  }

  TestClass* _ju_c_next;
};


/**
 * Base for assertion failures.
 */
class AssertionFailure : public std::logic_error {
  using super = std::logic_error;
 public:
   AssertionFailure(const std::string& s) : super(s) {}
};

#define ju_assert(pred, msg)\
  if (!(pred)) {\
    std::stringstream ss;\
    ss << __FILE__ << ":" <<  __LINE__ << ": " << msg;\
    throw JustUnit::AssertionFailure(ss.str());\
  }

#define ju_fail(msg)\
  ju_assert(false, msg)

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
