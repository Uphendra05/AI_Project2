#pragma once
#include "../../src/model.h"
#include "../PhysicsCollisionTypes.h"
#include"../PhysicsObject.h"


struct Point2
{

	
	Point2() = default;

	glm::vec3 curPosition = glm::vec3(0.0f);
	glm::vec3 oldPosition = glm::vec3(0.0f);

	glm::vec3 offsetFromCentre;
	std::vector<Vertex> sM_vertexData;
	bool isLocked = false;

	void SetPointToCentre(std::vector<Vertex>& vertex, glm::vec3& center)
	{
		sM_vertexData.reserve(vertex.size());

		for (Vertex& vert : vertex)
		{
			offsetFromCentre = vert.Position - center;
			sM_vertexData.push_back(vert);
		}

		bool sexyBreakpoint = true;
	}


	


};


struct Constraints2
{
	Constraints2() = default;
	Constraints2(Point2* pA, Point2* pB) : pointA(pA), pointB(pB)
	{
		length = glm::distance(pointA->curPosition, pointB->curPosition);

	};

	Point2* pointA;
	Point2* pointB;

	float length = 0;
	bool isActive = true;
};


class SoftbodyMeshes : public Model
{

public:

	SoftbodyMeshes();
	~SoftbodyMeshes();

	bool isSoftBodyActive = true;

	void InitializeSoftBody();

	void SimulatePoints(float deltaTime);
	void SimulateConstraints(float deltaTime);
	void StartSimultaion(float deltaTime);

	void AddLockSphere(unsigned int Index, float radius);
	void LockPoint(unsigned int Index, float radius);

	void PointAndConstraintsSetup();

	void UpdateMeshVertices();


	void CalculateMatrixForVertices(std::shared_ptr<Mesh> vertSize) const;
	void SetupPoints();
	void SetupConstraints();

	// Softbody for mesh

	// void Setup mesh of points and constraints
	// CalculateMatrixForVertices
	// update mesh

	void CalculateMatrixForMeshPoints(std::shared_ptr<Mesh> vertSize) ;

	void DrawProperties()override;
	void SceneDraw()override;

	void Start() override;
	void Update(float deltaTime) override;
	void Render()override;
	void OnDestroy()override;


private:

	bool showDebug = true;
	float tightnessFactor = 1;


	void CleanZeroes(glm::vec3& value);
	void UpdateNormals();

	const double MAX_DELTATIME = 1.0 / 60.0;

	float renderRadius = 0.25f;
	float lockRadius;
	glm::vec3 lockSphereCenter;

	glm::vec3 acceleration = glm::vec3(0.0f, -1.0f, 0.0f);

	std::vector<Triangle> m_ListOfTriangles;
	std::vector<Point2*> m_ListOfPoints;
	std::vector<Constraints2*> m_ListOfConstraints;

	// Beads Simulation




};

