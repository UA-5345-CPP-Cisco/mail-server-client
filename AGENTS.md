# AGENTS.md

## Project Overview

This directory contains libraries and applications that demonstrate a client-server email system and an SMTP-based message delivery architecture.

The overall architecture consists of:

* **Desktop Mail Client** — A Qt-based GUI application for managing a user's mailbox.
* **Backend Mail Service** — An HTTP server that handles user-level mail operations.
* **SMTP Server** — An SMTP server responsible for accepting, relaying, and delivering messages within an isolated domain network.

## Message Flow

The main message flow is:

1. The Mail Client communicates with the Backend Mail Service over HTTP.
2. When a user sends a message, the Backend Mail Service uses the SMTP Server SDK to submit the message to the SMTP Server.
3. The SMTP Server processes and delivers the message.

The Backend Mail Service and SMTP Server currently use the same database.

## Technology Stack

* C++17
* Qt 6.0
* Boost:
    * Asio
    * Beast
    * JSON
* Crypto++ Modern
* SQLite
* CMake

## Code Conventions

* For non-Qt libraries and applications, follow `docs/CPP_CODE_CONVENTIONS.md`.
* Preserve the existing style of the component being modified.
* Avoid unrelated formatting or refactoring.
* Use modern C++17 features where they improve clarity and safety, for example smart pointers.
* Prefer Boost implementations over usage of macros for cross-platform builds
* Keep public interfaces backward-compatible unless the task explicitly requires an API change.

## Project Directory Structure

The project consists of the following directories:

* `/apps` — User-facing and server applications.

    * `/apps/mail-client` — Qt-based GUI mail client.
    * `/apps/mail-server-neo` — Latest version of the backend mail service.
    * `/apps/smtp-server` — SMTP server.
* `/libs` — Shared libraries and reusable components.

    * `/libs/logger` — General-purpose logging implementation.
    * `/libs/mail-server-sdk` — SDK for communicating with the Backend Mail Service.
    * `/libs/mail-storage` — SQLite-based wrapper around the mail service database.
    * `/libs/smtp-server-sdk` — SDK for communicating with the SMTP Server.
    * `/libs/thread-pool` — Thread-pool implementation.
* `/config` — Application configuration files.
* `/data` — Example and test data.
* `/docs` — Project documentation and code conventions.
* `/scripts` — Development, build, formatting, and testing scripts.
* `/tests` — Tests for applications and libraries.

## Development Scripts

The following scripts are available under `/scripts`:

* `build-and-run-qt-mail-client.sh` — Builds and runs the Qt Mail Client.
* `clang-check-format.sh` — Checks C++ source formatting.
* `clang-force-format.sh` — Applies C++ source formatting.
* `cmake-configure-debug.sh` — Configures project components for a debug build.
* `run-tests.sh` — Runs the project test suite.

Run scripts from the repository root unless a script documents otherwise.

## Instructions for Coding Agents

When modifying this repository:

1. Read the relevant component's existing code and nearby tests before making changes.
2. Follow `docs/CPP_CODE_CONVENTIONS.md` where applicable.
3. Keep changes limited to the requested task.
4. Do not modify generated files, build output, test data, or configuration files unless required.
5. Add or update tests when behavior changes or a bug is fixed.
6. Run formatting checks after changing C++ source files.
7. Run the relevant tests before completing the task.
8. Document any tests or checks that could not be run.

## Validation

For changes to C++ code, run:

```bash
./scripts/clang-check-format.sh
./scripts/run-tests.sh
```

For build-related changes, also verify that the debug configuration succeeds:

```bash
./scripts/cmake-configure-debug.sh
```

## Completion Criteria

A change is complete when:

* The requested behavior is implemented.
* Relevant tests pass.
* Formatting checks pass.
* No unrelated files are changed.
* User-facing or architectural changes are documented where necessary.
