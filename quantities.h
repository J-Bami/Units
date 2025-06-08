#ifndef UNITS_QUANTITIES_H
#define UNITS_QUANTITIES_H

#include "quantity.h"
#include "dimensions.h"

namespace units
{
    namespace quantities
    {
		//define some standard quantities

        using Dimensionless = Quantity<>;

        using Time = Quantity<dimensions::Time>;
        using Length = Quantity<dimensions::Length>;
        using Mass = Quantity<dimensions::Mass>;
        using Current = Quantity<dimensions::Current>;
        using Temperature = Quantity<dimensions::Temperature>;
        using Luminosity = Quantity<dimensions::Luminosity>;
        using Amount = Quantity<dimensions::Amount>;

        using Currency = Quantity<dimensions::Currency>;
        using Dimensionless = Quantity<>;

		using Angle = Quantity<dimensions::Angle>; //Dimensionless; //we can change this to be its own distinct quantity if we want
        
		using Frequency = DivideType<Dimensionless, Time>;
		using Area = MultiplyType<Length, Length>;
		using Volume = MultiplyType<Area, Length>;
		
		//kinematics
		using Height = Length;
		using Width = Length;
		using Depth = Length;
		using Distance = Length; //scalar
		using Displacement = Length; //vector

		using Velocity = DivideType<Length, Time>; //v = s/t
		using Speed = Velocity;
		using Acceleration = DivideType<Velocity, Time>; //a = dv/dt
		using Jerk = DivideType<Acceleration, Time>; //j = da/dt

		using AngularVelocity = DivideType<Angle, Time>;
		using AngularAcceleration = DivideType<AngularVelocity, Time>;

		//kinetics
		using Force = MultiplyType<Mass, Acceleration>;  //F = ma
		using Momentum = MultiplyType<Mass, Velocity>;
		using Impulse = Momentum;
		using MomentOfInertia = MultiplyType<Area, Mass>;
		using AngularMomentum = MultiplyType<MomentOfInertia, AngularVelocity>;
		using Energy = MultiplyType<Force, Length>;
		using Moment = DivideType<Energy, Angle>; 
		using Torque = Moment;
		using Power = DivideType<Energy, Time>;
		using Intensity = DivideType<Power, Area>;
		using AreaDensity = DivideType<Mass, Area>;

		//Mechanics & Fluids
		using Density = DivideType<Mass, Volume>;
		using SpecificVolume = DivideType<Volume, Mass>;
		using Pressure = DivideType<Energy, Volume>;
		using Stress = Pressure;
		using Strain = Dimensionless;
		using Stiffness = DivideType<Force, Distance>;
		using DynamicViscosity = MultiplyType<Pressure, Time>;
		using KinematicViscosity = DivideType<DynamicViscosity, Density>;

		//flow rate
		using MassFlowRate = DivideType<Mass, Time>;
		using VolumetricFlowRate = DivideType<Volume, Time>;

		//Thermodynamics
		using Entropy = DivideType<Energy, Temperature>; //low entropy => useful energy...
		using SpecificHeat = DivideType<Energy, MultiplyType<Mass, Temperature>>;
		using SpecificEnergy = DivideType<Energy, Mass>;
		using HeatFluxDensity = DivideType<Power, Area>;

		//Electrics
		using Charge = MultiplyType<Current, Time>;
		using ChargeDensity = DivideType<Charge, Volume>;
		using ElectricPotential = DivideType<Energy, Charge>;
		using Voltage = ElectricPotential;
		using ElectricField = DivideType<Voltage, Length>;
		using Resistance = DivideType<Voltage, Current>;
		using ElectricalResistivity = MultiplyType<Resistance, Length>;
		using Impedance = Resistance; //complex
		using ElectricalConductance = DivideType<Dimensionless, Resistance>;
		using MagneticFluxDensity = DivideType<Force, MultiplyType<Velocity, Charge>>;
		using MagneticFlux = MultiplyType<MagneticFluxDensity, Area>;
		using Capacitance = DivideType<Charge, Voltage>;
		using CurrentDensity = DivideType<Current, Area>;

		//chemistry
		using ChemicalPotential = DivideType<Energy, Amount>;
		using Concentration = DivideType<Amount, Volume>;
    }
}


#endif