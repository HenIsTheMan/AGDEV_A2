---[[

function CalcTextChangingScale(elapsedTime)
	return CalcTextChangingScaleHost(math.cos(elapsedTime * 4.0) * 0.5 + 0.5);
end

--]]