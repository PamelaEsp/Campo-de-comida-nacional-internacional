#pragma once

int getSecondsDiff(std::chrono::steady_clock::time_point start) {
	auto finish = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed = finish - start;

	return elapsed.count();
}