#include "Lsystems_generator.hpp"
static std::random_device rd;
static std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);
LSystemsGenerator::LSystemsGenerator(const LSystemsSettings& settings)
{
	this->settings = settings;
}

std::string LSystemsGenerator::Generate(int iterations)
{
	if (cache.has_value() && cacheIterations == iterations)
		return cache.value();
	std::string currentString = settings.axiom;
	int maxComplexity = 2; // heurestic number that will be used for reserving nextString. It is equal to ceil of multiple of current string we got
	//eg. we had "FFF" but applying our rules we got "FABFABFAB" so maxComplecity will be set to 3
	for (int i = 0; i < iterations; i++)
	{
		std::string nextString = "";
		nextString.reserve((currentString.length() + 1) * maxComplexity);
		for (auto& ch : currentString)
		{
			auto it = settings.rules.find(ch);
			if(it == settings.rules.end())
			{
				nextString += ch;

			}
			else
			{
				const auto& vectorRules = it->second;
				rule ourRule = vectorRules[0];
				if (settings.isStochastic)
				{
					float randomVal = dis(gen);
					float sum = 0.0f;
					
					for (auto& r : vectorRules)
					{
						sum += r.probability;
						if (randomVal <= sum)
						{
							ourRule = r;
							break;
						}

					}

				}
				nextString += ourRule.successor;

			}
		}
		maxComplexity = std::fmax(std::ceil((float)nextString.length() / (float)currentString.length()),maxComplexity);
		currentString = std::move(nextString);
		
	}
	cacheIterations = iterations;
	cache = currentString;
	return currentString;
}

void LSystemsGenerator::Invalidate()
{
	cache.reset();
	cacheIterations = -1;
}
