//
// Created by jjfan on 4/3/2024.
//

#ifndef PROJECT_RELATION_H
#define PROJECT_RELATION_H

#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class relation {
    public:
        relation();
        relation(const string& n);
        relation(vector<string>& attrs);
        relation(const string& n, vector<string>& attrs);
        relation(const relation& other);
        ~relation();
        string getName() const;
        void setName(string n);
        int getRowCount() const;
        int getColumnCount() const;
        int getColumnIndex(const string& attr) const;
        vector<string> getAttributes() const;
        bool addAttribute(const string& attr);
        bool addAttributes(vector<string>& attrs);
        vector<vector<int>> getData() const;
        void insertTuple(vector<int>& tup);
        const vector<int> & getTuple(unsigned int idx) const;
        relation project(const string& attr) const;
        relation project(vector<string>& attrs) const;
        string toString() const;
        relation& operator=(const relation& other);
        friend std::ostream& operator<<(std::ostream& os, relation const& r);
        relation naturalJoin(const relation& other) const;
        relation semiJoin(const relation& other) const;
        static relation executeLineJoin(const vector<relation>& relations);
        static relation executeLineJoinByChaining(const vector<relation>& relations);

    private:
        unordered_map<int, vector<int>> buildMapForAttr(const string& attr) const;
        bool areRowsJoinable(int thisRow, int otherRow, const relation& other, unordered_set<string>& sharedAttr) const;
        vector<int> mergeRows(int thisRow, int otherRow, const relation& other, const vector<string>& unionAttr) const;
        int max_element(int col) const;

        static string rowToString(const vector<int>& row, const vector<int>& widths);
        static string rowToString(const vector<string>& row, const vector<int>& widths);
        static unordered_set<string> getIntersection(vector<string> v1, vector<string> v2);
        static int sum(vector<int>& widths);

        /* private structs */
        // Hash function
        struct hashFunction
        {
            size_t operator()(const vector<int>& myVector) const
            {
                std::hash<int> hasher;
                size_t answer = 0;

                for (int i : myVector)
                {
                    answer ^= hasher(i) + 0x9e3779b9 + (answer << 6) + (answer >> 2);
                }
                return answer;
            }
        };

        /* properties */
        string name;
        map<string, int> attributes;
        vector<vector<int>> data;
};


#endif //PROJECT_RELATION_H
