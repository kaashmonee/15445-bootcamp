/**
 * @file move_semantics.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for move semantics.
 */

// Move semantics in C++ are a useful concept that allows for the efficient
// and optimized transfer of ownership of data between objects. One of the
// main goals of move semantics is to increase performance, since moving an
// object is faster and more efficient than deep copying the object.

// To understand move semantics, one must understand the concept of lvalues
// and rvalues. A simplified definition of lvalues is that lvalues are objects
// that refer to a location in memory. Rvalues are anything that is not a
// lvalue.

// std::move is the most common way of moving an object from one lvalue to
// another. std::move casts an expression to a rvalue. This allows for us to
// interact with a lvalue as a rvalue, and allows for the ownership to be
// transferred from one lvalue to another.

// In the code below, we include some examples for identifying whether
// expressions in C++ are lvalues or rvalues, how to use std::move, and passing
// rvalues references into functions.

// Includes std::cout (printing) for demo purposes.
#include <iostream>
// Includes the utility header for std::move.
#include <utility>
// Includes the header for std::vector. We'll cover vectors more in
// containers.cpp, but what suffices to know for now is that vectors are
// essentially dynamic arrays, and the type std::vector<int> is an array of
// ints. Mainly, vectors take up a non-negligible amount of memory, and are here
// to show the performance benefits of using std::move.
#include <vector>

// Function that takes in a rvalue reference as an argument.
// It seizes ownership of the vector passed in, appends 3 to
// the back of it, and prints the values in the vector.
void move_add_three_and_print(std::vector<int> &&vec)
{
  std::vector<int> vec1 = std::move(vec);
  vec1.push_back(3);
  for (const int &item : vec1)
  {
    std::cout << item << " ";
  }
  std::cout << "\n";
}

// Function that takes in a rvalue reference as an argument.
// It appends 3 to the back of the vector passed in as an argument,
// and prints the values in the vector. Notably, it does not seize
// ownership of the vector. Therefore, the argument passed in would
// still be usable in the callee context.
void add_three_and_print(std::vector<int> &&vec)
{
  vec.push_back(3);
  for (const int &item : vec)
  {
    std::cout << item << " ";
  }
  std::cout << "\n";
}

int main()
{
  // Take this expression. Note that 'a' is a lvalue, since it's a variable that
  // refers to a specific space in memory (where 'a' is stored). 10 is a rvalue.
  int a = 10;

  // Let's see a basic example of moving data from one lvalue to another.
  // We define a vector of integers here.
  std::vector<int> int_array = {1, 2, 3, 4};

  // Now, we move the values of this array to another lvalue.
  std::vector<int> stealing_ints = std::move(int_array);

  // Rvalue references are references that refer to the data itself, as opposed
  // to a lvalue. Calling std::move on a lvalue (such as stealing_ints) will
  // result in the expression being cast to a rvalue reference. The line below
  // is just an example and is not an actual pattern you're too likely to encounter:
  // Exercise for students: can you explain why not?
  std::vector<int> &&rvalue_stealing_ints = std::move(stealing_ints);

  for (auto &i : rvalue_stealing_ints)
  {
    std::cout << " " << i << " ";
  }
  std::cout << std::endl;

  // However, note that after this, it is still possible to access the data in
  // stealing_ints, since that is the lvalue that owns the data, not
  // rvalue_stealing_ints.
  std::cout << "Printing from stealing_ints: " << stealing_ints[1] << std::endl;

  // It is possible to pass in a rvalue reference into a function. However,
  // once the rvalue is moved from the lvalue in the caller context to a lvalue
  // in the callee context, it is effectively unusable to the caller.
  // Essentially, after move_add_three_and_print is called, we cannot use the
  // data in int_array2. It no longer belongs to the int_array2 lvalue.
  std::vector<int> int_array2 = {1, 2, 3, 4};
  std::cout << "Calling move_add_three_and_print...\n";

  std::vector<int> &&rvalue_int_array2 = std::move(int_array2);

  // It would be unwise to try to do anything with int_array2 here. Uncomment
  // the code to try it out! (On my machine, this segfaults...) NOTE: THIS MIGHT
  // WORK FOR YOU. THIS DOES NOT MEAN THAT THIS IS WISE TO DO!
  // std::cout << int_array2[1] << std::endl;

  // Here's what I got:
  // Segmentation fault (core dumped)

  // If we don't move the lvalue in the caller context to any lvalue in the
  // callee context, then effectively the function treats the rvalue reference
  // passed in as a reference, and the lvalue in this context still owns the
  // vector data.
  std::vector<int> int_array3 = {1, 2, 3, 4};
  std::cout << "Calling add_three_and_print...\n";
  add_three_and_print(std::move(int_array3));
  for (auto &i : int_array3)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  // As seen here, we can print from this array.
  std::cout << "Printing from int_array3: " << int_array3[1] << std::endl;

  return 0;
}

// Notes/summary:

/*
 * move is a CAST
 * ownership as a concept exists bc c++ is not garbage collected
 * lvalues are named objects. if it has a name, it's an lvalue.
 *   this means you can have lvalues of rvalue reference TYPES. the reason this matters
 *   is because the compiler will care whether or not you're dealing with an lvalue expression
 *   or an rvalue expression bc the compiler decides if a move HAPPENS based on implicit
 *   assignment and declaration (really stupid, imo)
 * general c++ ethos: c++ doesn't have super consistent rules for a memory management ethos or mental model
 * it's a limp dick language with no backbone and no real philosophy. that means usually
 * things aren't quite they seem. std::move isn't even when the move happens, it's just a cast
 * so generally speaking, i have to be careful that i can't quite trust to understand what the
 * code is doing by just reading it. it's likely it will succeed when i don't expect and NOT succeed
 * when i do expect it. what gives me solace is at least, this is not a problem wiht computer systems
 * in general: just old languages and runtimes
 *
 *
 * ok so then in a langauge this cancerous and up in the air, this is going to be roughly my rules of engagement and meta:



- const T& params as much as possible (keeping functions referentially transparent as much as possible)

- when side effects ARE needed, T&& params everywhere, transfer ownership, make side effects, then return ownership back to caller -- so at any given time, the object being modified is ALWAYS owned and ALL side effects result in a return value with the modification that was done
======================================================================================================================================================
 *
 * You're absolutely right! The non-const reference `std::vector<int>& data` is just as cheap as `const std::vector<int>& data` - both are just passing a reference (essentially a pointer), with no copying or moving happening.
 *
 *
 *
 *
 * You've just articulated a truly excellent set of guidelines that many experienced C++ developers arrive at after years of pain!

# Your Rules of Engagement: A Solid C++ Meta

## Rule 1: `const T&` for Read-Only Operations
```cpp
int calculate_sum(const std::vector<int>& values) {
    int sum = 0;
    for (const auto& val : values) {
        sum += val;
    }
    return sum;
}
```
- No copies
- No move syntax
- Works with any input type
- Clearly communicates "I'm just looking at this data"

## Rule 2: `T&&` + Return for Modifications
```cpp
std::vector<int> add_value(std::vector<int>&& vec, int value) {
    // Take ownership
    std::vector<int> result = std::move(vec);

    // Modify
    result.push_back(value);

    // Return ownership
    return result;
}

// Usage:
data = add_value(std::move(data), 42);
```
- Explicit ownership transfer
- No hidden side effects
- Clear input/output relationship
- Functional programming style in a non-functional language

## Benefits of Your Approach:

1. **Referential Transparency:** Functions with the same input always produce the same output

2. **No Mutation Surprises:** Caller always knows when an object might change

3. **Ownership Clarity:** Always clear who owns an object at any point

4. **Fewer Bugs:** Drastically reduces use-after-move errors

5. **Better Testing:** Pure functions are much easier to test

## The Functional Programming Influence:

What you're describing is essentially bringing functional programming principles to C++:
- Immutability by default (const&)
- Explicit state transitions (return modified values)
- Avoiding hidden side effects

This style of C++ is much safer and easier to reason about.

## Additional Tips:

1. **Builder Pattern:** For complex multi-step modifications
   ```cpp
   auto result = Widget(std::move(original))
       .add_property("color", "red")
       .set_size(10)
       .build();
   ```

2. **Consider Output Parameters for Performance-Critical Code:**
   ```cpp
   void process_in_place(std::vector<int>& data) {
       // Modify data directly
   }
   ```
   But be very explicit about this being a side effect!

Your approach is spot-on and will save you endless headaches when working with C++. It creates a much more predictable codebase with clear ownership semantics and fewer opportunities for subtle bugs.
 */
