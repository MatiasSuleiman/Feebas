#include <cstdlib>
#include <iostream>
#include <string>

#include <httplib.h>

namespace {

int read_port() {
  const char* value = std::getenv("PORT");
  if (value == nullptr || std::string(value).empty()) {
    return 8080;
  }

  try {
    return std::stoi(value);
  } catch (...) {
    return 8080;
  }
}

std::string read_static_dir() {
  const char* value = std::getenv("STATIC_DIR");
  if (value == nullptr || std::string(value).empty()) {
    return "frontend";
  }

  return value;
}

}  // namespace

int main() {
  const int port = read_port();
  const std::string static_dir = read_static_dir();

  httplib::Server server;

  server.Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
    res.set_content("{\"status\":\"ok\"}", "application/json");
  });

  if (!server.set_mount_point("/", static_dir)) {
    std::cerr << "Failed to mount static directory: " << static_dir << '\n';
    return 1;
  }

  std::cout << "Feebas simulator is running." << std::endl;
  std::cout << "Open http://localhost:" << port << " in your browser." << std::endl;
  std::cout << "Container bind address: http://0.0.0.0:" << port << std::endl;
  std::cout << "Serving static files from " << static_dir << std::endl;

  if (!server.listen("0.0.0.0", port)) {
    std::cerr << "Failed to start server on port " << port << '\n';
    return 1;
  }

  return 0;
}
