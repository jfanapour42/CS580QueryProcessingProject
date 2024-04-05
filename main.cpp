#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>
#include "relation.h"

using namespace std;

/*
 * Some globals for random number generation

std::random_device rd;*/

/*
 * Definitions for time keeping
 */
typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::microseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

/*
 * Returns the amount of time taken to execute a function in microseconds and stores
 * the return value of func into result.
 */
template<typename F, typename... Args, typename R>
double funcTime(F func, R& result, Args&&... args) {
    // Measure the time it takes to execute func
    TimeVar t1 = timeNow();
    result = func(std::forward<Args>(args)...);
    double time = duration(timeNow()-t1);

    return time;
}

/*
 * Returns random integer in range from lb to ub inclusive.
 */
int getRandomInt(int lb, int ub) {
    unsigned seed = timeNow().time_since_epoch().count();
    srand(seed);

    return (rand() % (ub - lb + 1)) + lb;
}

/*
 * Shuffles input vector resulting in random permutation of it
 */
template <typename T>
void shuffleVector(vector<T>& vec) {
    // obtain a time-based seed:
    //unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    unsigned seed = timeNow().time_since_epoch().count();
    default_random_engine e(seed);

    shuffle(vec.begin(), vec.end(), e);
}

void executeProblem1Experiments() {
    // Populate vec1
    vector<vector<int>> vec1
    {
        {1, 2},
        {4, 5},
        {7, 8},
        {10, 9},
        {1, 3},
        {2, 2},
        {11, 3},
        {6, 7},
        {11, 5},
        {12, 15}
    };
    // Populate vec2
    vector<vector<int>> vec2
    {
        {2, 4},
        {5, 2},
        {7, 8},
        {13, 9},
        {1, 3},
        {5, 9},
        {15, 6},
        {3, 7},
        {3, 5},
        {9, 4}
    };

    vector<string> attrs1{"A", "B"};
    vector<string> attrs2{"B", "C"};
    relation r1(attrs1);
    relation r2(attrs2);

    // Populate relations
    for (vector<int> v : vec1) {
        r1.insertTuple(v);
    }
    for (vector<int> v : vec2) {
        r2.insertTuple(v);
    }

    relation r3 = r1.naturalJoin(r2);

    cout << "Relation R1: " << endl;
    cout << r1 << endl;
    cout << "Relation R2: " << endl;
    cout << r2 << endl;
    cout << "Join between relations R1 and R2: " << endl;
    cout << r3 << endl;
}

void executeProblem2Experiments() {
    // populate vec1
    vector<vector<int>> vec1
    {
        {1, 2},
        {4, 5},
        {7, 8},
        {10, 9},
        {1, 3},
        {2, 2},
        {11, 3},
        {6, 7},
        {11, 5},
        {12, 15}
    };
    // populate vec2
    vector<vector<int>> vec2
    {
        {2, 4},
        {5, 2},
        {7, 8},
        {13, 9},
        {1, 3},
        {5, 9},
        {15, 6},
        {3, 7},
        {3, 5},
        {9, 4}
    };
    // populate vec3
    vector<vector<int>> vec3
    {
        {4, 8},
        {5, 2},
        {8, 9},
        {6, 21},
        {1, 3},
        {10, 2},
        {9, 20},
        {3, 8},
        {3, 12},
        {9, 15}
    };
    // populate vec4
    vector<vector<int>> vec4
    {
        {8, 16},
        {8, 1},
        {7, 12},
        {15, 37},
        {3, 3},
        {12, 7},
        {21, 6},
        {2, 4},
        {3, 5},
        {20, 5}
    };

    vector<string> attrs1{"A", "B"};
    vector<string> attrs2{"B", "C"};
    vector<string> attrs3{"C", "D"};
    vector<string> attrs4{"D", "E"};
    relation r1(attrs1);
    relation r2(attrs2);
    relation r3(attrs3);
    relation r4(attrs4);

    // Populate relations
    for (vector<int> v : vec1) {
        r1.insertTuple(v);
    }
    for (vector<int> v : vec2) {
        r2.insertTuple(v);
    }
    for (vector<int> v : vec3) {
        r3.insertTuple(v);
    }
    for (vector<int> v : vec4) {
        r4.insertTuple(v);
    }

    relation chain = r1.naturalJoin(r2).naturalJoin(r3).naturalJoin(r4);

    vector<relation> lineQuery{r1, r2, r3, r4};
    relation lineJoinResult = relation::executeLineJoin(lineQuery);

    cout << "Relation R1: " << endl;
    cout << r1 << endl;
    cout << "Relation R2: " << endl;
    cout << r2 << endl;
    cout << "Relation R3: " << endl;
    cout << r3 << endl;
    cout << "Relation R4: " << endl;
    cout << r4 << endl;
    cout << "Result of chain of natural joins of R1, R2, R3, & R4: " << endl;
    cout << chain << endl;
    cout << "Result of line join using Yanankakis algorithm (Problem 2) of R1, R2, R3, & R4: " << endl;
    cout << lineJoinResult << endl;
}

void executeProblem3Experiments() {
    // populate vec1
    vector<vector<int>> vec1
    {
        {1, 2},
        {4, 5},
        {7, 8},
        {10, 9},
        {1, 3},
        {2, 2},
        {11, 3},
        {6, 7},
        {11, 5},
        {12, 15}
    };
    // populate vec2
    vector<vector<int>> vec2
    {
        {2, 4},
        {5, 2},
        {7, 8},
        {13, 9},
        {1, 3},
        {5, 9},
        {15, 6},
        {3, 7},
        {3, 5},
        {9, 4}
    };
    // populate vec3
    vector<vector<int>> vec3
    {
        {4, 8},
        {5, 2},
        {8, 9},
        {6, 21},
        {1, 3},
        {10, 2},
        {9, 20},
        {3, 8},
        {3, 12},
        {9, 15}
    };
    // populate vec4
    vector<vector<int>> vec4
    {
        {8, 16},
        {8, 1},
        {7, 12},
        {15, 37},
        {3, 3},
        {12, 7},
        {21, 6},
        {2, 4},
        {3, 5},
        {20, 5}
    };

    vector<string> attrs1{"A", "B"};
    vector<string> attrs2{"B", "C"};
    vector<string> attrs3{"C", "D"};
    vector<string> attrs4{"D", "E"};
    relation r1(attrs1);
    relation r2(attrs2);
    relation r3(attrs3);
    relation r4(attrs4);

    // Populate relations
    for (vector<int> v : vec1) {
        r1.insertTuple(v);
    }
    for (vector<int> v : vec2) {
        r2.insertTuple(v);
    }
    for (vector<int> v : vec3) {
        r3.insertTuple(v);
    }
    for (vector<int> v : vec4) {
        r4.insertTuple(v);
    }

    relation chain = r1.naturalJoin(r2).naturalJoin(r3).naturalJoin(r4);

    vector<relation> lineQuery{r1, r2, r3, r4};
    relation lineJoinResult = relation::executeLineJoinByChaining(lineQuery);

    cout << "Relation R1: " << endl;
    cout << r1 << endl;
    cout << "Relation R2: " << endl;
    cout << r2 << endl;
    cout << "Relation R3: " << endl;
    cout << r3 << endl;
    cout << "Relation R4: " << endl;
    cout << r4 << endl;
    cout << "Result of chain of natural joins of R1, R2, R3, & R4: " << endl;
    cout << chain << endl;
    cout << "Result of line join by chaining (Problem 3) of R1, R2, R3, & R4: " << endl;
    cout << lineJoinResult << endl;
}

void executeProblem4Experiments() {
    vector<vector<int>> vec1, vec2, vec3;

    // populate vec1
    for (int i = 1; i <= 100; i++) {
        vector<int> tup;
        tup.push_back(i);
        tup.push_back(getRandomInt(1, 5000));
        vec1.push_back(tup);
    }
    // populate vec2
    for (int i = 1; i <= 100; i++) {
        vector<int> tup;
        tup.push_back(getRandomInt(1, 5000));
        tup.push_back(i);
        vec2.push_back(tup);
    }
    // populate vec3
    for (int i = 1; i <= 100; i++) {
        vector<int> tup;
        tup.push_back(i);
        tup.push_back(i);
        vec3.push_back(tup);
    }

    vector<string> attrs1{"A", "B"};
    vector<string> attrs2{"B", "C"};
    vector<string> attrs3{"C", "D"};
    relation r1(attrs1);
    relation r2(attrs2);
    relation r3(attrs3);

    // Populate relations
    for (vector<int> v : vec1) {
        r1.insertTuple(v);
    }
    for (vector<int> v : vec2) {
        r2.insertTuple(v);
    }
    for (vector<int> v : vec3) {
        r3.insertTuple(v);
    }

    vector<relation> lineQuery{r1, r2, r3};
    // Measure time taken for line join query (problem 2)
    relation lineJoinResult;
    double timeLineJoin = funcTime(relation::executeLineJoin, lineJoinResult, lineQuery);

    // Measure time taken for line join query by chaining (problem 3)
    relation lineJoinByChainingResult;
    double timeLineJoinByChaining = funcTime(relation::executeLineJoinByChaining, lineJoinByChainingResult, lineQuery);

    cout << "Time taken for line join (Problem 2): " << timeLineJoin << " microseconds." << endl;
    cout << "Time taken for line join by chaining (Problem 3): " << timeLineJoinByChaining << " microseconds."  << endl;

    cout << endl;

    if (lineJoinResult.getData() == lineJoinByChainingResult.getData()) {
        cout << "The two methods of executing the query produced equivalent results." << endl;
    } else {
        cout << "The two methods of executing the query did not produced equivalent results." << endl;
    }

    cout << "\n\n";
    cout << "Results of the line join query." << endl;
    cout << lineJoinResult << endl;

    cout << "\n\n";
    cout << "Results of the line join query by chaining." << endl;
    cout << lineJoinByChainingResult << endl;
}

void executeProblem5Experiments() {
    vector<vector<int>> vec1, vec2, vec3;

    // populate vec1
    for (int i = 1; i <= 1000; i++) {
        vector<int> tup;
        tup.push_back(i);
        tup.push_back(5);
        vec1.push_back(tup);
    }
    for (int i = 1001; i <= 2000; i++) {
        vector<int> tup;
        tup.push_back(i);
        tup.push_back(7);
        vec1.push_back(tup);
    }
    vector<int> temp1{2001,2002};
    vec1.push_back(temp1);
    // populate vec2
    for (int i = 1; i <= 1000; i++) {
        vector<int> tup;
        tup.push_back(5);
        tup.push_back(i);
        vec2.push_back(tup);
    }
    for (int i = 1001; i <= 2000; i++) {
        vector<int> tup;
        tup.push_back(7);
        tup.push_back(i);
        vec2.push_back(tup);
    }
    vector<int> temp2{2002,8};
    vec2.push_back(temp2);
    // populate vec3
    for (int i = 1; i <= 2000; i++) {
        vector<int> tup;
        tup.push_back(getRandomInt(2002, 3000));
        tup.push_back(getRandomInt(1, 3000));
        vec3.push_back(tup);
    }
    vector<int> temp3{8, 30};
    vec3.push_back(temp3);

    // Shuffle vec1, vec2, & vec3
    shuffleVector(vec1);
    shuffleVector(vec2);
    shuffleVector(vec3);

    vector<string> attrs1{"A", "B"};
    vector<string> attrs2{"B", "C"};
    vector<string> attrs3{"C", "D"};
    relation r1(attrs1);
    relation r2(attrs2);
    relation r3(attrs3);

    // Populate relations
    for (vector<int> v : vec1) {
        r1.insertTuple(v);
    }
    for (vector<int> v : vec2) {
        r2.insertTuple(v);
    }
    for (vector<int> v : vec3) {
        r3.insertTuple(v);
    }

    vector<relation> lineQuery{r1, r2, r3};
    // Measure time taken for line join query (problem 2)
    relation lineJoinResult;
    double timeLineJoin = funcTime(relation::executeLineJoin, lineJoinResult, lineQuery);

    // Measure time taken for line join query by chaining (problem 3)
    relation lineJoinByChainingResult;
    double timeLineJoinByChaining = funcTime(relation::executeLineJoinByChaining, lineJoinByChainingResult, lineQuery);

    cout << "Time taken for line join (Problem 2): " << timeLineJoin << " microseconds." << endl;
    cout << "Time taken for line join by chaining (Problem 3): " << timeLineJoinByChaining << " microseconds."  << endl;

    cout << endl;

    if (lineJoinResult.getData() == lineJoinByChainingResult.getData()) {
        cout << "The two methods of executing the query produced equivalent results." << endl;
    } else {
        cout << "The two methods of executing the query did not produced equivalent results." << endl;
    }

    cout << "\n\n";
    cout << "Results of the line join query." << endl;
    cout << lineJoinResult << endl;

    cout << "\n\n";
    cout << "Results of the line join query by chaining." << endl;
    cout << lineJoinByChainingResult << endl;
}

int main() {
    cout << "Output for experiment 1: " << endl;
    cout << "\n\n\n";
    executeProblem1Experiments();
    cout << "\n\n\n";
    cout << "Output for experiment 2: " << endl;
    cout << "\n\n\n";
    executeProblem2Experiments();
    cout << "\n\n\n";
    cout << "Output for experiment 3: " << endl;
    cout << "\n\n\n";
    executeProblem3Experiments();
    cout << "\n\n\n";
    cout << "Output for experiment 4: " << endl;
    cout << "\n\n\n";
    executeProblem4Experiments();
    cout << "\n\n\n";
    cout << "Output for experiment 5: " << endl;
    cout << "\n\n\n";
    executeProblem5Experiments();
}
