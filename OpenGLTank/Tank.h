#pragma once

#define DRAW_BASE_FUNCTION_ID 0
#define DRAW_LOWERARM_FUNCTION_ID 1
#define DRAW_JOINT_FUNCTION_ID 3

typedef struct TreeNode
{
	GLfloat matrix[16];
	int drawFunctionID;
	struct TreeNode* sibling;
	struct TreeNode* child;
} TreeNode;

class Tank
{
public:
	float yRotBase;
	float xPos;
	float yPos;
	float zPos;
	GLUquadricObj *base;
	GLUquadricObj *turretQuadratic;
	GLUquadricObj *jointQuadratic;
	// Following Nodes need to available in HandleKeyDown
	TreeNode* tree;
	TreeNode* turretNode;
	TreeNode* jointNode;

	Vector3f Position;
	Vector3f Rotation;
	Vector3f Direction;
	Vector3f TurretPosition;
	Vector3f TurretDirection;
	float TurnSpeed = 200;
	float TurretTurnSpeed = 100;
	float MaxSpeed = 35;
	float Drag = 15;

	Tank();
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void Draw();
	void DrawBase();
	void DrawTurret();
	void DrawJoint();
	void Update();
	void HandleKeyDown(WPARAM wParam);
	void HandleKeyUp(WPARAM wParam);
	~Tank();

private:
	int accelerationState = 0;
	float currentAcceleration = 0;
	float currentSpeed = 0;
	int rotationState = 0;
	int turretRotationState = 0;
	int turretPitchState = 0;

	float turretRotation = 90;
	float turretPitch = 22.5;

	float tRot = 0;
	float tPitch = PI / 8;
};



