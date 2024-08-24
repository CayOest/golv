#include <algorithm>
#include <functional>
#include <gtest/gtest.h>

#include <golv/algorithm/negamax.hpp>
#include <golv/games/connectfour.hpp>
#include <golv/games/tictactoe.hpp>

TEST(negamax, tictactoe_tie)
{
    golv::tictactoe game;
    auto solution = negamax(game);
    ASSERT_EQ(solution, 0);
}

TEST(negamax, tictactoe_win)
{
    golv::tictactoe game;
    game.apply_action(4);
    game.apply_action(1);
    auto solution = negamax(game);
    ASSERT_EQ(solution, 1);
}

TEST(negamax, tictactoe_loss)
{
    golv::tictactoe game;
    game.apply_action(1);
    game.apply_action(4);
    game.apply_action(7);
    auto solution = negamax(game);
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
}

TEST(negamax, tictactoe_win_ordered)
{
    golv::tictactoe game;
    game.apply_action(4);
    game.apply_action(1);
    auto solution = negamax(game, ordering);
    ASSERT_EQ(solution, 1);
}

TEST(negamax, tictactoe_loss_ordered)
{
    golv::tictactoe game;
    game.apply_action(1);
    game.apply_action(4);
    game.apply_action(7);
    auto solution = negamax(game, ordering);
    ASSERT_EQ(solution, -1);
}

TEST(negamax, tictactoe_with_memory)
{
    golv::tictactoe game;
    auto solution =
      golv::negamax(game, std::less<golv::tictactoe::move_type>{}, golv::unordered_table<golv::tictactoe>{});
    ASSERT_EQ(solution, 0);
}

namespace {
auto connectfour_ordering = [](golv::connectfour::move_type left, golv::connectfour::move_type right) {
    auto dist_left = std::abs(static_cast<int>(left) - static_cast<int>(golv::connectfour::width / 2));
    auto dist_right = std::abs(static_cast<int>(right) - static_cast<int>(golv::connectfour::width / 2));
    return dist_left < dist_right;
};

const std::vector<golv::connectfour::move_type> scenario1 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6 };

} // namespace

TEST(negamax, connectfour_raw)
{
    golv::connectfour game;
    const auto moves = scenario1;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::negamax(game);
    ASSERT_EQ(solution, +1);
}

TEST(negamax, connectfour_ordered)
{
    golv::connectfour game;
    const auto moves = scenario1;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::negamax(game, connectfour_ordering);
    ASSERT_EQ(solution, +1);
}

TEST(negamax, connectfour_ordered_lookup)
{
    golv::connectfour game;
    const auto moves = scenario1;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::negamax(game, connectfour_ordering, golv::unordered_table<golv::connectfour>{});
    ASSERT_EQ(solution, +1);
}

namespace {
const std::vector<golv::connectfour::move_type> scenario2 = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                                                              1, 3, 2, 3, 3, 3, 3, 3, 5, 2, 5 };
}

TEST(negamax, connectfour_ordered_2)
{
    golv::connectfour game;
    const auto moves = scenario2;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::negamax(game, connectfour_ordering);
    ASSERT_EQ(solution, -1);
}

TEST(negamax, connectfour_ordered_lookup_2)
{
    golv::connectfour game;
    const auto moves = scenario2;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::negamax(game, connectfour_ordering, golv::unordered_table<golv::connectfour>{});
    ASSERT_EQ(solution, -1);
}

TEST(negamax, connectfour_unordered_lookup_2)
{
    golv::connectfour game;
    const auto moves = scenario2;
    std::for_each(std::begin(moves), std::end(moves), [&game](auto move) { game.apply_action(move); });

    auto solution = golv::negamax_with_memory(game);
    ASSERT_EQ(solution, -1);
}