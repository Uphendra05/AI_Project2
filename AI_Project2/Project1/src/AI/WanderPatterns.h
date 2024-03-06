#pragma once
#include <glm/glm.hpp>
#include "../model.h"
#include <vector>



class WanderPatterns : public Model
{

public:

	WanderPatterns();
	~WanderPatterns();

	struct Waypoint
	{
		glm::vec3 position;
		glm::vec3 rotation;


	};

	std::vector<Waypoint> waypoints = {
	{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -90.0f, 0.0f)},  // Waypoint 0
	{glm::vec3(10.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f)},    // Waypoint 1
	{glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f)},   // Waypoint 2
	{glm::vec3(10.0f, 0.0f, 10.0f), glm::vec3(0.0f, 90.0f, 0.0f)}    // Waypoint 3
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

	float movementSpeed = .5f;
	int waypointIndex;
	float elapsedTime;

	glm::vec3 startPos;
	glm::vec3 startRot;

};

