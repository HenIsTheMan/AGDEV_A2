#pragma once

bool LMB = false;
bool RMB = false;
bool endLoop = false;
bool firstCall = 1;
float dt = 0.f;
float leftRightMB = 0.f;
float pitch = 0.f;
float yaw = 0.f;
float windowMouseX = 0.f;
float windowMouseY = 0.f;
float SENS = .05f;
float angularFOV = 45.f;
int optimalWinXPos;
int optimalWinYPos;
int optimalWinWidth;
int optimalWinHeight;
int windowWidth;
int windowHeight;