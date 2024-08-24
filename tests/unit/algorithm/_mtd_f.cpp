#include <algorithm>
#include <functional>
#include <gtest/gtest.h>

#include <golv/algorithm/mtd_f.hpp>
#include <golv/games/connectfour.hpp>
#include <golv/games/tictactoe.hpp>

using namespace golv;

TEST(mtd_f, tictactoe_tie)
{
    golv::tictactoe game;
    auto solution = mtd_f(game).solve(0);
    ASSERT_EQ(solution, 0);
}

TEST(mtd_f, tictactoe_win)
{
    golv::tictactoe game;
    game.apply_action(4);
    game.apply_action(1);
    auto solution = mtd_f(game).solve(0);
    ASSERT_EQ(solution, 1);
}

TEST(mtd_f, tictactoe_loss)
{
    golv::tictactoe game;
    game.apply_action(1);
    game.apply_action(4);
    game.apply_action(7);
    auto solution = mtd_f(game).solve(0);
    ASSERT_EQ(solution, -1);
}

namespace {
auto ordering = [](golv::tictactoe::move_type left, golv::tictactoe::move_type right) {
    if (left == 4)
        return true;
    else {
        if (right == 4)
            return false;
        else
            return left == 1 || left == 3 || left == 5 || left == 7;
    }
};

auto connectfour_ordering = [](golv::connectfour::move_type left, golv::connectfour::move_type right) {
    auto dist_left = std::abs(static_cast<int>(left) - static_cast<int>(golv::connectfour::width / 2));
    auto dist_right = std::abs(static_cast<int>(right) - static_cast<int>(golv::connectfour::width / 2));
    return dist_left < dist_right;
};

const std::vector<golv::connectfour::move_type> scenario1 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6 };

} // namespace

TEST(mtd_f, connectfour_1)
{
    golv::connectfour game;
    const auto moves = scenario1;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::mtd_f(game).solve(0);
    ASSERT_EQ(solution, +1);
}

namespace {
const std::vector<golv::connectfour::move_type> scenario2 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                                                              1, 3, 2, 3, 3, 3, 3, 3, 5, 2, 5 };
}

TEST(mtd_f, connectfour_2)
{
    golv::connectfour game;
    const auto moves = scenario2;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::mtd_f(game).solve(-1);
    ASSERT_EQ(solution, -1);
}

namespace {
const std::vector<golv::connectfour::move_type> scenario3 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 3, 2, 3, 3, 3, 3, 3 };
}

TEST(mtd_f, connectfour_3)
{
    golv::connectfour game;
    const auto moves = scenario3;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::mtd_f(game).solve(0);
    ASSERT_EQ(solution, -1);
}