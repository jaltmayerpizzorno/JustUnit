/*
 * MIT License
 * Copyright (c) 2021 Juan Altmayer Pizzorno
 */

#include "JustUnit.hxx"


using namespace JustUnit;

Linkage<TestClass>& TestClass::_ju_c_linkage() {
  // this is safer than a static member, as it is initialized on demand on 1st usage,
  // whereas a static member's constructor may only run after a TestClass' constructor
  static Linkage<TestClass> linkage;
  return linkage;
}


TestClass::TestClass() {
  _ju_c_linkage().link(this, &TestClass::_ju_c_next);
}


void TestClass::run() {
  for (TestMethodRegistry* m = _ju_m_linkage.first; m; m = m->next) {
    try {
      // XXX startUp
      m->func();
      // XXX tearDown
    }
    catch (std::exception& e) {
      std::cerr << m->name << " " << e.what() << "\n";
    }
  }
}


void TestClass::runAll() {
  for (TestClass* t = _ju_c_linkage().first; t; t = t->_ju_c_next) {
    t->run();
  }
}


int main() {
  TestClass::runAll();
}
