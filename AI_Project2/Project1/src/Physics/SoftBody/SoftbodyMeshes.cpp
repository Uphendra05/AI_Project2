#include "SoftbodyMeshes.h"
#include "../../src/Physics/PhysicsEngine.h"

glm::vec3 CenterCalculation(std::vector<Vertex>& vertex)
{
	glm::vec3 centre = glm::vec3(0);

	for (Vertex& vert : vertex)
	{
		centre += vert.Position;
	}

	centre /= (float)(vertex.size());

	return centre;
}

SoftbodyMeshes::SoftbodyMeshes()
{
}

SoftbodyMeshes::~SoftbodyMeshes()
{
}

void SoftbodyMeshes::InitializeSoftBody()
{
	PointAndConstraintsSetup();
	PhysicsEngine::GetInstance().AddSoftBodyMeshObject(this);
}

void SoftbodyMeshes::SimulatePoints(float deltaTime)
{
	if (deltaTime > MAX_DELTATIME)
	{
		deltaTime = MAX_DELTATIME;
	}

	for (Point2* point : m_ListOfPoints)
	{
		if (point->isLocked)
		{
			continue;
		}
		else
		{
			glm::vec3 currentPosition = point->curPosition;
			glm::vec3 prevPosition = point->oldPosition;

			point->curPosition += (currentPosition - prevPosition) + (this->acceleration * (float)(deltaTime * deltaTime));
			point->oldPosition = currentPosition;
		}

	}
}

void SoftbodyMeshes::SimulateConstraints(float deltaTime)
{
	const unsigned int MAX_ITERATION = 3;

	for (size_t i = 0; i < MAX_ITERATION; i++)
	{
		for (Constraints2* constraint : m_ListOfConstraints)
		{
			if (constraint->isActive)
			{

				Point2* pointA = constraint->pointA;
				Point2* pointB = constraint->pointB;

				glm::vec3 delta = pointB->curPosition - pointA->curPosition;

				float deltaLength = glm::length(delta);

				if (deltaLength != 0)
				{
					float diff = (deltaLength - constraint->length) / deltaLength;


					if (!pointA->isLocked)
					{
						pointA->curPosition += delta * 0.5f * diff * tightnessFactor;
					}


					if (!pointB->isLocked)
					{
						pointB->curPosition -= delta * 0.5f * diff * tightnessFactor;
					}


					CleanZeroes(pointA->curPosition);
					CleanZeroes(pointB->curPosition);
				}




			}

		}
	}
}

void SoftbodyMeshes::StartSimultaion(float deltaTime)
{
	m_ListOfPoints[0]->isLocked = true;

	SimulatePoints(deltaTime);
	SimulateConstraints(deltaTime);
	UpdateMeshVertices();
}

void SoftbodyMeshes::AddLockSphere(unsigned int Index, float radius)
{
}

void SoftbodyMeshes::LockPoint(unsigned int Index, float radius)
{
}

void SoftbodyMeshes::PointAndConstraintsSetup()
{
	SetupPoints();

	for (std::shared_ptr<Mesh> mesh : meshes)
	{
		CalculateMatrixForMeshPoints(mesh);
		
	}

	SetupConstraints();

	
}

void SoftbodyMeshes::UpdateMeshVertices()
{
	glm::vec3 verticesPos;
	glm::mat4 inverseMat = transform.GetModelInverseMatrixNoTranspose();
	int i = 0;
	for (Point2* point : m_ListOfPoints)
	{

		verticesPos = inverseMat * glm::vec4(point->curPosition, 1.0f);
		verticesPos += point->offsetFromCentre;		
		point->sM_vertexData[i].Position = verticesPos;
		i++;

	}

	UpdateNormals();

	for (std::shared_ptr<Mesh> mesh : meshes)
	{
		mesh->UpdateVertices();
	}
}

void SoftbodyMeshes::CalculateMatrixForVertices(std::shared_ptr<Mesh> vertSize) const
{
}

void SoftbodyMeshes::SetupPoints()
{
	m_ListOfPoints.reserve(meshes.size());

	glm::mat4 modelMat = transform.GetModelMatrix();

	for (std::shared_ptr<Mesh>& mesh : meshes)
	{
		Point2* node = new Point2();

		node->curPosition = modelMat * glm::vec4(CenterCalculation(mesh->vertices), 1);
		node->SetPointToCentre(mesh->vertices, node->curPosition);
		node->oldPosition = node->curPosition;

		m_ListOfPoints.push_back(node);

	}

}

void SoftbodyMeshes::SetupConstraints()
{

	for (size_t i = 0; i < m_ListOfPoints.size() - 1; i++)
	{
		Point2* nodeA = m_ListOfPoints[i];
		Point2* nodeB = m_ListOfPoints[i+1];

		m_ListOfConstraints.push_back(new Constraints2(nodeA, nodeB));
	}
}

void SoftbodyMeshes::CalculateMatrixForMeshPoints(std::shared_ptr<Mesh> vertSize) 
{
	glm::mat4 modelMatrix = transform.GetModelMatrix();

	for (size_t index = 0; index < vertSize->vertices.size(); index++)
	{

		glm::vec4 modelVertexPos = modelMatrix * glm::vec4(vertSize->vertices[index].Position, 1.0f);

		vertSize->vertices[index].Position = modelVertexPos;
		


	}


}

void SoftbodyMeshes::DrawProperties()
{
	Model::DrawProperties();

	if (!ImGui::TreeNodeEx("Softbody properties", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGui::Text("Show Debug");
	ImGui::SameLine();
	ImGui::Checkbox("##ShowDebug", &showDebug);

	ImGui::Text("tightnessFactor");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150);
	ImGui::InputFloat("##tightnessFactor", &tightnessFactor, 0, 0.1, "%.2f");

	ImGui::Text("LockRadius");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150);
	ImGui::InputFloat("##lockRadius", &lockRadius, 0, 0.1, "%.2f");



	ImGui::SetNextItemWidth(80);
	ImGui::Text("LockCentre");
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(80);
	ImGui::DragFloat("##X", &lockSphereCenter.x);
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(80);
	ImGui::DragFloat("###Y", &lockSphereCenter.y);
	ImGui::SameLine();
	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(80);
	ImGui::DragFloat("###Z", &lockSphereCenter.z);


	ImGui::NewLine();
	ImGui::TreePop();

	if (!ImGui::TreeNodeEx("List of points ", ImGuiTreeNodeFlags_OpenOnArrow))
	{
		return;
	}
	for (int i = 0; i < m_ListOfPoints.size(); ++i)
	{
		Point2*& point = m_ListOfPoints[i];


		ImGui::Text("locked");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::Checkbox(("##locked" + std::to_string(i)).c_str(), &point->isLocked);




		ImGui::SetNextItemWidth(80);
		ImGui::Text("points");
		ImGui::SameLine();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat(("##X" + std::to_string(i)).c_str(), &point->curPosition.x);
		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat(("###Y" + std::to_string(i)).c_str(), &point->curPosition.y);
		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat(("###Z" + std::to_string(i)).c_str(), &point->curPosition.z);

		ImGui::NewLine();
	}
	ImGui::TreePop();
}

void SoftbodyMeshes::SceneDraw()
{
	Model::SceneDraw();
}

void SoftbodyMeshes::Start()
{
}

void SoftbodyMeshes::Update(float deltaTime)
{
}

void SoftbodyMeshes::Render()
{
	if (!showDebug) return;

	for (Point2* point : m_ListOfPoints)
	{
		GraphicsRender::GetInstance().DrawSphere(point->curPosition, renderRadius, glm::vec4(0, 1, 1, 1), true);
	}

	for (Constraints2* stick : m_ListOfConstraints)
	{
		GraphicsRender::GetInstance().DrawLine(stick->pointA->curPosition, stick->pointB->curPosition, glm::vec4(1, 1, 0, 1));
	}
}

void SoftbodyMeshes::OnDestroy()
{
}

void SoftbodyMeshes::CleanZeroes(glm::vec3& value)
{
}

void SoftbodyMeshes::UpdateNormals()
{

	for (std::shared_ptr<Mesh> mesh : meshes)
	{

		for (size_t i = 0; i < mesh->vertices.size(); i++)
		{
			mesh->vertices[i].Normal = glm::vec3(0);
		}
	}

	for (std::shared_ptr<Mesh> mesh : meshes)
	{
		for (size_t i = 0; i < mesh->indices.size(); i += 3)
		{
			unsigned int vertAIndex = mesh->indices[i + 0];
			unsigned int vertBIndex = mesh->indices[i + 1];
			unsigned int vertCIndex = mesh->indices[i + 2];


			Vertex& vertexA = mesh->vertices[vertAIndex];
			Vertex& vertexB = mesh->vertices[vertBIndex];
			Vertex& vertexC = mesh->vertices[vertCIndex];

			glm::vec3 vertA = vertexA.Position;
			glm::vec3 vertB = vertexB.Position;
			glm::vec3 vertC = vertexC.Position;

			glm::vec3 triangleEdgeAtoB = vertB - vertA;
			glm::vec3 triangleEdgeAtoC = vertC - vertA;


			glm::vec3 normal = glm::normalize(glm::cross(triangleEdgeAtoB, triangleEdgeAtoC));


			CleanZeroes(normal);

			{
				vertexA.Normal.x += normal.x;
				vertexA.Normal.y += normal.y;
				vertexA.Normal.z += normal.z;

				vertexB.Normal.x += normal.x;
				vertexB.Normal.y += normal.y;
				vertexB.Normal.z += normal.z;

				vertexC.Normal.x += normal.x;
				vertexC.Normal.y += normal.y;
				vertexC.Normal.z += normal.z;
			}



		}

	}
}
