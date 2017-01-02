# react.cpp
c++ reaction utilities

## connection
Represents a connection to a responsive object.
```c++
rvalue.connect()
  .set_function([](int value) {
      std::cout << "rvalue is now 10!" << std::endl;
    })
  .set_predicate([](int value) {
      return value == 10;
    })
  .set_once();
```

## rvalue
T object wrapper with responsive signals to mutations.

```c++
react::value<int> rvalue { 5 };
rvalue.connect(&print_value<int>); // returns a react::connection
rvalue = 10; // calls print_value with 10
```
