#include "RandomWaypoints.h"
#include "../Utilities/Lerp.h"
#include "../GraphicsRender.h"

RandomWaypoints::RandomWaypoints()
{
	LoadModel("Models/DefaultCube/DefaultCube.fbx");
	meshes[0]->meshMaterial->material()->SetBaseColor(glm::vec4(1, 0, 0, 1));
	GraphicsRender::GetInstance().AddModelAndShader(this, GraphicsRender::GetInstance().defaultShader);

}

RandomWaypoints::~RandomWaypoints()
{
}

void RandomWaypoints::MoveAgent(const glm::vec3& targetPosition, const glm::vec3& targetRotation, float deltaTime)
{

	transform.position = Lerp(transform.position, targetPosition, deltaTime);
}

void RandomWaypoints::CalculateNextWaypoint(float deltaTime)
{


	if (waypointIndex < waypoints.size())
	{
		const Waypoint& CurWaypoint = waypoints[waypointIndex];

		MoveAgent(CurWaypoint.position, transform.rotation, deltaTime);

		glm::vec3 currentPosition = transform.position;

		if (glm::distance(currentPosition, CurWaypoint.position) < 1.5f)
		{

			waypointIndex = std::rand() % waypoints.size();
		}

	}
}

void RandomWaypoints::DrawProperties()
{
	Model::DrawProperties();
}

void RandomWaypoints::SceneDraw()
{
	Model::SceneDraw();
}

void RandomWaypoints::Start()
{
}

void RandomWaypoints::Update(float deltaTime)
{
	CalculateNextWaypoint(deltaTime);
}

void RandomWaypoints::Render()
{
}

void RandomWaypoints::OnDestroy()
{
}
