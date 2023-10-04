#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

constexpr double G = 6.674E-11;
constexpr double PI = 3.14159265358979323846;

// helper for reading from input streams
template <typename T, typename CharT, typename Traits>
auto read_from(std::basic_istream<CharT, Traits>& is) -> T {
  T value;
  is >> value;
  return value;
}

// helper for reading a single line
template <typename CharT, typename Traits>
auto read_line(std::basic_istream<CharT, Traits>& is) -> string {
  string line;
  std::getline(is, line);
  return line;
}

struct Vec3d {
  int x;
  int y;
  int z;

  Vec3d() = default;

  auto operator*=(const int rhs) -> Vec3d& {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }

  friend auto operator<<(std::ostream& os, const Vec3d& v) -> std::ostream& {
    return os << v.x << ", " << v.y << ", " << v.z;
  }
};

inline auto operator*(const Vec3d& lhs, const int rhs) -> Vec3d {
  auto result = lhs;
  result *= rhs;
  return result;
}

inline auto operator*(const int lhs, const Vec3d& rhs) -> Vec3d {
  auto result = rhs;
  result *= lhs;
  return result;
}

class Body {
  string name{"none"};
  string orbit_name{"none"};
  double mass{0};
  Vec3d position{0, 0, 0};
  Vec3d velocity{0, 0, 0};
  Vec3d acceleration{0, 0, 0};

 public:
  Body() = default;
  Body(string name, string orbit_name, double mass, Vec3d position,
       Vec3d velocity, Vec3d acceleration)
      : name{std::move(name)},
        orbit_name{std::move(orbit_name)},
        mass{mass},
        position{position},
        velocity{velocity},
        acceleration{acceleration} {}

  friend auto operator<<(std::ostream& os, const Body& b) -> std::ostream& {
    return os << b.name << ", " << b.orbit_name << ", " << b.mass << ", "
              << b.position << ", " << b.velocity << ", " << b.acceleration;
  }

 private:
  static auto set_accelerations(vector<Body>& bodies, int acc) -> void {
    for (auto& b : bodies) {
      b.acceleration *= acc;
    }
  }

  friend class SolarSystem;
};

class SolarSystem {
  vector<Body> bodies;

  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> pos_dist;
  std::uniform_real_distribution<> angle_dist;

 public:
  explicit SolarSystem(const string& path)
      : gen(rd()), pos_dist(0, 10), angle_dist(0, 2 * PI) {
    auto file = std::ifstream(path);

    if (!file.is_open()) {
      throw std::system_error(errno, std::system_category(), path);
    }
    // skip the first line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto suns_mass = parse_sun(file);

    while (true) {
      const auto line = read_line(file);
      if (line.empty()) {
        break;
      }

      auto iss = std::istringstream(line);

      const auto name = read_from<string>(iss);
      const auto orbit_name = read_from<string>(iss);
      const auto mass = read_from<double>(iss);

      if (orbit_name != "Sun") {
        bodies.emplace_back(name, orbit_name, mass, Vec3d{}, Vec3d{}, Vec3d{});
        continue;
      }

      read_from<double>(iss);  // skipping diameter
      const auto perihelion = read_from<double>(iss);
      const auto aphelion = read_from<double>(iss);
      const auto radius = (perihelion + aphelion) / 2;
      const auto random_angle = angle_dist(gen);
      const auto rcos = radius * std::cos(random_angle);
      const auto rsin = radius * std::sin(random_angle);

      const auto position = random_position();

      const auto velocity_magnitude = std::sqrt(G * suns_mass / radius);
      const auto velocity =
          Vec3d{static_cast<int>(velocity_magnitude * rcos),
                static_cast<int>(velocity_magnitude * rsin), 0};

      const auto acceleration_magnitude =
          velocity_magnitude * velocity_magnitude / radius;
      const auto acceleration =
          Vec3d{static_cast<int>(acceleration_magnitude * rcos),
                static_cast<int>(acceleration_magnitude * rsin), 0};

      cout << "body name: " << name << " orbit: " << orbit_name << endl;
      cout << "orbital velocity is: " << velocity_magnitude << endl;
      cout << "centripetal acceleration: " << acceleration_magnitude << endl;
      bodies.emplace_back(name, orbit_name, mass, position, velocity,
                          acceleration);
    }
  }

  void step_forward(int acc) { Body::set_accelerations(bodies, acc); }

  friend auto operator<<(std::ostream& os, const SolarSystem& s)
      -> std::ostream& {
    for (const auto& b : s.bodies) {
      os << b << '\n';
    }
    return os;
  }

 private:
  auto random_position() -> Vec3d {
    return {pos_dist(gen), pos_dist(gen), pos_dist(gen)};
  }

  auto parse_sun(std::ifstream& file) -> double {
    const auto line = read_line(file);
    auto iss = std::istringstream(line);

    const auto name = read_from<string>(iss);
    const auto orbit_name = read_from<string>(iss);
    const auto mass = read_from<double>(iss);

    bodies.emplace_back(name, orbit_name, mass, Vec3d{}, Vec3d{}, Vec3d{});
    return bodies.at(0).mass;
  }
};

auto main() -> int try {
  cout << "########" << endl;
  cout << "Main Problem" << endl;
  cout << "########" << endl;

  auto s = SolarSystem("./solarsystem.dat");
  cout << s;
  cout << "=============" << endl;

  int acc = 5000;
  s.step_forward(acc);

  cout << s;
  cout << "====[ end ]====" << endl;
  cout << "               " << endl;
  return EXIT_SUCCESS;
} catch (const std::system_error& e) {
  cerr << "An error occured, likely having to do with a file: " << e.what()
       << endl;
  return EXIT_FAILURE;
} catch (const std::exception& e) {
  cerr << "An error occured: " << e.what() << endl;
  return EXIT_FAILURE;
} catch (...) {
  cerr << "An unknown error occured, something has gone very wrong " << endl;
  return EXIT_FAILURE;
}
