#pragma once
#ifndef _GAME_INCLUDE_
#define _GAME_INCLUDE_

#include <vector>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iosfwd>
#include "Engine\Graphics\Camera3D.hpp"
#include "Engine\Math\raw_vector.hpp"
#include "Engine/Graphics/ShaderProgram.hpp"
#include "Engine/TriangleMesh.hpp"
#include "GameCommon.hpp"
#include "Windows.h"
#include <unordered_map>
#include "Engine\Command.hpp"
#include "Engine\Utilities\DevConsole.hpp"
#include "Engine\WidgetBase.hpp"

class BufferReader;
class Player;
class PlaySet;
class XMLFontRenderer;
class Clock;
class JobManager;
class TestMemory;
class DevConsole;
class NetSession;
class UISystem;

class TheGame
{
public:
	TheGame(void);
	~TheGame(void);
	void Update(double deltaTimeSeconds);
	void Render();

	void KeyPressEvent(unsigned char theKey);
	void SetKeyNotReleased(unsigned char theKey);
	void KeyReleaseEvent(unsigned char theKey);


	void OnMouseEvent(MouseEvent me);

	void DrawDebugQuad();
	void SwitchToFixedFunctionPipeline();
	void SetUpPerspectiveProjection();
	void ApplyCameraTransform( const Camera3D& camera );
	void DrawAxes();
	Vec2 GetMouseMovementSinceLastChecked();
	void DrawTexturedDebugQuadModernGL();
	void UpdateCameraFromMouseAndKeyboard(Camera3D& camera, double deltaSeconds);

	static TheGame& GetInstance(){
		static TheGame instance;
		return instance;
	}
	void ParseArgumentList(const std::string& args);
	
public:
	Camera3D m_camera;
	ShaderProgram* m_shaderProgram;
	std::string m_currentModel;
	TriangleMesh* m_triangleMesh;
	Texture* m_modelDiffuseTexture;
	Texture* m_normalsTexture;
	std::vector<Vertex> m_verticesToRender;
	Vec3 m_up;

	Player* m_playerOne;
	Player* m_playerTwo;

	XMLFontRenderer* m_fontRenderer;
	bool m_showProfilingStats;

	Clock* m_gameClock;
	JobManager* m_jobManager;

	unsigned char* m_testData;

	KeyState m_keyStates[256];

	size_t m_testSize;
	TestMemory* m_testMemory;
	DevConsole* m_devConsole;

	void StartHosting(short port);
	
	void StartHosting(ConsoleCommandArgs &arg);
	void Init();

	NetSession* m_gameSession;
	UISystem* m_uiSystem;
};

inline void StartHosting(ConsoleCommandArgs &arg);
inline bool StringEndsWith(const std::string& value, const std::string& ending);
inline bool DoesFileNeedCooking(const std::string& fileName);
bool StringReplace(std::string& str, const std::string& from, const std::string& to);
void StringReplaceAll(std::string& str, const std::string& from, const std::string& to);
void TestError(void* data);
void MyFunc(NamedProperties& np);

void Funky(NamedProperties& np);
void OnGenerateFiles(const Command* command);
void TestMsgBox(void* data);
void ShowHash(void* data);
void StartHosting(ConsoleCommandArgs &arg);
#endif
