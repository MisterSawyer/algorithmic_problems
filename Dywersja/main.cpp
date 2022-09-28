#include <iostream>
#include <list>
#include <vector>
#include <utility>

struct Enters {
    std::vector<unsigned int>In[4];
};

bool CheckSolution(const std::vector<std::vector<bool>>& Solution) {
    for (unsigned int i = 0; i < Solution.size(); i++) {
        for (unsigned int ii = i + 1; ii < Solution[i].size(); ii++) {
            if (Solution[i][ii] == false)return false;
        }
    }
    return true;
}

void Solve() {
    unsigned int NumberOfPoints;
    std::cin >> NumberOfPoints;
    std::vector<Enters> PointEnters;
    PointEnters.reserve(NumberOfPoints);
    PointEnters.resize(NumberOfPoints);

    unsigned input;
    for (unsigned i = 0; i < NumberOfPoints; i++) {
        for (unsigned int c = 0; c < 4; c++) {
            std::cin >> input;
            input--;
            PointEnters[input].In[c].push_back(i);
        }
    }

    std::vector<std::vector<bool>> Solution;
    Solution.resize(NumberOfPoints);
    for (unsigned i = 0; i < NumberOfPoints; i++) {
        Solution[i].resize(NumberOfPoints, false);
    }

    std::list<std::pair<unsigned, unsigned>> SolutionQueue;
    for (unsigned i = 0; i < NumberOfPoints; i++) {
        SolutionQueue.push_back(std::make_pair(i, i));
        Solution[i][i] = true;
    }

    while (SolutionQueue.empty() == false) {
        std::pair<unsigned, unsigned> ActualSolution = SolutionQueue.front();
        SolutionQueue.pop_front();
        for (unsigned dir = 0; dir < 4; dir++)
        {
            for (unsigned i = 0; i < PointEnters[ActualSolution.first].In[dir].size(); i++)
            {
                for (unsigned ii = 0; ii < PointEnters[ActualSolution.second].In[dir].size(); ii++)
                {
                    unsigned startfirst = PointEnters[ActualSolution.first].In[dir][i];
                    unsigned startsecond = PointEnters[ActualSolution.second].In[dir][ii];

                    if (Solution[startfirst][startsecond] == true) continue;

                    Solution[startfirst][startsecond] = true;
                    Solution[startsecond][startfirst] = true;
                    SolutionQueue.push_back(std::make_pair(startfirst, startsecond));
                }
            }
        }
    }

    if (CheckSolution(Solution) == true)
        std::cout << "TAK";
    else
        std::cout << "NIE";
}

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    unsigned int Z;
    std::cin >> Z;
    for (Z; Z > 0; Z--)
    {
        Solve();
        std::cout << std::endl;
    }
}