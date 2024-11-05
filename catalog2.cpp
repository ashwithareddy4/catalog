#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <json/json.h> // Make sure to link against the jsoncpp library

// Function to decode a value in a given base
int decodeValue(const std::string &baseStr, const std::string &valueStr) {
    int base = std::stoi(baseStr);
    int value = 0;
    int power = 1;

    // Process the value string in reverse to decode it
    for (int i = valueStr.length() - 1; i >= 0; --i) {
        value += (valueStr[i] - '0') * power;
        power *= base;
    }

    return value;
}

// Function to compute the constant term using Lagrange interpolation
int computeConstantTerm(const std::vector<int> &xValues, const std::vector<int> &yValues) {
    int n = xValues.size();
    int c = 0; // This will hold the value of the constant term

    for (int i = 0; i < n; ++i) {
        int term = yValues[i]; // Start with y_i
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                term *= xValues[j] / (xValues[i] - xValues[j]); // Lagrange basis polynomial
            }
        }
        c += term; // Add the current term to the constant
    }

    return c;
}

int main() {
    // Prompt user for JSON input
    std::string jsonString;
    std::cout << "Enter the JSON input: ";
    std::getline(std::cin, jsonString);

    // Parse JSON input
    Json::Value root;
    Json::CharReaderBuilder reader;
    std::istringstream s(jsonString);
    std::string errs;

    if (!Json::parseFromStream(reader, s, &root, &errs)) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return 1;
    }

    int n = root["keys"]["n"].asInt();
    int k = root["keys"]["k"].asInt();
    std::vector<int> xValues;
    std::vector<int> yValues;

    // Read the roots from JSON
    for (const auto& key : root.getMemberNames()) {
        if (key != "keys") {
            int x = std::stoi(key);
            int y = decodeValue(root[key]["base"].asString(), root[key]["value"].asString());
            xValues.push_back(x);
            yValues.push_back(y);
        }
    }

    // Calculate the secret (constant term c)
    int secret = computeConstantTerm(xValues, yValues);

    // Output the result
    std::cout << "The secret (constant term c) is: " << secret << std::endl;

    return 0;
}