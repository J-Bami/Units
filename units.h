#ifndef UNITS_COMMON_UNITS_H
#define UNITS_COMMON_UNITS_H

#include "conversions.h"
#include "quantities.h"
#include "unit.h"

//#define DECLARE_STANDARD_UNIT(prefix, )

#define DECLARE_MAGNITUDE_UNIT(prefix, unitName, quantity)\
	template<typename FloatType> using prefix##unitName = Unit<FloatType, quantity, ::units::conversions::prefix>;

#define DECLARE_MACRO_UNITS(unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(deca, unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(hecta, unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(kilo, unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(mega, unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(giga, unitName, quantity)

#define DECLARE_MICRO_UNITS(unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(deci, unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(centi, unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(milli, unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(micro, unitName, quantity)\
DECLARE_MAGNITUDE_UNIT(nano, unitName, quantity)

#define DECLARE_MAGNITUDE_UNITS(unitName, quantity) DECLARE_MICRO_UNITS(unitName, quantity) DECLARE_MACRO_UNITS(unitName, quantity)

namespace units
{
	template<typename FloatType>
	using seconds = Unit<FloatType, quantities::Time>;
	DECLARE_MICRO_UNITS(seconds, quantities::Time)

	template<typename FloatType>
	using metres = Unit<FloatType, quantities::Length>;
	DECLARE_MICRO_UNITS(metres, quantities::Length)
	DECLARE_MAGNITUDE_UNIT(kilo, metres, quantities::Length)

	template<typename FloatType>
	using kilograms= Unit<FloatType, quantities::Mass>;

	template<typename FloatType>
	using amperes = Unit<FloatType, quantities::Current>;
	DECLARE_MAGNITUDE_UNITS(ampere, quantities::Current);


	template<typename FloatType>
	using newtons = Unit<FloatType, quantities::Force>;
	DECLARE_MAGNITUDE_UNITS(newtons, quantities::Force);
}

#endif