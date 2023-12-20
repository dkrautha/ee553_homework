// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute
// this software, either in source code form or as a compiled binary, for any
// purpose, commercial or non-commercial, and by any means.
//
// In jurisdictions that recognize copyright laws, the author or authors of this
// software dedicate any and all copyright interest in the software to the public
// domain. We make this dedication for the benefit of the public at large and to
// the detriment of our heirs and successors. We intend this dedication to be an
// overt act of relinquishment in perpetuity of all present and future rights to
// this software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
#include <array>  // for array
#include <cerrno>  // for errno
#include <cmath>  // for fabs, cos, sin, sqrt, NAN
#include <cstdlib>  // for size_t, EXIT_FAILURE, EXIT_SUCCESS
#include <fstream>  // for operator<<, char_traits, basic_ostream, endl
#include <functional>  // for reference_wrapper
#include <iostream>  // for cout
#include <limits>  // for numeric_limits
#include <optional>  // for optional
#include <random>  // for random_device, uniform_real_distribution
#include <sstream>  // for istringstream, ostringstream
#include <string>  // for basic_string, operator<<, string, operator>>
#include <system_error>  // for system_category, system_error
#include <utility>  // for move
#include <variant>  // for visit, variant
#include <vector>  // for vector, allocator

using std::cout;
using std::endl;
using std::string;
using std::vector;

// global constants
constexpr double G = 6.674E-11;
constexpr double PI = 3.14159265358979323846;

// 3 dimensional vector class
struct Vec3d {
  private:
    using data_type = std::array<double, 3>;
    data_type values;

  public:
    // constructors
    explicit Vec3d() : values {0, 0, 0} {}

    explicit Vec3d(double x, double y, double z) : values {x, y, z} {}

    // accessors
    auto x() const -> double {
        return values[0];
    }

    auto x_mut() -> double& {
        return values[0];
    }

    auto y() const -> double {
        return values[1];
    }

    auto y_mut() -> double& {
        return values[1];
    }

    auto z() const -> double {
        return values[2];
    }

    auto z_mut() -> double& {
        return values[2];
    }

    // utility methods
    auto zero() {
        for (auto& v : values) {
            v = 0;
        }
    }

    auto magnitude() const -> double {
        return std::sqrt(x() * x() + y() * y() + z() * z());
    }

    auto magnitude_squared() const -> double {
        return x() * x() + y() * y() + z() * z();
    }

    auto normalize() const -> Vec3d {
        auto const mag = magnitude();
        return Vec3d {x() / mag, y() / mag, z() / mag};
    }

    auto abs() const -> Vec3d {
        return Vec3d {std::fabs(x()), std::fabs(y()), std::fabs(z())};
    }

    // iterator methods
    auto begin() -> data_type::iterator {
        return values.begin();
    }

    auto begin() const -> data_type::const_iterator {
        return values.begin();
    }

    auto cbegin() const -> data_type::const_iterator {
        return values.cbegin();
    }

    auto end() -> data_type::iterator {
        return values.end();
    }

    auto end() const -> data_type::const_iterator {
        return values.end();
    }

    auto cend() const -> data_type::const_iterator {
        return values.cend();
    }

    // access operator overloads
    auto operator[](std::size_t pos) const -> double const& {
        return values[pos];
    }

    auto operator[](std::size_t pos) -> double& {
        return values[pos];
    }

    // arithmetic operator overloads
    auto operator+=(double const rhs) -> Vec3d& {
        for (auto& v : values) {
            v += rhs;
        }
        return *this;
    }

    auto operator+=(Vec3d const& rhs) -> Vec3d& {
        x_mut() += rhs.x();
        y_mut() += rhs.y();
        z_mut() += rhs.z();
        return *this;
    }

    auto operator-=(Vec3d const& rhs) -> Vec3d& {
        x_mut() -= rhs.x();
        y_mut() -= rhs.y();
        z_mut() -= rhs.z();
        return *this;
    }

    auto operator*=(double const rhs) -> Vec3d& {
        x_mut() *= rhs;
        y_mut() *= rhs;
        z_mut() *= rhs;
        return *this;
    }

    friend auto operator<<(std::ostream& os, Vec3d const& v) -> std::ostream& {
        return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    }
};

auto operator+(Vec3d const& lhs, double const rhs) -> Vec3d {
    auto result = lhs;
    result += rhs;
    return result;
}

auto operator+(Vec3d const& lhs, Vec3d const& rhs) -> Vec3d {
    auto result = lhs;
    result += rhs;
    return result;
}

auto operator-(Vec3d const& lhs, Vec3d const& rhs) -> Vec3d {
    auto result = lhs;
    result -= rhs;
    return result;
}

auto operator*(Vec3d const& lhs, double const rhs) -> Vec3d {
    auto result = lhs;
    result *= rhs;
    return result;
}

auto operator*(double const lhs, Vec3d const& rhs) -> Vec3d {
    auto result = rhs;
    result *= lhs;
    return result;
}

template<typename T, typename Parameter>
class StrongType {
  public:
    explicit StrongType(T const& value) : value_(value) {}

    explicit StrongType(T&& value) : value_(std::move(value)) {}

    auto get_mut() -> T& {
        return value_;
    }

    auto get() const -> T const& {
        return value_;
    }

  private:
    T value_;
};

using Position = StrongType<Vec3d, struct PositionParameter>;
using Velocity = StrongType<Vec3d, struct VelocityParameter>;
using Acceleration = StrongType<Vec3d, struct AccelerationParameter>;

class Body {
    string name_ {"none"};
    string orbit_name_ {"none"};
    double mass_ {0};
    Vec3d position_ {0, 0, 0};
    Vec3d velocity_ {0, 0, 0};
    Vec3d acceleration_ {0, 0, 0};

    friend class SolarSystem;

  public:
    Body() = default;

    Body(
        string name,
        string orbit_name,
        double mass,
        Position position,
        Velocity velocity,
        Acceleration acceleration
    ) :
        name_ {std::move(name)},
        orbit_name_ {std::move(orbit_name)},
        mass_ {mass},
        position_ {position.get()},
        velocity_ {velocity.get()},
        acceleration_ {acceleration.get()} {}

    auto name() const -> string const& {
        return name_;
    }

    auto mass() const -> double {
        return mass_;
    }

    auto position() const -> Vec3d const& {
        return position_;
    }

    friend auto operator<<(std::ostream& os, Body const& b) -> std::ostream& {
        return os << "Name: " << b.name_ << "\n\tOrbit: " << b.orbit_name_
                  << "\n\tMass: " << b.mass_ << "\n\tPosition: " << b.position_
                  << "\n\tVelocity: " << b.velocity_ << "\n\tAcceleration: " << b.acceleration_;
    }

  private:
    static auto set_accelerations(vector<Body>& bodies) {
        for (auto& b1 : bodies) {
            b1.acceleration_.zero();
            for (auto& b2 : bodies) {
                if (b2.name() == b1.name()) {
                    continue;
                }
                auto const delta_pos = b2.position_ - b1.position_;
                auto const rsquared = delta_pos.magnitude_squared();
                auto const accel_mag = G * b2.mass() / rsquared;
                auto const direction_to_other = delta_pos.normalize();
                auto const accel_to_other = accel_mag * direction_to_other;
                b1.acceleration_ += accel_to_other;
            }
        }
    }
};

template<typename T>
using Result = std::variant<T, std::string>;

class SolarSystem {
    vector<Body> bodies_;
    explicit SolarSystem(vector<Body> bodies) : bodies_(std::move(bodies)) {};

  public:
    static auto from_file(string const& path) -> Result<SolarSystem> {
        auto file = std::ifstream(path);

        if (!file.is_open()) {
            return std::system_error(errno, std::system_category(), path).what();
        }
        // skip the first line
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto bodies = vector<Body>();

        {  // parsing the sun
            string line;
            if (!std::getline(file, line)) {
                return "Error reading the sun from: " + path;
            }
            auto iss = std::istringstream(line);
            string name;
            string orbit;
            double mass = NAN;
            if (!(iss >> name >> orbit >> mass)) {
                return "Unable to read the name, orbit, and mass of the sun from: " + path;
            }
            if (name != "Sun") {
                return "The first body isn't the Sun.";
            }
            bodies.emplace_back(
                name,
                orbit,
                mass,
                Position {Vec3d {}},
                Velocity {Vec3d {}},
                Acceleration {Vec3d {}}
            );
        }
        auto const suns_mass = bodies[0].mass();

        std::mt19937 gen(std::random_device {}());
        std::uniform_real_distribution<> angle_dist(0, 2 * PI);

        // parsing the rest of the bodies
        string line;
        while (std::getline(file, line)) {
            auto iss = std::istringstream(line);

            string name;
            string orbit;
            double mass = NAN;
            if (!(iss >> name >> orbit >> mass)) {
                return "Unable to read the name, orbit, and mass of a body after "
                       "reading the sun: "
                    + path;
            }

            if (orbit != "Sun") {
                cout << "Skipping adding body to the simulation: " << name
                     << ", it doesn't orbit the sun." << endl;
                continue;
            }

            double diameter = NAN;
            double perihelion = NAN;
            double aphelion = NAN;
            if (!(iss >> diameter >> perihelion >> aphelion)) {
                std::ostringstream oss;
                oss << "Unable to read the diameter, perihelion, and aphelion of " << name
                    << " from: " << path;
                return oss.str();
            }
            auto const radius = (perihelion + aphelion) / 2;
            auto const random_angle = angle_dist(gen);

            // starting x = radius * cos(random_angle)
            // starting y = radius * sin(random_angle)
            auto const starting_position =
                Vec3d {radius * std::cos(random_angle), radius * std::sin(random_angle), 0};

            // velocity = sqrt(G * mass / radius)
            // velocity x = radius * cos(random_angle + PI / 2.0)
            // velocity y = radius * sin(random_angle + PI / 2.0)
            auto const velocity_mag = std::sqrt(G * suns_mass / radius);
            auto const velocity = Vec3d {
                velocity_mag * std::cos(random_angle + PI / 2.0),
                velocity_mag * std::sin(random_angle + PI / 2.0),
                0
            };

            // The starting position and velocity are calculated with orbital
            // mechanics equations, thanks stackoverflow
            // https://stackoverflow.com/questions/14845273/initial-velocity-vector-for-circular-orbit

            cout << "Adding body to the simulation: " << name << endl;
            cout << "\tStarting position is: " << starting_position << endl;
            cout << "\tStarting veclocity is: " << velocity << endl;
            cout << "\tRadius is: " << radius << endl;
            bodies.emplace_back(
                name,
                orbit,
                mass,
                Position {starting_position},
                Velocity {velocity},
                Acceleration {Vec3d {}}
            );
        }

        return SolarSystem(bodies);
    }

    auto time_step(double dt) {
        Body::set_accelerations(bodies_);
        for (auto& b : bodies_) {
            b.position_ += b.velocity_ * dt + 0.5 * b.acceleration_ * dt * dt;
            b.velocity_ += b.acceleration_ * dt;
        }
    }

    auto get_body(string const& name) -> std::optional<std::reference_wrapper<Body const>> {
        for (auto const& b : bodies_) {
            if (b.name() == name) {
                return std::reference_wrapper<Body const>(b);
            }
        }
        return {};
    }

    friend auto operator<<(std::ostream& os, SolarSystem const& s) -> std::ostream& {
        for (auto const& b : s.bodies_) {
            os << b << '\n';
        }
        return os;
    }
};

auto run_simulation(SolarSystem& s) {
    auto const runit = [](SolarSystem& s) {
        constexpr auto earth_year = 365 * 24 * 60 * 60;
        constexpr auto num_time_steps = 1000.0;
        constexpr auto dt = earth_year / num_time_steps;
        for (auto i = 0; i < num_time_steps; i += 1) {
            s.time_step(dt);
        }
    };

    if (auto const earth = s.get_body("Earth")) {
        cout << "Earth present in the solar system, using as a reference" << endl;
        auto const& ref = earth.value().get();
        cout << "Earth position before: " << ref.position() << endl;
        runit(s);
        cout << "Earth position after: " << ref.position() << endl;
        return;
    }
    cout << "Earth not present in the solar system" << endl;
    return runit(s);
}

// helper type for the visiting a variant
// from https://en.cppreference.com/w/cpp/utility/variant/visit
template<typename... Fs>
struct Overload: Fs... {
    using Fs::operator()...;
};

template<typename... Fs>
Overload(Fs...) -> Overload<Fs...>;

auto main() -> int {
    auto s = SolarSystem::from_file("./solarsystem.dat");

    return std::visit(
        Overload {
            [](SolarSystem& s) {
                cout << "\nSolar system loaded, displaying bodies:" << endl
                     << s << "\nEnd of solar system display\n\n"
                     << "Beginning simulation for one Earth year" << endl;
                run_simulation(s);
                cout << "\nFinal Simulation State:\n" << s << endl;
                return EXIT_SUCCESS;
            },
            [](std::string const& err) {
                cout << "An error occured: \n\t" << err << endl;
                return EXIT_FAILURE;
            }
        },
        s
    );
}
