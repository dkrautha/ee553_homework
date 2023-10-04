#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <functional>
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

// global constants
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

// 3 dimensional vector class
struct Vec3d {
  double x;
  double y;
  double z;

  auto zero() -> void {
    x = 0;
    y = 0;
    z = 0;
  }

  auto length() const -> double { return std::sqrt(x * x + y * y + z * z); }

  auto abs() const -> Vec3d {
    return Vec3d{std::abs(x), std::abs(y), std::abs(z)};
  }

  auto operator+=(const double rhs) -> Vec3d& {
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
  }

  auto operator+=(const Vec3d& rhs) -> Vec3d& {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  auto operator-=(const Vec3d& rhs) -> Vec3d& {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  auto operator*=(const double rhs) -> Vec3d& {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }

  friend auto operator<<(std::ostream& os, const Vec3d& v) -> std::ostream& {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  }
};

auto operator+(const Vec3d& lhs, const double rhs) -> Vec3d {
  auto result = lhs;
  result += rhs;
  return result;
}

auto operator+(const Vec3d& lhs, const Vec3d& rhs) -> Vec3d {
  auto result = lhs;
  result += rhs;
  return result;
}

auto operator-(const Vec3d& lhs, const Vec3d& rhs) -> Vec3d {
  auto result = lhs;
  result -= rhs;
  return result;
}

auto operator*(const Vec3d& lhs, const double rhs) -> Vec3d {
  auto result = lhs;
  result *= rhs;
  return result;
}

auto operator*(const double lhs, const Vec3d& rhs) -> Vec3d {
  auto result = rhs;
  result *= lhs;
  return result;
}

class Body {
  string name_{"none"};
  string orbit_name_{"none"};
  double mass_{0};

 public:
  Vec3d position{0, 0, 0};
  Vec3d velocity{0, 0, 0};
  Vec3d acceleration{0, 0, 0};

  Body() = default;
  Body(string name, string orbit_name, double mass, Vec3d position,
       Vec3d velocity, Vec3d acceleration)
      : name_{std::move(name)},
        orbit_name_{std::move(orbit_name)},
        mass_{mass},
        position{position},
        velocity{velocity},
        acceleration{acceleration} {}

  auto name() const -> const string& { return name_; }

  auto mass() const -> double { return mass_; }
  auto mass_mut() -> double& { return mass_; }

  friend auto operator<<(std::ostream& os, const Body& b) -> std::ostream& {
    return os << "Name: " << b.name_ << "\n\tOrbit: " << b.orbit_name_
              << "\n\tMass: " << b.mass_ << "\n\tPosition: " << b.position
              << "\n\tVelocity: " << b.velocity
              << "\n\tAcceleration: " << b.acceleration;
  }
};

class SolarSystem {
  vector<Body> bodies;

 public:
  explicit SolarSystem(const string& path) {
    auto file = std::ifstream(path);

    if (!file.is_open()) {
      throw std::system_error(errno, std::system_category(), path);
    }
    // skip the first line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    {  // parsing the sun
      auto iss = std::istringstream(read_line(file));
      const auto name = read_from<string>(iss);
      const auto orbit = read_from<string>(iss);
      const auto mass = read_from<double>(iss);
      bodies.emplace_back(name, orbit, mass, Vec3d{}, Vec3d{}, Vec3d{});
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> angle_dist(0, 2 * PI);

    // parsing the rest of the bodies
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
        // if we aren't orbiting the sun, skip it
        cout << "Skipping adding body to the simulation: " << name
             << ", it doesn't orbit the sun." << endl;
        continue;
      }

      read_from<double>(iss);  // skipping diameter
      const auto perihelion = read_from<double>(iss);
      const auto aphelion = read_from<double>(iss);
      const auto radius = (perihelion + aphelion) / 2;
      const auto random_angle = angle_dist(gen);
      const auto random_x = radius * cos(random_angle);
      const auto random_y = radius * sin(random_angle);

      const auto starting_position = Vec3d{random_x, random_y, 0};

      cout << "Adding body to the simulation: " << name << endl;
      cout << "\tStarting position is: " << starting_position << endl;
      cout << "\tRadius is: " << radius << endl;
      bodies.emplace_back(name, orbit_name, mass, starting_position, Vec3d{},
                          Vec3d{});
    }
  }

  void time_step(double dt) {
    for (auto& b1 : bodies) {
      b1.acceleration.zero();
      for (auto& b2 : bodies) {
        if (b2.name() != b1.name()) {
          // F = ma = G * M * m / r^2
          // a = G * M * m / (m * r^2)
          // a = G * M / r^2
          const auto dist = (b1.position - b2.position).length();
          const auto a1 = G * b2.mass() / (dist * dist * dist);
          b1.acceleration += a1;
          const auto a2 = G * b1.mass() / (dist * dist * dist);
          b2.acceleration += a2;
        }
      }
    }
    for (auto& b : bodies) {
      // d = v * dt + 0.5 * a * dt^2
      b.position += b.velocity * dt + 0.5 * b.acceleration * dt * dt;
      // vf = vi + a * dt
      b.velocity += b.acceleration * dt;
    }
  }

  auto get_body(const string& name)
      -> std::optional<std::reference_wrapper<const Body>> {
    for (const auto& b : bodies) {
      if (b.name() == name) {
        return std::reference_wrapper<const Body>(b);
      }
    }
    return {};
  }

  friend auto operator<<(std::ostream& os, const SolarSystem& s)
      -> std::ostream& {
    for (const auto& b : s.bodies) {
      os << b << '\n';
    }
    return os;
  }
};

constexpr double earth_year = 365 * 24 * 60 * 60;
constexpr int num_time_steps = 1000;
constexpr double dt = earth_year / num_time_steps;

auto vec_check_difference(const Vec3d& v1, const Vec3d& v2) -> bool {
  return (v1.x - v2.x) < 1e-6 && (v1.y - v2.y) < 1e-6 && (v1.z - v2.z) < 1e-6;
}

auto main() -> int try {
  auto s = SolarSystem("./solarsystem.dat");
  cout << "\nSolar system loaded, displaying bodies:" << endl
       << s << "\nEnd of solar system display\n"
       << endl;

  const auto earth_position_begin = s.get_body("Earth").value().get().position;
  cout << "Beginning simulation for one Earth year" << endl;
  for (int i = 0; i < num_time_steps; i += 1) {
    s.time_step(dt);
  }
  const auto earth_position_end = s.get_body("Earth").value().get().position;
  if (vec_check_difference(earth_position_begin, earth_position_end)) {
    cout << "Simulation successful" << endl;
  } else {
    cout << "Simulation failed, Earth may have moved too much" << endl;
    return EXIT_FAILURE;
  }
  cout << "Displaying bodies after simulation, Earth is in roughtly the same "
          "place\n"
       << endl;
  cout << s;
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
