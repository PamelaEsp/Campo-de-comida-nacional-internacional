#pragma once

#include "PointLight.h"

int getSecondsDiff(std::chrono::steady_clock::time_point start) {
	auto finish = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed = finish - start;

	return elapsed.count();
}

float getAttenuationValue(float constant, float linear, float quadratic, float distance) {
	// Calculate the attnuation value for pointlights
	return 1.0 / (constant + linear * distance + quadratic * (distance * distance));
}

int getNearestLightIndex(PointLight lights[], int lightsCount, glm::vec3 tuxPos) {
	int nearestIndex = 0;
	float nearestDistance = 99999999999999.0f;

	float distance = 0.0f;

	for (int i = 0; i < lightsCount; i++) {

		distance = glm::distance(lights[i].getPosition(), tuxPos);

		if (distance < nearestDistance) {
			nearestIndex = i;
			nearestDistance = distance;
		}
	}

	return nearestIndex;
} 