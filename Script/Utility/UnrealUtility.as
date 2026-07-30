mixin bool ASIsValid(const UObject Object)
{
	return IsValid(Object);
}

mixin bool ASIsNull(const UObject Object)
{
	return !IsValid(Object);
}
