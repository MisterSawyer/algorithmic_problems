#include <iostream>
#include <vector>
#include <queue>

struct QueueEntry {
    QueueEntry(double initialVotes_, unsigned int party_, unsigned int divider_)
    {
        initialVotes = initialVotes_;
        party = party_;
        divider = divider_;
        votes = initialVotes / (double)divider;
    }

    double votes;
    unsigned int party;
    unsigned int divider;
    double initialVotes;
};

struct Compare {
    bool operator()(const QueueEntry& a, QueueEntry& b)
    {
        if (a.votes > b.votes)return false;
        if (a.votes < b.votes)return true;
        if (a.divider > b.divider)return false;
        if (a.divider < b.divider)return true;
        if (a.party < b.party)return false;
        return true;
    }
};

void Solve()
{
    unsigned int M;
    unsigned int N;
    std::cin >> N >> N;

    std::priority_queue<QueueEntry, std::vector<QueueEntry>, Compare>mandatsDivisor;

    unsigned int votes;
    for (unsigned int i = 0; i < N; i++)
    {
        std::cin >> votes;
        mandatsDivisor.push(QueueEntry(votes, i, 1));
    }

    std::vector<unsigned int>partyMandates;
    partyMandates.resize(N);

    for (unsigned int i = 0; i < M; i++)
    {
        QueueEntry entry = mandatsDivisor.top();
        partyMandates[entry.party] = entry.divider;
        mandatsDivisor.pop();

        mandatsDivisor.push(QueueEntry(entry.initialVotes, entry.party, entry.divider + 1));
    }

    for (unsigned int i = 0; i < partyMandates.size(); i++)
    {
        std::cout << partyMandates[i] << std::endl;
    }
}

int main() {

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    unsigned int Z = 0;
    std::cin >> Z;
    for (Z; Z > 0; Z--)Solve();

    return 0;
}