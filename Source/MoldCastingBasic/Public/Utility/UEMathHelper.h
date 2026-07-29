#pragma once

#include "CoreMinimal.h"

class MOLDCASTINGBASIC_API UEMathHelper
{
private:
	UEMathHelper() {}

public:
	template<typename ValueType>
	static ValueType GetSign(ValueType Value)
	{
		return Value >= 0 ? static_cast<ValueType>(1) : static_cast<ValueType>(-1);
	}

	template<typename ValueType>
	static ValueType GetSign(ValueType LHS, ValueType RHS)
	{
		return LHS * RHS >= 0 ? static_cast<ValueType>(1) : static_cast<ValueType>(-1);
	}

	static FRotator RotationFromX_Safe(const FVector& Direction);

	static float GetLengthXY(const FVector& Vector);
};
