# C++ Style Guide

## Agenda

- Key ideas
- Layout
- Naming
- Formatting
- Comments

## Key Ideas

Code should be written to minimize the time it would take for someone else to understand it.

## Layout

- Header files
- Source files

### Header Files

Following is the template that should be used to organize class declaration in `.h` file:

- All headers files should have `#pragma once` guard to prevent multiple inclusion.
- Order your includes in header file as follows:
  - System files.
  - Other 3-rd party libraries `.h` files.
  - Your project `.h` files.
- Avoid using forward declarations where possible. Just `#include` the headers you need.
- Namespaces wrap the entire header file after includes and forward declarations of classes from other namespaces.
- Define functions inline only when they are small (10 lines or less).

Example 

```c++
#pragma once // HEADER GUARDS

// SYSTEM INCLUDES
// 3RD PARTY INCLUDES
// PROJECT INCLUDES
// FORWARD REFERENCES

namespace ISXNAMESPACE
{

class ClassName
{
// LIFECYCLE
    // Default constructor
    ClassName();
    // Copy constructor
    ClassName(const ClassName& other);
    // Destructor
    ~ClassName();
    
// OPERATORS
    // Assignment operator
    ClassName& operator=(ClassName& other);
    
// OPERATIONS
    
// ACCESS
protected:
private:
// MEMBER VARIABLES
};

// INLINE FUNCTIONS

} // namespace ISXNAMESPACE
```

### Source Files

Following is the template that should be used to organize class declaration in `.h` file:

- All header files should have `#pragma once` guard to prevent multiple inclusions.
- Order your includes in source file as follows:
  - Corresponding header file.
  - System files.
  - Other 3-rd party libraries `.h` files.
  - Your project's `.h` files.
- Avoid using forward declarations where possible. Just `#include` the headers you need.
- Namespaces wrap the entire source file after includes and forward declarations of classes from other namespaces.
- Define functions inline only when they are small, say, 10 lines or less.

Example:

```c++
#include "ClassName.h"

// SYSTEM INCLUDES
// 3RD PARTY INCLUDES
// PROJECT INCLUDES

namespace ISXNAMESPACE
{

// TYPEDEFS AND ENUMS
// CONSTANCES
// LOCAL FUNCTION
// LIFECYCLE
// OPERATORS
// OPERATIONS
// ACCESS

} // namespace ISXNAMESPACE
```

## Naming

- File Names
- Type Names
- Function Names
- Namespace Names
- Enumerator Names
- Macro Names
- Exceptions to Naming Rules

### File Names

- File name starts with a capital letter and have a capital letter for each new word, with no underscores.
- C++ files should end in `.cpp` and header files should end in `.h`.
- In general, every `.cpp` file should have an associated `.h` file.

Example: `UserCredentials.h`, `UserCredentials.cpp`

### Type Names

- Variable Names
- Common Variable Names
- Class Data Members
- Struct Data Members
- Global Variables
- Constant Names

### Type Names

- The names of all types - classes, structs, typedefs, and enums - have the same naming convention.
- Type names should start with a capital letter and have a capital letter for each new word. No underscores.
- Do not use prefixes like capital "C" (for "class") or "T" or "Type" (for "types"). Do not use suffix "_t" for type names.
- Interfaces classes are named like ordinary classes and should be prefixed with "I".

Example:

```c++
class VisionDevice
{
    
};

struct ChunkReader
{
    
};

enum SessionEvents
{
 
};

typedef vector<SSOScreen*> SSOScreens;

class ISession
{

};
```

#### Variable Names

- The names of variables and data members are all lowercase, with underscores between words.
- Do not use Hungarian notation.
- Consider adding the principal class name to the variable name, like "cancel_button".
- Do not use class/category name based prefixes, like "btnCancel".
- Do not use special notation for pointers or references, like `p_state`, `r_state`.
- Avoid generic names, like `tmp` and `retval`.
- Prefer `min` and `max` for limits.
- Add words like `is`, `has`, `can` or `should` for booleans.

Example: `string table_name;`

#### Class Data Members

- Non-static data members of classes are named like ordinary nonmember variables and should be prefixed with `m_`.
- Static data members of classes are named like ordinary nonmember variables and should be prefixed with `s_`.

Example:

```c++
class TableInfo
{
private:
    string m_table_name;
    static Pool<TableInfo*> s_pool;
};
```

#### Struct Data Members

Data members of structs, both static and non-static, are named like ordinary nonmember variables.

```c++
struct UrlTableProperties
{
    string name;
    string Pool<UrlTableProperties>* pool;
};
```

#### Global Variables

For global variables, which should be rare in any case, but if you use one, consider prefixing it with `g_` to easily distinguish it from local variables.

Example: `ISXAgent* g_agent;`

#### Constant Variables

Constants defined globally or within a class should be named with all capitals and underscores.

Example: `const int FADEOUT_DURATION = 1000;`

### Function Names

- Regular Functions
- Accessors and Mutators

#### Regular Functions

Functions should start with a capital letter and have a capital letter for each new word. No underscores.

Example: `bool CompareInsensitive(const wstring& one, const wstring& two);`

#### Accessors and Mutators

Accessors and mutators (get and set functions) should match the name of the variable they are getting or setting. The getter should be prefixed `get_` and setter with `set_`.

Example:

```c++
class Config
{
public:
    bool get_loggin_allowed() const;
    void set_loggin_allowed(const bool allowed);
private:
    bool m_logging_allowed;    
};
```

###  Namespace Names

Namespace names should start with a capital letter and have a capital letter for each new word. Historically it starts with `ISX` prefix (aka Inner Side Extension).

Example: 

```c++
namespace ISXStringUtils
{

} // namespace ISXStringUtils
```

### Enumerator Names

Enumerators should be named either like constants or use `PascalCase`.

Example:

```c++
enum UrlTableErrors
{
    OK = 0,
    OUT_OF_MEMORY = 1,
    MALFORMED_INPUT = 2,
};

enum State
{
    Unknown,
    Off,
    On
};

// To avoid homonymous enumerators consider adding enumeration name, as follows:
enum State
{
    UnknownState,
    OffState,
    OnState
};
```

### Macro Names

In general macros should not be used. However, if they are absolutely needed, then they should be named with all capitals and underscores.

Example:

```c++
#define PROCESS_CALLBACK_MSG CallbackManager::ProcessCallback(__FUNCTION__, msg)
```

### Exceptions to Naming Rules

If you are naming something that is analogous to an existing C or C++ entity then you can follow the existing naming convention scheme.

## Formatting

- Line Length
- Spaces vs Tabs
- Function Declarations and Definitions
- Lambda Expressions
- Conditionals and Loops
- Pointer and Reference Expressions
- Boolean Expressions
- Return Values
- Namespace Formatting
- Preprocessor Directives
- Class Format
- Constructor Initializer Lists
- Horizontal Whitespace
- Vertical Whitespace

### Line Length

Modern equipment has wide screens that can easily show longer lines. A line should fit to screen.

### Spaces vs Tabs

Use only tabs.

### Function Declarations and Definitions

- Return type on the same line as function name, parameters on the same line if they fit.
- If you cannot fit the return type and the function name on a single line, break between them.
- The open curly brace is at new line.
- There is never a space between the function name and the open parenthesis.
- There is never a space between the parenthesis and the parameters.
- When defining a function, parameter order is: inputs, then outputs.
- Use `::` for global functions.
- Avoid unneeded negation. Be positive. Prefer names as IsDone() to names as IsNotDone().
- Use words in the normal language order.

Functions look like this:

```c++
ReturnType ClassName::FunctionName(Type par_name1, Type par_name2)
{
    DoSomething();
}
```

If you have too much text to fit on the one line:

```c++
ReturnType ClassName::ReallyLongFunctionName(Type par_name1, Type par_name2,
                                             Type par_name3)
{
    DoSomething();
}
```

or 

```c++
ReturnType ClassName::ReallyLongFunctionName(
        Type par_name1, 
        Type par_name2,
        Type par_name3)
{
    DoSomething();
}
```

### Lambda Expressions. Conditionals and Loops

- Prefer no spaces inside parentheses.
- A space between the `if` and the open parenthesis.
- The open curly brace is at new line.
- Short conditional statements may be written on one line if this enhances readability.
- In general, curly braces are not required for single-line statements. However, if one part of an `if-else` statement uses curly braces, the other part must too.

```c++
if (condition)
{
    
}
else if ()
{
    
} 
else
{
    
}
```

or 

```c++
if (x == FOO) return new Foo();
```

or 

```c++
if (condition)
    DoSomething();
```

### Pointer and Reference Expressions

- No spaces around period or arrow.
- Pointer operators have no space after the * or &.
- Prefer place the asterisk adjacent to the type.

Example:

```c++
x = *p;
p = &x;
x = r.y;
x = r->y;
char* c;
const string& str;
```

### Boolean Expressions

If boolean expression is too long, be consistent in how you break up the lines.

Example:

```c++
if (this_one_thing > this_other_thing &&
    a_third_thing == a_forth_thing &&
    yet_another && last_one)
{
    
}
```

### Return Values

- Do not needlessly surround the return expression with parentheses.

Example: `return result;`.


### Namespace Formatting

- The contents of namespaces are not intended.

Example:

```c++
namespace ISXUtils
{

class StringUtils
{
    ...
};

} // namespace ISXUtils
```

### Preprocessor Directives

The hash mark that starts a preprocessor directive should always be at the beginning of the line.

```c++
#if NOTIFY // No spaces after #
    NotifyClient();
#endif
```

### Class Format

- Any base class name should be on the same line as the subclass name, subject to the column limit.
- The open curly brace is at new line.
- The `public:`, `protected:` and `private:` keywords should not be indented.
- Do not leave a blank line after these keywords.
- The public section should be first, followed by the protected and finally the private section.
- It allowed to have few `public`, `protected`, `private` sections.
- Within each section, the declaration should be in the following order:
  - Typedefs and Enums
  - Constants (static const data members)
  - Constructors
  - Destructors
  - Operators
  - Methods, including static methods
  - Data Members (except static const data members)

Example: 

```c++
class MyClass : public ISXUnknownImpl
{
public: // No indent.
    MyClass(); // Regular 1 tab indent.
    explicit MyClass(int var);
    ~MyClass() {}
    
    void SomeFunction();
    void SomeFunctionThatDoesNothing() {}
    
    int get_some_var() const { return m_some_var; }
    void set_some_var(int var) { m_some_var = var; }
    
private:
    bool SomeInternalFunction();
    
    int m_some_var;
};
```

If a class has a few base classes, prefer to use subsequent lines indented 2 tabs

Example:

```c++
class VisionDevice
    : public ISXUnknownImpl // Putting the colon on the first line
    , public IDevice // Prefer putting the comma on the next line. It helps to avoid conflict during merging
    , public IVisionDevice
    , public ISXEventListenerContainerImpl
{
    
};
```

### Constructor Initializer Lists

- Constructor initializer lists can be all on one line or with subsequent lines indented four spaces. 
- There are two acceptable formats for initializer lists:

```c++
// When it all fits on one line:
MyClass::MyClass(int var) : m_some_var(var), m_some_other_var(var + 1) {}
```

or 

```c++
// When it requires multiple lines, indent 2 tabs, putting the colon on the first initializer line:
MyClass::MyClass(int var)
    : m_some_var(var) // 2 tabs indent
    , m_some_other_var(var + 1) // Prefer putting the comma on the next line. It helps to avoid conflict during merging 
{
    
}
```

### Horizontal Whitespace

Example:

```c++
// Assignment operators always have spaces arount them.
int i = 0; // Semicolons have no space before them.

// Binary operators usually have spaces around them.
v = w * x + y / z;

// No spaces separating unary operators and their arguments.
x = -5;
++x;
if (x && !y)

// Spaces around the colon in inheritance and initializer lists.
class Foo : public Bar
{
public:
    Foo(int b) : Bar() {} // No spaces inside empty braces.
    void Reset() { Clear(); } // Spaces separating braces from implementation.
};

while (test) {} // There is usually no space inside parentheses.

// For loops always have a space after the semicolon. Prefer to not have a space before the semicolon.
for (int i = 0; i < 5; ++i) {}

// Range-based for loops always have a space before and after the colon.
for (auto x : counts)
{
    ...
}
switch (i)
{
case 1: // No space before colon in a switch сase.
    ...
case 2: break; // Use a space after a colon if there's code after it.
}

// No spaces inside the angle brackets < and >, before <, or between > in a cast.
vector<string> x;
y = static_cast<char*>(x);
```

### Vertical Whitespace

Minimize use of vertical whitespace.

## Comments

- Don't comment on facts that can be derived quickly from the code itself.

### File comments

There is not mandatory file comments

### Class Comments

- External interfaces definition should have and accompanying comment that describes what it is for.

Example:

```c++
// The IBluetoothDevice interface represents the main functinality of the remote Bluetooth device.
class __declspec(novtable) IBluetoothDevice : public ISXUnknown 
{
    
}
```

- However, class comment is not mandatory for leaf or inner classes that are used only within the module.
- Prefer "self-documenting" function name instead of the comments for inner classes in the module.

### Function Comments

- Every function declaration in external interfaces should have comments immediately preceding it that describe what the function does and how to use it.
- In external interfaces prefer to use IN OUT macros before parameters to describe type of arguments.

Example:

```c++
class __declspec(novtable) IBluetoothDevice : public ISXUnknown 
{
public:
    // Retrieves the friendly Bluetooth name of the remote device.
    virtual HRESULT GetName(OUT wstring& device_name) = 0;
    // Retrieves list of uuids of Bluetooth service on the device that have been discovered.
    virtual HRESULT GetServices(OUT Uuids& service uuids) = 0;
}
```

- Comments for function are not mandatory for leaf of inner classes that used only within the module.
- Prefer "self-documenting" function name instead of comments. A good name is better than a good comment it will be seen everywhere the function is used.

### Implementation Comments

- Do not comment a function in `.cpp` file if it's already documented in header file. It's redundancy.
- Use an inline comment to comment "Named Function Parameter" in case if context of function call is vague.

Example: 

```c++
Connect(10, false);
Connect(/*timeout_ms =*/ 10, /*use_encryption =*/ false); // Better
```
