#include "theGame.hpp"

#include <time.h>
#include <csignal>

#include <algorithm>
#include <iterator>
#include <iosfwd>
#include <streambuf>
#include <fstream>
#include <wchar.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "Engine/Graphics/Camera3D.hpp"
#include "Engine/Math/raw_vector.hpp"
#include "Engine/Math/Vertex.hpp"
#include "Engine/MeshUtilities.hpp"
#include "Engine/Utilities/Time.hpp"
#include "Engine/Clock.hpp"
#include "Engine/Utilities/DevConsole.hpp"
#include "Engine/ProfileSection.hpp"
#include "Engine/JobManager.hpp"
#include "Engine/LoadFileJob.hpp"
#include "Engine/GenerateFilesJob.hpp"
#include "Engine/ReverseBufferJob.hpp"
#include "Engine/HashBufferJob.hpp"
#include "Engine/Assert.hpp"
#include "Engine/TestMemory.hpp"
#include "Engine/Utilities/XMLFontRenderer.hpp"
#include "Engine/NetSystem.hpp"
#include "Engine/NetSession.hpp"
#include "Engine/EventSystem.hpp"
#include "Engine/UISystem.hpp"

using namespace std;

struct TestS : public Object {
	void Test(NamedProperties& np) {
		DebuggerPrintf("%s", "TESTS CALLBACKHIT1\n");
	}
	void Test2(NamedProperties& np) {
		DebuggerPrintf("%s", "TESTS CALLBACKHIT2\n");
	}

	void Test3(NamedProperties& np) {
		DebuggerPrintf("%s", "TESTS CALLBACKHIT3\n");
	}

	void Test4(NamedProperties& np) {
		DebuggerPrintf("%s", "TESTS CALLBACKHIT4\n");
	}

	virtual ~TestS() {

	}
};

TheGame::TheGame(void)
	: m_shaderProgram(nullptr)
	, m_triangleMesh(nullptr)
	, m_normalsTexture(nullptr)
	, m_modelDiffuseTexture(nullptr),
	m_fontRenderer(new XMLFontRenderer()),
	m_showProfilingStats(true),
	m_jobManager(new JobManager()),
	m_testSize(0),
	m_testData(nullptr),
	m_testMemory(new TestMemory()),
	m_devConsole(DevConsole::GetInstance()),
	m_uiSystem(new UISystem())
{
	Clock::InitializeMasterClock();
	NetSystem::GetInstance();
	EventSystem::CreateInstance();

	m_camera.m_position = Vec3( 42.f, 42.f, 42.f );
	m_camera.m_orientation.pitchDegreesAboutY = 20.f;
	m_camera.m_orientation.yawDegreesAboutZ = 225.f;

	m_shaderProgram = new ShaderProgram("BasicUnlit");
	
	m_up = Vec3(0.f, 0.f, 1.f);
	m_gameClock = new Clock(Clock::s_masterClock);

	TestS* t = new TestS();
	RegisterEventSubscriber("UpdateEvent", *t, &TestS::Test);
	RegisterEventSubscriber("UpdateEvent2", *t, &TestS::Test2);
	RegisterEventSubscriber("ExitEvent1", *t, &TestS::Test3);
	RegisterEventSubscriber("RenderEvent1", *t, &TestS::Test4);
}


TheGame::~TheGame(void)
{

}


void TheGame::SetUpPerspectiveProjection()
{
	float aspect = (16.f / 9.f); // VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT;
	float fovX = 70.f;
	float fovY = (fovX / aspect);
	float zNear = 0.1f;
	float zFar = 1000.f;

	Renderer& myRenderer = Renderer::GetInstance();
	myRenderer.SetIdentity();
	myRenderer.SetPerspective(fovY, aspect, zNear, zFar );
}

void TheGame::ApplyCameraTransform( const Camera3D& camera )
{
	Renderer& myRenderer = Renderer::GetInstance();

	// Put us in our preferred coordinate system: +X is east (forward), +Y is north, +Z is up
	myRenderer.RotateMatrix( -90.f, 1.f, 0.f, 0.f ); // lean "forward" 90 degrees, to put +Z up (was +Y)
	myRenderer.RotateMatrix( 90.f, 0.f, 0.f, 1.f ); // spin "left" 90 degrees, to put +X forward (was +Y)

	// Orient the view per the camera's orientation
	myRenderer.RotateMatrix( -camera.m_orientation.rollDegreesAboutX, 1.f, 0.f, 0.f );
	myRenderer.RotateMatrix( -camera.m_orientation.pitchDegreesAboutY, 0.f, 1.f, 0.f );
	myRenderer.RotateMatrix( -camera.m_orientation.yawDegreesAboutZ, 0.f, 0.f, 1.f );

	// Position the view per the camera's position
	myRenderer.TranslateMatrix(-camera.m_position.x(), -camera.m_position.y(), -camera.m_position.z());
}


void TestError(void* data) {
	(void)data;
	int x = 3;
	RECOVERABLE_ASSERT(x == 2);
}


void MyFunc(NamedProperties& np) {
	DebuggerPrintf("Callback Function 1!!!\n");
}

void DoTests() {
	struct TestS  {
	public:

		void Test(NamedProperties& np) {
			DebuggerPrintf("%s", "TESTS CALLBACKHIT1\n");
		}
		void Test2(NamedProperties& np) {
			DebuggerPrintf("%s", "TESTS CALLBACKHIT2\n");
		}

		TestS::TestS() {

		}
	};

	TestS t;
	TestS t2;

	// 	RegisterEventCallback("Sunrise", Funky);
	// 	FireEvent("Sunrise");
	// 	RegisterEventCallback("Sunrise", MyFunc);
	// 	FireEvent("Sunrise");
	// 	UnregisterEventCallback("Sunrise", MyFunc);

	RegisterEventSubscriber("Sunrise", t, &TestS::Test);
	RegisterEventSubscriber("Sunrise", t, &TestS::Test2);
	RegisterEventSubscriber("Sunrise", t2, &TestS::Test2);
	//UnregisterObjMethodForEvent("Sunrise", t, &TestS::Test);
	UnregisterEventForObject("Sunrise", t);

}

//Call Update on each type of object that needs to be updated
void TheGame::Update(double deltaTimeSeconds) {

	for (int index = 0; index < 256; index++) {
		m_keyStates[index].m_hasKeyJustBeenReleased = false;
	}

	//DoTests();

	//UpdateCameraFromMouseAndKeyboard( m_camera, deltaTimeSeconds );
	Clock::s_masterClock->AdvanceTime((float)deltaTimeSeconds);
	NetSystem* networkSystem = NetSystem::GetInstance();
	networkSystem->Tick();
	m_uiSystem->Update(deltaTimeSeconds);
}

inline bool FileExists (const std::string& name) {
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}   
}


void TheGame::SwitchToFixedFunctionPipeline()
{

}


void TheGame::DrawDebugQuad()
{
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	float m_radius = 100.f;
	glDisable(GL_TEXTURE_2D);
	glPointSize( 5.f );
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.f,0.f);
		glVertex3f(-m_radius,m_radius, 0.0f);              // Top Left

		glTexCoord2f(0.f,1.f);
		glVertex3f(-m_radius,-m_radius, 0.0f);              // Bottom Left

		glTexCoord2f(1.f,1.f);
		glVertex3f( m_radius,-m_radius, 0.0f);              // Bottom Right

		glTexCoord2f(1.f,0.f);
		glVertex3f( m_radius,m_radius, 0.0f);              // Top Right
	}
	glEnd();
}

void TheGame::DrawAxes() {

	std::vector<Vertex> lineVertices;
	float line_length = 1000.f;

	Vertex currentVertex;
	currentVertex.m_color = RGBA(255, 0, 0, 255);
	currentVertex.m_position = Vec3(0.f, 0.f, 0.f);
	lineVertices.push_back(currentVertex);

	currentVertex.m_position = Vec3(line_length, 0.f, 0.f);
	lineVertices.push_back(currentVertex);

	currentVertex.m_position = Vec3(0.f, 0.f, 0.f);
	currentVertex.m_color = RGBA(0, 255, 0, 255);
	lineVertices.push_back(currentVertex);

	currentVertex.m_position = Vec3(0.f, line_length, 0.f);
	lineVertices.push_back(currentVertex);

	currentVertex.m_color = RGBA(0, 0, 255, 255);
	currentVertex.m_position = Vec3(0.f, 0.f, 0.f);
	lineVertices.push_back(currentVertex);

	currentVertex.m_position = Vec3(0.f, 0.f, line_length);
	lineVertices.push_back(currentVertex);

	glLineWidth(1.f);
	glDisable(GL_DEPTH_TEST);
	Renderer::GetInstance().RenderPrimitives(GL_LINES, lineVertices);

	glLineWidth(4.f);
	glEnable(GL_DEPTH_TEST);
	Renderer::GetInstance().RenderPrimitives(GL_LINES, lineVertices);

}

void TheGame::UpdateCameraFromMouseAndKeyboard( Camera3D& camera, double deltaSeconds )
{
	// Update camera orientation (yaw and pitch only) from mouse x,y movement
	const float degreesPerMouseDelta = 0.03f;
	Vec2 mouseDeltas = GetMouseMovementSinceLastChecked();

	camera.m_orientation.yawDegreesAboutZ -= (degreesPerMouseDelta * mouseDeltas.x());
	camera.m_orientation.pitchDegreesAboutY += (degreesPerMouseDelta * mouseDeltas.y());

	camera.m_orientation.pitchDegreesAboutY = max(min(camera.m_orientation.pitchDegreesAboutY,89.f),-89.f);

	// Update camera position based on which (if any) movement keys are down
	float cameraYawRadians = DegreesToRadians(camera.m_orientation.yawDegreesAboutZ);
	Vec3 cameraForwardXYVector( cos( cameraYawRadians ), sin( cameraYawRadians ), 0.f );

	Vec3 movementVector( 0.f, 0.f, 0.f );

	if(m_keyStates[ 'W' ].m_isPressed ) {
		movementVector += cameraForwardXYVector;
	}

	if (m_keyStates['S'].m_isPressed) {
		movementVector -= cameraForwardXYVector;
	}

	if (m_keyStates['D'].m_isPressed) {
		Vec3 cameraLeftXYVector = cameraForwardXYVector.Cross(m_up);
		movementVector += cameraLeftXYVector;
	}

	if (m_keyStates['A'].m_isPressed) {
		Vec3 cameraRightXYVector = m_up.Cross(cameraForwardXYVector);
		movementVector += cameraRightXYVector;
	}

	if (m_keyStates[VK_SPACE].m_isPressed) {
		movementVector += m_up;
	}

	if (m_keyStates['Z'].m_isPressed) {
		movementVector -= m_up;
	}


	if (m_keyStates[VK_SHIFT].m_isPressed) {
		camera.m_position += (movementVector * ((float)(deltaSeconds))) * 8.f * kMovementSpeed;
	}
	else {
		camera.m_position += (movementVector * ((float)(deltaSeconds))) * kMovementSpeed;
	}
}

void Funky(NamedProperties& np)
{
	DebuggerPrintf("Callback Function 2!!!\n");
}

void TheGame::ParseArgumentList(const std::string& args)
{
	CommandList commands = ParseCommands(args);
	ProcessCommands(commands);
	FreeCommands(commands);
}

 void OnGenerateFiles(const Command* command)
{
	int arg0;
	int arg1;
	if (!command->GetInt(0, &arg0) || !command->GetInt(1, &arg1)) {
		return;
	}

	//GenerateFiles(arg0, arg1, "Data/RandomFiles");

	MessageBoxA(NULL, "OnWriteFiles Succeeded.", "", MB_OK);
}

Vec2 TheGame::GetMouseMovementSinceLastChecked()
{

	HWND hWnd = ::GetForegroundWindow();
	DWORD dwProcessIdFocus;
	// Use Windows API GetWindowThreadProcessId to find the window's process ID.
	GetWindowThreadProcessId(hWnd, &dwProcessIdFocus);

	// Use Windows API GetCurrentProcessId to find 'this' process ID.
	DWORD dwProcessIdParent = GetCurrentProcessId();

	bool thisAppHasFocus = dwProcessIdFocus == dwProcessIdParent;

	Vec2i centerCursorPos = Vec2i((int)(SCREEN_WIDTH / 2.f), (int)(SCREEN_HEIGHT / 2.f));
	Vec2i cursorPos;

	if (thisAppHasFocus) {
		myWinWrapper.MyGetCursorPos(cursorPos);
		myWinWrapper.MySetCursorPos(centerCursorPos.x(), centerCursorPos.y());
	}
	else {
		return Vec2(0.f, 0.f);
	}

	Vec2i mouseDeltaInts(cursorPos.x() - centerCursorPos.x(), cursorPos.y() - centerCursorPos.y());
	Vec2 mouseDeltas((float)mouseDeltaInts.x(), (float)mouseDeltaInts.y());
	return mouseDeltas;
}

void TheGame::DrawTexturedDebugQuadModernGL() {
	std::vector<Vertex3D_PCT> vertPositions;

	Vertex3D_PCT vert;
	vert.m_color = RGBA(0, 255, 0, 255);

	vert.m_position = Vec3(0.f, 0.f, 0.f);
	vert.m_texCoords = Vec2(0.f, 0.f);
	vertPositions.push_back(vert);

	vert.m_position = Vec3(500.f, 0.f, 0.f);
	vert.m_texCoords = Vec2(1.f, 0.f);
	vertPositions.push_back(vert);

	vert.m_position = Vec3(500.f, 500.f, 0.f);
	vert.m_texCoords = Vec2(1.f, 1.f);
	vertPositions.push_back(vert);

	vert.m_position = Vec3(0.f, 500.f, 0.f);
	vert.m_texCoords = Vec2(0.f, 1.f);
	vertPositions.push_back(vert);

	//Renderer::GetInstance().RenderTexturedQuadList(vertPositions, "Data/Fonts/Calibri_0.png");
}

void TestMsgBox(void* data)
{
	(void)data;
	MessageBoxA(NULL, "Test Message Succeeded.", "", MB_OK);
}



void ShowHash(void* data)
{
	size_t* msg = (size_t*)data;

	std::stringstream hashStream;
	hashStream << *msg << " IS THE HASH" << std::endl;
	std::string hash = hashStream.str();
	MessageBoxA(NULL, hash.c_str(), "", MB_OK);
	delete data;
}

//Call Render on each type of object that needs to be rendered
void TheGame::Render()
{

	//std::vector<std::pair<RGBA, int>> hiColors;
	//hiColors.push_back(std::make_pair(RGBA(255, 255, 255, 255), 1));
	//m_devConsole->ConsolePrint("HIIIIIIII!", hiColors);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetUpPerspectiveProjection();
	ApplyCameraTransform(m_camera);

	//DrawTexturedDebugQuadModernGL();
	DrawAxes();

	Renderer& myRenderer = Renderer::GetInstance();
	myRenderer.SetOrtho(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT, 0, 1);

// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	//m_fontRenderer->DrawString("Hi", RGBA(0, 255, 0, 255), Vec3(100.f, 100.f, 0.f), 50.f);

	m_devConsole->Render();
	m_uiSystem->Render();
	//DrawTexturedDebugQuadModernGL();
}

void TheGame::KeyPressEvent(unsigned char theKey) {
	m_keyStates[theKey].m_isPressed = true;
	m_keyStates[theKey].m_hasKeyJustBeenReleased = false;

	if (theKey == 96) {
		m_devConsole->ToggleVisibility();
	}

	if (m_devConsole->m_enabled) {

		if (isalnum(theKey) || ispunct(theKey) || (theKey == 190))
			m_devConsole->AppendChar(theKey);

		if (theKey == VK_RETURN) {
			m_devConsole->SubmitCommand();
			m_devConsole->ClearCommand();
		}

		else if (theKey == VK_ESCAPE) {
			m_devConsole->ClearCommand();
		}

		//TODO: HANDLE OTHER CASES
		if (theKey == VK_BACK) {
			m_devConsole->PopCharFromCommand();
		}
		else { //Only Append Printable Characters
			if (theKey == VK_SPACE)
				m_devConsole->AppendChar(theKey);
		}
	}

	m_uiSystem->OnKeyboardEvent(theKey);
}

void TheGame::StartHosting(ConsoleCommandArgs &arg)
{
	short port = (short)atoi(arg.m_argsList[1].c_str());
	StartHosting(port);
}

void TheGame::Init()
{
	//DevConsoleRegister("host", "host <port>", StartHosting);
}



void TheGame::StartHosting(short port)
{
	NetSystem* system = NetSystem::GetInstance();
	NetSession* session = system->CreateSession(1337);

	if (session->Host(port)) {
		m_gameSession = session;
		//m_devConsole->ConsolePrintf("Game Hosted: %s", m_gameSession->GetHostAddrName());
		m_gameSession->Listen(true);
	}
	else {
		system->DestroySession(m_gameSession);
		//m_devConsole->ConsolePrintf("Failed to host.");
	}
}


void TheGame::SetKeyNotReleased(unsigned char theKey) {
	m_keyStates[theKey].m_hasKeyJustBeenReleased = false;
}

void TheGame::KeyReleaseEvent(unsigned char theKey) {
	if (m_keyStates[theKey].m_isPressed)
		m_keyStates[theKey].m_hasKeyJustBeenReleased = true;

	m_keyStates[theKey].m_isPressed = false;

	if (theKey == 'T') {
		m_showProfilingStats = !m_showProfilingStats;
	}

	if (theKey == 'P') {
		m_gameClock->TogglePause();
	}

	if (theKey == '5') {
		m_jobManager->PostJob(new GenerateFilesJob(100, 1000, "Data/RandomFiles"), TestMsgBox, nullptr);
			//m_jobManager->EnqueueJob(new LoadFileJob("Data/testRead.txt", m_testData, m_testSize), TestError, nullptr);
	}

	if (theKey == '6') {
		unsigned char* buffer;
		m_jobManager->PostJob(new LoadFileJob("Data/RandomFiles/RandomFile0.dat", buffer), TestMsgBox, nullptr);
		//m_jobManager->EnqueueJob(new LoadFileJob("Data/testRead.txt", m_testData, m_testSize), TestError, nullptr);
	}

	if (theKey == '7') {
		std::vector<unsigned char> buffer;
		LoadFileToVector("Data/RandomFiles/ReverseFile.txt", buffer);
		unsigned char* data = buffer.data();
		m_jobManager->PostJob(new ReverseBufferJob(data, buffer.size()), TestMsgBox, nullptr);
		//m_jobManager->E8nqueueJob(new LoadFileJob("Data/testRead.txt", m_testData, m_testSize), TestError, nullptr);
	}

	if (theKey == '8') {
		std::vector<unsigned char> buffer;
		LoadFileToVector("Data/RandomFiles/ReverseFile.txt", buffer);
		size_t* hash = new size_t;
		*hash = 0;
		m_jobManager->PostJob(new HashBufferJob(buffer, hash), ShowHash, hash);

	}
	if (theKey == '4') {
		//RECOVERABLE_ERROR("You have a headache.");
	}
}

void TheGame::OnMouseEvent(MouseEvent me)
{
	m_uiSystem->OnMouseEvent(me);
}
