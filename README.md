# JustUnit:  simple framework for JUnit-like tests in C++
 
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](LICENSE) 

## How to use
Create a test class like this:

```C++
...
#include "JustUnit.hxx"

class MyTestClass : public JustUnit::TestClass {
 public:
  JU_TEST(testSomething) {
    Foo foo;
    ju_assert_eq(42, foo.bar(), "foo.baz=" << foo.baz);
  }
  
  JU_TEST(testSomethingElse) {
    ju_assert(false, "test missing, write me!");
  }
  
  ...

} myTestClass;
```

and then compile/link it with `JustUnit`, e.g.:

```bash
g++ --std=c++11 ... -o tests mytestclass.cxx JustUnit.cxx
```

`JustUnit.cxx` provides a `main()` that runs through all test methods of all test classes it can see (I do some C++ trickery to compensate for the lack of reflection).

That 3rd parameter in the assertions is passed to a stream, so you can easily customize the assertion failure message, adding those details about the test you will wish you had once you see a failure. See `JustUnit.hxx` for details.

It should work with C++11 and later.

## History

Years ago I co-developed a test framework while leading [Port25](http://www.port25.com). I was recently writing some C++ and wished I had that still, so I whipped this one together. This one is free, so use if you like it :)
