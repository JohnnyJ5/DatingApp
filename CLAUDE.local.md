# Local Development Guidelines

## Project Context

This is a multi-threaded Crow web server. PostgreSQL connections are **not thread-safe** — do not share a single connection across threads with a mutex. Instead, rely on the database itself to handle data contention (transactions, `SELECT ... FOR UPDATE`, constraints, triggers). Each request handler should acquire its own connection from a connection pool or open a short-lived connection.

## C++ Coding Standards

- RAII for all resource management; no raw `new`/`delete`
- Prefer `std::unique_ptr`/`std::shared_ptr` over raw pointers
- Mark functions `[[nodiscard]]` when ignoring the return value is a bug
- Use `const` by default; only drop when mutation is required
- Prefer `std::string_view` over `const std::string&` for read-only params
- No `using namespace std;` in headers
- Prefer structured bindings and range-for over index loops
- All SQL queries must use parameterized statements — no string concatenation
- Functions > ~40 lines warrant a refactor consideration
- Avoid global mutable state; if shared state is unavoidable, document the invariant and thread-safety contract explicitly

## Database Contention Rules

- Prefer database-level locking (`SELECT ... FOR UPDATE`, advisory locks) over application-level mutexes for shared data
- Use transactions to group related writes; let PostgreSQL's MVCC handle read isolation
- Triggers and constraints (already in `db/schema.sql`) are the authoritative source of business-rule enforcement — don't duplicate that logic in C++
