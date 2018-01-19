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
	float rotLowerArm = -45;
	float xPos;
	float yPos;
	float zPos;
	GLUquadricObj *base;
	GLUquadricObj *baseCover;
	GLUquadricObj *lowerArm;
	GLUquadricObj *joint;
	// Following Nodes need to available in HandleKeyDown
	TreeNode* tree;
	TreeNode* lowerArmNode;
	TreeNode* jointNode;

	Vector3f Position;
	Vector3f Rotation;
	Vector3f Direction;
	float TurnSpeed = 200;
	float MaxSpeed = 50;
	float Drag = 10;

	Tank();
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void Draw();
	void DrawBase();
	void DrawLowerArm();
	void DrawJoint();
	void Update();
	void HandleKeyDown(WPARAM wParam);
	void HandleKeyUp(WPARAM wParam);
	~Tank();

private:
	int accelerationState = 0;
	float currentAcceleration = 0;
	float currentSpeed = 0;;
	int rotationState = 0;
};



