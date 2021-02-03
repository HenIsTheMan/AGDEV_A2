---[[

function CalcTextChangingScale(elapsedTime)
	return CalcTextChangingScaleHost(math.cos(elapsedTime * 4.0) * 0.5 + 0.5, 1.5, 0.9);
end

--]]