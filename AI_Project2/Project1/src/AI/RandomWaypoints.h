#pragma once
#include <glm/glm.hpp>
#include "../model.h"
#include <vector>

class Camera;

class RandomWaypoints : public Model
{

public:

	RandomWaypoints();
	RandomWaypoints(float patrolSpeed, float waitTime, float alertDistance, Camera* target);
	~RandomWaypoints();

	struct Waypoint
	{
		glm::vec3 position;
		

	};

	std::vector<Waypoint> waypoints = 
	{
	{glm::vec3(0.0f, 0.0f, 0.0f)     },    // Waypoint 0
	{glm::vec3(-10.0f, 0.0f, 10.0f)  },    // Waypoint 1
	{glm::vec3(20.0f, 0.0f, 0.0f)    },    // Waypoint 2
	{glm::vec3(30.0f, 0.0f, 30.0f)   }     // Waypoint 3
	};

	void MoveAgent(const glm::vec3& targetPosition, const glm::vec3& targetRotation, float deltaTime);
	void CalculateNextWaypoint(float deltaTime);


	void DrawProperties()override;
	void SceneDraw()override;

	void Start() override;
	void Update(float deltaTime) override;
	void Render()override;
	void OnDestroy()override;

	void RenderAlertDistance();
	float CalculateDistance(glm::vec3& transform, glm::vec3& targetTransform);
	

private:

	Model* alertDistanceModel;
	Camera* sceneCam;

	float movementSpeed = .5f;
	int waypointIndex;
	float elapsedTime;

	float patrolSpeed;
	float waitTime;
	float alertDistance;

	glm::vec3 startPos;
	glm::vec3 startRot;

};

