#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <unordered_set>


namespace stlb
{

    namespace utility
    {

        struct rule
        {
            const char from;
            const std::string to;

            rule(
                const char& from,
                const std::string& to
            ) noexcept : from(from), to(to) {}

            ~rule() noexcept = default;

            bool operator== (const rule& rhs) const noexcept {
                return from == rhs.from && to == rhs.to;
            }
        };

        class grammar
        {

        private:

            const char _start_symb;
            const std::vector<char> _N;
            const std::vector<char> _sigma;
            const std::vector<rule> _rules;

        public:

            grammar(
                const char& start_symb,
                const std::vector<char>& N,
                const std::vector<char>& sigma,
                const std::vector<rule>& rules
            ) noexcept : _start_symb(start_symb), _N(N),
                         _sigma(sigma), _rules(rules) {}

            ~grammar() noexcept = default;

            const rule& get_start() const noexcept {
                return *std::find_if(_rules.begin(), _rules.end(), [&](const rule& cur_rule) noexcept {
                    return cur_rule.from == _start_symb;
                });
            }

            const std::vector<rule>& get_rules() const noexcept {
                return _rules;
            }

        };

        class earley_parser
        {

        private:

            struct state
            {
                const rule& state_rule;
                const size_t point;
                const size_t origin;

                state(
                    const rule& state_rule,
                    const size_t& point,
                    const size_t& origin
                ) noexcept : state_rule(state_rule),
                             point(point), origin(origin) {}

                ~state() noexcept = default;

                bool operator== (const state& rhs) const noexcept {
                    return state_rule == rhs.state_rule &&
                           point == rhs.point && origin == rhs.origin;
                }

                char cur_symb() const noexcept {
                    return state_rule.to[point];
                }
            };

            struct state_hash
            {
                size_t operator() (const state& key) const noexcept {
                    return std::hash<char>()(key.state_rule.from) ^
                           std::hash<std::string>()(key.state_rule.to) ^
                           std::hash<size_t>()(key.point) ^
                           std::hash<size_t>()(key.origin);
                }
            };

            using hashed_set = std::unordered_set<state, state_hash>;

            const grammar _grammar;
            const std::vector<rule>& _rules;

            void predict_(
                const size_t& ind,
                std::vector<hashed_set>& states
            ) const noexcept {
                std::vector<state> new_states;
                for (const state& cur_state : states[ind]) {
                    for (const rule& cur_rule : _grammar.get_rules()) {
                        if (cur_rule.from == cur_state.cur_symb()) {
                            new_states.emplace_back(cur_rule, 0, ind);
                        }
                    }
                }
                for (const auto& cur_state : new_states) {
                    states[ind].insert(cur_state);
                }
            }

            void scan_(
                const size_t& ind,
                const char& symb,
                std::vector<hashed_set>& states
            ) const noexcept {
                for (const state& cur_state : states[ind]) {
                    if (cur_state.cur_symb() == symb) {
                        states[ind + 1].emplace(
                            cur_state.state_rule,
                            cur_state.point + 1,
                            cur_state.origin
                        );
                    }
                }
            }

            void complete_(
                const size_t& ind,
                std::vector<hashed_set>& states
            ) const noexcept {
                std::vector<state> new_states;
                for (const state& state1 : states[ind]) {
                    if (state1.point != state1.state_rule.to.size()) {
                        continue;
                    }
                    for (const state& state2 : states[state1.origin]) {
                        if (state1.state_rule.from == state2.cur_symb()) {
                            new_states.emplace_back(
                                state2.state_rule,
                                state2.point + 1,
                                state2.origin
                            );
                        }
                    }
                }
                states[ind].insert(new_states.begin(), new_states.end());
            }

            bool finalize_(
                const size_t& len,
                std::vector<hashed_set>& states
            ) const noexcept {
                return states[len].find(state(_grammar.get_start(), 1, 0)) != states[len].end();
            }

        public:

            earley_parser(
                const grammar& input_grammar
            ) noexcept : _grammar(input_grammar),
                         _rules(_grammar.get_rules()) {}

            ~earley_parser() noexcept = default;

            bool calculate(const std::string& word) const noexcept {
                std::vector<hashed_set> states(word.size() + 1);
                states.front().emplace(_grammar.get_start(), 0, 0);
                while (true) {
                    const size_t cur_size = states.front().size();
                    predict_(0, states);
                    complete_(0, states);
                    if (states[0].size() == cur_size) {
                        break;
                    }
                }
                for (size_t i = 1; i <= word.size(); ++i) {
                    scan_(i - 1, word[i - 1], states);
                    while (true) {
                        const size_t cur_size = states[i].size();
                        predict_(i, states);
                        complete_(i, states);
                        if (states[i].size() == cur_size) {
                            break;
                        }
                    }
                }
                return finalize_(word.size(), states);
            }

        };

    }

}
