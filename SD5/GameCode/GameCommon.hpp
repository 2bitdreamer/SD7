#pragma once
#define _USE_MATH_DEFINES
struct BlockInfo;
#include <math.h>
#include <string>
#include <map>
#include "Engine/Math/raw_vector.hpp"
#include <vector>
#include "Engine/Utilities/EngineCommon.hpp"
#include <stdlib.h>

const unsigned char SKY_LIGHT = 4;
const unsigned char FIRE_LIGHT = 9;
const float kMovementSpeed = 12.f;
const int numBlocksInChunk = 16*16*128;
const float g_oneOverNumBlocks = 1.f/32.f;
typedef Vec3 WorldCoords;
extern std::vector<Vec3> g_debugPoints;
extern bool g_flatTerrain;
extern std::map<BlockInfo, double> g_liveFire;
const double g_fireTTL = 7.f;

const int xbits = 0xf;
const int ybits = 0xf0;


#ifndef _MATH_FUNCS
#define _MATH_FUNCS


const std::string Stringf( const char* format, ... );
const std::string Stringf( const int maxLength, const char* format, ... );
void DebuggerPrintf( const char* messageFormat, ... );

#endif

