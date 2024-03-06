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

	std::vector<Waypoint> listOfWaypoints = 
	{
	{  glm::vec3(0.0f, 0.0f, 0.0f)     },    // Waypoint 0
	{  glm::vec3(-10.0f, 0.0f, 10.0f)  },    // Waypoint 1
	{  glm::vec3(20.0f, 0.0f, 0.0f)    },    // Waypoint 2
	{  glm::vec3(30.0f, 0.0f, 30.0f)   },    // Waypoint 3
	{  glm::vec3(-33.0f, 0.0f, 0.0f)    },
	{  glm::vec3(-9.30f, 0.0f, -28.50f)    },
	{  glm::vec3(36.2f, 0.0f, -30.0f)    },
	{  glm::vec3(12.4f, 0.0f, -41.5f)    },
	{  glm::vec3(-38.2f, 0.0f, -41.5f)    },
	{  glm::vec3(-38.2f, 0.0f, 36.2f)    },
	{  glm::vec3(-1.6f, 0.0f, 36.2f)    },
	{  glm::vec3(13.9f, 0.0f,-35.4f)    },
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

	void RenderWaypoints();


private:

	Model* alertDistanceModel;
	Model* waypointModel;
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

