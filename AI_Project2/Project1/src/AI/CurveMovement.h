#pragma once
#include <glm/glm.hpp>
#include "../model.h"
#include <vector>


class CurveMovement : public Model
{


public:

	CurveMovement();
	~CurveMovement();

	struct Waypoint
	{
		glm::vec3 position;
		glm::vec3 rotation;


	};

	std::vector<Waypoint> waypoints = {
	{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -90.0f, 0.0f)},  // Waypoint 0
	{glm::vec3(20.0f, -20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},    // Waypoint 1
	{glm::vec3(40.0f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f)},   // Waypoint 2
	{glm::vec3(20.0f, 20.0f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f)}    // Waypoint 3
	};

	void MoveAgent(const glm::vec3& targetPosition, const glm::vec3& targetRotation, float deltaTime);
	void CalculateNextWaypoint(float deltaTime);


	void DrawProperties()override;
	void SceneDraw()override;

	void Start() override;
	void Update(float deltaTime) override;
	void Render()override;
	void OnDestroy()override;

private:

	glm::vec3 CalculateCubicBezier(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	float movementSpeed = .5f;
	int waypointIndex;
	float elapsedTime;
	float curveDuration = 8.0f;
	glm::vec3 startPos;
	glm::vec3 startRot;

};

