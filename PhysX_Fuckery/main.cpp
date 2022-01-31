#define GLM_FORCE_CTOR_INIT 
#define STB_IMAGE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS
//#define PX_PHYSX_STATIC_LIB
#include <PxConfig.h>

#include <PxPhysicsAPI.h>
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <chrono>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>
#include <glm/glm.hpp>

void InitWindow(GLFWwindow*& window, const std::string& title)
{
	if (!glfwInit())
	{
		std::cerr << "GLFW failed to initialize!" << std::endl;
		exit(-1);
	}

	window = glfwCreateWindow(800, 600, title.c_str(), NULL, NULL);




	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwMakeContextCurrent(window);

	//if (glewInit() != GLEW_OK)
	//{
	//	std::cerr << "GLEW failed to initialize!" << std::endl;
	//	exit(-1);
	//}

	//glfwSwapInterval(0);

	(glEnable(GL_DEPTH_TEST));
	(glEnable(GL_BLEND));
	//GLCall(glEnable(GL_TEXTURE_2D));

	(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	(glClearColor(0.270, 0.670, 0.929, 1.0));
	//GLCall(glClearColor(1.0, 0.7, 0.4, 1.0));

	//set up input callbacks


	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mgInput::MouseCallback);
	//glfwSetScrollCallback(window, scroll_callback);
	//glfwSetKeyCallback(window, mgInput::KeyCallback);
}


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};
using namespace physx;
PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxCooking* gCooking = nullptr;
PxCookingParams* gCookingParams = nullptr;
PxPhysics* gPhysics = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;

PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxReal stackZ = 10.0f;

std::vector<Vertex> Heightmap;
std::vector<unsigned int> Heightmap_Indices;
int HMImgWidth, HMImgHeight;




std::vector<Vertex> LoadHeightMap(const std::string& path)
{
	std::vector<Vertex> heightmap;
	int width, height, nrChannels, hm_height = 1, hm_width = 1;
	stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(path.c_str(), &HMImgWidth, &HMImgHeight, &nrChannels, 0);
	if (data) {
		/*GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;*/

		//heightmap.resize(HMImgHeight);


		for (int i = 0; i < HMImgHeight; i++)
		{
			for (int j = 0; j < HMImgWidth; j++)
			{
				unsigned char* pixelOffset = data + (i + HMImgHeight * j) * nrChannels;
				unsigned char r = pixelOffset[0];

				heightmap.push_back({
					{((float)hm_height / HMImgHeight) * i, (float)r, ((float)hm_width / HMImgWidth) * j},
					{0.0f, 1.0f, 0.0f},
					glm::vec2((float)j / HMImgWidth , (float)(HMImgHeight - i) / HMImgHeight) * 25.0f
					});

			}
		}

		std::vector< std::vector<std::pair<glm::vec3, glm::vec3>> > tempNormals;
		tempNormals.resize(HMImgHeight);
		for (unsigned int i = 0; i < HMImgHeight; i++)
			tempNormals[i].resize(HMImgWidth);

		for (int i = 0; i < HMImgHeight - 1; i++)
		{
			for (int j = 0; j < HMImgWidth - 1; j++)
			{

				const auto& vertexA = heightmap[i * HMImgHeight + j].position;
				const auto& vertexB = heightmap[i * HMImgHeight + j + 1].position;
				const auto& vertexC = heightmap[(i + 1) * HMImgHeight + j + 1].position;
				const auto& vertexD = heightmap[(i + 1) * HMImgHeight + j].position;

				const auto triangleNormalA = glm::cross(vertexB - vertexA, vertexA - vertexD);
				const auto triangleNormalB = glm::cross(vertexD - vertexC, vertexC - vertexB);

				tempNormals[i][j] = std::make_pair(glm::normalize(triangleNormalA), glm::normalize(triangleNormalB));


				/*unsigned char* pixelOffset = data + (i + HMImgHeight * j) * nrChannels;
				unsigned char r = pixelOffset[0];

				float L = (j - 1 > 0) ? heightmap[i * HMImgHeight + j - 1].position.y : 0;
				float R = (j + 1 < HMImgWidth) ? heightmap[i * HMImgHeight + j + 1].position.y : 0;
				float T = (i - 1 > 0) ? heightmap[(i - 1) * HMImgHeight + j - 1].position.y : 0;
				float B = (i + 1 > HMImgWidth) ? heightmap[(i + 1) * HMImgHeight + j - 1].position.y : 0;

				heightmap[i * HMImgHeight + j].normal = -glm::normalize(glm::vec3((L - R) / 2, -1, (B - T) / 2));*/

				//std::cout << " " << heightmap[i * HMImgHeight + j].normal.x << " " << heightmap[i * HMImgHeight + j].normal.y << " " << heightmap[i * HMImgHeight + j].normal.z << std::endl;
			}
		}


		for (auto i = 0; i < HMImgHeight; i++)
		{
			for (auto j = 0; j < HMImgWidth; j++)
			{
				const auto isFirstRow = i == 0;
				const auto isFirstColumn = j == 0;
				const auto isLastRow = i == HMImgHeight - 1;
				const auto isLastColumn = j == HMImgWidth - 1;

				auto finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

				// Look for triangle to the upper-left
				if (!isFirstRow && !isFirstColumn)
				{
					finalVertexNormal += tempNormals[i - 1][j - 1].first;
				}

				// Look for triangles to the upper-right
				if (!isFirstRow && !isLastColumn)
				{

					finalVertexNormal += tempNormals[i - 1][j].first + tempNormals[i - 1][j].second;
				}

				// Look for triangle to the bottom-right
				if (!isLastRow && !isLastColumn)
				{
					finalVertexNormal += tempNormals[i][j].first;
				}

				// Look for triangles to the bottom-right
				if (!isLastRow && !isFirstColumn)
				{
					finalVertexNormal += tempNormals[i][j - 1].first;
					finalVertexNormal += tempNormals[i][j - 1].second;
				}

				// Store final normal of j-th vertex in i-th row
				std::swap(finalVertexNormal.y, finalVertexNormal.z);
				//std::swap(finalVertexNormal.y, finalVertexNormal.x);
				finalVertexNormal.x = -finalVertexNormal.x;
				//finalVertexNormal.y = -finalVertexNormal.y;
				//finalVertexNormal.z = -finalVertexNormal.z;
				heightmap[i * HMImgHeight + j].normal = glm::normalize(finalVertexNormal);

			}
		}


		//std::cout << std::endl;


	}
	else {
		std::cout << "Failed to load texture: " << path << std::endl;
	}
	stbi_image_free(data);
	return heightmap;
}

PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}

void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	//PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	PxShape* shape = gPhysics->createShape(PxSphereGeometry(halfExtent), *gMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), i) * halfExtent);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 3.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}

//#define FUCKERY
int main() {
#ifndef FUCKERY
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);


	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	PxTolerancesScale ts;
	ts.length = 11;
	ts.speed = 11;
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, ts, true, gPvd);
	gCookingParams = new PxCookingParams(ts);
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, *gCookingParams);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(8);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);


	auto heightmap = LoadHeightMap("../assets/heightmaps/submarine_128x128.png");
	PxI16* heightData = (PxI16*)malloc(sizeof(PxI16) * HMImgWidth * HMImgHeight);
	memset(heightData, NULL, HMImgWidth * HMImgHeight);
	for (unsigned int i = 0; i < heightmap.size(); i++) {
		heightData[i] = (PxI16)heightmap[i].position.y;
	}

	PxHeightFieldSample* gHFSamples = (PxHeightFieldSample*)malloc(sizeof(PxHeightFieldSample) * HMImgWidth * HMImgHeight);
	for (unsigned int i = 0; i < heightmap.size(); i++) {
		gHFSamples[i].height = (PxI16)heightmap[i].position.y;
	}


	PxHeightFieldDesc gHFDesc;
	gHFDesc.format = PxHeightFieldFormat::eS16_TM;
	gHFDesc.nbColumns = HMImgWidth;
	gHFDesc.nbRows = HMImgHeight;
	gHFDesc.samples.data = gHFSamples;
	gHFDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightField* HeightFlield = gCooking->createHeightField(
		gHFDesc,
		gPhysics->getPhysicsInsertionCallback()
	);

	float heightscale = 0.3f;
	float rowscale = 2.4f;
	float colscale = 2.4f;

	PxI16 nbMaterials = 1;
	PxMaterial* materials = (PxMaterial*)malloc(sizeof(PxMaterial) * nbMaterials);
	memset(materials, NULL, nbMaterials);
	materials[0] = *gMaterial;
	auto HFTransform = PxTransform({ (-HMImgWidth * rowscale) / 2,-256 * heightscale,(-HMImgHeight * colscale) / 2 });
	PxHeightFieldGeometry HFGeometry(HeightFlield, PxMeshGeometryFlags(), heightscale, rowscale, colscale);
	//PxRigidActor *ground = PxCreateStatic()
	PxRigidStatic *ground = PxCreateStatic(*gPhysics, HFTransform, HFGeometry, *gMaterial, PxTransform({ 0,0,0 }));
	//PxShape* HFShape = PxRigidActorExt::createExclusiveShape(*ground, HFGeometry, materials, nbMaterials);

	gScene->addActor(*ground);

	heightmap.clear();
	free(heightData);
	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	//gScene->addActor(*groundPlane);

	//for (PxU32 i = 0; i < 5; i++)
	//	createStack(PxTransform(PxVec3(0, 5, stackZ -= 10.0f)), 10, 2.0f);
	for (unsigned int i = 0; i < 40; i++)
	{
		PxShape* shape = gPhysics->createShape(PxSphereGeometry(0.5f), *gMaterial);
		PxTransform transform(0.f, 6.f + i, 0.f);
		PxRigidDynamic* body = gPhysics->createRigidDynamic(transform);
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 3.0f);
		gScene->addActor(*body);
		shape->release();
	}

	using namespace std::chrono_literals;
	auto lastTime = std::chrono::system_clock::now();

	auto nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	std::vector<PxRigidActor*> actors(nbActors);
	gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
	
	GLFWwindow* window = nullptr;
	InitWindow(window, "Te iubesc!");
	//run simulation
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	while (true) {
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);
		auto thisTime = std::chrono::system_clock::now();
		auto deltaTime = thisTime - lastTime;
		lastTime = thisTime;
		auto dt_millisec = std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count();
		auto dt_microsec = std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count();
		glfwSetWindowTitle(window, (std::string("Te iubesc, chiar daca mergi cu ") + std::to_string(1000000.0 / dt_microsec)+ "FPS!").c_str());
		deltaTime.count();
		if (dt_millisec > 0)
		{
			for (const auto& actor : actors)
			{
				actor->userData;
				//std::cout << actor->getNbShapes() << std::endl;
				if (actor->is<PxRigidDynamic>()) {
					auto dyn_actor = static_cast<PxRigidDynamic*>(actor);
					if(dyn_actor->getAngularVelocity().magnitude() < 96)
					dyn_actor->addTorque({ 10, 0.f, 0.f });
				}
			}
			gScene->simulate(1.0f / (1000.0f / dt_millisec));
			//gScene->simulate(1.0f / 60.0f);
			gScene->fetchResults(true);
		}
		//std::this_thread::sleep_for(1ms);
		glfwPollEvents();
	}
#else
#endif
}