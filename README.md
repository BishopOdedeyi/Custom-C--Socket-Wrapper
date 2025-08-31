# Custom-C--Socket-Wrapper
A lightweight C++ wrapper around low-level sockets, providing a clean and modern API for TCP/UDP networking.


> A modern C++ socket wrapper built on Boost.Asio, providing a simple object-oriented API for TCP and UDP networking.

---

### 🔹 README.md

````markdown
# Custom C++ Socket Wrapper (Boost.Asio)

A lightweight, modern C++ socket wrapper built on top of **Boost.Asio**,  
providing a cleaner and more intuitive interface for TCP/UDP networking.  

This library is designed to simplify socket programming in C++,  
while still giving access to low-level control when needed.

---

## ✨ Features
- Object-oriented C++ wrapper around Boost.Asio sockets  
- Supports **TCP** and **UDP**  
- Simplified API for `bind`, `connect`, `listen`, `accept`, `send`, and `recv`  
- Cross-platform (Linux, macOS, Windows)  
- DNS resolution (`gethostname`, `gethostbyname`)  
- RAII-based resource management  
- Optional socket options (e.g., reuse address)  

---

## 📦 Installation

Clone the repository:
```bash
git clone https://github.com/BishopOdedeyi/Custom-C--Socket-Wrapper.git
cd Custom-C--Socket-Wrapper
````

Make sure you have **Boost.Asio** installed (header-only since Boost 1.70+):

* On Ubuntu/Debian:

  ```bash
  sudo apt-get install libboost-all-dev
  ```
* On macOS (Homebrew):

  ```bash
  brew install boost
  ```
* On Windows, download Boost from [Boost.org](https://www.boost.org/).

---

## 🚀 Usage Example

### TCP Server

```cpp
#include "Socket.hpp"
#include <iostream>

int main() {
    boost::asio::io_context io;
    Socket server(io, AF_INET, Socket::TCP, 0);

    server.bind("127.0.0.1", 8080);
    server.listen();

    std::cout << "Server listening on 127.0.0.1:8080\n";

    auto client = server.accept();
    std::string msg = client.recv(1024);

    std::cout << "Client says: " << msg << std::endl;
    client.send("Hello from server!");

    return 0;
}
```

### TCP Client

```cpp
#include "Socket.hpp"
#include <iostream>

int main() {
    boost::asio::io_context io;
    Socket client(io, AF_INET, Socket::TCP, 0);

    client.connect("127.0.0.1", 8080);
    client.send("Hello from client!");

    std::string reply = client.recv(1024);
    std::cout << "Server replied: " << reply << std::endl;

    return 0;
}
```

---

## 🛠 Requirements

* C++17 or newer
* Boost.Asio (Boost 1.70+ recommended)

---

## 📜 License

MIT License – free to use in open-source and commercial projects.

---

## 🤝 Contributing

Pull requests and issues are welcome!
If you have improvements (async API, SSL support, etc.), feel free to contribute.
