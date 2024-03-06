#include "CurveMovement.h"
#include "../Utilities/Lerp.h"
#include "../GraphicsRender.h"

CurveMovement::CurveMovement()
{
	LoadModel("Models/DefaultCube/DefaultCube.fbx");
	meshes[0]->meshMaterial->material()->SetBaseColor(glm::vec4(0, 0, 1, 1));
	GraphicsRender::GetInstance().AddModelAndShader(this, GraphicsRender::GetInstance().defaultShader);


}

CurveMovement::~CurveMovement()
{


}

void CurveMovement::MoveAgent(const glm::vec3& targetPosition, const glm::vec3& targetRotation, float deltaTime)
{
    transform.position = Lerp(transform.position, targetPosition, deltaTime);

}

void CurveMovement::CalculateNextWaypoint(float deltaTime)
{


	if (waypointIndex < listOfWaypoints.size() - 3)
	{
		const Waypoint& p0 = listOfWaypoints[waypointIndex];
		const Waypoint& p1 = listOfWaypoints[waypointIndex + 1];
		const Waypoint& p2 = listOfWaypoints[waypointIndex + 2];
		const Waypoint& p3 = listOfWaypoints[waypointIndex + 3];

		float t = elapsedTime / curveDuration;
		glm::vec3 curvePosition = CalculateCubicBezier(t, p0.position, p1.position, p2.position, p3.position);

		MoveAgent(curvePosition, transform.rotation, deltaTime);
		
		glm::vec3 currentPosition = transform.position;
		if (glm::distance(currentPosition, p3.position) < 1.5f) 
{

			waypointIndex += 3;

			if (waypointIndex >= listOfWaypoints.size() - 3)
			{

				waypointIndex = 0;
			}
			elapsedTime = 0.0f;

		}
		else
		{
			elapsedTime += deltaTime;
		}
	}



}

void CurveMovement::Update(float deltaTime)
{
	CalculateNextWaypoint(deltaTime);
}


void CurveMovement::DrawProperties()
{
	Model::DrawProperties();
}

void CurveMovement::SceneDraw()
{
	Model::SceneDraw();

}

void CurveMovement::Start()
{
	startPos = transform.position;
}


void CurveMovement::Render()
{
}

void CurveMovement::OnDestroy()
{
}

glm::vec3 CurveMovement::CalculateCubicBezier(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 result;
	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	result = uuu * p0 + 5 * uu * t * p1 + 3 * u * tt * p2 + ttt * p3;

	return result;
}
