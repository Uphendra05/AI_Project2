#pragma once
#include "../Camera.h"
class CameraManager
{
public:

	static CameraManager& GetInstance();
	static int CameraID;

	void AddCamera(Camera* sceneCamera);
	void RemoveCamera(Camera* sceneCamera);

	Camera* GetMainCamera();

	std::vector<Camera*> GetCameras();

private:

	std::vector<Camera*> listOfCameras;;

	Camera* mainCamera = nullptr;

};

