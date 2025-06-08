#include "conversions.h"
#include "quantities.h"
#include "quantity.h"
#include "unit.h"
#include "units.h"
#include "util.h"

#include <iostream>

struct MyStruct
{

};

struct Time {};
struct Length {};
struct Mass {};


CREATE_RATIO_CONVERSION(MilliConversion, 0.001)
CREATE_RATIO_CONVERSION(KiloConversion, 1000)

#define PRINT_EXPR(...) std::cout << "Expression '" #__VA_ARGS__ "': " << (__VA_ARGS__) << '\n';
#define PRINT_TYPE_NAME(obj) std::cout << "TYPE OF (" #obj "): " << typeid(decltype(obj)).name() << '\n';



int main()
{
    using TimeD = units::Dimension<Time, 1>;
    using MassD = units::Dimension<Mass, 1>;
    using LengthD = units::Dimension<Length, 1>;

    units::Quantity<MassD, units::Dimension<Length, 2>, units::Dimension<Time, -2>> myQuantity;
    myQuantity;

    std::cout << units::b_has_multiply<int, int> << '\n';
    std::cout << units::b_has_multiply<int, MyStruct> << '\n';

    units::quantities::QuantityType<
        LengthD,
        MassD,
        TimeD,
        LengthD,
        MassD,
        TimeD/**/> q;
    std::cout << typeid(q).name() << '\n';

    using QLLTT = units::Quantity< units::Dimension<Length, 2>, units::Dimension<Time, 2>>;
    using QTTLL = units::Quantity<units::Dimension<Time, 2>, units::Dimension<Length, 2>>;
    using QLL = units::Quantity<units::Dimension<Length, 1>, units::Dimension<Length, 1>>;
    using QL = units::Quantity<units::Dimension<Length, 1>>;


    PRINT_EXPR(units::b_contains_dimension<LengthD, QL>);
    PRINT_EXPR(units::b_contains_dimension<TimeD, QL>);
    PRINT_EXPR(units::b_contains_dimension<MassD, QTTLL>);
    PRINT_EXPR(units::b_contains_dimension<TimeD, QTTLL>);
    //PRINT_EXPR(units::b_are_same_quantity<QL, QL>);
    PRINT_EXPR(units::b_is_same<QTTLL, QLLTT>);
    PRINT_EXPR(units::b_is_same<QTTLL, QLL>);

    QTTLL q1, q2;
    QLL qll;

    using Q3 = decltype(q1 + q2);
    //using Q4 = decltype(qll + q2); //should NOT compile
    
    using Q5 = decltype(q1 * q2);
    using Q6 = decltype(q1 / q2);
    PRINT_TYPE_NAME(q1 * q2);
    PRINT_TYPE_NAME(q1 / q2);
    units::quantities::AngularVelocity w;
    PRINT_TYPE_NAME(w);

    units::Unit<float, units::quantities::Velocity> mPerSecond( 4.f );
    units::Unit<double, units::quantities::Distance> metres(1);
    units::centimetres<double> height(168);
    units::centimetres<float> hatHeight(15);
    auto totalHeight = height + hatHeight;
    PRINT_TYPE_NAME(totalHeight);
    PRINT_EXPR(totalHeight.value());

    auto sth = metres + metres;

    units::b_is_unit<decltype(metres)>;
    units::b_is_unit<int>;
    
    units::kilograms<float> mymass(71);
    
    PRINT_TYPE_NAME(mymass * 3.f);
    PRINT_EXPR((mymass * 3.).value());

    //auto notSthg = metres + mPerSecond;
     auto time = metres / mPerSecond;
    //PRINT_EXPR(time.value());
    //PRINT_TYPE_NAME(time);

     units::kilonewtons<double> someForce;

    return 0;
}