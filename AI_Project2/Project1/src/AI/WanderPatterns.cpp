#include "WanderPatterns.h"
#include "../Utilities/Lerp.h"
#include "../GraphicsRender.h"

WanderPatterns::WanderPatterns()
{
	LoadModel("Models/DefaultCube/DefaultCube.fbx");
	meshes[0]->meshMaterial->material()->SetBaseColor(glm::vec4(0, 1, 0, 1));
	GraphicsRender::GetInstance().AddModelAndShader(this, GraphicsRender::GetInstance().defaultShader);
	

}

WanderPatterns::~WanderPatterns()
{


}

void WanderPatterns::MoveAgent(const glm::vec3& targetPosition, const glm::vec3& targetRotation, float deltaTime)
{
	transform.position = Lerp(transform.position, targetPosition, deltaTime);

}

void WanderPatterns::CalculateNextWaypoint(float deltaTime)
{

	if (waypointIndex < waypoints.size())
	{
		const Waypoint& CurWaypoint = waypoints[waypointIndex];

		MoveAgent(CurWaypoint.position, transform.rotation, deltaTime);

		glm::vec3 currentPosition = transform.position;

		if (glm::distance(currentPosition, CurWaypoint.position) < 1.5f)
		{

			waypointIndex++;

			if (waypointIndex == waypoints.size())
			{
				waypointIndex = 0;
			}
		}

	}


}

void WanderPatterns::Update(float deltaTime)
{
	CalculateNextWaypoint(deltaTime);
}


void WanderPatterns::DrawProperties()
{
	Model::DrawProperties();
}

void WanderPatterns::SceneDraw()
{
	Model::SceneDraw();

}

void WanderPatterns::Start()
{
	startPos = transform.position;
}


void WanderPatterns::Render()
{
}

void WanderPatterns::OnDestroy()
{
}
