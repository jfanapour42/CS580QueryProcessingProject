//
// Created by jjfan on 4/3/2024.
//

#include <iostream>
#include <unordered_map>
#include <climits>
#include "relation.h"

relation::relation() {
    this->name = "";
}

relation::relation(const string& n) {
    this->name = n;
}

relation::relation(vector<string>& attrs) {
    this->name = "";

    int idx = 0;
    for (string attr : attrs) {
        this->attributes.emplace(attr, idx);
        idx++;
    }
}

relation::relation(const string& n, vector<string>& attrs) {
    this->name = n;

    int idx = 0;
    for (string attr : attrs) {
        this->attributes.emplace(attr, idx);
        idx++;
    }
}

relation::relation(const relation& other) {
    this->name = other.name;
    this->attributes = other.attributes;
    this->data = other.data;
}

relation::~relation() = default;

string relation::getName() const {
    return this->name;
}

void relation::setName(string n) {
    this->name = n;
}

int relation::getRowCount() const {
    return this->data.size();
}

int relation::getColumnCount() const {
    return this->attributes.size();
}

int relation::getColumnIndex(const string& attr) const {
    if (this->attributes.count(attr) != 0) {
        return this->attributes.at(attr);
    }
    return -1;
}

vector<string> relation::getAttributes() const {
    vector<string> keys(this->attributes.size());

    for (const auto& kv : this->attributes) {
        string attr = kv.first;
        int idx = kv.second;
        keys[idx] = attr;
    }

    return keys;
}

bool relation::addAttribute(const string& attr) {
    if (this->attributes.count(attr) == 0) {
        int colCount = this->getColumnCount();

        this->attributes.emplace(attr, colCount);

        for (auto row : this->data) {
            row.push_back(0);
        }

        return true;
    }
    return false;
}

bool relation::addAttributes(vector<string>& attrs) {
    int colCount = this->getColumnCount();

    // Check that all strings in attrs do not already exist as attributes in relation
    for (string attr : attrs) {
        if (this->attributes.count(attr) != 0) {
            return false;
        }
    }

    for (string attr : attrs) {
        this->attributes.emplace(attr, colCount);

        for (auto row : this->data) {
            row.push_back(0);
        }

        colCount++;
    }

    return true;
}

vector<vector<int>> relation::getData() const {
    return this->data;
}

void relation::insertTuple(vector<int>& tup) {
    if (tup.size() == this->getColumnCount()) {
        this->data.push_back(tup);
    }
}

const vector<int>& relation::getTuple(unsigned int idx) const {
    if (idx < this->getRowCount() && idx >= 0) {
        return this->data.at(idx);
    } else {
        throw std::out_of_range("Index out of range!");
    }
}

relation relation::project(const string& attr) const {
    relation res;
    int colIdx = this->getColumnIndex(attr);
    int rowCount = this->getRowCount();

    if (colIdx != -1) {
        unordered_set<int> seenVals;
        res.addAttribute(attr);

        for (int i = 0; i < rowCount; i++) {
            int val = this->data[i][colIdx];
            if (seenVals.count(val) == 0) {
                seenVals.insert(val);
                vector<int> tup{val};
                res.insertTuple(tup);
            }
        }
    }

    return res;
}

relation relation::project(vector<string>& attrs) const {
    relation res;
    vector<string> validAttrs;
    vector<int> colIdxs;

    for (string attr : attrs) {
        int idx = this->getColumnIndex(attr);

        if (idx != -1) {
            validAttrs.push_back(attr);
            colIdxs.push_back(idx);
        }
    }

    if (!validAttrs.empty()) {
        unordered_set<vector<int>, relation::hashFunction> seenVals;
        res.addAttributes(validAttrs);

        for (int i = 0; i < this->getRowCount(); i++) {
            vector<int> tup;

            for (int idx : colIdxs) {
                tup.push_back(this->data[i][idx]);
            }

            if (seenVals.count(tup) == 0) {
                seenVals.insert(tup);
                res.insertTuple(tup);
            }
        }
    }

    return res;
}

string relation::toString() const {
    int colCount = this->attributes.size(), rowCount = this->getRowCount();
    vector<string> attrs(colCount);
    vector<int> widths(colCount);
    int totalWidth;
    string res;

    for (const auto& kv : this->attributes) {
        string attr = kv.first;
        int idx = kv.second;
        attrs[idx] = attr;
        widths[idx] = attr.size() + 1;
    }

    for (int i = 0; i < colCount; i++) {
        int maxVal = this->max_element(i);
        widths[i] = max((int)(to_string(maxVal).size()) + 1, widths[i]);
    }

    res += rowToString(attrs, widths) +'\n';
    totalWidth = sum(widths) + colCount - 1;

    string temp;
    for (int i = 0; i < totalWidth; i++) {
        temp += '-';
    }
    res += temp + '\n';

    for (int i = 0; i < rowCount; i++) {
        res += rowToString(this->getTuple(i), widths) + '\n';
    }

    return res;
}

relation& relation::operator=(const relation& other) {

    if (this != &other) {
        this->name = other.name;
        this->attributes = other.attributes;
        this->data = other.data;
    }
    return *this; // Return a reference to the current object
}

std::ostream& operator<<(std::ostream& os, relation const& r)
{
    return os << r.toString();
}

/*
 * Static join methods
 */

/*
 * Returns the result of a Full join between "this" relation and other relation (passed as argument)
 * The key to implementation is the creation of the hash map H on the first shared attributes between the two
 * relations. Values of the key attribute are mapped onto tuples the value occurs in within the second relation (other).
 * Once the hashmap is created, we iterate through the tuples of "this" relation and check if any tuples in the other
 * relation contain the current value for the keyed attribute using the hashmap. If this check returns tuples in the
 * other relation, we then check that the values for all other shared attributes are equivalent between the current
 * tuple in "this" relation and the tuple in the other relation. If this is true, then we can add the new merged tuple
 * to the resultant full join relation.
 */
relation relation::naturalJoin(const relation& other) const{
    vector<string> attr1 = this->getAttributes();
    vector<string> attr2 = other.getAttributes();
    unordered_set<string> sharedAttr = getIntersection(attr1, attr2);
    relation res;

    if (!sharedAttr.empty()) {
        vector<string> unionAttr = attr1;
        for (string attr : attr2) {
            if (sharedAttr.count(attr) == 0) {
                unionAttr.push_back(attr);
            }
        }
        res.addAttributes(unionAttr);

        string keyAttr = *sharedAttr.begin();
        unordered_map<int, vector<int>> valMap = other.buildMapForAttr(keyAttr);
        int thisKeyCol = this->getColumnIndex(keyAttr);

        for (int i = 0; i < this->getRowCount(); i++) {
            int thisKeyVal = this->data[i][thisKeyCol];
            if (valMap.count(thisKeyVal) != 0) {
                for (int idx : valMap[thisKeyVal]) {
                    if (this->areRowsJoinable(i, idx, other, sharedAttr)) {
                        vector<int> mergedTup = this->mergeRows(i, idx, other, unionAttr);
                        res.insertTuple(mergedTup);
                    }
                }
            }
        }
    }

    return res;
}

relation relation::semiJoin(const relation& other) const {
    vector<string> attr = this->getAttributes();
    return this->naturalJoin(other).project(attr);
}

/*
 * Evaluates the line join query of the form
 * q(A1, ... , Ak+1) :- R1(A1, A2), R2(A2, A3), ..., Rk(Ak, Ak+1)
 * Input is assumed to be k binary relations in order of line join query.
 * Input of not assumed form will produce unexpected results.
 * Algorithm executes in O(N + OUT) by performing a simplified version
 * of Yanankakis algorithm
 */
relation relation::executeLineJoin(const vector<relation>& relations) {
    int k = relations.size();
    vector<relation> prunedRelations(k);

    if (k == 0) {
        return relation();
    } else if (k == 1) {
        return relations[0];
    }

    // removes dangling tuples by performing a semi-join reduction sweep from the tail to the head
    // of the line join and then another semi-join reduction sweep from the head to the tail.
    prunedRelations[k-1] = relations[k-1];
    for (int i = k-2; i >= 0; i--) {
        relation r = relations[i].semiJoin(relations[i+1]);
        prunedRelations[i] = r;
    }

    for (int i = 1; i < k; i++) {
        relation r = prunedRelations[i].semiJoin(prunedRelations[i-1]);
        prunedRelations[i] = r;
    }

    // Join relations in post-order traversal (from tail to head)
    for (int i = k-2; i >= 0; i--) {
        relation r = prunedRelations[i].naturalJoin(prunedRelations[i+1]);
        prunedRelations[i] = r;
    }

    // the first relation will have the result of the line join query
    return prunedRelations[0];
}

/*
 * Evaluates the line join query of the form
 * q(A1, ... , Ak+1) :- R1(A1, A2), R2(A2, A3), ..., Rk(Ak, Ak+1)
 * Input is assumed to be k binary relations in order of line join query.
 * Input of not assumed form will produce unexpected results.
 * This algorithm computes the join by chaining natural joins.
 * It computes R1-2 = R1 join R2, then R1-3 = R1-2 join R3 and so. In the
 * end it computes R1-k = R1-(k-1) join Rk.
 */
relation relation::executeLineJoinByChaining(const vector<relation>& relations) {
    int k = relations.size();
    relation res;

    if (k == 0) {
        return res;
    } else if (k == 1) {
        return relations[0];
    }

    res = relations[0];

    for (int i = 1; i < k; i++) {
        res = res.naturalJoin(relations[i]);
    }

    return res;
}

/*
 * Private functions
 */

unordered_map<int, vector<int>> relation::buildMapForAttr(const string& attr) const {
    unordered_map<int, vector<int>> map;
    int colIdx = this->getColumnIndex(attr);

    for (int i = 0; i < this->getRowCount(); i++) {
        int val = this->data[i][colIdx];

        if (map.count(val) == 0) {
            vector<int> indexes;
            indexes.push_back(i);
            map[val] = indexes;
        } else {
            map[val].push_back(i);
        }
    }

    return map;
}

bool relation::areRowsJoinable(int thisRow, int otherRow, const relation& other, unordered_set<string>& sharedAttr) const {
    vector<int> thisTup = this->getTuple(thisRow);
    vector<int> otherTup = other.getTuple(otherRow);

    for (string attr : sharedAttr) {
        int thisCol = this->getColumnIndex(attr);
        int otherCol = other.getColumnIndex(attr);

        if (thisTup[thisCol] != otherTup[otherCol])
            return false;
    }

    return true;
}

vector<int> relation::mergeRows(int thisRow, int otherRow, const relation& other, const vector<string>& unionAttr) const {
    vector<int> res;

    for (string attr : unionAttr) {
        int thisCol = this->getColumnIndex(attr);
        int otherCol = other.getColumnIndex(attr);

        if (thisCol != -1) {
            res.push_back(this->data[thisRow][thisCol]);
        } else {
            res.push_back(other.data[otherRow][otherCol]);
        }
    }

    return res;
}

int relation::max_element(int col) const {
    int maxVal = INT_MIN;

    for (int i = 0; i < this->getRowCount(); i++) {
        maxVal = max(maxVal, this->data[i][col]);
    }

    return maxVal;
}

string relation::rowToString(const vector<int>& row, const vector<int>& widths) {
    int rowCount = row.size();
    string res;

    for (int i = 0; i < rowCount; i++) {
        string s = to_string(row[i]);
        int len = s.size(), w = widths[i];

        if (i != rowCount - 1) {
            for (int j = len; j < w; j++) {
                s += " ";
            }

            res += s;
            res += "|";
        } else {
            res += s;
        }
    }

    return res;
}

string relation::rowToString(const vector<string>& row, const vector<int>& widths) {
    int rowCount = row.size();
    string res;

    for (int i = 0; i < rowCount; i++) {
        string s = row[i];
        int len = s.size(), w = widths[i];

        if (i != rowCount - 1) {
            for (int j = len; j < w; j++) {
                s += " ";
            }

            res += s;
            res += "|";
        } else {
            res += s;
        }
    }

    return res;
}

unordered_set<string> relation::getIntersection(vector<string> v1, vector<string> v2) {
    vector<string> intersect(min(v1.size(), v2.size()));
    unordered_set<string> set;

    set_intersection(v1.begin(), v1.end(),
                     v2.begin(), v2.end(),
                     intersect.begin());

    for (string val : intersect) {
        if (val != "")
            set.insert(val);
    }

    return set;
}

int relation::sum(vector<int>& widths) {
    int sum = 0;

    for (int w : widths) {
        sum += w;
    }

    return sum;
}
