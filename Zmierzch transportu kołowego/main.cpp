#include <iostream>
#include <vector>

struct Cost
{
	// koszt poddrzewa jeśli zatrudnimy policjanta
	unsigned long long withPoliceman = 0;
	// koszt poddrzewa jeśli nie zatrudnimy policjanta
	unsigned long long withoutPoliceman = 0;
	// czy poddrzewo policzone?
	bool calculated = false;
	// czy policjant zatrudniody w tym wierzchołku?
	bool taken = false;
};

Cost CalculateRecursiveCost(unsigned v, unsigned parent, const std::vector<std::vector<unsigned>>& graph,
	std::vector<Cost> & DP, const std::vector<unsigned> & cost)
{
	// już zostaliśmy policzeni mozemy zwrócić wartość tablicy dynamic programing
	if (DP[v].calculated == true)return DP[v];

	// jesli mamy byc policjantem to na pewno startujemy z kosztu zatrudnienia tego policjanta
	DP[v].withPoliceman = cost[v];

	// koszt bez policjanta bedzie zalezny TYLKO od naszych sąsiadów
	DP[v].withoutPoliceman = 0;

	for (unsigned nei : graph[v])
	{
		if (nei == parent)continue;
		// optymalny koszt poddrzewa naszego sąsiada
		Cost neiCost = CalculateRecursiveCost(nei, v, graph, DP, cost);

		//v is police, neighbour can be both
		DP[v].withPoliceman += std::min(neiCost.withoutPoliceman, neiCost.withPoliceman);

		//v is not police, neighbour MUST be police
		DP[v].withoutPoliceman += neiCost.withPoliceman;
	}

	// wierzchołek V jest policzony i minimalny
	DP[v].calculated = true;

	return DP[v];
}

void CheckTaken(unsigned v, unsigned parent, bool isTaken, const std::vector<std::vector<unsigned>>& graph, std::vector<Cost> & DP)
{
	DP[v].taken = isTaken;

	for (unsigned nei : graph[v])
	{
		if (nei == parent)continue;

		// jesli nie jest taken to znaczy ze wszyscy sasiedzi są taken
		if (isTaken == false)
			CheckTaken(nei, v, true, graph, DP);
		else
		{
			// jesli jest taken to znaczy ze sąsiad moze być czymkolwiek - tym co jest mniejsze
			if (DP[nei].withPoliceman <= DP[nei].withoutPoliceman)
				// lepiej jest dla sąsiada być taken
				CheckTaken(nei, v, true, graph, DP);
			else
				// lepiej jest dla sąsiada nie być taken
				CheckTaken(nei, v, false, graph, DP);
		}	
	}
}

void Solve()
{
	unsigned N = 0;
	std::cin >> N;

	std::vector<unsigned> cost;
	cost.resize(N);

	std::vector<Cost> DP;
	DP.resize(N);

	//koszt utrzymania policjantów
	for (unsigned i = 0; i < N; ++i)std::cin >> cost[i];


	std::vector<std::vector<unsigned>> graph;
	graph.resize(N);

	unsigned inputA = 0;
	unsigned inputB = 0;

	for (unsigned i = 0; i < N - 1; ++i)
	{
		std::cin >> inputA >> inputB;
		--inputA;
		--inputB;

		graph[inputA].push_back(inputB);
		graph[inputB].push_back(inputA);
	}

	CalculateRecursiveCost(0, 0, graph, DP, cost);
	CheckTaken(0, 0, (DP[0].withPoliceman <= DP[0].withoutPoliceman ? true : false), graph, DP);


	std::cout << std::min(DP[0].withPoliceman, DP[0].withoutPoliceman) << std::endl;

	for (unsigned int i = 0; i < DP.size(); ++i)
	{
		std::cout << (DP[i].taken ? "1" : "0");
	}
	std::cout << std::endl;
}

int main()
{
	unsigned Z = 0;
	std::cin >> Z;
	while (Z-- > 0)Solve();

	return 0;
}