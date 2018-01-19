#include "stdafx.h"
#include "Tank.h"

Texture bodyTexture;

Tank::Tank()
{
	joint = gluNewQuadric();
	gluQuadricDrawStyle(joint, GLU_FILL);

	base = gluNewQuadric();
	gluQuadricDrawStyle(base, GLU_FILL);

	lowerArm = gluNewQuadric();
	gluQuadricDrawStyle(lowerArm, GLU_FILL);

	ColorInt light{ 0, 250 , 0 };
	ColorInt dark{ 0, 100, 0 };
	vector<vector<ColorInt>> colors;
	int divisions = 16;
	for (int i = 0; i < divisions; i++)
	{
		colors.push_back(vector<ColorInt>(divisions));
		for (int j = 0; j < divisions; j++)
		{
			colors[i][j] = (j % 2) == 0 ? light : dark;
			colors[i][j] = ((j % 2) == 0 || (i % 2) == 0) ? dark : light;
		}
	}
	bodyTexture.Initialise(colors);
}


Tank::~Tank()
{}

int TreeID = 0;
int LowerArmID = 10;
int JointID = 20;

void Tank::BuildTree()
{
	// The root of the tree (represents the base of Robot Arm)
	tree = new TreeNode;
	tree->child = 0;
	tree->sibling = 0;
	tree->drawFunctionID = DRAW_BASE_FUNCTION_ID;
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);
	// The lower arm
	lowerArmNode = new TreeNode;
	lowerArmNode->child = 0;
	lowerArmNode->sibling = 0;
	lowerArmNode->drawFunctionID = DRAW_LOWERARM_FUNCTION_ID;
	glLoadIdentity();
	glTranslatef(0, 1, 0);
	glRotatef(rotLowerArm, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lowerArmNode->matrix);
	// The first joint
	jointNode = new TreeNode;
	jointNode->child = 0;
	jointNode->sibling = 0;
	jointNode->drawFunctionID = DRAW_JOINT_FUNCTION_ID;
	glLoadIdentity();
	glTranslatef(0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, jointNode->matrix);
	// The second joint
	TreeNode* jointNode2 = new TreeNode;
	jointNode2->child = 0;
	jointNode2->sibling = 0;
	jointNode2->drawFunctionID = DRAW_JOINT_FUNCTION_ID;
	glLoadIdentity();
	glTranslatef(0, 0, 2);
	glGetFloatv(GL_MODELVIEW_MATRIX, jointNode2->matrix);
	// Plugging the nodes in the tree together
	tree->child = lowerArmNode;
	lowerArmNode->sibling = jointNode;
}

void Tank::Draw()
{
	glEnable(GL_LIGHTING);
	DrawTree(tree);
	glDisable(GL_LIGHTING);
}

void Tank::DrawTree(TreeNode* root)
{
	if (root == 0)
		return;
	glPushMatrix();
	glMultMatrixf(root->matrix);
	DrawFunction(root->drawFunctionID);
	if (root->child != 0)
		DrawTree(root->child);
	glPopMatrix();
	if (root->sibling != 0)
		DrawTree(root->sibling);
}

void Tank::DrawFunction(int id)
{
	switch (id)
	{
		case DRAW_BASE_FUNCTION_ID:
			DrawBase();
			break;
		case DRAW_LOWERARM_FUNCTION_ID:
			DrawLowerArm();
			break;
		case DRAW_JOINT_FUNCTION_ID:
			DrawJoint();
			break;
	}
}

void Tank::DrawBase()
{
	glPushMatrix();
	SetMaterial(&darkGreenMaterial);
	vector<Vector3f> wheelPositons = 
	{
		Vector3f(0.5,0,1),Vector3f(-0.8,0,1), Vector3f(0.5,0,-1), Vector3f(-0.8,0,-1),
		Vector3f(0.5,0,0.33),Vector3f(-0.8,0,0.33), Vector3f(0.5,0,-0.33), Vector3f(-0.8,0,-0.33)
	};
	for (int i = 0; i < wheelPositons.size(); i++)
	{
		Vector3f wheelposition = wheelPositons[i];
		glPushMatrix();
		glTranslatef(wheelposition.X, wheelposition.Y, wheelposition.Z);
		glTranslatef(0, 0.3, 0);
		glRotatef(90, 0, 1, 0);
		gluCylinder(base, 0.3, 0.3, 0.3, 16, 16);
		float k = 0.0f;
		glBegin(GL_TRIANGLE_FAN);
		for (k = 0.0f; k <= 360; k++)
			glVertex2f(0.3*cos(3.142 * k / 180.0), 0.3*sin(3.142 * k / 180.0));
		glEnd();
		glTranslatef(0, 0, 0.3);
		glBegin(GL_TRIANGLE_FAN);
		for (k = 0.0f; k <= 360; k++)
			glVertex2f(0.3*cos(3.142 * k / 180.0), 0.3*sin(3.142 * k / 180.0));
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, bodyTexture.Image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SetMaterial(&greenPlasticMaterial);
	glTranslatef(0, 0.2, 0);

	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(0.7, 0, 1);
	glTexCoord2f(0, 0.7);
	glVertex3f(0.7, 0, -1);
	glTexCoord2f(0.7, 0.7);
	glVertex3f(-0.7, 0, -1);
	glTexCoord2f(0.7, 0);
	glVertex3f(-0.7, 0, 1);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.70, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(0.7, 0, 1);
	glTexCoord2f(0, 0.7);
	glVertex3f(0.7, 0, -1);
	glTexCoord2f(0.7, 0.7);
	glVertex3f(-0.7, 0, -1);
	glTexCoord2f(0.7, 0);
	glVertex3f(-0.7, 0, 1);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.35, -1);
	glRotatef(90, 1, 0, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(0.7, 0, 0.35);
	glTexCoord2f(0, 0.35);
	glVertex3f(0.7, 0, -0.35);
	glTexCoord2f(0.7, 0.35);
	glVertex3f(-0.7, 0, -0.35);
	glTexCoord2f(0.7, 0);
	glVertex3f(-0.7, 0, 0.35);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.35, 1);
	glRotatef(90, 1, 0, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(0.7, 0, 0.35);
	glTexCoord2f(0, 0.35);
	glVertex3f(0.7, 0, -0.35);
	glTexCoord2f(0.7, 0.35);
	glVertex3f(-0.7, 0, -0.35);
	glTexCoord2f(0.7, 0);
	glVertex3f(-0.7, 0, 0.35);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, 0.70, 0);
	glRotatef(90, 0, 0, 1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 1);
	glTexCoord2f(0.1, 0.2);
	glVertex3f(0, 0, -1);
	glTexCoord2f(0.05, 1);
	glVertex3f(-0.7, 0, -1);
	glTexCoord2f(0.7, 0);
	glVertex3f(-0.7, 0, 1);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, 0.70, 0);
	glRotatef(90, 0, 0, 1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 1);
	glTexCoord2f(0.1, 0.2);
	glVertex3f(0, 0, -1);
	glTexCoord2f(0.05, 1);
	glVertex3f(-0.7, 0, -1);
	glTexCoord2f(0.7, 0);
	glVertex3f(-0.7, 0, 1);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Tank::DrawLowerArm()
{
	glPushMatrix();
	SetMaterial(&redPlasticMaterial);
	gluCylinder(lowerArm, 0.15, 0.15, 2, 64, 64);
	glPopMatrix();
}

void Tank::DrawJoint()
{
	glPushMatrix();
	SetMaterial(&yellowPlasticMaterial);
	gluSphere(joint, 0.4, 64, 64);
	glPopMatrix();
}

void Tank::HandleKeyDown(WPARAM wParam)
{
	// Handle the various key-presses
	// TODO: Handle all other key-presses
	switch (wParam)
	{
		case VK_LEFT:
			glPushMatrix();
			glRotatef(TurnSpeed * Clock::DeltaTime, 0, 1, 0);
			glMultMatrixf(lowerArmNode->matrix);
			glGetFloatv(GL_MODELVIEW_MATRIX, lowerArmNode->matrix);// get & stores transform
			glPopMatrix();
			break;

		case VK_RIGHT:
			glPushMatrix();
			glRotatef(-TurnSpeed * Clock::DeltaTime, 0, 1, 0);
			glMultMatrixf(lowerArmNode->matrix);
			glGetFloatv(GL_MODELVIEW_MATRIX, lowerArmNode->matrix);// get & stores transform
			glPopMatrix();
			break;

		case VK_UP:
			glMultMatrixf(lowerArmNode->matrix);
			glRotatef(-TurnSpeed * Clock::DeltaTime, 1, 0, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lowerArmNode->matrix);// get & stores transform
			break;

		case VK_DOWN:
			glMultMatrixf(lowerArmNode->matrix);
			glRotatef(TurnSpeed * Clock::DeltaTime, 1, 0, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lowerArmNode->matrix);// get & stores transform
			break;

		case 0x57: // W
			glMultMatrixf(tree->matrix);
			glTranslatef(0 , 0, MaxSpeed * Clock::DeltaTime);
			glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);// get & stores transform
			break;

		case 0x53: // S
			glMultMatrixf(tree->matrix);
			glTranslatef(0, 0, MaxSpeed * -Clock::DeltaTime);
			glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);// get & stores transform
			break;

		case 0x41: // A
			glMultMatrixf(tree->matrix);
			glRotatef(TurnSpeed * Clock::DeltaTime ,0, 1, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);// get & stores transform
			break;

		case 0x44: // D
			glMultMatrixf(tree->matrix);
			glRotatef(-TurnSpeed * Clock::DeltaTime, 0, 1, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);// get & stores transform
			break;
			break;

	}
	// Update the position & rotation of the complete robot arm
	glLoadIdentity();
	// TODO: Update the rotation of the lower arm
	// TODO: Update the rotation of the upper arm
}
