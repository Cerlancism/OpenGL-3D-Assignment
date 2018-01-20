#include "stdafx.h"
#include "Tank.h"

Texture bodyTexture;

Tank::Tank()
{
	jointQuadratic = gluNewQuadric();
	gluQuadricDrawStyle(jointQuadratic, GLU_FILL);

	base = gluNewQuadric();
	gluQuadricDrawStyle(base, GLU_FILL);

	turrentQuadratic = gluNewQuadric();
	gluQuadricDrawStyle(turrentQuadratic, GLU_FILL);

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
{

}

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
	glRotatef(-22.5, 1, 0, 0);
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

void Tank::Update()
{
	currentAcceleration = sin(abs(currentSpeed) / MaxSpeed * (PI)) * 33 + 1;
	currentSpeed = abs(currentSpeed) >= MaxSpeed ? currentSpeed : currentSpeed += (accelerationState * currentAcceleration * Clock::DeltaTime);
	float currentDrag = Drag * Clock::DeltaTime * (abs(currentSpeed) / 10) + 0.1 * Clock::DeltaTime;
	float positiveDrag = currentSpeed - currentDrag, negativeDrag = currentSpeed + currentDrag;
	currentSpeed = currentSpeed > 0 ? positiveDrag < 0 ? 0 : positiveDrag : negativeDrag > 0 ? 0 : negativeDrag;

	Rotation.Y = fmod(Rotation.Y * 180.0 / PI + (rotationState * TurnSpeed * Clock::DeltaTime), 360.0f);
	Rotation.Y = Rotation.Y < 0 ? 360 + Rotation.Y : Rotation.Y;
	Rotation.Y = Rotation.Y * PI / 180.0;

	Direction.X = sin(Rotation.Y);
	Direction.Z = cos(Rotation.Y);

	Position.X += Direction.X * currentSpeed * Clock::DeltaTime;
	Position.Z += Direction.Z * currentSpeed * Clock::DeltaTime;

	if (turrentPitch >= 0 || turrentPitch <= 45)
	{
		float newRotation = turrentPitch + turrentPitchState * TurretTurnSpeed * Clock::DeltaTime;
		newRotation = newRotation < 0 ? 0 : newRotation;
		newRotation = newRotation > 45 ? 45 : newRotation;
		turrentPitch = newRotation;
		if (turrentPitch == 0 && turrentPitchState == -1)
		{
			turrentPitchState = 0;
		}
		if (turrentPitch == 45 && turrentPitchState == 1)
		{
			turrentPitchState = 0;
		}
	}

	tRot += turretRotationState * TurretTurnSpeed * Clock::DeltaTime * PI / 180.0;
	tPitch += turrentPitchState * TurretTurnSpeed * Clock::DeltaTime * PI / 180.0;
	TurrentDirection.X = sin(Rotation.Y + tRot);
	TurrentDirection.Z = cos(Rotation.Y + tRot);
	TurrentDirection.Y = sin(tPitch);
	TurrentDirection.X *= cos(tPitch);
	TurrentDirection.Z *= cos(tPitch);

	TurrentPosition = TurrentDirection * 1.5 + Position + Vector3f(0, 1, 0);

	glPushMatrix();
	glMultMatrixf(tree->matrix);
	glRotatef(rotationState * TurnSpeed * Clock::DeltaTime, 0, 1, 0);
	glTranslatef(0, 0, currentSpeed * Clock::DeltaTime);
	glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);// get & stores transform
	glLoadIdentity();
	glPopMatrix();

	glPushMatrix();
	glRotatef(turretRotationState * TurretTurnSpeed * Clock::DeltaTime, 0, 1, 0);
	glMultMatrixf(lowerArmNode->matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, lowerArmNode->matrix);// get & stores transform
	glLoadIdentity();
	glPopMatrix();

	glPushMatrix();
	glMultMatrixf(lowerArmNode->matrix);
	glRotatef(turrentPitchState * -TurretTurnSpeed * Clock::DeltaTime, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lowerArmNode->matrix);// get & stores transform
	glPopMatrix();
}

void Tank::Draw()
{
	glEnable(GL_LIGHTING);
	DrawTree(tree);

	glPushMatrix();
	glTranslatef(TurrentPosition.X, TurrentPosition.Y, TurrentPosition.Z);
	SetMaterial(&yellowPlasticMaterial);
	gluSphere(jointQuadratic, 0.25, 8, 8);
	glPopMatrix();

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
			DrawTurret();
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

void Tank::DrawTurret()
{
	glPushMatrix();
	SetMaterial(&redPlasticMaterial);
	gluCylinder(turrentQuadratic, 0.15, 0.15, 1.5, 64, 64);
	glPopMatrix();
}

void Tank::DrawJoint()
{
	glPushMatrix();
	SetMaterial(&yellowPlasticMaterial);
	gluSphere(jointQuadratic, 0.4, 64, 64);
	glPopMatrix();
}

void Tank::HandleKeyDown(WPARAM wParam)
{
	// Handle the various key-presses
	// TODO: Handle all other key-presses
	switch (wParam)
	{
		case VK_LEFT:
			turretRotationState = 1;
			break;

		case VK_RIGHT:
			turretRotationState = -1;
			break;

		case VK_UP:
			turrentPitchState = 1;
			break;

		case VK_DOWN:
			turrentPitchState = -1;
			break;

		case 0x57: // W
			accelerationState = 1;
			break;

		case 0x53: // S
			accelerationState = -1;
			break;

		case 0x41: // A
			rotationState = 1;
			break;

		case 0x44: // D
			rotationState = -1;
			break;
	}

	glLoadIdentity();
}

void Tank::HandleKeyUp(WPARAM wParam)
{
	switch (wParam)
	{
		case VK_LEFT:
			turretRotationState = 0;
			break;

		case VK_RIGHT:
			turretRotationState = 0;
			break;

		case VK_UP:
			turrentPitchState = 0;
			break;

		case VK_DOWN:
			turrentPitchState = 0;
			break;

		case 0x57: // W
			accelerationState = 0;
			break;

		case 0x53: // S
			accelerationState = 0;
			break;

		case 0x41: // A
			rotationState = 0;
			break;

		case 0x44: // D
			rotationState = 0;
			break;
	}
}
