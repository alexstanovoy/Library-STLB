#include <stack>
#include <queue>
#include <vector>
#include <string>


namespace stlb
{

    namespace utility
    {

        struct operand
        {

            int32_t operands_number;
            const std::string str;

            operand(const int32_t operands_number, const std::string& str)
                : operands_number(operands_number), str(str) {}

        };


        class __infix_to_prefix
        {

        private:

            std::string _expr;
            std::vector<operand> _prefix;
            std::vector<operand> _func;
            std::vector<size_t> _comma_counter;

            bool is_digit_(const char symb) noexcept {
                return '0' <= symb && symb <= '9';
            }

            bool is_operand_(const char symb) noexcept {
                return symb == '+' || symb == '-' || symb == '*' || symb == '/';
            }

            bool is_alpha_(const char symb) noexcept {
                return ('a' <= symb && symb <= 'z') || ('A' <= symb && symb <= 'Z') || symb == '_';
            }

            std::string get_num_(size_t& ind) noexcept {
                std::string str;
                while (ind < _expr.size() && is_digit_(_expr[ind])) {
                    str += _expr[ind++];
                }
                return str;
            }

            std::string get_name_(size_t& ind) noexcept {
                std::string str;
                while (ind < _expr.size() && (is_digit_(_expr[ind]) || is_alpha_(_expr[ind]))) {
                    str += _expr[ind++];
                }
                return str;
            }

            void pull_to_prefix_() noexcept {
                _prefix.push_back(_func.back());
                _func.pop_back();
            }

            int32_t get_priority_(const operand& op) const noexcept {
                if (op.operands_number == 1) {
                    if (op.str.front() == '+' || op.str.front() == '-') {
                        return 30;
                    }
                }
                else {
                    if (op.str.front() == '+' || op.str.front() == '-') {
                        return 10;
                    }
                    if (op.str.front() == '*' || op.str.front() == '/') {
                        return 20;
                    }
                }
                return 0;
            }

            bool is_open_bracket_() const noexcept {
                return _func.back().str.front() == '(';
            }

            bool cmp_priority_(
                const operand& lhs,
                const operand& rhs
            ) const noexcept {
                return get_priority_(lhs) <= get_priority_(rhs);
            }

        public:

            __infix_to_prefix(const std::string& expression) noexcept {
                for (char i : expression) {
                    if (i != ' ') {
                        _expr += i;
                    }
                }
                _comma_counter.push_back(0);
            };

            std::vector<operand> convert() noexcept {
                for (size_t ind = 0; ind < _expr.size();) {
                    if (is_digit_(_expr[ind])) {
                        _prefix.emplace_back(-1, get_num_(ind));
                    }
                    else if (is_alpha_(_expr[ind])) {
                        std::string str = get_name_(ind);
                        if (ind >= _expr.size() || _expr[ind] != '(') {
                            _prefix.emplace_back(-1, str);
                        }
                        else {
                            _func.emplace_back(0, str);
                            _comma_counter.push_back(0);
                        }
                    }
                    else if (_expr[ind] == ',') {
                        while (!is_open_bracket_()) {
                            pull_to_prefix_();
                        }
                        ++ind, ++_comma_counter.back();
                    }
                    else if (is_operand_(_expr[ind])) {
                        operand current(
                            ind == 0 || is_operand_(_expr[ind - 1]) ||
                            _expr[ind - 1] == ',' || _expr[ind - 1] == '(' ? 1 : 2,
                            std::string(1, _expr[ind])
                        );
                        while (!_func.empty() && !is_open_bracket_() && cmp_priority_(current, _func.back())) {
                            pull_to_prefix_();
                        }
                        _func.push_back(current);
                        ++ind;
                    }
                    else if (_expr[ind] == '(') {
                        _func.emplace_back(0, "(");
                        ++ind;
                    }
                    else if (_expr[ind] == ')') {
                        while (!_func.empty() && !is_open_bracket_()) {
                            pull_to_prefix_();
                        }
                        _func.pop_back();
                        if (!_func.empty() && !is_open_bracket_() && _func.back().operands_number == 0) {
                            pull_to_prefix_();
                            if (_expr[ind - 1] != '(') {
                                _prefix.back().operands_number = _comma_counter.back() + 1;
                            }
                            _comma_counter.pop_back();
                        }
                        ++ind;
                    }
                }
                while (!_func.empty()) {
                    pull_to_prefix_();
                }
                return _prefix;
            }

        };


        auto infix_to_prefix(const std::string& str) noexcept {
            return __infix_to_prefix(str).convert();
        }

    }

}
