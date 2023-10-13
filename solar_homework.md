# Solar Homework

## Vec3d

I've made two major improvements beyond what was required for `Vec3d`.

The first is using the standard library's `std::array` type to store the values instead of three separate doubles. This makes the implementation of `operator[]` and iterator functions for use in a range based for loop trivial, as each of these can just delegate to those of the standard library type.

The second improvement is the inclusion of a number of utility methods: `magnitude()`, `magnitude_squared()`, `normalize()`, and `abs()`. These are used in the orbital mechanics equations often enough that abstracting them into the object makes sense.

## Strong Types

I'm a major proponent of using strong types for parameters of functions, especially when there are multiple parameters of the same type next to each other in a function signature. Take the `Body` constructor as an example, it has three `Vec3d` parameters right next to each other. Somebody could easily mix up the order on accident, and finding the source of that bug could be very difficult as there won't be any compilation or runtime errors, just incorrect calculations.

One common solution to make it more obvious that the order of these parameters matters is making aliases with `typedef` or `using`. The problem with doing so is how weak type aliases are, the compiler will gladly let you mix them together as you please. Strong types avoid this by not being aliases, and instead very thin wrappers around a type. That means if you try to use a `Acceleration` as a `Velocity` you get a compilation error.

To help ensure that separate strong types are actually separate, there is a second template parameter `Parameter` where the user of a strong type is expected to put some sort of tag type, usually an empty struct with a unique name.

## Body

There are two main changes to the `Body` class. The first is modifying the constructor to use strong types for the three vector parameters, the reasons for which are explained in the strong types section. The second major change was to the function for setting the accelerations of the bodies.

The original instructions asked us to increment the accelerations of each body by the value passed into the function. This didn't make a lot of sense to me as it doesn't really simulate anything, so I learned how to perform an N-body simulation using orbital mechanics formulas, and implemented that within the function. Each body in the simulation exerts some force on every other body, and those forces are summed together to determine the new acceleration for each body. How this new acceleration affects the velocity and position is handled inside the `SolarSystem` class.

## SolarSystem

The first major change I made was to modify the process for constructing a `SolarSystem` object. I dislike exceptions in C++ because they aren't in the signature of a function, and therefore aren't obvious to the caller of a function. Because constructors aren't functions with a return type, they can't return any sort of error, and the only option is to throw an exception. To remedy this, there's only a single constructor for the `SolarSystem` type, and it has been made private. To make a `SolarSystem` you must use the static factory function `from_file()`, which returns a `std::variant`, a type-safe tagged union from the standard library. The variant either contains a successfully constructed `SolarSystem`, or a `std::string` containing an error message with some information on what went wrong.

Inside the body of the `from_file()` function the following error cases are handled:

- If the file can't be opened
- The second line of the file can't be read (containing the Sun's information)
- The name, orbit, and mass of the Sun can't be parsed correctly
- The name of the first body (what should be the Sun) isn't the Sun
- If the name, orbit, and mass of any subsequent bodies can't be parsed correctly
- If the diameter, perihelion, and aphelion of any subsequent bodies can't be parsed correctly

If none of these error cases are triggered, a random angle is generated, and starting position calculated for each body based on an approximate radius (using the average of perihelion and aphelion). A starting velocity is then calculated using orbital mechanics in a direction perpendicular to the direction from the planet to the Sun.

After all bodies have been parsed and had initial locations and velocities calculated, the `SolarSystem` is constructed and returned.

The second major change is to what happens in the step forward function. Instead of taking an acceleration to apply to each body, it takes a time interval in seconds as a double that is used to modify the position and velocity of the bodies according to the fundamental kinematic equations.

I've also added a `get_body()` function which receives a name and returns an optional reference to the body with the given name. If the body is found the `std::optional` will contain a `std::reference_wrapper<const Body>`. Otherwise, it will have `std::nullopt`, which is the indicator for an empty optional type.

## Running the Simulation

The main function has been heavily simplified, and only has one job. It tries to construct a `SolarSystem` and then either calls a separate function to run the simulation (`run_simulation()`), or prints out the error that occurred in constructing the `SolarSystem` and exits. Deciding which of these two paths to take is determined using the `std::visit()` function from the standard library. `std::visit()` accepts something that has `operator()` defined on it for each type inside a variant, and calls the appropriate overload based on what type the variant contains at runtime. Getting this to work requires a bit of template metaprogramming, but luckily cpprefernce.com shows how this can be done easily.

The `run_simulation` function is also fairly simple. It checks to see whether there is a planet named Earth in the simulation, and if so prints its location out before and after the simulation is run. The simulation itself is run for a single Earth year with 1000 individual time steps. If the simulation works properly then the start and end positions of the Earth should be in roughly the same location as they started. If Earth isn't present, then the simulation is run without a reference planet being logged.
