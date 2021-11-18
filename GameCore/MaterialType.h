#pragma once

namespace GameCore {

	enum class MaterialType {
		Material,
	};

	static bool isMaterial(MaterialType type) {
		return type == MaterialType::Material;
	}
}