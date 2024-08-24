#pragma once

#include <concepts>

template <class GameT>
concept Game = requires(GameT g) {
                   typename GameT::move_type;
                   typename GameT::move_range;
                   typename GameT::player_type;
                   typename GameT::value_type;
                   typename GameT::state_type;

                   {
                       g.legal_actions()
                       } -> std::convertible_to<typename GameT::move_range>;
                   {
                       g.current_player()
                       } -> std::convertible_to<typename GameT::player_type>;
                   {
                       g.apply_action(typename GameT::move_type())
                   };
                   {
                       g.undo_action(typename GameT::move_type())
                   };
                   {
                       g.is_terminal()
                       } -> std::convertible_to<bool>;
                   {
                       g.value()
                       } -> std::convertible_to<typename GameT::value_type>;
                   {
                       g.is_max()
                       } -> std::convertible_to<bool>;
                   {
                       g.state()
                       } -> std::convertible_to<typename GameT::state_type>;
               };