#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>
#include <unordered_map>

//sumaryczna liczba negatywnych pozwole�
int nNegativePermisions = 0;

// klasa kt�ra z pary (X,Y) -> liczy unikalny hash, potrzebne do stworzenia unordered_map
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        // XOR (^) jest przemienny wi�c X^Y = Y^X
        // dzieki temu para (X,Y) ma taki sam hash jak (Y,X)
        auto hash = std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        return hash;
    }
};

//klasa kt�ra por�wnuje czy dana para jest r�wna innej parze:
// konkretnie czy zawiera w sobie te same3 warto�ci (X, Y) bez wzgl�du na kolejno��
struct pair_equal
{
    template <class T1, class T2>
    constexpr bool operator()(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const
    {
        // por�wnujemy to sprawdzaj�c czy mniejsze wartosci s� sobie r�wne
        // i czy wi�ksze s� sobie r�wne
        if (std::min(lhs.first, lhs.second) != std::min(rhs.first, rhs.second))return false;
        if (std::max(lhs.first, lhs.second) != std::max(rhs.first, rhs.second))return false;

        return true;
    }
};

void ChangeMajor(const unsigned city,
    const std::vector<std::vector<unsigned>>& cityConnections,
    std::unordered_map<std::pair<unsigned, unsigned>, bool, pair_hash, pair_equal>& cityHighwayPermision)
{
    // przechodzimy si� po wszystkich s�siadach danego miasta
    for (const unsigned& neighbourCity : cityConnections[city])
    {
        // je�eli to by�o negatywne pozwolenie - to zaraz je zmienimy na pozytywne
        // wiec sumaryczna ilosc negatywnych pozwole� maleje o jeden
        if (cityHighwayPermision.at(std::make_pair(city, neighbourCity)) == false)nNegativePermisions--;
        // w przeciwnym wypadku by�o to pozytywne po��czenie wiec sumaryczna ilosc negatywych zaraz uro�nie
        else nNegativePermisions++;

        //zmieniamy pozwolenie na budow� na przeciwne
        cityHighwayPermision[std::make_pair(city, neighbourCity)] = !cityHighwayPermision[std::make_pair(city, neighbourCity)];
    }
}

void Solve()
{
    // ilo�� negatywnych pozwole�
    nNegativePermisions = 0;

    // ilo�� miast
    unsigned nCities;

    //ilo�� autostrad
    unsigned nHighways;

    //wczytywanie danych
    std::cin >> nCities >> nHighways;

    //po��czenia mi�dzy miastami
    std::vector<std::vector<unsigned>> cityConnections;

    //pozwolenia na budow� autostrady
    std::unordered_map<std::pair<unsigned int, unsigned int>, bool, pair_hash, pair_equal> cityHighwayPermision;

    // wiemy na pewno �e miast jest nCities mo�emy wiec zarezerwowa� taki wektor
    cityConnections.resize(nCities);
    cityHighwayPermision.reserve(nHighways);

    unsigned inputCityA = 0;
    unsigned inputCityB = 0;
    char inputPermission = 0;
    for (unsigned i = 0; i < nHighways; ++i)
    {
        std::cin >> inputCityA >> inputCityB >> inputPermission;

        // ��czymy miasta
        // warto�ci wej�ciowe numerowane s� od 1 - st�d te� to --cityA oraz --cityB
        cityConnections[--inputCityA].push_back(--inputCityB);
        cityConnections[inputCityB].push_back(inputCityA);

        cityHighwayPermision[std::make_pair(inputCityA, inputCityB)] = inputPermission == '+' ? true : false;

        // je�li by�o to negatywne pozwolenie to zwiekszamy liczbe sumrycznych negatywnych pozwole�
        if (cityHighwayPermision[std::make_pair(inputCityA, inputCityB)] == false)nNegativePermisions++;
    }

    //Tablica ju� sprawdzonych miast
    bool* checked = new bool[nCities];
    //zerujmey tablic�
    std::memset(checked, 0, nCities * sizeof(bool));

    //lista miast do sprawdzenia
    std::list<unsigned> toCheck;

    // dodajemy pocz�tkowe miasto
    toCheck.push_front(0);
    checked[0] = true;

    // lista s�siad�w do zmiany
    std::vector<unsigned> answer;

    // maksymalnie mo�emy zmieni� nCities s�siad�w, wiec od razu mo�emy sobie tyle zarezerwowa�
    answer.reserve(nCities);

    while (toCheck.size() > 0)
    {
        // wybieramy pierwsze miasto z kolejki do sprawdzenia
        unsigned city = toCheck.front();

        // usuwamy z kolejki
        toCheck.pop_front();

        // przechodzimy si� po wszystkich s�siaduj�cych z 'city' miastach
        for (const unsigned& neighbourCity : cityConnections[city])
        {
            // je�li mamy 0 liczb� negatywnych pozwole� to ju� nie musimy przegl�da� dalej
            // s�siad�w tego miasta
            if (nNegativePermisions == 0)break;

            // je�li dany s�siad ju� zosta� sprawdzony to go pomijamy
            if (checked[neighbourCity])continue;

            // w innym przypadku dodajemy s�siada !na koniec! (BFS) kolejki do sprawdzenia
            toCheck.push_back(neighbourCity);

            // i m�wimy �e zosta� sprawdzony (aby nie dodawa� go wielokrotnie do kolejki)
            checked[neighbourCity] = true;

            // je�li si� oka�e �e pomi�dzy nami a s�siadem nie mo�na zbudowa� autostrady
            if (cityHighwayPermision.at({ city, neighbourCity }) == false)
            {
                // to musimy zmieni� burmistrza u s�siada
                // nie mo�emy tego zrobi� u nas
                // poniewa� nie mo�na zmienia� burmistrza w miescie nr. 1 od kt�rego zaczeli�my
                // wi�c indukcj� matematyczn� mo�emy pokaza�
                // �e musimy zmienia� s�siad�w w kierunku "na zewn�trz" od aktualnie sprawdzanego miasta
                answer.push_back(neighbourCity + 1); // plus 1 poniewa� liczymy od 0

                // funkcja zmienia burmistrza u s�siada jednocze�nie modyfikuj�c na przeciwne
                // wszystkie pozwolenia na drogi wychodz�ce z tego miasta
                ChangeMajor(neighbourCity, cityConnections, cityHighwayPermision);
            }
        }
        // je�li  mamy 0 liczb� negatywnych pozwole� to ju� nawet nie musimy przegl�da� dalej
        // innych miast w og�le
        if (nNegativePermisions == 0)break;
    }

    delete[] checked;


    //je�li sumaryczna liczba pozowle� jest r�na od zera, to nie da sie zmieni� tak burmistrz�w
    if (nNegativePermisions != 0) std::cout << "NIE";
    else
    {
        //odpowied� ma by� w porz�dku rosn�cym
        std::sort(answer.begin(), answer.end());

        // wypisujemy nasz� odpowied�
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