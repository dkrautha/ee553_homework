#include <cerrno>        // for errno
#include <cmath>         // for fabs, sqrt, cos, sin
#include <cstdlib>       // for EXIT_FAILURE
#include <exception>     // for exception
#include <fstream>       // for operator<<, basic_ostream, endl, ostream
#include <functional>    // for reference_wrapper
#include <iostream>      // for cout, cerr
#include <limits>        // for numeric_limits
#include <optional>      // for optional
#include <random>        // for random_device, uniform_real_distribution
#include <sstream>       // for istringstream
#include <string>        // for char_traits, string, operator<<, allocator
#include <system_error>  // for system_error, system_category
#include <utility>       // for move
#include <vector>        // for vector

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

  auto zero() {
    x = 0;
    y = 0;
    z = 0;
  }

  auto magnitude() const -> double { return std::sqrt(x * x + y * y + z * z); }
  auto magnitude_squared() const -> double { return x * x + y * y + z * z; }

  auto normalize() const -> Vec3d {
    auto mag = magnitude();
    return Vec3d{x / mag, y / mag, z / mag};
  }

  auto abs() const -> Vec3d {
    return Vec3d{std::fabs(x), std::fabs(y), std::fabs(z)};
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

  static auto set_accelerations(vector<Body>& bodies) {
    for (auto& b1 : bodies) {
      b1.acceleration.zero();
      for (auto& b2 : bodies) {
        if (b2.name() == b1.name()) {
          continue;
        }
        const auto delta_pos = b2.position - b1.position;
        const auto rsquared = delta_pos.magnitude_squared();
        const auto accel_mag = G * b2.mass() / rsquared;
        const auto direction_to_other = delta_pos.normalize();
        const auto accel_to_other = accel_mag * direction_to_other;
        b1.acceleration += accel_to_other;
      }
    }
  };
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
    const auto suns_mass = bodies[0].mass();

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

      const auto starting_position =
          Vec3d{radius * cos(random_angle), radius * sin(random_angle), 0};

      const auto velocity_mag = std::sqrt(G * suns_mass / radius);
      const auto velocity =
          Vec3d{velocity_mag * cos(random_angle + PI / 2.0),
                velocity_mag * sin(random_angle + PI / 2.0), 0};

      cout << "Adding body to the simulation: " << name << endl;
      cout << "\tStarting position is: " << starting_position << endl;
      cout << "\tStarting veclocity is: " << velocity << endl;
      cout << "\tRadius is: " << radius << endl;
      bodies.emplace_back(name, orbit_name, mass, starting_position, velocity,
                          Vec3d{});
    }
  }

  auto time_step(double dt) {
    Body::set_accelerations(bodies);
    for (auto& b : bodies) {
      b.position += b.velocity * dt + 0.5 * b.acceleration * dt * dt;
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

auto run_simulation(SolarSystem& s) {
  // constexpr auto earth_year = 365.24 * 24 * 60 * 60;
  constexpr auto earth_year = 365.24 * 24 * 60 * 60;
  constexpr auto num_time_steps = 1000.0;
  constexpr auto dt = earth_year / num_time_steps;

  const auto run = [](SolarSystem& s) {
    for (int i = 0; i < num_time_steps; i += 1) {
      s.time_step(dt);
    }
  };

  const auto earth = s.get_body("Earth");

  if (!earth.has_value()) {
    cout << "Earth not present in the solar system" << endl;
    return run(s);
  }
  cout << "Earth present in the solar system, using as a reference" << endl;
  const auto& ref = earth.value().get();

  cout << "Earth position before: " << ref.position << endl;
  run(s);
  cout << "Earth position after: " << ref.position << endl;
}

auto main() -> int try {
  auto s = SolarSystem("./solarsystem.dat");

  cout << "\nSolar system loaded, displaying bodies:" << endl
       << s << "\nEnd of solar system display\n\n"
       << "Beginning simulation for one Earth year" << endl;

  run_simulation(s);

  cout << "\nFinal Simulation State:\n" << s << endl;
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
