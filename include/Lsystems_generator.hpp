#pragma once
#include <mesh_data.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <optional>
struct rule
{
	std::string successor;
	float probability;
};
struct LSystemsSettings
{
	std::unordered_map<char, std::vector<rule>> rules;
	std::string axiom;
	bool isStochastic = false;
	void AddRule(char c,const std::string& successor, float probability = 1.0f)
	{
		rules[c].push_back({ successor, probability });
	}

};
class LSystemsGenerator
{
public:
	LSystemsGenerator(const LSystemsSettings& settings);
	std::string Generate(int iterations);
	void Invalidate();
private:
	LSystemsSettings settings;
	std::optional<std::string> cache;
	int cacheIterations = -1;
	
};