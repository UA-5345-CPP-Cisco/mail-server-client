# GitHub Copilot System Instructions - ISX Core Project

Generate C++ and QML code for this project. No conversational text, no explanation — output code only, unless
explanation is explicitly requested.

## Output Rules

- Refactor or bug-fix in an existing file: output only the changed method/block, not the whole file. Mark omitted
  regions with `// ... unmodified code`.
- No comments that restate what the code already shows. If a function is documented in the `.h`, don't repeat that doc
  in the `.cpp`.
- Indentation: tabs, never spaces.

## File Layout

### Header (`.h`)

- Guard: `#pragma once` only — no `#ifndef`.
- Include order: system → third-party (Qt) → project.
- Prefer including the real header over forward-declaring.
- Inline method bodies only if ≤10 lines.

### Source (`.cpp`)

- Include order: PCH (`StdAfx.h`, if used) → matching header → system → third-party → project.

### Namespaces

- Wrap the entire file, placed after includes. Contents are flush left (no extra indent level).
- Prefix `ISX` (`ISXMail`, `ISXAnyType`).

### Class layout

- Base class on the same line: `class Foo : public Bar`
- Access specifiers in order `public:` / `protected:` / `private:` — not indented, no blank line after.
- Order inside each section: typedefs/enums → constants (incl. static const members) → constructors → destructor →
  operators → methods (incl. static) → data members.

## Naming

| Element                         | Convention                                           | Example               |
|---------------------------------|------------------------------------------------------|-----------------------|
| File                            | PascalCase                                           | `UserCredentials.cpp` |
| Class / struct / enum / typedef | PascalCase, no `C`/`T`/`Type` prefix, no `_t` suffix | `EmailProvider`       |
| Interface                       | PascalCase, `I` prefix                               | `IEmailProvider`      |
| Function                        | PascalCase                                           | `AddData`             |
| Getter / setter                 | `get_`/`set_` + snake_case                           | `get_searched_text`   |
| Local var / argument            | snake_case                                           | `current_index`       |
| Boolean var                     | snake_case, `is_`/`has_`/`can_`/`should_` prefix     | `is_starred`          |
| Class member, non-static        | `m_` + snake_case                                    | `m_current_page`      |
| Class member, static            | `s_` + snake_case                                    | `s_instance_count`    |
| Struct member                   | snake_case, no prefix                                | `send_to`             |
| Global                          | `g_` + snake_case                                    | `g_logger`            |
| Constant                        | UPPER_SNAKE_CASE                                     | `MAX_RETRY_COUNT`     |
| Macro                           | avoid; if unavoidable, UPPER_SNAKE_CASE              | —                     |

No generic names (`tmp`, `retval`).

## Whitespace and Braces

Allman style — opening brace on its own line. One space between a keyword (`if`/`for`/`while`) and `(`, none inside the
parens. Single-line bodies skip braces unless another branch of the same `if`/`else` needs them.

```cpp
if (word.isEmpty())
{
	return true;
}
```
