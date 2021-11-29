#include "pch.h"
#include "../GameCore/Object3D.h"

using namespace GameCore;

TEST(Object3DTest, TestObjectInstacing) {
	Object3D* obj1 = new Object3D();
	
	ASSERT_NE(obj1, nullptr);
	ASSERT_EQ(obj1->type, ObjectType::Object3D);
	ASSERT_EQ(obj1->id, 0);

	Object3D* obj2 = new Object3D();
	ASSERT_NE(obj2, nullptr);
	ASSERT_EQ(obj2->type, ObjectType::Object3D);
	ASSERT_EQ(obj2->id, 1);
}

TEST(Object3DTest, TestApplyMatrix4) {
	Object3D* a = new Object3D();
	mat4 m(1.0f);
	vec3 exceptedPos(2, 3, 4);
	quat exceptedQuat((float)(0.5 * sqrt(2)), (float)(0.5 * sqrt(2)), 0, 0);

	// rotate x axis
	m = rotate(m, radians(90.f), vec3(1.f, 0.f, 0.f));
	a->position = vec3(2, 3, 4);
	a->applyMatrix4(m);
	
	ASSERT_EQ(a->position, exceptedPos);
	ASSERT_TRUE(
		abs(a->quaternion.x - exceptedQuat.x) <= 0.001f &&
		abs(a->quaternion.y - exceptedQuat.y) <= 0.001f &&
		abs(a->quaternion.z - exceptedQuat.z) <= 0.001f);
}

TEST(Object3DTest, TestLookAt) {
	Object3D* a = new Object3D();
	a->lookAt(vec3(0.f, -1.f, 1.f));
	vec3 rotationRAD = eulerAngles(a->quaternion);
	vec3 rotationDEG = degrees(rotationRAD);
	ASSERT_EQ(rotationDEG.x, 45.f);
}