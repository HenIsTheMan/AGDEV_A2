#pragma once
#include "GlobalStructs.h"

bool Key(const char& key);

void SetUpTex(const SetUpTexsParams& params, uint& texRefID);
void SetUpCubemap(uint& cubemapRefID, const std::vector<cstr>& faces);