#ifndef UNITS_DIMENSIONS_H
#define UNITS_DIMENSIONS_H

#include "dimension.h"
#include "tags.h"

namespace units
{
    namespace dimensions
    {
        //define some useful, standard dimensions

        using Length = Dimension<tags::Length, 1>;
        using Time = Dimension<tags::Time, 1>;
        using Mass = Dimension<tags::Mass, 1>;
        using Temperature = Dimension<tags::Temperature, 1>;
        using Current = Dimension<tags::Current, 1>;
        using Amount = Dimension<tags::Amount, 1>;
        using Luminosity = Dimension<tags::Luminosity, 1>;

        using Currency = Dimension<tags::Currency, 1>;
        using Angle = Dimension<tags::Angle, 1>;
    }
}


#endif 