# 🧠 credis — C++ meets Redis, and they hit it off

> *"Why pay for Redis when you can lose sleep building your own?"*

Welcome to **credis**, a lightweight, Redis-compatible, in-memory data store handcrafted in C++. It speaks RESP, juggles multiple clients like a caffeinated octopus, and remembers things even after you kill it (persistence, not trauma).


---

## 🎯 The Mission

Build a Redis clone that:
- Talks fluent **RESP** (Redis Serialization Protocol)
- Handles **strings, lists, and hashes** like a champ
- Serves **multiple clients at once** without falling over
- **Persists to disk** so your data survives a `Ctrl+C` panic-quit

Think of it as Redis's scrappy younger sibling — same vocabulary, home-cooked internals.

---

## 📋 The Basics

| | |
|---|---|
| **Name** | `my_redis_server` |
| **Default Port** | `6379` (yes, the iconic one) |
| **Language** | C++17+ |
| **Vibe** | Systems programming, but make it fun |

`credis` implements a Redis clone in C++, serving up common Redis commands over a plain TCP socket using the RESP protocol. It supports:

- **Common Commands**: `PING`, `ECHO`, `FLUSHALL`
- **Key/Value**: `SET`, `GET`, `KEYS`, `TYPE`, `DEL`/`UNLINK`, `EXPIRE`, `RENAME`
- **Lists**: `LGET`, `LLEN`, `LPUSH`/`RPUSH` (multi-element!), `LPOP`/`RPOP`, `LREM`, `LINDEX`, `LSET`
- **Hashes**: `HSET`, `HGET`, `HEXISTS`, `HDEL`, `HKEYS`, `HVALS`, `HLEN`, `HGETALL`, `HMSET`

Data automatically dumps itself to `dump.my_rdb` every **300 seconds** and on graceful shutdown, like a diligent little squirrel burying acorns. On startup, it tries to dig those acorns back up.

---

## 🗂️ Repository Structure

```
├── include/                       # Public headers
│   ├── RedisCommandHandler.h
│   ├── RedisDatabase.h
│   └── RedisServer.h
├── src/                           # Implementation files
│   ├── RedisCommandHandler.cpp
│   ├── RedisDatabase.cpp
│   ├── RedisServer.cpp
│   └── main.cpp                   # Entry point — where the magic ignites
├── Concepts,UseCases&Tests.md     # Design concepts and command use cases
├── Makefile                       # Build rules
├── README.md                      # You are here 📍
└── test_all.sh                    # Bash script for all tests
```

---

## 🔧 Installation

You'll need a C++17 (or later) compiler. That's the toll for entry.

```bash
make
make clean
# from project root
make
```

Or, if you enjoy typing things manually like it's 1999:

```bash
g++ -std=c++17 -pthread -Iinclude src/*.cpp -o my_redis_server
```

---

## 🚀 Usage

### Firing Up the Server

```bash
./my_redis_server            # listens on 6379
./my_redis_server 6380       # listens on 6380, for the rebels
```

On startup, `credis` checks for a previous life:

```
Database loaded from dump.my_rdb
# or, if it's your first rodeo
No dump found or load failed; starting with an empty database.
```

A background thread quietly dumps the database every 5 minutes — no fanfare, just diligence.

To gracefully shut things down (and force an immediate save), hit **Ctrl+C**. `credis` will save its work and exit like a professional, not slam the laptop shut like the rest of us.

### Talking to the Server

Connect with the standard `redis-cli`, or your own custom RESP client:

```bash
redis-cli -p 6379
```

Example session:

```
127.0.0.1:6379> PING
PONG

127.0.0.1:6379> SET foo "bar"
OK

127.0.0.1:6379> GET foo
"bar"
```

Look at that. It just *works*. Chills.

---

## 📚 Supported Commands

### Common
| Command | What it does |
|---|---|
| `PING` | `PING` → `PONG` (the digital equivalent of "you up?") |
| `ECHO <msg>` | `ECHO <msg>` → `<msg>` |
| `FLUSHALL` | Nukes all data. Use responsibly. |

### Key/Value
| Command | What it does |
|---|---|
| `SET <key> <value>` | Store a string |
| `GET <key>` | Retrieve a string, or `nil` if it never existed |
| `KEYS *` | List all keys |
| `TYPE <key>` | `string` / `list` / `hash` / `none` |
| `DEL`/`UNLINK <key>` | Delete a key |
| `EXPIRE <key> <seconds>` | Set a TTL — a self-destruct timer, minus the drama |
| `RENAME <old> <new>` | Rename a key |

### Lists
| Command | What it does |
|---|---|
| `LGET <key>` | All elements |
| `LLEN <key>` | Length |
| `LPUSH`/`RPUSH <key> <v1> [v2 ...]` | Push multiple values, left or right |
| `LPOP`/`RPOP <key>` | Pop one, from either end |
| `LREM <key> <count> <value>` | Remove occurrences |
| `LINDEX <key> <index>` | Get element at index |
| `LSET <key> <index> <value>` | Set element at index |

### Hashes
| Command | What it does |
|---|---|
| `HSET <key> <field> <value>` | Set a field |
| `HGET <key> <field>` | Get a field |
| `HEXISTS <key> <field>` | Does the field exist? |
| `HDEL <key> <field>` | Delete a field |
| `HLEN <key>` | Field count |
| `HKEYS <key>` | All fields |
| `HVALS <key>` | All values |
| `HGETALL <key>` | All field/value pairs |
| `HMSET <key> <f1> <v1> [f2 v2 ...]` | Set multiple fields at once |

---

## 🏗️ Design & Architecture

- **Concurrency**: Every client gets its own `std::thread` — a private table just for them.
- **Synchronization**: A single `std::mutex db_mutex` guards all in-memory stores, so nobody trips over anybody else's data.
- **Data Stores**:
  - `kv_store` (`unordered_map<string,string>`) — for strings
  - `list_store` (`unordered_map<string,vector<string>>`) — for lists
  - `hash_store` (`unordered_map<string,unordered_map<string,string>>`) — for hashes
- **Expiration**: Lazy eviction via `purgeExpired()` on every access, backed by a `expiry_map` of TTLs.
- **Persistence**: A simplified, text-based "RDB" — dump/load straight to `dump.my_rdb`.
- **Singleton Pattern**: `RedisDatabase::getInstance()` ensures there's only ever one source of truth.
- **RESP Parsing**: A custom parser inside `RedisCommandHandler` that handles both inline and array-format commands.

---

## 🙌 Why "credis"?

**C**++ + **Redis** = **credis**. Simple math, solid pun. If you have a better name, keep it to yourself.

Happy hacking, and may your mutexes always lock in order. 🔒
