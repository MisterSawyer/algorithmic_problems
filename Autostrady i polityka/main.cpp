#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>
#include <unordered_map>

//sumaryczna liczba negatywnych pozwoleñ
int nNegativePermisions = 0;

// klasa która z pary (X,Y) -> liczy unikalny hash, potrzebne do stworzenia unordered_map
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        // XOR (^) jest przemienny wiêc X^Y = Y^X
        // dzieki temu para (X,Y) ma taki sam hash jak (Y,X)
        auto hash = std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        return hash;
    }
};

//klasa która porównuje czy dana para jest równa innej parze:
// konkretnie czy zawiera w sobie te same3 wartoœci (X, Y) bez wzglêdu na kolejnoœæ
struct pair_equal
{
    template <class T1, class T2>
    constexpr bool operator()(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const
    {
        // porównujemy to sprawdzaj¹c czy mniejsze wartosci s¹ sobie równe
        // i czy wiêksze s¹ sobie równe
        if (std::min(lhs.first, lhs.second) != std::min(rhs.first, rhs.second))return false;
        if (std::max(lhs.first, lhs.second) != std::max(rhs.first, rhs.second))return false;

        return true;
    }
};

void ChangeMajor(const unsigned city,
    const std::vector<std::vector<unsigned>>& cityConnections,
    std::unordered_map<std::pair<unsigned, unsigned>, bool, pair_hash, pair_equal>& cityHighwayPermision)
{
    // przechodzimy siê po wszystkich s¹siadach danego miasta
    for (const unsigned& neighbourCity : cityConnections[city])
    {
        // je¿eli to by³o negatywne pozwolenie - to zaraz je zmienimy na pozytywne
        // wiec sumaryczna ilosc negatywnych pozwoleñ maleje o jeden
        if (cityHighwayPermision.at(std::make_pair(city, neighbourCity)) == false)nNegativePermisions--;
        // w przeciwnym wypadku by³o to pozytywne po³¹czenie wiec sumaryczna ilosc negatywych zaraz uroœnie
        else nNegativePermisions++;

        //zmieniamy pozwolenie na budowê na przeciwne
        cityHighwayPermision[std::make_pair(city, neighbourCity)] = !cityHighwayPermision[std::make_pair(city, neighbourCity)];
    }
}

void Solve()
{
    // iloœæ negatywnych pozwoleñ
    nNegativePermisions = 0;

    // iloœæ miast
    unsigned nCities;

    //iloœæ autostrad
    unsigned nHighways;

    //wczytywanie danych
    std::cin >> nCities >> nHighways;

    //po³¹czenia miêdzy miastami
    std::vector<std::vector<unsigned>> cityConnections;

    //pozwolenia na budowê autostrady
    std::unordered_map<std::pair<unsigned int, unsigned int>, bool, pair_hash, pair_equal> cityHighwayPermision;

    // wiemy na pewno ¿e miast jest nCities mo¿emy wiec zarezerwowaæ taki wektor
    cityConnections.resize(nCities);
    cityHighwayPermision.reserve(nHighways);

    unsigned inputCityA = 0;
    unsigned inputCityB = 0;
    char inputPermission = 0;
    for (unsigned i = 0; i < nHighways; ++i)
    {
        std::cin >> inputCityA >> inputCityB >> inputPermission;

        // ³¹czymy miasta
        // wartoœci wejœciowe numerowane s¹ od 1 - st¹d te¿ to --cityA oraz --cityB
        cityConnections[--inputCityA].push_back(--inputCityB);
        cityConnections[inputCityB].push_back(inputCityA);

        cityHighwayPermision[std::make_pair(inputCityA, inputCityB)] = inputPermission == '+' ? true : false;

        // jeœli by³o to negatywne pozwolenie to zwiekszamy liczbe sumrycznych negatywnych pozwoleñ
        if (cityHighwayPermision[std::make_pair(inputCityA, inputCityB)] == false)nNegativePermisions++;
    }

    //Tablica ju¿ sprawdzonych miast
    bool* checked = new bool[nCities];
    //zerujmey tablicê
    std::memset(checked, 0, nCities * sizeof(bool));

    //lista miast do sprawdzenia
    std::list<unsigned> toCheck;

    // dodajemy pocz¹tkowe miasto
    toCheck.push_front(0);
    checked[0] = true;

    // lista s¹siadów do zmiany
    std::vector<unsigned> answer;

    // maksymalnie mo¿emy zmieniæ nCities s¹siadów, wiec od razu mo¿emy sobie tyle zarezerwowaæ
    answer.reserve(nCities);

    while (toCheck.size() > 0)
    {
        // wybieramy pierwsze miasto z kolejki do sprawdzenia
        unsigned city = toCheck.front();

        // usuwamy z kolejki
        toCheck.pop_front();

        // przechodzimy siê po wszystkich s¹siaduj¹cych z 'city' miastach
        for (const unsigned& neighbourCity : cityConnections[city])
        {
            // jeœli mamy 0 liczbê negatywnych pozwoleñ to ju¿ nie musimy przegl¹daæ dalej
            // s¹siadów tego miasta
            if (nNegativePermisions == 0)break;

            // jeœli dany s¹siad ju¿ zosta³ sprawdzony to go pomijamy
            if (checked[neighbourCity])continue;

            // w innym przypadku dodajemy s¹siada !na koniec! (BFS) kolejki do sprawdzenia
            toCheck.push_back(neighbourCity);

            // i mówimy ¿e zosta³ sprawdzony (aby nie dodawaæ go wielokrotnie do kolejki)
            checked[neighbourCity] = true;

            // jeœli siê oka¿e ¿e pomiêdzy nami a s¹siadem nie mo¿na zbudowaæ autostrady
            if (cityHighwayPermision.at({ city, neighbourCity }) == false)
            {
                // to musimy zmieniæ burmistrza u s¹siada
                // nie mo¿emy tego zrobiæ u nas
                // poniewa¿ nie mo¿na zmieniaæ burmistrza w miescie nr. 1 od którego zaczeliœmy
                // wiêc indukcj¹ matematyczn¹ mo¿emy pokazaæ
                // ¿e musimy zmieniaæ s¹siadów w kierunku "na zewn¹trz" od aktualnie sprawdzanego miasta
                answer.push_back(neighbourCity + 1); // plus 1 poniewa¿ liczymy od 0

                // funkcja zmienia burmistrza u s¹siada jednoczeœnie modyfikuj¹c na przeciwne
                // wszystkie pozwolenia na drogi wychodz¹ce z tego miasta
                ChangeMajor(neighbourCity, cityConnections, cityHighwayPermision);
            }
        }
        // jeœli  mamy 0 liczbê negatywnych pozwoleñ to ju¿ nawet nie musimy przegl¹daæ dalej
        // innych miast w ogóle
        if (nNegativePermisions == 0)break;
    }

    delete[] checked;


    //jeœli sumaryczna liczba pozowleñ jest ró¿na od zera, to nie da sie zmieniæ tak burmistrzów
    if (nNegativePermisions != 0) std::cout << "NIE";
    else
    {
        //odpowiedŸ ma byæ w porz¹dku rosn¹cym
        std::sort(answer.begin(), answer.end());

        // wypisujemy nasz¹ odpowiedŸ
        for (const auto& city : answer)
            std::cout << city << " ";
    }
    std::cout << "\n";
}

int main()
{
    unsigned Z;
    std::cin >> Z;

    while (Z-- > 0)Solve();

    return 0;
}