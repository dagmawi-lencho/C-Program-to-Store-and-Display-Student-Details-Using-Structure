#include <bits/stdc++.h>
using namespace std;

// Simple expression evaluator (recursive descent).
// Supports: + - * / ^, parentheses, unary +/-, functions: sin, cos, tan, log, ln, sqrt, abs, exp
// Usage: run and type expressions; type "quit" or "exit" to leave.

class Parser {
    string s;
    size_t i = 0;

    void skipSpaces() {
        while (i < s.size() && isspace((unsigned char)s[i])) ++i;
    }

    bool match(char c) {
        skipSpaces();
        if (i < s.size() && s[i] == c) { ++i; return true; }
        return false;
    }

    // parse a number or parenthesized expression or function call
    double parsePrimary() {
        skipSpaces();
        if (i >= s.size()) throw runtime_error("Unexpected end of input");

        // parenthesis
        if (match('(')) {
            double val = parseExpression();
            if (!match(')')) throw runtime_error("Expected ')'");
            return val;
        }

        // function or identifier
        if (isalpha((unsigned char)s[i])) {
            size_t start = i;
            while (i < s.size() && (isalpha((unsigned char)s[i]) || isdigit((unsigned char)s[i]))) ++i;
            string name = s.substr(start, i - start);
            // support implicit multiplication after functions? not here.
            double arg = parsePrimary(); // function call must be followed by argument without parentheses too (like sin90) or with parentheses
            // But to require parentheses, you could adjust: if next token isn't '(', treat previous as variable.
            // We'll support parentheses usually: if arg is parsed as e.g. parentheses then ok.
            if (name == "sin") return sin(arg);
            if (name == "cos") return cos(arg);
            if (name == "tan") return tan(arg);
            if (name == "asin") return asin(arg);
            if (name == "acos") return acos(arg);
            if (name == "atan") return atan(arg);
            if (name == "sqrt") return sqrt(arg);
            if (name == "log" || name == "ln") return log(arg);
            if (name == "log10") return log10(arg);
            if (name == "exp") return exp(arg);
            if (name == "abs") return fabs(arg);
            throw runtime_error("Unknown function: " + name);
        }

        // number
        bool neg = false;
        if (s[i] == '+') { ++i; }
        if (s[i] == '-') { neg = true; ++i; }
        skipSpaces();
        size_t start = i;
        bool hasDigits = false;
        while (i < s.size() && (isdigit((unsigned char)s[i]) || s[i] == '.')) { if (isdigit((unsigned char)s[i])) hasDigits = true; ++i; }
        if (start == i || !hasDigits) {
            // If we've consumed unary sign but no number, maybe it was unary operator before parenthesis/function
            // rollback and handle unary in parseFactor
            i = start - (neg ? 1 : 0);
            throw runtime_error("Expected number or '('");
        }
        double val = stod(s.substr(start, i - start));
        return neg ? -val : val;
    }

    // handle unary +/-
    double parseFactor() {
        skipSpaces();
        if (match('+')) return parseFactor();
        if (match('-')) return -parseFactor();
        // try number / parenthesis / function
        return parsePrimary();
    }

    // exponentiation right-associative
    double parsePower() {
        double left = parseFactor();
        skipSpaces();
        if (match('^')) {
            double right = parsePower();
            return pow(left, right);
        }
        return left;
    }

    // * / %
    double parseTerm() {
        double val = parsePower();
        while (true) {
            skipSpaces();
            if (match('*')) val *= parsePower();
            else if (match('/')) {
                double d = parsePower();
                if (d == 0) throw runtime_error("Division by zero");
                val /= d;
            } else if (match('%')) {
                double d = parsePower();
                if (d == 0) throw runtime_error("Modulo by zero");
                val = fmod(val, d);
            } else break;
        }
        return val;
    }

    // + -
    double parseExpression() {
        double val = parseTerm();
        while (true) {
            skipSpaces();
            if (match('+')) val += parseTerm();
            else if (match('-')) val -= parseTerm();
            else break;
        }
        return val;
    }

public:
    double eval(const string& str) {
        s = str;
        i = 0;
        double result = parseExpression();
        skipSpaces();
        if (i != s.size()) throw runtime_error("Unexpected token at position " + to_string(i));
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << "Simple C++ Calculator. Type 'quit' or 'exit' to end.\n";
    Parser p;
    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line.empty()) continue;
        // trim
        auto trim = [](string &t) {
            size_t a = t.find_first_not_of(" \t\r\n");
            size_t b = t.find_last_not_of(" \t\r\n");
            if (a == string::npos) { t.clear(); return; }
            t = t.substr(a, b - a + 1);
        };
        trim(line);
        if (line == "quit" || line == "exit") break;
        try {
            double res = p.eval(line);
            // print integer if close to integer
            double intpart;
            if (modf(res, &intpart) == 0.0) cout << (long long)intpart << '\n';
            else cout << res << '\n';
        } catch (const exception& ex) {
            cout << "Error: " << ex.what() << '\n';
        }
    }
    return 0;
}