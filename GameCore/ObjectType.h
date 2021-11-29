#pragma once

namespace GameCore {

	/* Contains only inherited Object3D classes */
	enum class ObjectType {
		// base on all 3d objects
		Object3D,
		// camera type
		BaseCamera,
		PerspectiveCamera,
		OrthographicCamera,
		// ligh type
		BaseLight,
		AmbientLight,
		DirectionalLight,
		PointLight,
		SpotLight,
		// Mesh type
		Mesh,
		// Primitive
		Primitive,
		Skybox,
	};

	static bool isObject3D(ObjectType type) {
		return type == ObjectType::Object3D;
	}

	static bool isCamera(ObjectType type) {
		return type == ObjectType::BaseCamera || type == ObjectType::PerspectiveCamera || type == ObjectType::OrthographicCamera;
	}

	static bool isLight(ObjectType type) {
		return type == ObjectType::BaseLight ||
			type == ObjectType::AmbientLight ||
			type == ObjectType::DirectionalLight ||
			type == ObjectType::PointLight ||
			type == ObjectType::SpotLight;
	}

	static bool isMesh(ObjectType type) {
		return type == ObjectType::Mesh;
	}
}