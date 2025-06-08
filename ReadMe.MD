The aim of this library is to be  stand-alone library that can provide compile-time AND runtime units
and related conversions on demand, and allow any users to define their own types in a simple and elaborate way
to aid in equation checking. 

To do this, it will help to define some things.

Quantity: Something that is finitely measurable - mass, length, force, energy, currency etc.
The Quantity class in this library is defined by its dimensions, and there are compile-time functions which
allow Quantities to be conditionally added, subtracted, multiplied and divided, which in turn defines which 
Quantities can interact and subsequently which units can interact.

Dimension: A Dimension is an _aspect_ of a Quantity - so a Quantity has a _set_ of Dimenisons that define it.
    For example, the quantity _speed_ has dimensions [L^1, T^-1]
    we represent dimensions here as a class template which includes a _Tag_ (empty class/struct) and an _exponenent_ (integer)

The Unit class itself takes in three template parameters:
template<typename NumericType, typename QuantityType, typename ConversionType>
- The Numeric type is a type that has the operators +, -, *, \ all defined, such as a float, double or any similar user-defined type,
    like a mathematical vector, or matrix, or similar

- The quantity type is as explained above, there are example quantities that are defined in quantities.h; the units depend on
the types defined by the arithmetic operations between different quantities. Units can only be added or subtracted if the quantities match,
but multiplication and division can occur between units with different quantity types, creating a unit with new quantity defined
by the multiplication/division by the two units that were used in the operation.

- The conversion type is a struct/class which provides two templated static functions: unitToStandard<T>(T x) and standardToUnit<T>(T x).
The unit class inherits this as a helper to convert a unit type from a scaled unit to one that can be considered unscaled.
This allows users to define not only their own conversions, but also what is considered as a 'standard' unit.
If your code base considers a unit length of 1 to be one centimetrr, then the conversion to 1 metre can be a x*100.
Different units have different conversions, and these could be dynamic, like in the case of currency. So we leave the implementation 
of the conversion between units up to the user; but we define some standard and simple conversions anyways in conversions.h

Future:
 - it would be good to provide more elaborate examples and tests
 - Checking how much overhead it adds to computations would also be useful
