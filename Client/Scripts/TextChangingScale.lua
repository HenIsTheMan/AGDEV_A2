---[[

function CalcTextChangingScale(elapsedTime)
	local spd = 4.0
	local mag = 1.2
	local startScale = 1.5
	local endScale = 0.9

	return CalcTextChangingScaleHost((math.cos(elapsedTime * spd) * 0.5 + 0.5) * mag, startScale, endScale);
end

--]]